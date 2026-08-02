#include "SKRenderer/SKSelectionOverlayRenderer.h"

#include "SKGeometry/SKGeometryRegistry.h"
#include "SKLineRender/SKLineRender.h"
#include "SKRenderer/SKMeshRenderer.h"
#include "SKRenderer/SKEdgeDrawPacket.h"
#include "SKRenderer/SKPacketBuildUtils.h"
#include "SKRenderer/SKRenderScratchBuffer.h"
#include "SKRenderer/SKGpuResidentBufferCache.h"
#include "SKRenderer/SKSceneUploadQueue.h"
#include "SKRenderer/SKRenderBatching.h"

#include <algorithm>

namespace Skylark
{
    namespace
    {
        enum class EHighlightKind : uint8
        {
            None = 0,
            Hover,
            Selected,
        };

        static bool SameObjectIdentity(const FSKObjectId& A, const FSKObjectId& B)
        {
            if (!A.IsValid() || !B.IsValid()) return false;
            if (A.LegacyValue != 0 && B.LegacyValue != 0) return A.LegacyValue == B.LegacyValue;
            return A.DocumentGuid == B.DocumentGuid && A.ObjectGuid == B.ObjectGuid;
        }

        static bool MatchesQuery(const FSKObjectId& Owner, const FSKObjectId& Query)
        {
            if (!Owner.IsValid() || !Query.IsValid()) return false;
            if (!SameObjectIdentity(Owner, Query)) return false;
            if (Query.SubId == 0 || Query.Type == ESKObjectEntityType::Body || Query.Type == ESKObjectEntityType::Element || Query.Type == ESKObjectEntityType::Unknown)
            {
                return true;
            }
            return Owner.SubId == Query.SubId && (Owner.Type == Query.Type || Query.Type == ESKObjectEntityType::Unknown);
        }

        static EHighlightKind ResolveHighlightKind(const FSKObjectId& Owner, const FSKViewInfo& View)
        {
            if (!Owner.IsValid()) return EHighlightKind::None;
            if (!SKSelectionMaskMatches(Owner.Type, View.Selection.Style.HighlightEntityMask)) return EHighlightKind::None;

            if (View.Selection.Style.bShowSelection)
            {
                for (const auto& Selected : View.Selection.SelectedTopoIds)
                {
                    if (MatchesQuery(Owner, Selected)) return EHighlightKind::Selected;
                }
            }
            if (View.Selection.Style.bShowHover)
            {
                for (const auto& Hovered : View.Selection.HoveredTopoIds)
                {
                    if (MatchesQuery(Owner, Hovered)) return EHighlightKind::Hover;
                }
            }
            return EHighlightKind::None;
        }

        static uint32 OverrideAlpha(uint32 ColorRGBA8, float Alpha01)
        {
            const float Clamped = std::clamp(Alpha01, 0.0f, 1.0f);
            const uint32 A = static_cast<uint32>(Clamped * 255.0f + 0.5f);
            return (ColorRGBA8 & 0x00FFFFFFu) | (A << 24u);
        }

        static const ISKRHIResidentLineBuffer* GetOutlineBuffer(const FSKResidentEdgeSectionBuffers& Section)
        {
            if (Section.Sharp) return Section.Sharp.get();
            if (Section.Boundary) return Section.Boundary.get();
            if (Section.All) return Section.All.get();
            return nullptr;
        }

        static FSKRHIResidentTriangleDrawParams MakeFillParams(const FSKMatrix4f& LocalToWorld, const FSKViewInfo& View, uint32 FillColor)
        {
            FSKRHIResidentTriangleDrawParams Params;
            Params.LocalToWorld = LocalToWorld;
            Params.View = View.Matrices.View;
            Params.Projection = View.Matrices.Projection;
            Params.bDepthTest = true;
            Params.bDepthWrite = false;
            Params.bCullBackFace = false;
            Params.bAlphaBlend = true;
            Params.bUseOverrideColor = true;
            Params.OverrideColorRGBA8 = FillColor;
            return Params;
        }

        static FSKRHIResidentLineDrawParams MakeOutlineParams(const FSKMatrix4f& LocalToWorld, const FSKViewInfo& View, float Width, uint32 Color)
        {
            FSKRHIResidentLineDrawParams Params;
            Params.LocalToWorld = LocalToWorld;
            Params.View = View.Matrices.View;
            Params.Projection = View.Matrices.Projection;
            Params.bDepthTest = false;
            Params.Width = Width;
            Params.bUseOverrideColor = true;
            Params.OverrideColorRGBA8 = Color;
            return Params;
        }

        static void AppendOutlineFallbackFromCachedEdges(const FSKEdgeSectionDrawPacket& Section, const FSKMatrix4f& LocalToWorld, const FSKViewInfo& View, uint32 ColorRGBA8, TArray<FSKRHILineVertex>& Out)
        {
            const auto& Lines = !Section.Sharp.empty() ? Section.Sharp : (!Section.Boundary.empty() ? Section.Boundary : Section.All);
            if (Lines.empty())
            {
                return;
            }

            Out.reserve(Out.size() + Lines.size() * 2);
            for (const auto& Line : Lines)
            {
                const FSKVector3f P0W = SKTransformPoint(LocalToWorld, Line.P0Local);
                const FSKVector3f P1W = SKTransformPoint(LocalToWorld, Line.P1Local);
                const auto Clip0 = FSKLineRenderer::TransformToClip(P0W, View.Matrices);
                const auto Clip1 = FSKLineRenderer::TransformToClip(P1W, View.Matrices);
                FSKRHILineVertex V0; V0.X = Clip0.X; V0.Y = Clip0.Y; V0.Z = Clip0.Z; V0.W = Clip0.W; V0.ColorRGBA8 = ColorRGBA8;
                FSKRHILineVertex V1; V1.X = Clip1.X; V1.Y = Clip1.Y; V1.Z = Clip1.Z; V1.W = Clip1.W; V1.ColorRGBA8 = ColorRGBA8;
                Out.push_back(V0); Out.push_back(V1);
            }
        }
    }

    void FSKSelectionOverlayRenderer::DrawSelectionOverlay(ISKRHICommandList& Cmd, const FSKSceneView& SceneView, const FSKViewInfo& View)
    {
        if (!SceneView.GeometryRegistry || !View.Selection.Style.bEnable)
        {
            return;
        }

        auto& UploadQueue = FSKSceneUploadQueue::Get();
        UploadQueue.Reset();
        UploadQueue.QueueVisibleScene(SceneView, SK_SceneUpload_Mesh | SK_SceneUpload_Edge);
        UploadQueue.Resolve(Cmd, SceneView);

        TArray<FSKGeometryDrawBatch> GeometryBatches;
        FSKRenderBatching::BuildGeometryBatches(SceneView, GeometryBatches);

        auto& Scratch = FSKRenderScratchBuffer::GetTriangleScratch();
        Scratch.Reset();
        auto& LineScratch = FSKRenderScratchBuffer::GetLineScratch();
        LineScratch.Reset();
        auto& OutlineLines = LineScratch.Vertices;

        bool bIssuedResidentDraws = false;
        auto& GpuResident = FSKGpuResidentBufferCache::Get();

        for (const auto& GeometryBatch : GeometryBatches)
        {
            const FSKGeometryData* Geo = SceneView.GeometryRegistry->Find(GeometryBatch.GeometryKey);
            if (!Geo)
            {
                continue;
            }

            const FSKMeshDrawPacket& Packet = FSKMeshRenderer::GetResidencyCache().GetOrBuildMeshPacket(*Geo);
            const FSKEdgeDrawPacket& EdgePacket = FSKMeshRenderer::GetResidencyCache().GetOrBuildEdgePacket(*Geo);
            const auto& ResidentMesh = GpuResident.GetOrCreateMeshBuffers(Cmd, *Geo);
            const auto& ResidentEdges = GpuResident.GetOrCreateEdgeBuffers(Cmd, *Geo);

            for (const auto& Item : GeometryBatch.Items)
            {
                if (!Item.LocalToWorld)
                {
                    continue;
                }
                const FSKMatrix4f& LocalToWorld = *Item.LocalToWorld;

                for (SIZE_T SectionIndex = 0; SectionIndex < Packet.Sections.size(); ++SectionIndex)
                {
                    const auto& Section = Packet.Sections[SectionIndex];
                    const EHighlightKind Kind = ResolveHighlightKind(Section.OwnerId, View);
                    if (Kind == EHighlightKind::None)
                    {
                        continue;
                    }

                    const bool bSelected = (Kind == EHighlightKind::Selected);
                    const uint32 FillColor = bSelected
                        ? OverrideAlpha(View.Selection.Style.SelectedFillRGBA8, View.Selection.Style.SelectedFillAlpha)
                        : OverrideAlpha(View.Selection.Style.HoverFillRGBA8, View.Selection.Style.HoverFillAlpha);

                    if (View.Selection.Style.bTintFill)
                    {
                        if (ResidentMesh.bResidentSupported && SectionIndex < ResidentMesh.Sections.size() && ResidentMesh.Sections[SectionIndex].Buffer)
                        {
                            Cmd.DrawResidentTriangleBuffer(*ResidentMesh.Sections[SectionIndex].Buffer, MakeFillParams(LocalToWorld, View, FillColor));
                            bIssuedResidentDraws = true;
                        }
                        else
                        {
                            FSKPacketBuildUtils::AppendTintSectionIndexed(Section, LocalToWorld, View, FillColor, Scratch.Vertices, Scratch.Indices);
                        }
                    }

                    if (View.Selection.Style.bOutline)
                    {
                        if (ResidentEdges.bResidentSupported && SectionIndex < ResidentEdges.Sections.size())
                        {
                            if (const ISKRHIResidentLineBuffer* Outline = GetOutlineBuffer(ResidentEdges.Sections[SectionIndex]))
                            {
                                Cmd.DrawResidentLineBuffer(*Outline, MakeOutlineParams(LocalToWorld, View, View.Selection.Style.OutlineWidth, View.Selection.Style.OutlineRGBA8));
                                bIssuedResidentDraws = true;
                            }
                        }
                        else if (SectionIndex < EdgePacket.Sections.size())
                        {
                            AppendOutlineFallbackFromCachedEdges(EdgePacket.Sections[SectionIndex], LocalToWorld, View, View.Selection.Style.OutlineRGBA8, OutlineLines);
                        }
                    }
                }
            }
        }

        if (!Scratch.Vertices.empty() && !Scratch.Indices.empty())
        {
            FSKRHITriangleDrawParams FillParams;
            FillParams.bDepthTest = true;
            FillParams.bDepthWrite = false;
            FillParams.bCullBackFace = false;
            FillParams.bAlphaBlend = true;
            Cmd.DrawIndexedTriangleList(Scratch.Vertices.data(), (uint32)Scratch.Vertices.size(), Scratch.Indices.data(), (uint32)Scratch.Indices.size(), FillParams);
            bIssuedResidentDraws = true;
        }

        if (!OutlineLines.empty())
        {
            FSKRHILineDrawParams LineParams;
            LineParams.bDepthTest = false;
            LineParams.Width = View.Selection.Style.OutlineWidth;
            Cmd.DrawLineList(OutlineLines.data(), static_cast<uint32>(OutlineLines.size()), LineParams);
            bIssuedResidentDraws = true;
        }

        if (bIssuedResidentDraws)
        {
            Cmd.Flush();
        }
    }
}
