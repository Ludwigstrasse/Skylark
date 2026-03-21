#include "SKLineRender/SKLineRender.h"

#include "SKCore/SKAssert.h"

namespace Skylark
{
	FSKVector4f FSKLineRenderer::TransformToClip(const FSKVector3f& PWorld, const FSKViewMatrices& Matrices)
	{
		// Clip = World * View * Proj (row-major assumption).
		const FSKVector4f W(PWorld.X, PWorld.Y, PWorld.Z, 1.0f);
		const FSKVector4f V = SKTransformVector4(Matrices.View, W);
		const FSKVector4f C = SKTransformVector4(Matrices.Projection, V);
		return C;
	}

	void FSKLineRenderer::BuildEdgeLines(
		const FSKGeometryData& Geo,
		const FSKMatrix4f& LocalToWorld,
		const FSKEdgeRenderSettings& EdgeSettings,
		const FSKViewMatrices& Matrices,
		uint32 ColorRGBA8,
		TArray<FSKRHILineVertex>& Out)
	{
		const auto& Mesh = Geo.Mesh;
		const auto& Derived = Geo.Derived;

		for (SIZE_T SectionIndex = 0; SectionIndex < Mesh.Sections.size(); ++SectionIndex)
		{
			const auto& S = Mesh.Sections[SectionIndex];
			if (S.Vertices.empty())
			{
				continue;
			}

			const TArray<uint32>* LineIdx = nullptr;
			if (SectionIndex < Derived.SectionEdges.size())
			{
				const auto& E = Derived.SectionEdges[SectionIndex];
				switch (EdgeSettings.Display)
				{
				case FSKEdgeRenderSettings::EDisplay::Boundary: LineIdx = &E.Boundary; break;
				case FSKEdgeRenderSettings::EDisplay::Sharp:    LineIdx = &E.Sharp;    break;
				case FSKEdgeRenderSettings::EDisplay::All:
				default:                                       LineIdx = &E.All;      break;
				}
			}
			else if (SectionIndex < Derived.SectionWireframes.size())
			{
				LineIdx = &Derived.SectionWireframes[SectionIndex].LineIndices;
			}

			if (!LineIdx || LineIdx->size() < 2)
			{
				continue;
			}

			SK_CHECK((LineIdx->size() % 2) == 0);
			for (SIZE_T i = 0; i + 1 < LineIdx->size(); i += 2)
			{
				const uint32 I0 = (*LineIdx)[i + 0];
				const uint32 I1 = (*LineIdx)[i + 1];
				if (I0 >= S.Vertices.size() || I1 >= S.Vertices.size())
				{
					continue;
				}

				const auto& P0L = S.Vertices[I0].Position;
				const auto& P1L = S.Vertices[I1].Position;

				const FSKVector3f P0W = SKTransformPoint(LocalToWorld, P0L);
				const FSKVector3f P1W = SKTransformPoint(LocalToWorld, P1L);

				const auto C0 = TransformToClip(P0W, Matrices);
				const auto C1 = TransformToClip(P1W, Matrices);

				FSKRHILineVertex V0;
				V0.X = C0.X; V0.Y = C0.Y; V0.Z = C0.Z; V0.W = C0.W;
				V0.ColorRGBA8 = ColorRGBA8;

				FSKRHILineVertex V1;
				V1.X = C1.X; V1.Y = C1.Y; V1.Z = C1.Z; V1.W = C1.W;
				V1.ColorRGBA8 = ColorRGBA8;

				Out.push_back(V0);
				Out.push_back(V1);
			}
		}
	}

	void FSKLineRenderer::DrawEdges(ISKRHICommandList& Cmd, const FSKLineBuildInput& In)
	{
		if (!In.SceneView || !In.View)
		{
			return;
		}
		if (!In.SceneView->GeometryRegistry)
		{
			return;
		}

		TArray<FSKRHILineVertex> Vertices;
		Vertices.reserve(4096);

		// Non-instanced proxies
		for (const auto& Proxy : In.SceneView->VisibleProxies)
		{
			const FSKGeometryData* Geo = In.SceneView->GeometryRegistry->Find(Proxy.GeometryKey);
			if (!Geo)
			{
				continue;
			}
			BuildEdgeLines(*Geo, Proxy.LocalToWorld, In.EdgeSettings, In.View->Matrices, In.VisibleStyle.ColorRGBA8, Vertices);
		}

		// Instance batches
		for (const auto& Batch : In.SceneView->VisibleInstances)
		{
			const FSKGeometryData* Geo = In.SceneView->GeometryRegistry->Find(Batch.GeometryKey);
			if (!Geo)
			{
				continue;
			}
			for (const auto& LTW : Batch.InstanceLocalToWorld)
			{
				BuildEdgeLines(*Geo, LTW, In.EdgeSettings, In.View->Matrices, In.VisibleStyle.ColorRGBA8, Vertices);
			}
		}

		if (Vertices.empty())
		{
			return;
		}

		FSKRHILineDrawParams Params;
		Params.bDepthTest = !In.EdgeSettings.bOverlay;
		Params.Width = In.VisibleStyle.Width;
		Params.PatternMask = In.VisibleStyle.PatternMask;
		Params.PatternScale = In.VisibleStyle.PatternScale;

		Cmd.DrawLineList(Vertices.data(), (uint32)Vertices.size(), Params);
		Cmd.Flush();
	}


	void FSKLineRenderer::DrawEdgesForHitProxies(ISKRHICommandList& Cmd, const FSKLineBuildInput& In)
	{
		if (!In.SceneView || !In.View)
		{
			return;
		}
		if (!In.SceneView->GeometryRegistry)
		{
			return;
		}

		TArray<FSKRHILineVertex> Vertices;
		Vertices.reserve(4096);

		auto EncodeColor = [](uint32 HitProxyId) -> uint32
		{
			// Encode id into RGBA8.
			const uint8 R = (uint8)((HitProxyId >> 0) & 0xFF);
			const uint8 G = (uint8)((HitProxyId >> 8) & 0xFF);
			const uint8 B = (uint8)((HitProxyId >> 16) & 0xFF);
			const uint8 A = (uint8)((HitProxyId >> 24) & 0xFF);
			return SKPackRGBA8(R, G, B, A);
		};

		// Non-instanced proxies
		for (const auto& Proxy : In.SceneView->VisibleProxies)
		{
			const FSKGeometryData* Geo = In.SceneView->GeometryRegistry->Find(Proxy.GeometryKey);
			if (!Geo)
			{
				continue;
			}
			BuildEdgeLines(*Geo, Proxy.LocalToWorld, In.EdgeSettings, In.View->Matrices, EncodeColor(Proxy.HitProxyId), Vertices);
		}

		// Instance batches
		for (const auto& Batch : In.SceneView->VisibleInstances)
		{
			const FSKGeometryData* Geo = In.SceneView->GeometryRegistry->Find(Batch.GeometryKey);
			if (!Geo)
			{
				continue;
			}
			for (SIZE_T i = 0; i < Batch.InstanceLocalToWorld.size(); ++i)
			{
				const uint32 HitProxyId = (i < Batch.InstanceHitProxyId.size()) ? Batch.InstanceHitProxyId[i] : 0;
				BuildEdgeLines(*Geo, Batch.InstanceLocalToWorld[i], In.EdgeSettings, In.View->Matrices, EncodeColor(HitProxyId), Vertices);
			}
		}

		if (Vertices.empty())
		{
			return;
		}

		FSKRHILineDrawParams Params;
		Params.bDepthTest = false;
		Params.Width = 1.0f;
		Params.PatternMask = 0xFFFFFFFFu;
		Params.PatternScale = 1.0f;

		Cmd.DrawLineList(Vertices.data(), (uint32)Vertices.size(), Params);
		Cmd.Flush();
	}

}