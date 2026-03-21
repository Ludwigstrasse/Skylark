#include "SKRuntime/SKRuntimeEngine.h"

#include "SKCore/SKCoreLog.h"
#include "SKRHI/SKRHIFactory.h"
#include "SKScene/SKSceneGraph.h"


namespace Skylark
{
	// ---------------------------------------------
	FSKRuntimeEngine::FViewportImpl::FViewportImpl(const FSKViewportDesc& InDesc, ISKRHIDevice* InDevice, TSharedPtr<FSKSceneGraph> InScene)
		: Desc(InDesc)
		, Device(InDevice)
		, SharedScene(std::move(InScene))
	{
		ViewInfo.ViewMode = ViewMode;
		ViewInfo.Features.bFieldViz = true;
		ViewInfo.FieldViz.DeformScale = 1.0f;
		ViewInfo.FieldViz.ValueMin = 0.0f;
		ViewInfo.FieldViz.ValueMax = 100.0f;
		RecreateRHIResources(Device);
	}

	void FSKRuntimeEngine::FViewportImpl::ReleaseRHIResources()
	{
		SelectionTexture.reset();
		SwapChain.reset();
		Device = nullptr;
	}

	void FSKRuntimeEngine::FViewportImpl::RecreateRHIResources(ISKRHIDevice* InDevice)
	{
		ReleaseRHIResources();
		Device = InDevice;
		if (Device)
		{
			FSKRHISwapChainDesc SC;
			SC.Window = Desc.Window;
			SC.Width = static_cast<uint32>((Desc.Width <= 0) ? 1 : Desc.Width);
			SC.Height = static_cast<uint32>((Desc.Height <= 0) ? 1 : Desc.Height);
			SC.bVSync = Desc.bVSync;
			SwapChain = Device->CreateSwapChain(SC);
		}

		EnsureSelectionTexture();
	}

	void FSKRuntimeEngine::FViewportImpl::EnsureSelectionTexture()
	{
		if (!Device)
		{
			return;
		}

		const uint32 W = (Desc.Width <= 0) ? 1u : static_cast<uint32>(Desc.Width);
		const uint32 H = (Desc.Height <= 0) ? 1u : static_cast<uint32>(Desc.Height);

		if (SelectionTexture)
		{
			const auto& TD = SelectionTexture->GetDesc();
			if (TD.Width == W && TD.Height == H)
			{
				return;
			}
		}

		FSKRHITextureDesc TD{};
		TD.Width = W;
		TD.Height = H;
		TD.Format = ESKRHIFormat::R8G8B8A8_UNORM;
		TD.Flags = (uint32)(SK_Tex_RenderTarget | SK_Tex_CPUReadback);

		SelectionTexture = Device->CreateTexture2D(TD);
	}

	void FSKRuntimeEngine::FViewportImpl::Resize(int32 NewWidth, int32 NewHeight)
	{
		Desc.Width = NewWidth;
		Desc.Height = NewHeight;
		if (SwapChain)
		{
			SwapChain->Resize(static_cast<uint32>(NewWidth), static_cast<uint32>(NewHeight));
		}
		EnsureSelectionTexture();
	}

	// ---------------------------------------------
	FSKRuntimeEngine::FSKRuntimeEngine() = default;

	FSKRuntimeEngine::~FSKRuntimeEngine()
	{
		Shutdown();
	}

	static FSKMeshData SKMakeUnitCubeMesh(uint32 HitProxyId)
	{
		FSKMeshData Mesh;
		FSKMeshSection Section;
		Section.HitProxyId = HitProxyId;

		auto AddV = [&](float x, float y, float z)
		{
			FSKVertex V;
			V.Position = FSKVector3f(x, y, z);
			V.Normal = FSKVector3f(0, 0, 1);
			Section.Vertices.push_back(V);
		};

		AddV(-0.5f, -0.5f,  0.5f); // 0
		AddV(-0.5f,  0.5f,  0.5f); // 1
		AddV( 0.5f,  0.5f,  0.5f); // 2
		AddV( 0.5f, -0.5f,  0.5f); // 3
		AddV(-0.5f, -0.5f, -0.5f); // 4
		AddV(-0.5f,  0.5f, -0.5f); // 5
		AddV( 0.5f,  0.5f, -0.5f); // 6
		AddV( 0.5f, -0.5f, -0.5f); // 7

		const uint32 Idx[] = {
			0,1,2,  0,2,3, // front
			4,6,5,  4,7,6, // back
			4,5,1,  4,1,0, // left
			3,2,6,  3,6,7, // right
			1,5,6,  1,6,2, // top
			4,0,3,  4,3,7, // bottom
		};
		Section.Indices.assign(Idx, Idx + (sizeof(Idx) / sizeof(uint32)));
		Mesh.Sections.push_back(Section);
		return Mesh;
	}

	bool FSKRuntimeEngine::CreateAndInitRHI(ESKRHIApi RequestedApi)
	{
		FSKRHIDeviceDesc RD;
		RD.Api = RequestedApi;
		RD.bEnableDebugLayer = InitParams.bEnableRHIDebug;

		auto TryInitRHI = [&](ESKRHIApi Api) -> bool
		{
			RD.Api = Api;
			RHIDevice = SKCreateRHIDevice(RD);
			if (!RHIDevice)
			{
				return false;
			}
			if (!RHIDevice->Init(RD))
			{
				RHIDevice.reset();
				return false;
			}
			return true;
		};

		if (TryInitRHI(RequestedApi))
		{
			return true;
		}

		if (RequestedApi == ESKRHIApi::D3D12 && TryInitRHI(ESKRHIApi::D3D11))
		{
			SK_LOG(GLogSkylark, ESKLogVerbosity::Warning, "SKRuntime: D3D12 init failed, using D3D11 fallback.");
			return true;
		}

		if (RequestedApi != ESKRHIApi::Vulkan && TryInitRHI(ESKRHIApi::Vulkan))
		{
			SK_LOG(GLogSkylark, ESKLogVerbosity::Warning, "SKRuntime: requested RHI init failed, using Vulkan fallback.");
			return true;
		}

		if (RequestedApi != ESKRHIApi::OpenGL && TryInitRHI(ESKRHIApi::OpenGL))
		{
			SK_LOG(GLogSkylark, ESKLogVerbosity::Warning, "SKRuntime: requested RHI init failed, using OpenGL fallback.");
			return true;
		}

		if (RequestedApi != ESKRHIApi::Null && TryInitRHI(ESKRHIApi::Null))
		{
			SK_LOG(GLogSkylark, ESKLogVerbosity::Warning, "SKRuntime: requested RHI init failed, using NullRHI fallback.");
			return true;
		}

		return false;
	}

	bool FSKRuntimeEngine::Init(const FSKEngineInitParams& Params)
	{
		InitParams = Params;
		if (!CreateAndInitRHI(Params.RhiApi))
		{
			SK_LOG(GLogSkylark, ESKLogVerbosity::Error, "SKRuntime: RHI init failed.");
			return false;
		}

		// Streaming (V8): background queue for IO/parse/CPU build tasks.
		Streaming = std::make_unique<FSKStreamingQueue>();
		Streaming->Start();

		// BIM visibility controller (V8)
		BimVisibility = std::make_shared<FSKBimVisibilitySet>();

		// SceneGraph (OSG-aligned)
		SceneGraph = std::make_shared<FSKSceneGraph>();

		// Minimal demo content (V8): create a small BIM-like multi-level instanced scene.
		// - Register one cube geometry.
		// - Create 4 levels, 2 categories per level, each as an InstanceNode.
		{
			FSKGeometryBuildSettings Build;
			Build.bBuildWireframe = true;
			Build.bBuildEdgeCategories = true;
			Build.CreaseAngleDeg = 30.0f;

			const uint64 CubeGeoKey = GeometryRegistry.RegisterMesh("DemoCube", SKMakeUnitCubeMesh(1), Build);

			// V9 demo: CAE field data (scalar + displacement) bound to cube geometry.
			{
				const FSKGeometryData* Geo = GeometryRegistry.Find(CubeGeoKey);
				if (Geo)
				{
					// Flatten vertex count
					uint32 TotalV = 0;
					for (const auto& Sec : Geo->Mesh.Sections) { TotalV += (uint32)Sec.Vertices.size(); }

					TArray<float> Stress;
					Stress.resize(TotalV);

					TArray<FSKVector3f> Disp;
					Disp.resize(TotalV);

					uint32 Base = 0;
					for (const auto& Sec : Geo->Mesh.Sections)
					{
						for (SIZE_T i = 0; i < Sec.Vertices.size(); ++i)
						{
							const auto& P = Sec.Vertices[i].Position;
							const float s = (P.Z + 0.5f) * 100.0f;
							Stress[Base + (uint32)i] = s;

							// Small sinusoidal displacement in Z
							Disp[Base + (uint32)i] = FSKVector3f(0.0f, 0.0f, 0.05f * std::sin(P.X * 6.0f) * std::cos(P.Y * 6.0f));
						}
						Base += (uint32)Sec.Vertices.size();
					}

					auto& FS = FieldRegistry.GetOrCreateFieldSet(CubeGeoKey);

					FSKFieldKey KStress;
					KStress.Name = "Stress";
					KStress.Domain = ESKFieldDomain::Vertex;
					KStress.Type = ESKFieldValueType::Scalar;

					FSKFieldStats SStat;
					SStat.MinValue = 0.0f;
					SStat.MaxValue = 100.0f;
					SStat.bHasRange = true;

					FS.RegisterScalar(KStress, std::move(Stress), SStat);

					FSKFieldKey KDisp;
					KDisp.Name = "Displacement";
					KDisp.Domain = ESKFieldDomain::Vertex;
					KDisp.Type = ESKFieldValueType::Vector3;

					FS.RegisterVector(KDisp, std::move(Disp), {});
				}
			}

			// V9 demo: one leader annotation (placeholder, line-only).
			{
				FSKAnnotation A;
				A.Text = "Demo Note";
				A.WorldA = FSKVector3f(0.0f, 0.0f, 0.0f);
				A.WorldB = FSKVector3f(2.0f, 2.0f, 0.0f);
				A.Style.ColorRGBA8 = 0xFF202020u;
				A.Style.LineWidth = 2.0f;
				AnnotationManager.Add(A);
			}

			// Category bits (IFC-like): 0=Wall, 1=Door
			constexpr uint64 CatWall = 1ull << 0;
			constexpr uint64 CatDoor = 1ull << 1;

			FSKGuid DocGuid;
			DocGuid.A = 0x11111111u;
			DocGuid.B = 0x22222222u;

			uint32 HitId = 1;

			for (uint32 Level = 0; Level < 4; ++Level)
			{
				const uint64 LevelMask = 1ull << Level;
				auto LevelNode = std::make_shared<FSKNode>("Level" + std::to_string(Level));
				LevelNode->SetLevelMask(LevelMask);
				LevelNode->SetCategoryMask(~0ull);
				LevelNode->SetSystemMask(~0ull);

				// Two categories
				auto Walls = std::make_shared<FSKInstanceNode>("Walls");
				Walls->SetGeometryKey(CubeGeoKey);
				Walls->SetCategoryMask(CatWall);
				Walls->SetLevelMask(LevelMask);

				auto Doors = std::make_shared<FSKInstanceNode>("Doors");
				Doors->SetGeometryKey(CubeGeoKey);
				Doors->SetCategoryMask(CatDoor);
				Doors->SetLevelMask(LevelMask);

				// Build a grid per level
				const int Grid = 20;
				const float Spacing = 2.0f;
				const float BaseZ = (float)Level * 3.0f;

				for (int y = 0; y < Grid; ++y)
				{
					for (int x = 0; x < Grid; ++x)
					{
						const float Tx = (float)x * Spacing;
						const float Ty = (float)y * Spacing;
						const float Tz = BaseZ;

						FSKObjectId O;
						O.DocumentGuid = DocGuid;
						O.ObjectGuid.A = (uint64)Level * 100000ull + (uint64)y * 1000ull + (uint64)x;
						O.ObjectGuid.B = O.ObjectGuid.A ^ 0xA5A5A5A5A5A5A5A5ull;
						O.SubId = 0;
						O.Type = ESKObjectEntityType::Element;

						const FSKMatrix4f LT = FSKMatrix4f::Translation(Tx, Ty, Tz);
						const uint32 ThisHit = ++HitId;

						// Split instances between walls/doors (simple pattern)
						if (((x + y) & 7) == 0)
						{
							Doors->AddInstance(LT, ThisHit, O);
						}
						else
						{
							Walls->AddInstance(LT, ThisHit, O);
						}
					}
				}

				LevelNode->AddChild(Walls);
				LevelNode->AddChild(Doors);
				SceneGraph->GetRoot()->AddChild(LevelNode);
			}

			SceneGraph->RebuildIndex();
		}

		Pipeline = SKCreateDefaultPipeline();

		// RenderThread (V11)
		if (Params.bUseRenderThread)
		{
			RenderThread = std::make_unique<FSKRenderThread>();
			RenderThread->Start();
		}


		return true;
	}

	void FSKRuntimeEngine::Shutdown()
	{
		// Stop render thread first (V11)
		if (RenderThread)
		{
			RenderThread->Flush();
			RenderThread->Stop();
			RenderThread.reset();
		}

		{
			std::lock_guard<std::mutex> Lock(ViewportMutex);
			Viewports.clear();
		}

		Pipeline.reset();
		SceneGraph.reset();

		if (Streaming)
		{
			Streaming->Stop();
			Streaming.reset();
		}
		BimVisibility.reset();


		if (RHIDevice)
		{
			RHIDevice->Shutdown();
			RHIDevice.reset();
		}
	}

	void FSKRuntimeEngine::Tick(float DeltaSeconds)
	{
		FSKFrameParams Frame{};
		Frame.DeltaSeconds = DeltaSeconds;

		// Snapshot viewports (thread-safe)
		TArray<FViewportImpl*> Snapshot;
		{
			std::lock_guard<std::mutex> Lock(ViewportMutex);
			Snapshot.reserve(Viewports.size());
			for (auto& VP : Viewports)
			{
				if (VP)
				{
					Snapshot.push_back(VP.get());
				}
			}
		}

		auto RenderWork = [this, Snapshot = std::move(Snapshot), Frame]()
		{
			for (auto* VP : Snapshot)
			{
				if (!VP)
				{
					continue;
				}

				// Update/Cull (OSG-like) executed on render thread (UE-like separation)
				VP->GetSceneViewMutable().GeometryRegistry = &GeometryRegistry;
				VP->GetSceneViewMutable().FieldRegistry = &FieldRegistry;
				VP->GetSceneViewMutable().AnnotationManager = &AnnotationManager;

				// LWC (V11): propagate view origin into scene view for camera-relative transforms
				if (auto* VP_View = dynamic_cast<ISKViewportView*>(VP))
				{
					VP->GetSceneViewMutable().ViewOrigin = VP_View->GetViewInfo().ViewOrigin;
				}

				if (SceneGraph)
				{
					FSKUpdateVisitor Update;
					SceneGraph->Traverse(Update);

					FSKSceneCullParams CP;
					if (auto* VP_View = dynamic_cast<ISKViewportView*>(VP))
					{
						const auto& VI = VP_View->GetViewInfo();
						CP.Filter.CategoryMask = VI.BimFilter.CategoryMask;
						CP.Filter.LevelMask = VI.BimFilter.LevelMask;
						CP.Filter.SystemMask = VI.BimFilter.SystemMask;
						CP.Filter.ViewVisibilityMask = VI.BimFilter.ViewVisibilityMask;
					}
					CP.Visibility = BimVisibility.get();
					CP.bBuildInstanceBatches = true;

					FSKCullVisitor Cull(VP->GetSceneViewMutable(), CP);
					SceneGraph->Traverse(Cull);
				}

				if (Pipeline)
				{
					Pipeline->RenderFrame(*VP, Frame);
				}
			}

		};

		if (RenderThread && RenderThread->IsRunning())
		{
			RenderThread->Enqueue("SkylarkFrame", std::move(RenderWork));
		}
		else
		{
			RenderWork();
		}
	}

	ESKRHIApi FSKRuntimeEngine::GetCurrentRHIApi() const
	{
		return RHIDevice ? RHIDevice->GetApi() : ESKRHIApi::Null;
	}

	bool FSKRuntimeEngine::RecreateRHI(ESKRHIApi NewApi)
	{
		if (GetCurrentRHIApi() == NewApi)
		{
			return true;
		}

		if (RenderThread)
		{
			RenderThread->Flush();
			RenderThread->Stop();
			RenderThread.reset();
		}

		{
			std::lock_guard<std::mutex> Lock(ViewportMutex);
			for (auto& VP : Viewports)
			{
				if (VP)
				{
					VP->ReleaseRHIResources();
				}
			}
		}

		Pipeline.reset();

		if (RHIDevice)
		{
			RHIDevice->Shutdown();
			RHIDevice.reset();
		}

		if (!CreateAndInitRHI(NewApi))
		{
			SK_LOG(GLogSkylark, ESKLogVerbosity::Error, "SKRuntime: RecreateRHI failed.");
			return false;
		}

		{
			std::lock_guard<std::mutex> Lock(ViewportMutex);
			for (auto& VP : Viewports)
			{
				if (VP)
				{
					VP->RecreateRHIResources(RHIDevice.get());
				}
			}
		}

		Pipeline = SKCreateDefaultPipeline();
		InitParams.RhiApi = GetCurrentRHIApi();

		if (InitParams.bUseRenderThread)
		{
			RenderThread = std::make_unique<FSKRenderThread>();
			RenderThread->Start();
		}

		return true;
	}

	ISKViewport* FSKRuntimeEngine::CreateViewport(const FSKViewportDesc& Desc)
	{
		std::lock_guard<std::mutex> Lock(ViewportMutex);
		Viewports.push_back(std::make_unique<FViewportImpl>(Desc, RHIDevice.get(), SceneGraph));
		return Viewports.back().get();
	}

	void FSKRuntimeEngine::DestroyViewport(ISKViewport* Viewport)
	{
		std::lock_guard<std::mutex> Lock(ViewportMutex);
		for (auto It = Viewports.begin(); It != Viewports.end(); ++It)
		{
			if (It->get() == Viewport)
			{
				Viewports.erase(It);
				return;
			}
		}
	}
}
