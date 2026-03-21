#include "SKFieldViz/SKFieldViz.h"

#include "SKLineRender/SKLineRender.h"
#include "SKGeometry/SKGeometryRegistry.h"
#include "SKCore/SKAssert.h"

namespace Skylark
{
	static void SKAppendLegendRect(const FSKLegendDesc& L, uint32 Color, TArray<FSKRHILineVertex>& Out)
	{
		// Legend in NDC (-1..1). Convert normalized viewport rect to NDC.
		const float x0 = -1.0f + L.X * 2.0f;
		const float y0 = -1.0f + L.Y * 2.0f;
		const float x1 = -1.0f + (L.X + L.W) * 2.0f;
		const float y1 = -1.0f + (L.Y + L.H) * 2.0f;

		auto V = [&](float x, float y)
		{
			FSKRHILineVertex R;
			R.X = x; R.Y = y; R.Z = 0.0f; R.W = 1.0f;
			R.ColorRGBA8 = Color;
			return R;
		};

		// Rectangle border
		Out.push_back(V(x0, y0)); Out.push_back(V(x1, y0));
		Out.push_back(V(x1, y0)); Out.push_back(V(x1, y1));
		Out.push_back(V(x1, y1)); Out.push_back(V(x0, y1));
		Out.push_back(V(x0, y1)); Out.push_back(V(x0, y0));

		// Tick marks (simple)
		const int Ticks = 5;
		for (int i = 0; i <= Ticks; ++i)
		{
			const float t = (float)i / (float)Ticks;
			const float y = y0 + (y1 - y0) * t;
			Out.push_back(V(x1, y)); Out.push_back(V(x1 + 0.03f, y));
		}
	}

	void FSKFieldVizOverlay::DrawLegend(ISKRHICommandList& Cmd, const FSKLegendDesc& L, uint32 ColorRGBA8)
	{
		TArray<FSKRHILineVertex> Verts;
		Verts.reserve(64);
		SKAppendLegendRect(L, ColorRGBA8, Verts);

		FSKRHILineDrawParams Params;
		Params.bDepthTest = false;
		Params.Width = 1.0f;
		Cmd.DrawLineList(Verts.data(), (uint32)Verts.size(), Params);
	}

	void FSKFieldVizOverlay::Draw(ISKRHICommandList& Cmd,
		const FSKSceneView& SceneView,
		const FSKViewInfo& View,
		const FSKFieldRegistry* FieldRegistry,
		const FSKFieldVizParams& Params)
	{
		(void)View;

		// V9: legend scaffold (line primitives).
		DrawLegend(Cmd, Params.Legend, SKPackRGBA8(40, 40, 40, 255));

		// V9: deformation overlay (edge lines displaced by vector field), if available.
		if (!FieldRegistry || !SceneView.GeometryRegistry)
		{
			return;
		}

		const auto* GeoReg = SceneView.GeometryRegistry;
		const float S = Params.DeformScale;

		TArray<FSKRHILineVertex> Verts;
		Verts.reserve(8192);

		auto AppendDeformedEdges = [&](const FSKGeometryData& Geo, const FSKMatrix4f& LocalToWorld)
		{
			const FSKFieldSet* FS = FieldRegistry->FindFieldSet(Geo.GeometryKey);
			if (!FS)
			{
				return;
			}
			const FSKVectorField* Disp = FS->FindVector(Params.ActiveDeformField);
			if (!Disp || Disp->Values.empty())
			{
				return;
			}

			// Flattened vertex indexing
			uint32 Base = 0;
			for (SIZE_T SectionIndex = 0; SectionIndex < Geo.Mesh.Sections.size(); ++SectionIndex)
			{
				const auto& Sec = Geo.Mesh.Sections[SectionIndex];
				const auto& Edges = (SectionIndex < Geo.Derived.SectionEdges.size())
					? Geo.Derived.SectionEdges[SectionIndex].All
					: Geo.Derived.SectionWireframes[SectionIndex].LineIndices;

				if (Edges.size() < 2)
				{
					Base += (uint32)Sec.Vertices.size();
					continue;
				}

				for (SIZE_T i = 0; i + 1 < Edges.size(); i += 2)
				{
					const uint32 I0 = Edges[i + 0];
					const uint32 I1 = Edges[i + 1];
					if (I0 >= Sec.Vertices.size() || I1 >= Sec.Vertices.size())
					{
						continue;
					}

					const uint32 F0 = Base + I0;
					const uint32 F1 = Base + I1;
					if (F0 >= (uint32)Disp->Values.size() || F1 >= (uint32)Disp->Values.size())
					{
						continue;
					}

					const auto P0L = Sec.Vertices[I0].Position;
					const auto P1L = Sec.Vertices[I1].Position;

					const auto D0 = Disp->Values[F0];
					const auto D1 = Disp->Values[F1];

					const FSKVector3f P0W = SKTransformPoint(LocalToWorld, FSKVector3f(P0L.X + D0.X * S, P0L.Y + D0.Y * S, P0L.Z + D0.Z * S));
					const FSKVector3f P1W = SKTransformPoint(LocalToWorld, FSKVector3f(P1L.X + D1.X * S, P1L.Y + D1.Y * S, P1L.Z + D1.Z * S));

					const auto C0 = FSKLineRenderer::TransformToClip(P0W, View.Matrices);
					const auto C1 = FSKLineRenderer::TransformToClip(P1W, View.Matrices);

					FSKRHILineVertex V0;
					V0.X = C0.X; V0.Y = C0.Y; V0.Z = C0.Z; V0.W = C0.W;
					V0.ColorRGBA8 = SKPackRGBA8(30, 120, 255, 255);

					FSKRHILineVertex V1;
					V1.X = C1.X; V1.Y = C1.Y; V1.Z = C1.Z; V1.W = C1.W;
					V1.ColorRGBA8 = SKPackRGBA8(30, 120, 255, 255);

					Verts.push_back(V0);
					Verts.push_back(V1);
				}

				Base += (uint32)Sec.Vertices.size();
			}
		};

		for (const auto& Proxy : SceneView.VisibleProxies)
		{
			const FSKGeometryData* Geo = GeoReg->Find(Proxy.GeometryKey);
			if (!Geo) continue;
			AppendDeformedEdges(*Geo, Proxy.LocalToWorld);
		}
		for (const auto& Batch : SceneView.VisibleInstances)
		{
			const FSKGeometryData* Geo = GeoReg->Find(Batch.GeometryKey);
			if (!Geo) continue;
			for (const auto& LTW : Batch.InstanceLocalToWorld)
			{
				AppendDeformedEdges(*Geo, LTW);
			}
		}

		if (!Verts.empty())
		{
			FSKRHILineDrawParams P;
			P.bDepthTest = false;
			P.Width = 1.0f;
			Cmd.DrawLineList(Verts.data(), (uint32)Verts.size(), P);
		}
	}
}
