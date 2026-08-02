#include "SKRenderer/SKTriangleSceneRenderer.h"

#include "SKGeometry/SKGeometryRegistry.h"
#include "SKRenderer/SKRenderBatching.h"

#include <algorithm>

namespace Skylark
{
	namespace
	{
		static uint32 PackRGBA8(uint8 R, uint8 G, uint8 B, uint8 A)
		{
			return (uint32)R | ((uint32)G << 8u) | ((uint32)B << 16u) | ((uint32)A << 24u);
		}

		static FSKMatrix4f MakeLocalToClip(const FSKMatrix4f& LocalToWorld, const FSKViewMatrices& Matrices)
		{
			return SKMatrixMultiply(Matrices.Projection, SKMatrixMultiply(Matrices.View, LocalToWorld));
		}

		static FSKRHITriangleInstance MakeInstance(const FSKMatrix4f& LocalToWorld, const FSKViewMatrices& Matrices, uint32 ColorRGBA8)
		{
			FSKRHITriangleInstance Result{};
			// The simple cross-backend triangle shader consumes the instance matrix as the
			// final clip transform.  The previous code stored LocalToWorld here, so any
			// instanced/proxy-batched draw sent world coordinates directly to SV_Position
			// and the CAD model disappeared on D3D/OpenGL/Vulkan-style paths.
			Result.LocalToWorld = MakeLocalToClip(LocalToWorld, Matrices);
			Result.TintRGBA8 = ColorRGBA8;
			return Result;
		}

		static uint32 MakeSectionColor(uint64 GeometryKey, SIZE_T SectionIndex, bool bXRay)
		{
			const uint8 Base = static_cast<uint8>(40u + (GeometryKey * 17u + SectionIndex * 37u) % 120u);
			const uint8 R = static_cast<uint8>(std::min<int>(255, Base + 50));
			const uint8 G = static_cast<uint8>(std::min<int>(255, Base + 70));
			const uint8 B = static_cast<uint8>(std::min<int>(255, Base + 90));
			const uint8 A = bXRay ? 96u : 255u;
			return PackRGBA8(R, G, B, A);
		}

		static uint32 MakeIdColor(uint32 HitProxyId)
		{
			return PackRGBA8(
				static_cast<uint8>((HitProxyId >> 0) & 0xFFu),
				static_cast<uint8>((HitProxyId >> 8) & 0xFFu),
				static_cast<uint8>((HitProxyId >> 16) & 0xFFu),
				static_cast<uint8>((HitProxyId >> 24) & 0xFFu));
		}

		static bool ShouldDrawSection(SIZE_T SectionIndex, int32 RequestedSectionIndex)
		{
			return RequestedSectionIndex < 0 || static_cast<SIZE_T>(RequestedSectionIndex) == SectionIndex;
		}

		static void BuildLocalSpaceSection(
			const FSKMeshSection& Section,
			uint32 ColorRGBA8,
			TArray<FSKRHITriangleVertex>& OutVertices,
			TArray<uint32>& OutIndices)
		{
			OutVertices.clear();
			OutIndices.clear();

			if (Section.Vertices.empty() || Section.Indices.size() < 3)
			{
				return;
			}

			OutVertices.reserve(Section.Vertices.size());
			OutIndices.reserve(Section.Indices.size());

			for (const auto& Vertex : Section.Vertices)
			{
				FSKRHITriangleVertex OutVertex;
				OutVertex.X = Vertex.Position.X;
				OutVertex.Y = Vertex.Position.Y;
				OutVertex.Z = Vertex.Position.Z;
				OutVertex.W = 1.0f;
				OutVertex.ColorRGBA8 = ColorRGBA8;
				OutVertices.push_back(OutVertex);
			}

			for (uint32 Index : Section.Indices)
			{
				OutIndices.push_back(Index);
			}
		}

		static FSKRHITriangleDrawParams MakeSolidDrawParams(ESKViewMode ViewMode)
		{
			FSKRHITriangleDrawParams Params;
			Params.bDepthTest = true;
			Params.bDepthWrite = (ViewMode != ESKViewMode::XRay);
			Params.bCullBackFace = false;
			Params.bAlphaBlend = (ViewMode == ESKViewMode::XRay);
			Params.bApplyTransform = true;
			Params.Transform = FSKMatrix4f::Identity();
			return Params;
		}

		static FSKRHITriangleDrawParams MakeSelectionDrawParams()
		{
			FSKRHITriangleDrawParams Params;
			Params.bDepthTest = true;
			Params.bDepthWrite = true;
			Params.bCullBackFace = false;
			Params.bAlphaBlend = false;
			Params.bApplyTransform = true;
			Params.Transform = FSKMatrix4f::Identity();
			return Params;
		}
	}

	void FSKTriangleSceneRenderer::DrawSolidScene(ISKRHICommandList& Cmd, const FSKTriangleSceneBuildInput& In)
	{
		if (!In.SceneView || !In.View || !In.SceneView->GeometryRegistry)
		{
			return;
		}

		const auto DrawParams = MakeSolidDrawParams(In.View->ViewMode);

		// CAD scenes frequently contain tens of thousands of draw items that reuse a
		// small set of source geometries/sections.  Do not expand every proxy into a
		// giant CPU-side clip-space vertex buffer each frame.  Batch by
		// (GeometryKey, SectionIndex) and submit one instanced draw per section.
		TArray<FSKInstancedGeometryBatch> GeometryBatches;
		FSKRenderBatching::BuildInstancedGeometryBatches(*In.SceneView, GeometryBatches);

		TArray<FSKRHITriangleVertex> LocalVertices;
		TArray<uint32> LocalIndices;
		TArray<FSKRHITriangleInstance> Instances;
		LocalVertices.reserve(4096);
		LocalIndices.reserve(8192);
		Instances.reserve(1024);

		bool bIssuedDraws = false;
		for (const auto& Batch : GeometryBatches)
		{
			const FSKGeometryData* Geo = In.SceneView->GeometryRegistry->Find(Batch.GeometryKey);
			if (!Geo || Batch.Instances.empty())
			{
				continue;
			}

			for (SIZE_T SectionIndex = 0; SectionIndex < Geo->Mesh.Sections.size(); ++SectionIndex)
			{
				if (!ShouldDrawSection(SectionIndex, Batch.SectionIndex))
				{
					continue;
				}

				const uint32 Color = MakeSectionColor(Geo->GeometryKey, SectionIndex, In.View->ViewMode == ESKViewMode::XRay);
				BuildLocalSpaceSection(Geo->Mesh.Sections[SectionIndex], Color, LocalVertices, LocalIndices);
				if (LocalVertices.empty() || LocalIndices.empty())
				{
					continue;
				}

				Instances.clear();
				Instances.reserve(Batch.Instances.size());
				for (const FSKInstancedVisibleItem& Item : Batch.Instances)
				{
					Instances.push_back(MakeInstance(Item.LocalToWorld, In.View->Matrices, 0xFFFFFFFFu));
				}

				Cmd.DrawIndexedInstancedTriangleList(
					LocalVertices.data(), static_cast<uint32>(LocalVertices.size()),
					LocalIndices.data(), static_cast<uint32>(LocalIndices.size()),
					Instances.data(), static_cast<uint32>(Instances.size()),
					DrawParams);
				bIssuedDraws = true;
			}
		}

		if (bIssuedDraws)
		{
			Cmd.Flush();
		}
	}

	void FSKTriangleSceneRenderer::DrawSelectionScene(ISKRHICommandList& Cmd, const FSKTriangleSceneBuildInput& In)
	{
		if (!In.SceneView || !In.View || !In.SceneView->GeometryRegistry)
		{
			return;
		}

		const auto DrawParams = MakeSelectionDrawParams();

		TArray<FSKInstancedGeometryBatch> GeometryBatches;
		FSKRenderBatching::BuildInstancedGeometryBatches(*In.SceneView, GeometryBatches);

		TArray<FSKRHITriangleVertex> LocalVertices;
		TArray<uint32> LocalIndices;
		TArray<FSKRHITriangleInstance> Instances;
		LocalVertices.reserve(4096);
		LocalIndices.reserve(8192);
		Instances.reserve(1024);

		bool bIssuedDraws = false;
		for (const auto& Batch : GeometryBatches)
		{
			const FSKGeometryData* Geo = In.SceneView->GeometryRegistry->Find(Batch.GeometryKey);
			if (!Geo || Batch.Instances.empty())
			{
				continue;
			}

			for (SIZE_T SectionIndex = 0; SectionIndex < Geo->Mesh.Sections.size(); ++SectionIndex)
			{
				if (!ShouldDrawSection(SectionIndex, Batch.SectionIndex))
				{
					continue;
				}

				const FSKMeshSection& Section = Geo->Mesh.Sections[SectionIndex];
				BuildLocalSpaceSection(Section, 0xFFFFFFFFu, LocalVertices, LocalIndices);
				if (LocalVertices.empty() || LocalIndices.empty())
				{
					continue;
				}

				Instances.clear();
				Instances.reserve(Batch.Instances.size());
				for (const FSKInstancedVisibleItem& Item : Batch.Instances)
				{
					const uint32 HitProxyId = (Section.HitProxyId != 0u) ? Section.HitProxyId : Item.HitProxyId;
					Instances.push_back(MakeInstance(Item.LocalToWorld, In.View->Matrices, MakeIdColor(HitProxyId)));
				}

				Cmd.DrawIndexedInstancedTriangleList(
					LocalVertices.data(), static_cast<uint32>(LocalVertices.size()),
					LocalIndices.data(), static_cast<uint32>(LocalIndices.size()),
					Instances.data(), static_cast<uint32>(Instances.size()),
					DrawParams);
				bIssuedDraws = true;
			}
		}

		if (bIssuedDraws)
		{
			Cmd.Flush();
		}
	}
}
