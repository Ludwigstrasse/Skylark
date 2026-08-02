#include "SKD3D12RHI/SKD3D12RHI.h"
#include "SKCore/SKAssert.h"
#include "SKCore/SKCoreLog.h"

#include "SKShaderCompiler/SKShaderCompiler.h"
#include "SKPipelineCache/SKPipelineCache.h"

#if defined(_WIN32)

	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif
	#ifndef NOMINMAX
		#define NOMINMAX
	#endif
	#include <windows.h>

	#include <wrl/client.h>
	#include <d3d12.h>
	#include <dxgi1_6.h>

	#include <algorithm>
	#include <cstring>

	using Microsoft::WRL::ComPtr;

namespace Skylark
{
	static DXGI_FORMAT SKToDXGIFormat(ESKRHIFormat Format)
	{
		switch (Format)
		{
		case ESKRHIFormat::R8G8B8A8_UNORM: return DXGI_FORMAT_R8G8B8A8_UNORM;
		case ESKRHIFormat::R32_UINT:       return DXGI_FORMAT_R32_UINT;
		case ESKRHIFormat::D24S8:          return DXGI_FORMAT_D24_UNORM_S8_UINT;
		default:                           return DXGI_FORMAT_UNKNOWN;
		}
	}

	static D3D12_RESOURCE_STATES SKInitialStateForTexture(const FSKRHITextureDesc& Desc)
	{
		if (Desc.Flags & SK_Tex_RenderTarget)
		{
			return D3D12_RESOURCE_STATE_RENDER_TARGET;
		}
		return D3D12_RESOURCE_STATE_COMMON;
	}

	// ------------------------------------------------------------
	// Upload ring allocator (UE-like linear upload)
	// ------------------------------------------------------------
	class FSKD3D12UploadRing
	{
	public:
		bool Init(ID3D12Device* InDevice, SIZE_T InSizeBytes)
		{
			Device = InDevice;
			SizeBytes = InSizeBytes;

			D3D12_HEAP_PROPERTIES Heap{};
			Heap.Type = D3D12_HEAP_TYPE_UPLOAD;

			D3D12_RESOURCE_DESC Desc{};
			Desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
			Desc.Width = SizeBytes;
			Desc.Height = 1;
			Desc.DepthOrArraySize = 1;
			Desc.MipLevels = 1;
			Desc.SampleDesc.Count = 1;
			Desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

			if (FAILED(Device->CreateCommittedResource(
				&Heap,
				D3D12_HEAP_FLAG_NONE,
				&Desc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&Buffer))))
			{
				return false;
			}

			if (FAILED(Buffer->Map(0, nullptr, (void**)&Mapped)))
			{
				Buffer.Reset();
				return false;
			}
			GpuBase = Buffer->GetGPUVirtualAddress();
			Offset = 0;
			return true;
		}

		void Shutdown()
		{
			if (Buffer)
			{
				Buffer->Unmap(0, nullptr);
			}
			Mapped = nullptr;
			Buffer.Reset();
			Device = nullptr;
			Offset = 0;
			SizeBytes = 0;
			GpuBase = 0;
		}

		struct FAlloc
		{
			void* Cpu = nullptr;
			D3D12_GPU_VIRTUAL_ADDRESS Gpu = 0;
			UINT64 Offset = 0;
		};

		FAlloc Allocate(SIZE_T Bytes, SIZE_T Alignment = 16)
		{
			FAlloc A;
			if (!Buffer || !Mapped || Bytes == 0)
			{
				return A;
			}

			if (Bytes > SizeBytes)
			{
				return A;
			}

			const UINT64 Aligned = (Offset + (UINT64)Alignment - 1) & ~((UINT64)Alignment - 1);
			if (Aligned + (UINT64)Bytes > (UINT64)SizeBytes)
			{
				// Wrap (safe because we fence per-frame in this simple backend).
				Offset = 0;
			}

			const UINT64 Final = (Offset + (UINT64)Alignment - 1) & ~((UINT64)Alignment - 1);
			A.Offset = Final;
			A.Cpu = Mapped + Final;
			A.Gpu = GpuBase + Final;
			Offset = Final + (UINT64)Bytes;
			return A;
		}

		ID3D12Resource* GetResource() const { return Buffer.Get(); }

	private:
		ID3D12Device* Device = nullptr;
		SIZE_T SizeBytes = 0;
		UINT64 Offset = 0;
		ComPtr<ID3D12Resource> Buffer;
		uint8* Mapped = nullptr;
		D3D12_GPU_VIRTUAL_ADDRESS GpuBase = 0;
	};

	// ------------------------------------------------------------
	// Texture2D
	// ------------------------------------------------------------
	class FSKD3D12Texture2D final : public ISKRHITexture2D
	{
	public:
		FSKD3D12Texture2D(FSKRHITextureDesc InDesc, ComPtr<ID3D12Resource> InRes)
			: Desc(std::move(InDesc)), Resource(std::move(InRes))
		{
		}

		const FSKRHITextureDesc& GetDesc() const override { return Desc; }

		ID3D12Resource* GetNative() const { return Resource.Get(); }
		D3D12_CPU_DESCRIPTOR_HANDLE GetRTV() const { return RTV; }
		bool HasRTV() const { return RTV.ptr != 0; }
		void SetRTV(D3D12_CPU_DESCRIPTOR_HANDLE In) { RTV = In; }

	private:
		FSKRHITextureDesc Desc{};
		ComPtr<ID3D12Resource> Resource;
		D3D12_CPU_DESCRIPTOR_HANDLE RTV{ 0 };
	};

	// ------------------------------------------------------------
	// SwapChain
	// ------------------------------------------------------------
	class FSKD3D12RHIDevice;

	class FSKD3D12SwapChain final : public ISKRHISwapChain
	{
	public:
		FSKD3D12SwapChain(FSKD3D12RHIDevice* InOwner, FSKRHISwapChainDesc InDesc)
			: Owner(InOwner)
			, Desc(std::move(InDesc))
		{
		}

		const FSKRHISwapChainDesc& GetDesc() const override { return Desc; }

		void Resize(uint32 NewWidth, uint32 NewHeight) override
		{
			Desc.Width = NewWidth;
			Desc.Height = NewHeight;
			bResizeRequested = true;
		}

		void Present() override;

		FSKD3D12RHIDevice* Owner = nullptr;
		FSKRHISwapChainDesc Desc{};
		bool bResizeRequested = false;
		ComPtr<IDXGISwapChain3> SwapChain;
		ComPtr<ID3D12DescriptorHeap> RtvHeap;
		TArray<ComPtr<ID3D12Resource>> BackBuffers;
		TArray<D3D12_CPU_DESCRIPTOR_HANDLE> RTVHandles;
	};

	// ------------------------------------------------------------
	// D3D12 CommandList (Immediate)
	// ------------------------------------------------------------
	struct FSKD3D12LinePipeline
	{
		ComPtr<ID3D12RootSignature> RootSig;
		ComPtr<ID3D12PipelineState> PSO;
		bool bReady = false;
	};

	struct FSKD3D12TrianglePipeline
	{
		ComPtr<ID3D12RootSignature> RootSig;
		ComPtr<ID3D12PipelineState> PSO;
		bool bReady = false;
	};

	class FSKD3D12CommandList final : public ISKRHICommandList
	{
	public:
		explicit FSKD3D12CommandList(class FSKD3D12RHIDevice* InOwner)
			: Owner(InOwner)
		{
		}

		void SetSwapChainRenderTarget(ISKRHISwapChain& InSwapChain) override;
		void SetRenderTargetTexture(ISKRHITexture2D& ColorTarget) override;
		void ClearRenderTarget(const FSKRHIClearColor& Color) override;
		void DrawLineList(const FSKRHILineVertex* Vertices, uint32 VertexCount, const FSKRHILineDrawParams& Params) override;
		void DrawTriangleList(const FSKRHITriangleVertex* Vertices, uint32 VertexCount, const FSKRHITriangleDrawParams& Params) override;
		void DrawIndexedTriangleList(const FSKRHITriangleVertex* Vertices, uint32 VertexCount, const uint32* Indices, uint32 IndexCount, const FSKRHITriangleDrawParams& Params) override;
		void DrawIndexedInstancedTriangleList(const FSKRHITriangleVertex* Vertices, uint32 VertexCount, const uint32* Indices, uint32 IndexCount, const FSKRHITriangleInstance* Instances, uint32 InstanceCount, const FSKRHITriangleDrawParams& Params) override;
		void Flush() override {}

		ID3D12GraphicsCommandList* CmdList = nullptr;
		class FSKD3D12RHIDevice* Owner = nullptr;

	private:
		FSKD3D12SwapChain* BoundSwapChain = nullptr;
		FSKD3D12Texture2D* BoundTexture = nullptr;
		D3D12_CPU_DESCRIPTOR_HANDLE BoundRTV{ 0 };
		uint32 BoundRTVWidth = 1;
		uint32 BoundRTVHeight = 1;
	};

	// ------------------------------------------------------------
	// Device
	// ------------------------------------------------------------
	class FSKD3D12RHIDevice final : public ISKRHIDevice
	{
	public:
		FSKD3D12RHIDevice()
			: Immediate(this)
		{
		}

		bool Init(const FSKRHIDeviceDesc& InDesc) override
		{
			Desc = InDesc;

			UINT FactoryFlags = 0;
			if (Desc.bEnableDebugLayer)
			{
				ComPtr<ID3D12Debug> Debug;
				if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&Debug))))
				{
					Debug->EnableDebugLayer();
					FactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
				}
			}

			if (FAILED(CreateDXGIFactory2(FactoryFlags, IID_PPV_ARGS(&Factory))))
			{
				SK_LOG(GLogSkylark, ESKLogVerbosity::Error, "D3D12: CreateDXGIFactory2 failed");
				return false;
			}

			ComPtr<IDXGIAdapter1> Adapter;
			if (Desc.AdapterIndex >= 0)
			{
				Factory->EnumAdapters1((UINT)Desc.AdapterIndex, &Adapter);
			}
			if (!Adapter)
			{
				Factory->EnumAdapters1(0, &Adapter);
			}
			if (!Adapter)
			{
				SK_LOG(GLogSkylark, ESKLogVerbosity::Error, "D3D12: no adapter");
				return false;
			}

			if (FAILED(D3D12CreateDevice(Adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&Device))))
			{
				SK_LOG(GLogSkylark, ESKLogVerbosity::Error, "D3D12: D3D12CreateDevice failed");
				return false;
			}

			D3D12_COMMAND_QUEUE_DESC Q{};
			Q.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
			if (FAILED(Device->CreateCommandQueue(&Q, IID_PPV_ARGS(&Queue))))
			{
				SK_LOG(GLogSkylark, ESKLogVerbosity::Error, "D3D12: CreateCommandQueue failed");
				return false;
			}

			if (FAILED(Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&Allocator))))
			{
				SK_LOG(GLogSkylark, ESKLogVerbosity::Error, "D3D12: CreateCommandAllocator failed");
				return false;
			}

			if (FAILED(Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, Allocator.Get(), nullptr, IID_PPV_ARGS(&CmdList))))
			{
				SK_LOG(GLogSkylark, ESKLogVerbosity::Error, "D3D12: CreateCommandList failed");
				return false;
			}
			CmdList->Close();

			if (FAILED(Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&Fence))))
			{
				SK_LOG(GLogSkylark, ESKLogVerbosity::Error, "D3D12: CreateFence failed");
				return false;
			}

			FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
			if (!FenceEvent)
			{
				return false;
			}

			Immediate.CmdList = CmdList.Get();

			// V13: shader compiler + disk cache
			ShaderCompiler = SKCreateDefaultShaderCompiler();
			ShaderCache = std::make_unique<FSKShaderBytecodeCache>(std::make_unique<FSKDiskBytecodeCache>(".sk_shadercache"));

			// V13: upload ring for dynamic draws
			if (!Upload.Init(Device.Get(), 128 * 1024 * 1024))
			{
				SK_LOG(GLogSkylark, ESKLogVerbosity::Warning, "D3D12: Upload ring init failed (line rendering may fail)");
			}

			return true;
		}

		void Shutdown() override
		{
			WaitGPU();
			SwapChains.clear();
			Line = {};
			Triangle = {};
			Upload.Shutdown();
			ShaderCache.reset();
			ShaderCompiler.reset();

			if (FenceEvent) { CloseHandle(FenceEvent); FenceEvent = nullptr; }
			CmdList.Reset();
			Allocator.Reset();
			Queue.Reset();
			Fence.Reset();
			Device.Reset();
			Factory.Reset();
		}

		void BeginFrame() override
		{
			bSubmittedByPresent = false;
			Allocator->Reset();
			CmdList->Reset(Allocator.Get(), nullptr);
			Immediate.CmdList = CmdList.Get();
		}

		void EndFrame() override
		{
			if (bSubmittedByPresent)
			{
				return;
			}
			CmdList->Close();
			ID3D12CommandList* Lists[] = { CmdList.Get() };
			Queue->ExecuteCommandLists(1, Lists);
			WaitGPU();
		}

		ISKRHICommandList& GetImmediateCommandList() override { return Immediate; }

		TUniquePtr<ISKRHISwapChain> CreateSwapChain(const FSKRHISwapChainDesc& InDesc) override
		{
			auto SC = std::make_unique<FSKD3D12SwapChain>(this, InDesc);
			if (!InDesc.Window.Handle)
			{
				return SC;
			}

			DXGI_SWAP_CHAIN_DESC1 S{};
			S.Width = InDesc.Width;
			S.Height = InDesc.Height;
			S.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			S.SampleDesc.Count = 1;
			S.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			S.BufferCount = InDesc.BufferCount;
			S.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

			ComPtr<IDXGISwapChain1> Swap1;
			if (FAILED(Factory->CreateSwapChainForHwnd(Queue.Get(), reinterpret_cast<HWND>(InDesc.Window.Handle), &S, nullptr, nullptr, &Swap1)))
			{
				SK_LOG(GLogSkylark, ESKLogVerbosity::Error, "D3D12: CreateSwapChainForHwnd failed");
				return SC;
			}
			Swap1.As(&SC->SwapChain);

			CreateSwapChainRTVs(*SC);
			SwapChains.push_back(SC.get());
			return SC;
		}

		TUniquePtr<ISKRHITexture2D> CreateTexture2D(const FSKRHITextureDesc& InDesc) override
		{
			D3D12_RESOURCE_DESC TD{};
			TD.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			TD.Width = std::max(1u, InDesc.Width);
			TD.Height = std::max(1u, InDesc.Height);
			TD.DepthOrArraySize = 1;
			TD.MipLevels = (UINT16)std::max(1u, InDesc.MipLevels);
			TD.Format = SKToDXGIFormat(InDesc.Format);
			TD.SampleDesc.Count = 1;
			TD.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
			TD.Flags = D3D12_RESOURCE_FLAG_NONE;
			if (InDesc.Flags & SK_Tex_RenderTarget)
			{
				TD.Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
			}

			D3D12_HEAP_PROPERTIES Heap{};
			Heap.Type = D3D12_HEAP_TYPE_DEFAULT;

			D3D12_CLEAR_VALUE Clear{};
			Clear.Format = TD.Format;
			Clear.Color[0] = 0;
			Clear.Color[1] = 0;
			Clear.Color[2] = 0;
			Clear.Color[3] = 0;

			ComPtr<ID3D12Resource> Res;
			const D3D12_RESOURCE_STATES InitState = SKInitialStateForTexture(InDesc);
			HRESULT Hr = Device->CreateCommittedResource(
				&Heap,
				D3D12_HEAP_FLAG_NONE,
				&TD,
				InitState,
				(InDesc.Flags & SK_Tex_RenderTarget) ? &Clear : nullptr,
				IID_PPV_ARGS(&Res));
			if (FAILED(Hr))
			{
				return nullptr;
			}

			auto Tex = std::make_unique<FSKD3D12Texture2D>(InDesc, Res);

			if (InDesc.Flags & SK_Tex_RenderTarget)
			{
				// Create a tiny RTV heap per texture (simple and safe).
				D3D12_DESCRIPTOR_HEAP_DESC Hd{};
				Hd.NumDescriptors = 1;
				Hd.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
				ComPtr<ID3D12DescriptorHeap> HeapRTV;
				if (SUCCEEDED(Device->CreateDescriptorHeap(&Hd, IID_PPV_ARGS(&HeapRTV))))
				{
					D3D12_CPU_DESCRIPTOR_HANDLE H = HeapRTV->GetCPUDescriptorHandleForHeapStart();
					Device->CreateRenderTargetView(Res.Get(), nullptr, H);
					TextureRTVHeaps[Tex.get()] = HeapRTV;
					Tex->SetRTV(H);
				}
			}

			return Tex;
		}

		bool ReadbackTexturePixelRGBA8(ISKRHITexture2D& Texture, uint32 X, uint32 Y, uint8 OutRGBA[4]) override
		{
			auto* Tex = dynamic_cast<FSKD3D12Texture2D*>(&Texture);
			if (!Tex || !OutRGBA)
			{
				return false;
			}

			ID3D12Resource* Src = Tex->GetNative();
			if (!Src)
			{
				return false;
			}

			// Create a readback buffer for one pixel (RGBA8) by copying a 1x1 region.
			const DXGI_FORMAT Fmt = SKToDXGIFormat(Tex->GetDesc().Format);
			if (Fmt != DXGI_FORMAT_R8G8B8A8_UNORM)
			{
				return false;
			}

			D3D12_RESOURCE_DESC TD = Src->GetDesc();

			D3D12_PLACED_SUBRESOURCE_FOOTPRINT Layout{};
			UINT NumRows = 0;
			UINT64 RowSizeInBytes = 0;
			UINT64 TotalBytes = 0;
			Device->GetCopyableFootprints(&TD, 0, 1, 0, &Layout, &NumRows, &RowSizeInBytes, &TotalBytes);

			D3D12_HEAP_PROPERTIES Heap{};
			Heap.Type = D3D12_HEAP_TYPE_READBACK;

			D3D12_RESOURCE_DESC BD{};
			BD.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
			BD.Width = TotalBytes;
			BD.Height = 1;
			BD.DepthOrArraySize = 1;
			BD.MipLevels = 1;
			BD.SampleDesc.Count = 1;
			BD.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

			ComPtr<ID3D12Resource> Readback;
			if (FAILED(Device->CreateCommittedResource(&Heap, D3D12_HEAP_FLAG_NONE, &BD, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&Readback))))
			{
				return false;
			}

			// Copy full subresource then sample pixel (simple, not optimal; ok for selection readback).
			D3D12_TEXTURE_COPY_LOCATION Dst{};
			Dst.pResource = Readback.Get();
			Dst.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
			Dst.PlacedFootprint = Layout;

			D3D12_TEXTURE_COPY_LOCATION SrcLoc{};
			SrcLoc.pResource = Src;
			SrcLoc.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
			SrcLoc.SubresourceIndex = 0;

			CmdList->CopyTextureRegion(&Dst, 0, 0, 0, &SrcLoc, nullptr);

			// Execute + wait
			CmdList->Close();
			ID3D12CommandList* Lists[] = { CmdList.Get() };
			Queue->ExecuteCommandLists(1, Lists);
			WaitGPU();

			// Re-open for further commands.
			Allocator->Reset();
			CmdList->Reset(Allocator.Get(), nullptr);
			Immediate.CmdList = CmdList.Get();

			void* Mapped = nullptr;
			D3D12_RANGE R{ 0, (SIZE_T)TotalBytes };
			if (FAILED(Readback->Map(0, &R, &Mapped)) || !Mapped)
			{
				return false;
			}

			const uint32 W = Tex->GetDesc().Width;
			const uint32 H = Tex->GetDesc().Height;
			X = std::min(X, std::max(1u, W) - 1u);
			Y = std::min(Y, std::max(1u, H) - 1u);

			const uint8* Base = (const uint8*)Mapped;
			const SIZE_T RowPitch = (SIZE_T)Layout.Footprint.RowPitch;
			const SIZE_T Offset = (SIZE_T)Y * RowPitch + (SIZE_T)X * 4u;
			OutRGBA[0] = Base[Offset + 0];
			OutRGBA[1] = Base[Offset + 1];
			OutRGBA[2] = Base[Offset + 2];
			OutRGBA[3] = Base[Offset + 3];

			Readback->Unmap(0, nullptr);
			return true;
		}

		ESKRHIApi GetApi() const override { return ESKRHIApi::D3D12; }

		ID3D12Device* GetNativeDevice() const { return Device.Get(); }
		ID3D12GraphicsCommandList* GetCmdList() const { return CmdList.Get(); }

		FSKD3D12LinePipeline& GetLinePipeline() { return Line; }
		FSKD3D12TrianglePipeline& GetTrianglePipeline() { return Triangle; }
		FSKD3D12UploadRing& GetUpload() { return Upload; }
		ISKShaderCompiler* GetShaderCompiler() const { return ShaderCompiler.get(); }
		FSKShaderBytecodeCache* GetShaderCache() const { return ShaderCache.get(); }

		void EnsureLinePipeline()
		{
			if (Line.bReady)
			{
				return;
			}

			if (!ShaderCompiler)
			{
				return;
			}

			// Compile or load cached bytecode.
			FSKShaderCompileRequest VSReq;
			VSReq.Language = ESKShaderLanguage::Hlsl;
			VSReq.Stage = ESKShaderStage::Vertex;
			VSReq.EntryPoint = "VSMain";
			VSReq.Profile = "vs_5_0";
			VSReq.DebugName = "SkylarkLineVS";
			VSReq.Source.Code =
				"struct VSIn { float4 Pos : POSITION; float4 Color : COLOR0; };"
				"struct PSIn { float4 Pos : SV_POSITION; float4 Color : COLOR0; };"
				"PSIn VSMain(VSIn v){ PSIn o; o.Pos=v.Pos; o.Color=v.Color; return o; }";

			FSKShaderCompileRequest PSReq;
			PSReq.Language = ESKShaderLanguage::Hlsl;
			PSReq.Stage = ESKShaderStage::Pixel;
			PSReq.EntryPoint = "PSMain";
			PSReq.Profile = "ps_5_0";
			PSReq.DebugName = "SkylarkLinePS";
			PSReq.Source.Code =
				"struct PSIn { float4 Pos : SV_POSITION; float4 Color : COLOR0; };"
				"float4 PSMain(PSIn i) : SV_Target0 { return i.Color; }";

			TArray<uint8> VSBytes;
			TArray<uint8> PSBytes;

			const FSKHash64 VSKey = SKComputeShaderKey(VSReq);
			const FSKHash64 PSKey = SKComputeShaderKey(PSReq);

			bool bVSHit = ShaderCache && ShaderCache->Get(VSKey, VSBytes);
			bool bPSHit = ShaderCache && ShaderCache->Get(PSKey, PSBytes);

			if (!bVSHit)
			{
				FSKShaderCompileOutput Out;
				if (!ShaderCompiler->Compile(VSReq, Out) || !Out.bSucceeded)
				{
					SK_LOG(GLogSkylark, ESKLogVerbosity::Error, "D3D12: line VS compile failed: %s", Out.Errors.c_str());
					return;
				}
				VSBytes = Out.Bytecode;
				if (ShaderCache) ShaderCache->Put(VSKey, VSBytes);
			}

			if (!bPSHit)
			{
				FSKShaderCompileOutput Out;
				if (!ShaderCompiler->Compile(PSReq, Out) || !Out.bSucceeded)
				{
					SK_LOG(GLogSkylark, ESKLogVerbosity::Error, "D3D12: line PS compile failed: %s", Out.Errors.c_str());
					return;
				}
				PSBytes = Out.Bytecode;
				if (ShaderCache) ShaderCache->Put(PSKey, PSBytes);
			}

			// Root signature: empty
			D3D12_ROOT_SIGNATURE_DESC RS{};
			RS.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
			ComPtr<ID3DBlob> Sig;
			ComPtr<ID3DBlob> Err;
			if (FAILED(D3D12SerializeRootSignature(&RS, D3D_ROOT_SIGNATURE_VERSION_1, &Sig, &Err)))
			{
				SK_LOG(GLogSkylark, ESKLogVerbosity::Error, "D3D12: SerializeRootSignature failed");
				return;
			}
			if (FAILED(Device->CreateRootSignature(0, Sig->GetBufferPointer(), Sig->GetBufferSize(), IID_PPV_ARGS(&Line.RootSig))))
			{
				SK_LOG(GLogSkylark, ESKLogVerbosity::Error, "D3D12: CreateRootSignature failed");
				return;
			}

			D3D12_INPUT_ELEMENT_DESC Layout[] = {
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
				{ "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM,    0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			};

			D3D12_GRAPHICS_PIPELINE_STATE_DESC Pso{};
			Pso.pRootSignature = Line.RootSig.Get();
			Pso.VS = { VSBytes.data(), VSBytes.size() };
			Pso.PS = { PSBytes.data(), PSBytes.size() };
			D3D12_BLEND_DESC Blend{};
			Blend.AlphaToCoverageEnable = FALSE;
			Blend.IndependentBlendEnable = FALSE;
			auto& RT0 = Blend.RenderTarget[0];
			RT0.BlendEnable = TRUE;
			RT0.LogicOpEnable = FALSE;
			RT0.SrcBlend = D3D12_BLEND_SRC_ALPHA;
			RT0.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
			RT0.BlendOp = D3D12_BLEND_OP_ADD;
			RT0.SrcBlendAlpha = D3D12_BLEND_ONE;
			RT0.DestBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA;
			RT0.BlendOpAlpha = D3D12_BLEND_OP_ADD;
			RT0.LogicOp = D3D12_LOGIC_OP_NOOP;
			RT0.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			Pso.BlendState = Blend;

			D3D12_RASTERIZER_DESC Rast{};
			Rast.FillMode = D3D12_FILL_MODE_SOLID;
			Rast.CullMode = D3D12_CULL_MODE_NONE;
			Rast.FrontCounterClockwise = FALSE;
			Rast.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
			Rast.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
			Rast.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
			Rast.DepthClipEnable = TRUE;
			Rast.MultisampleEnable = FALSE;
			Rast.AntialiasedLineEnable = TRUE;
			Rast.ForcedSampleCount = 0;
			Rast.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
			Pso.RasterizerState = Rast;

			D3D12_DEPTH_STENCIL_DESC DS{};
			DS.DepthEnable = FALSE;
			DS.StencilEnable = FALSE;
			DS.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
			DS.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
			DS.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
			DS.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
			Pso.DepthStencilState = DS;
			Pso.SampleMask = UINT_MAX;
			Pso.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
			Pso.NumRenderTargets = 1;
			Pso.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
			Pso.SampleDesc.Count = 1;
			Pso.InputLayout = { Layout, sizeof(Layout)/sizeof(Layout[0]) };

			if (FAILED(Device->CreateGraphicsPipelineState(&Pso, IID_PPV_ARGS(&Line.PSO))))
			{
				SK_LOG(GLogSkylark, ESKLogVerbosity::Error, "D3D12: CreateGraphicsPipelineState(line) failed");
				return;
			}

			Line.bReady = true;
		}


		void EnsureTrianglePipeline()
		{
			if (Triangle.bReady)
			{
				return;
			}

			if (!ShaderCompiler)
			{
				return;
			}

			FSKShaderCompileRequest VSReq;
			VSReq.Language = ESKShaderLanguage::Hlsl;
			VSReq.Stage = ESKShaderStage::Vertex;
			VSReq.EntryPoint = "VSMain";
			VSReq.Profile = "vs_5_0";
			VSReq.DebugName = "SkylarkTriangleVS";
			VSReq.Source.Code =
				"struct VSIn {"
				"  float4 Pos : POSITION;"
				"  float4 Color : COLOR0;"
				"  float4 InstRow0 : INSTANCE0;"
				"  float4 InstRow1 : INSTANCE1;"
				"  float4 InstRow2 : INSTANCE2;"
				"  float4 InstRow3 : INSTANCE3;"
				"  float4 InstColor : COLOR1;"
				"};"
				"struct PSIn { float4 Pos : SV_POSITION; float4 Color : COLOR0; };"
				"PSIn VSMain(VSIn v){"
				"  PSIn o;"
				"  float4x4 m = float4x4(v.InstRow0, v.InstRow1, v.InstRow2, v.InstRow3);"
				"  o.Pos = mul(m, v.Pos);"
				"  o.Color = v.Color * v.InstColor;"
				"  return o;"
				"}";

			FSKShaderCompileRequest PSReq;
			PSReq.Language = ESKShaderLanguage::Hlsl;
			PSReq.Stage = ESKShaderStage::Pixel;
			PSReq.EntryPoint = "PSMain";
			PSReq.Profile = "ps_5_0";
			PSReq.DebugName = "SkylarkTrianglePS";
			PSReq.Source.Code =
				"struct PSIn { float4 Pos : SV_POSITION; float4 Color : COLOR0; };"
				"float4 PSMain(PSIn i) : SV_Target0 { return i.Color; }";

			TArray<uint8> VSBytes;
			TArray<uint8> PSBytes;
			const FSKHash64 VSKey = SKComputeShaderKey(VSReq);
			const FSKHash64 PSKey = SKComputeShaderKey(PSReq);
			bool bVSHit = ShaderCache && ShaderCache->Get(VSKey, VSBytes);
			bool bPSHit = ShaderCache && ShaderCache->Get(PSKey, PSBytes);

			if (!bVSHit)
			{
				FSKShaderCompileOutput Out;
				if (!ShaderCompiler->Compile(VSReq, Out) || !Out.bSucceeded)
				{
					SK_LOG(GLogSkylark, ESKLogVerbosity::Error, "D3D12: triangle VS compile failed: %s", Out.Errors.c_str());
					return;
				}
				VSBytes = Out.Bytecode;
				if (ShaderCache) ShaderCache->Put(VSKey, VSBytes);
			}

			if (!bPSHit)
			{
				FSKShaderCompileOutput Out;
				if (!ShaderCompiler->Compile(PSReq, Out) || !Out.bSucceeded)
				{
					SK_LOG(GLogSkylark, ESKLogVerbosity::Error, "D3D12: triangle PS compile failed: %s", Out.Errors.c_str());
					return;
				}
				PSBytes = Out.Bytecode;
				if (ShaderCache) ShaderCache->Put(PSKey, PSBytes);
			}

			D3D12_ROOT_SIGNATURE_DESC RS{};
			RS.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
			ComPtr<ID3DBlob> Sig;
			ComPtr<ID3DBlob> Err;
			if (FAILED(D3D12SerializeRootSignature(&RS, D3D_ROOT_SIGNATURE_VERSION_1, &Sig, &Err)))
			{
				SK_LOG(GLogSkylark, ESKLogVerbosity::Error, "D3D12: SerializeRootSignature(triangle) failed");
				return;
			}
			if (FAILED(Device->CreateRootSignature(0, Sig->GetBufferPointer(), Sig->GetBufferSize(), IID_PPV_ARGS(&Triangle.RootSig))))
			{
				SK_LOG(GLogSkylark, ESKLogVerbosity::Error, "D3D12: CreateRootSignature(triangle) failed");
				return;
			}

			D3D12_INPUT_ELEMENT_DESC Layout[] = {
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,  0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,   0 },
				{ "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM,     0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,   0 },
				{ "INSTANCE", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,  0, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 },
				{ "INSTANCE", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 },
				{ "INSTANCE", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 },
				{ "INSTANCE", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 },
				{ "COLOR",    1, DXGI_FORMAT_R8G8B8A8_UNORM,     1, 64, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 },
			};

			D3D12_GRAPHICS_PIPELINE_STATE_DESC Pso{};
			Pso.pRootSignature = Triangle.RootSig.Get();
			Pso.VS = { VSBytes.data(), VSBytes.size() };
			Pso.PS = { PSBytes.data(), PSBytes.size() };

			D3D12_BLEND_DESC Blend{};
			auto& RT0 = Blend.RenderTarget[0];
			RT0.BlendEnable = TRUE;
			RT0.LogicOpEnable = FALSE;
			RT0.SrcBlend = D3D12_BLEND_SRC_ALPHA;
			RT0.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
			RT0.BlendOp = D3D12_BLEND_OP_ADD;
			RT0.SrcBlendAlpha = D3D12_BLEND_ONE;
			RT0.DestBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA;
			RT0.BlendOpAlpha = D3D12_BLEND_OP_ADD;
			RT0.LogicOp = D3D12_LOGIC_OP_NOOP;
			RT0.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			Pso.BlendState = Blend;

			D3D12_RASTERIZER_DESC Rast{};
			Rast.FillMode = D3D12_FILL_MODE_SOLID;
			Rast.CullMode = D3D12_CULL_MODE_NONE;
			Rast.FrontCounterClockwise = FALSE;
			Rast.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
			Rast.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
			Rast.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
			Rast.DepthClipEnable = TRUE;
			Rast.MultisampleEnable = FALSE;
			Rast.AntialiasedLineEnable = FALSE;
			Rast.ForcedSampleCount = 0;
			Rast.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
			Pso.RasterizerState = Rast;

			D3D12_DEPTH_STENCIL_DESC DS{};
			DS.DepthEnable = FALSE;
			DS.StencilEnable = FALSE;
			DS.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
			DS.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
			Pso.DepthStencilState = DS;

			Pso.SampleMask = UINT_MAX;
			Pso.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			Pso.NumRenderTargets = 1;
			Pso.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
			Pso.SampleDesc.Count = 1;
			Pso.InputLayout = { Layout, sizeof(Layout) / sizeof(Layout[0]) };

			if (FAILED(Device->CreateGraphicsPipelineState(&Pso, IID_PPV_ARGS(&Triangle.PSO))))
			{
				SK_LOG(GLogSkylark, ESKLogVerbosity::Error, "D3D12: CreateGraphicsPipelineState(triangle) failed");
				return;
			}

			Triangle.bReady = true;
		}

		void CreateSwapChainRTVs(FSKD3D12SwapChain& SC)
		{
			SC.BackBuffers.clear();
			SC.RTVHandles.clear();

			D3D12_DESCRIPTOR_HEAP_DESC Hd{};
			Hd.NumDescriptors = SC.Desc.BufferCount;
			Hd.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			ComPtr<ID3D12DescriptorHeap> Heap;
			if (FAILED(Device->CreateDescriptorHeap(&Hd, IID_PPV_ARGS(&Heap))))
			{
				return;
			}
			SC.RtvHeap = Heap;

			const UINT Stride = Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
			D3D12_CPU_DESCRIPTOR_HANDLE Base = Heap->GetCPUDescriptorHandleForHeapStart();
			SC.RTVHandles.resize(SC.Desc.BufferCount);
			SC.BackBuffers.resize(SC.Desc.BufferCount);

			for (UINT i = 0; i < SC.Desc.BufferCount; ++i)
			{
				ComPtr<ID3D12Resource> BB;
				SC.SwapChain->GetBuffer(i, IID_PPV_ARGS(&BB));
				D3D12_CPU_DESCRIPTOR_HANDLE H = Base;
				H.ptr += SIZE_T(i) * SIZE_T(Stride);
				Device->CreateRenderTargetView(BB.Get(), nullptr, H);
				SC.RTVHandles[i] = H;
				SC.BackBuffers[i] = BB;
			}
		}

		void WaitGPU()
		{
			if (!Queue || !Fence) return;
			const UINT64 V = ++FenceValue;
			Queue->Signal(Fence.Get(), V);
			if (Fence->GetCompletedValue() < V)
			{
				Fence->SetEventOnCompletion(V, FenceEvent);
				WaitForSingleObject(FenceEvent, INFINITE);
			}
		}

	private:
		friend class FSKD3D12CommandList;
		friend class FSKD3D12SwapChain;

		FSKRHIDeviceDesc Desc{};
		ComPtr<IDXGIFactory6> Factory;
		ComPtr<ID3D12Device> Device;
		ComPtr<ID3D12CommandQueue> Queue;
		ComPtr<ID3D12CommandAllocator> Allocator;
		ComPtr<ID3D12GraphicsCommandList> CmdList;
		ComPtr<ID3D12Fence> Fence;
		UINT64 FenceValue = 0;
		HANDLE FenceEvent = nullptr;
		bool bSubmittedByPresent = false;

		FSKD3D12CommandList Immediate;

		TArray<FSKD3D12SwapChain*> SwapChains;
		TMap<const FSKD3D12Texture2D*, ComPtr<ID3D12DescriptorHeap>> TextureRTVHeaps;

		TUniquePtr<ISKShaderCompiler> ShaderCompiler;
		TUniquePtr<FSKShaderBytecodeCache> ShaderCache;

		FSKD3D12UploadRing Upload;
		FSKD3D12LinePipeline Line;
		FSKD3D12TrianglePipeline Triangle;
	};

	// ------------------------------------------------------------
	// SwapChain Present (needs command list for barrier)
	// ------------------------------------------------------------
	void FSKD3D12SwapChain::Present()
	{
		if (!Owner || !SwapChain)
		{
			return;
		}

		// Transition current buffer to PRESENT before Present().
		const UINT Index = SwapChain->GetCurrentBackBufferIndex();
		if (Index < BackBuffers.size() && BackBuffers[Index])
		{
			D3D12_RESOURCE_BARRIER B{};
			B.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			B.Transition.pResource = BackBuffers[Index].Get();
			B.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			B.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
			B.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
			Owner->CmdList->ResourceBarrier(1, &B);
		}

		if (Owner->CmdList)
		{
			if (SUCCEEDED(Owner->CmdList->Close()))
			{
				ID3D12CommandList* Lists[] = { Owner->CmdList.Get() };
				Owner->Queue->ExecuteCommandLists(1, Lists);
				Owner->bSubmittedByPresent = true;
			}
		}

		SwapChain->Present(Desc.bVSync ? 1 : 0, 0);
		Owner->WaitGPU();
	}

	// ------------------------------------------------------------
	// CommandList impl
	// ------------------------------------------------------------
	void FSKD3D12CommandList::SetSwapChainRenderTarget(ISKRHISwapChain& InSwapChain)
	{
		BoundTexture = nullptr;
		BoundSwapChain = dynamic_cast<FSKD3D12SwapChain*>(&InSwapChain);
		if (!CmdList || !BoundSwapChain || !BoundSwapChain->SwapChain)
		{
			return;
		}

		const UINT Index = BoundSwapChain->SwapChain->GetCurrentBackBufferIndex();
		if (Index >= BoundSwapChain->RTVHandles.size())
		{
			return;
		}

		BoundRTV = BoundSwapChain->RTVHandles[Index];
		BoundRTVWidth = std::max(1u, BoundSwapChain->Desc.Width);
		BoundRTVHeight = std::max(1u, BoundSwapChain->Desc.Height);

		// Barrier PRESENT -> RENDER_TARGET
		if (Index < BoundSwapChain->BackBuffers.size() && BoundSwapChain->BackBuffers[Index])
		{
			D3D12_RESOURCE_BARRIER B{};
			B.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			B.Transition.pResource = BoundSwapChain->BackBuffers[Index].Get();
			B.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			B.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
			B.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
			CmdList->ResourceBarrier(1, &B);
		}

		CmdList->OMSetRenderTargets(1, &BoundRTV, FALSE, nullptr);

		D3D12_VIEWPORT VP{};
		VP.TopLeftX = 0;
		VP.TopLeftY = 0;
		VP.Width = (float)BoundRTVWidth;
		VP.Height = (float)BoundRTVHeight;
		VP.MinDepth = 0.0f;
		VP.MaxDepth = 1.0f;
		CmdList->RSSetViewports(1, &VP);

		D3D12_RECT Sc{};
		Sc.left = 0;
		Sc.top = 0;
		Sc.right = (LONG)BoundRTVWidth;
		Sc.bottom = (LONG)BoundRTVHeight;
		CmdList->RSSetScissorRects(1, &Sc);
	}

	void FSKD3D12CommandList::SetRenderTargetTexture(ISKRHITexture2D& ColorTarget)
	{
		BoundSwapChain = nullptr;
		BoundTexture = dynamic_cast<FSKD3D12Texture2D*>(&ColorTarget);
		if (!CmdList || !BoundTexture || !BoundTexture->HasRTV())
		{
			return;
		}
		BoundRTV = BoundTexture->GetRTV();
		BoundRTVWidth = std::max(1u, BoundTexture->GetDesc().Width);
		BoundRTVHeight = std::max(1u, BoundTexture->GetDesc().Height);

		CmdList->OMSetRenderTargets(1, &BoundRTV, FALSE, nullptr);

		D3D12_VIEWPORT VP{};
		VP.TopLeftX = 0;
		VP.TopLeftY = 0;
		VP.Width = (float)BoundRTVWidth;
		VP.Height = (float)BoundRTVHeight;
		VP.MinDepth = 0.0f;
		VP.MaxDepth = 1.0f;
		CmdList->RSSetViewports(1, &VP);

		D3D12_RECT Sc{};
		Sc.left = 0;
		Sc.top = 0;
		Sc.right = (LONG)BoundRTVWidth;
		Sc.bottom = (LONG)BoundRTVHeight;
		CmdList->RSSetScissorRects(1, &Sc);
	}

	void FSKD3D12CommandList::ClearRenderTarget(const FSKRHIClearColor& Color)
	{
		if (!CmdList || BoundRTV.ptr == 0)
		{
			return;
		}
		const float C[4] = { Color.R, Color.G, Color.B, Color.A };
		CmdList->ClearRenderTargetView(BoundRTV, C, 0, nullptr);
	}

	void FSKD3D12CommandList::DrawLineList(const FSKRHILineVertex* Vertices, uint32 VertexCount, const FSKRHILineDrawParams& Params)
	{
		(void)Params;
		if (!CmdList || !Owner || !Vertices || VertexCount < 2)
		{
			return;
		}

		Owner->EnsureLinePipeline();
		auto& Pipe = Owner->GetLinePipeline();
		if (!Pipe.bReady)
		{
			return;
		}

		const SIZE_T Bytes = (SIZE_T)VertexCount * sizeof(FSKRHILineVertex);
		auto Alloc = Owner->GetUpload().Allocate(Bytes, 16);
		if (!Alloc.Cpu || !Owner->GetUpload().GetResource())
		{
			return;
		}

		std::memcpy(Alloc.Cpu, Vertices, Bytes);

		D3D12_VERTEX_BUFFER_VIEW VB{};
		VB.BufferLocation = Alloc.Gpu;
		VB.SizeInBytes = (UINT)Bytes;
		VB.StrideInBytes = sizeof(FSKRHILineVertex);

		CmdList->SetGraphicsRootSignature(Pipe.RootSig.Get());
		CmdList->SetPipelineState(Pipe.PSO.Get());
		CmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
		CmdList->IASetVertexBuffers(0, 1, &VB);
		CmdList->DrawInstanced(VertexCount, 1, 0, 0);
	}


	void FSKD3D12CommandList::DrawTriangleList(const FSKRHITriangleVertex* Vertices, uint32 VertexCount, const FSKRHITriangleDrawParams& Params)
	{
		if (!Vertices || VertexCount < 3)
		{
			return;
		}

		TArray<uint32> Indices;
		Indices.resize(VertexCount);
		for (uint32 Index = 0; Index < VertexCount; ++Index)
		{
			Indices[Index] = Index;
		}
		DrawIndexedTriangleList(Vertices, VertexCount, Indices.data(), VertexCount, Params);
	}

	void FSKD3D12CommandList::DrawIndexedTriangleList(const FSKRHITriangleVertex* Vertices, uint32 VertexCount, const uint32* Indices, uint32 IndexCount, const FSKRHITriangleDrawParams& Params)
	{
		FSKRHITriangleInstance Identity{};
		Identity.LocalToWorld = Params.bApplyTransform ? Params.Transform : FSKMatrix4f::Identity();
		Identity.TintRGBA8 = 0xFFFFFFFFu;
		DrawIndexedInstancedTriangleList(Vertices, VertexCount, Indices, IndexCount, &Identity, 1u, Params);
	}

	void FSKD3D12CommandList::DrawIndexedInstancedTriangleList(
		const FSKRHITriangleVertex* Vertices,
		uint32 VertexCount,
		const uint32* Indices,
		uint32 IndexCount,
		const FSKRHITriangleInstance* Instances,
		uint32 InstanceCount,
		const FSKRHITriangleDrawParams& Params)
	{
		(void)Params;
		if (!CmdList || !Owner || !Vertices || !Indices || !Instances || VertexCount == 0 || IndexCount < 3 || InstanceCount == 0 || BoundRTV.ptr == 0)
		{
			return;
		}

		Owner->EnsureTrianglePipeline();
		auto& Pipe = Owner->GetTrianglePipeline();
		if (!Pipe.bReady)
		{
			return;
		}

		const SIZE_T VertexBytes = (SIZE_T)VertexCount * sizeof(FSKRHITriangleVertex);
		const SIZE_T IndexBytes = (SIZE_T)IndexCount * sizeof(uint32);
		const SIZE_T InstanceBytes = (SIZE_T)InstanceCount * sizeof(FSKRHITriangleInstance);

		auto VertexAlloc = Owner->GetUpload().Allocate(VertexBytes, 16);
		auto IndexAlloc = Owner->GetUpload().Allocate(IndexBytes, 4);
		auto InstanceAlloc = Owner->GetUpload().Allocate(InstanceBytes, 16);
		if (!VertexAlloc.Cpu || !IndexAlloc.Cpu || !InstanceAlloc.Cpu || !Owner->GetUpload().GetResource())
		{
			return;
		}

		std::memcpy(VertexAlloc.Cpu, Vertices, VertexBytes);
		std::memcpy(IndexAlloc.Cpu, Indices, IndexBytes);
		std::memcpy(InstanceAlloc.Cpu, Instances, InstanceBytes);

		D3D12_VERTEX_BUFFER_VIEW VBs[2]{};
		VBs[0].BufferLocation = VertexAlloc.Gpu;
		VBs[0].SizeInBytes = (UINT)VertexBytes;
		VBs[0].StrideInBytes = sizeof(FSKRHITriangleVertex);
		VBs[1].BufferLocation = InstanceAlloc.Gpu;
		VBs[1].SizeInBytes = (UINT)InstanceBytes;
		VBs[1].StrideInBytes = sizeof(FSKRHITriangleInstance);

		D3D12_INDEX_BUFFER_VIEW IB{};
		IB.BufferLocation = IndexAlloc.Gpu;
		IB.SizeInBytes = (UINT)IndexBytes;
		IB.Format = DXGI_FORMAT_R32_UINT;

		CmdList->SetGraphicsRootSignature(Pipe.RootSig.Get());
		CmdList->SetPipelineState(Pipe.PSO.Get());
		CmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		CmdList->IASetVertexBuffers(0, 2, VBs);
		CmdList->IASetIndexBuffer(&IB);
		CmdList->DrawIndexedInstanced(IndexCount, InstanceCount, 0, 0, 0);
	}

	// ------------------------------------------------------------
	// Factory
	// ------------------------------------------------------------
	TUniquePtr<ISKRHIDevice> SKCreateD3D12RHIDevice()
	{
		return std::make_unique<FSKD3D12RHIDevice>();
	}
}

#else

namespace Skylark
{
	TUniquePtr<ISKRHIDevice> SKCreateD3D12RHIDevice()
	{
		return nullptr;
	}
}

#endif
