#include "SKRenderer/SKMeshRenderer.h"

#include "SKGeometry/SKGeometryRegistry.h"
#include "SKRenderer/SKPacketBuildUtils.h"
#include "SKRenderer/SKRenderScratchBuffer.h"
#include "SKRenderer/SKGeometryResidencyCache.h"
#include "SKRenderer/SKGpuResidentBufferCache.h"
#include "SKRenderer/SKSceneUploadQueue.h"
#include "SKRenderer/SKRenderBatching.h"
#include "SKRenderer/SKInstanceDrawPacket.h"
#include "SKRenderer/SKIndirectReadyPacket.h"

namespace Skylark
{
    namespace
    {
        static uint32 PackRGBA8(uint8 R, uint8 G, uint8 B, uint8 A)
        {
            return (uint32)R | ((uint32)G << 8u) | ((uint32)B << 16u) | ((uint32)A << 24u);
        }
    }

    FSKGeometryResidencyCache& FSKMeshRenderer::GetResidencyCache()
    {
        return FSKGeometryResidencyCache::Get();
    }

    namespace
    {
        static FSKRHIResidentTriangleDrawParams MakeResidentParams(const FSKMatrix4f& LocalToWorld, const FSKViewInfo& View, uint32 OverrideColorRGBA8, bool bUseOverrideColor)
        {
            FSKRHIResidentTriangleDrawParams Params;
            Params.LocalToWorld = LocalToWorld;
            Params.View = View.Matrices.View;
            Params.Projection = View.Matrices.Projection;
            Params.bDepthTest = true;
            Params.bDepthWrite = (View.ViewMode != ESKViewMode::XRay);
            Params.bCullBackFace = false;
            Params.bAlphaBlend = (View.ViewMode == ESKViewMode::XRay);
            Params.bUseOverrideColor = bUseOverrideColor;
            Params.OverrideColorRGBA8 = OverrideColorRGBA8;
            return Params;
        }

        static uint32 PackBaseColor(const FSKVector3f& C)
        {
            return PackRGBA8(
                (uint8)std::clamp((int)(C.X * 255.0f), 0, 255),
                (uint8)std::clamp((int)(C.Y * 255.0f), 0, 255),
                (uint8)std::clamp((int)(C.Z * 255.0f), 0, 255),
                255u);
        }

        static void AppendFallbackPacket(const FSKMeshDrawPacket& Packet, const FSKMatrix4f& LocalToWorld, const FSKViewInfo& View, TArray<FSKRHITriangleVertex>& OutVertices, TArray<uint32>& OutIndices)
        {
            for (const auto& Section : Packet.Sections)
            {
                FSKPacketBuildUtils::AppendShadedSectionIndexed(Section, LocalToWorld, View, OutVertices, OutIndices);
            }
        }

        static bool DrawResidentPacket(ISKRHICommandList& Cmd, const FSKResidentMeshGeometryBuffers& Resident, const FSKMatrix4f& LocalToWorld, const FSKViewInfo& View)
        {
            bool bDrawn = false;
            for (const auto& Section : Resident.Sections)
            {
                if (!Section.Buffer)
                {
                    continue;
                }

                const uint32 BaseColorRGBA8 = PackBaseColor(Section.BaseColor);
                const auto Params = MakeResidentParams(LocalToWorld, View, BaseColorRGBA8, true);
                Cmd.DrawResidentTriangleBuffer(*Section.Buffer, Params);
                bDrawn = true;
            }
            return bDrawn;
        }
    }

    void FSKMeshRenderer::DrawVisibleMeshes(ISKRHICommandList& Cmd, const FSKSceneView& SceneView, const FSKViewInfo& View)
    {
        if (!SceneView.GeometryRegistry)
        {
            return;
        }

        auto& UploadQueue = FSKSceneUploadQueue::Get();
        UploadQueue.Reset();
        UploadQueue.QueueVisibleScene(SceneView, SK_SceneUpload_Mesh);
        UploadQueue.Resolve(Cmd, SceneView);

        TArray<FSKInstancedGeometryBatch> InstancedBatches;
        FSKRenderBatching::BuildInstancedGeometryBatches(SceneView, InstancedBatches);

        TArray<FSKInstanceDrawRun> InstanceRuns;
        FSKInstanceDrawPacketBuilder::BuildRuns(InstancedBatches, InstanceRuns);

        auto& Scratch = FSKRenderScratchBuffer::GetTriangleScratch();
        Scratch.Reset();
        bool bIssuedDraws = false;
        auto& GpuResident = FSKGpuResidentBufferCache::Get();
        auto& IndirectPackets = FSKIndirectReadyPacketCache::Get();

        for (const auto& Run : InstanceRuns)
        {
            const FSKGeometryData* Geo = SceneView.GeometryRegistry->Find(Run.GeometryKey);
            if (!Geo)
            {
                continue;
            }

            const auto& Resident = GpuResident.GetOrCreateMeshBuffers(Cmd, *Geo);
            const FSKMeshDrawPacket* Packet = nullptr;
            if (!Resident.bResidentSupported)
            {
                Packet = &GetResidencyCache().GetOrBuildMeshPacket(*Geo);
            }

            if (Packet)
            {
                (void)IndirectPackets.GetOrBuild(*Geo, *Packet);
            }

            for (const auto& Item : Run.Items)
            {
                if (Resident.bResidentSupported && DrawResidentPacket(Cmd, Resident, Item.LocalToWorld, View))
                {
                    bIssuedDraws = true;
                    continue;
                }

                if (Packet)
                {
                    AppendFallbackPacket(*Packet, Item.LocalToWorld, View, Scratch.Vertices, Scratch.Indices);
                }
            }
        }

        if (!Scratch.Vertices.empty() && !Scratch.Indices.empty())
        {
            FSKRHITriangleDrawParams Params;
            Params.bDepthTest = true;
            Params.bDepthWrite = (View.ViewMode != ESKViewMode::XRay);
            Params.bCullBackFace = false;
            Params.bAlphaBlend = (View.ViewMode == ESKViewMode::XRay);
            Cmd.DrawIndexedTriangleList(Scratch.Vertices.data(), (uint32)Scratch.Vertices.size(), Scratch.Indices.data(), (uint32)Scratch.Indices.size(), Params);
            bIssuedDraws = true;
        }

        if (bIssuedDraws)
        {
            Cmd.Flush();
        }
    }
}
