#include "SKRenderer/SKSelectionRenderer.h"

#include "SKGeometry/SKGeometryRegistry.h"
#include "SKRenderer/SKMeshRenderer.h"
#include "SKRenderer/SKPacketBuildUtils.h"
#include "SKRenderer/SKRenderScratchBuffer.h"
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

    namespace
    {
        static FSKRHIResidentTriangleDrawParams MakeParams(const FSKMatrix4f& LocalToWorld, const FSKViewInfo& View, uint32 HitProxyId)
        {
            FSKRHIResidentTriangleDrawParams Params;
            Params.LocalToWorld = LocalToWorld;
            Params.View = View.Matrices.View;
            Params.Projection = View.Matrices.Projection;
            Params.bDepthTest = true;
            Params.bDepthWrite = true;
            Params.bCullBackFace = false;
            Params.bAlphaBlend = false;
            Params.bUseOverrideColor = true;
            Params.OverrideColorRGBA8 = PackRGBA8(
                (uint8)((HitProxyId >> 0) & 0xFFu),
                (uint8)((HitProxyId >> 8) & 0xFFu),
                (uint8)((HitProxyId >> 16) & 0xFFu),
                (uint8)((HitProxyId >> 24) & 0xFFu));
            return Params;
        }

        static bool DrawResidentSelection(ISKRHICommandList& Cmd, const FSKResidentMeshGeometryBuffers& Resident, const FSKMatrix4f& LocalToWorld, const FSKViewInfo& View, uint32 HitProxyId)
        {
            bool bDrawn = false;
            const auto Params = MakeParams(LocalToWorld, View, HitProxyId);
            for (const auto& Section : Resident.Sections)
            {
                if (!Section.Buffer)
                {
                    continue;
                }
                Cmd.DrawResidentTriangleBuffer(*Section.Buffer, Params);
                bDrawn = true;
            }
            return bDrawn;
        }
    }

    void FSKSelectionRenderer::DrawSelectionIds(ISKRHICommandList& Cmd, const FSKSceneView& SceneView, const FSKViewInfo& View)
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
                Packet = &FSKMeshRenderer::GetResidencyCache().GetOrBuildMeshPacket(*Geo);
            }

            if (Packet)
            {
                (void)IndirectPackets.GetOrBuild(*Geo, *Packet);
            }

            for (const auto& Item : Run.Items)
            {
                if (Resident.bResidentSupported && DrawResidentSelection(Cmd, Resident, Item.LocalToWorld, View, Item.HitProxyId))
                {
                    bIssuedDraws = true;
                    continue;
                }

                if (Packet)
                {
                    for (const auto& Section : Packet->Sections)
                    {
                        FSKPacketBuildUtils::AppendSelectionSectionIndexed(Section, Item.LocalToWorld, Item.HitProxyId, View, Scratch.Vertices, Scratch.Indices);
                    }
                }
            }
        }

        if (!Scratch.Vertices.empty() && !Scratch.Indices.empty())
        {
            FSKRHITriangleDrawParams Params;
            Params.bDepthTest = true;
            Params.bDepthWrite = true;
            Params.bCullBackFace = false;
            Params.bAlphaBlend = false;
            Cmd.DrawIndexedTriangleList(Scratch.Vertices.data(), (uint32)Scratch.Vertices.size(), Scratch.Indices.data(), (uint32)Scratch.Indices.size(), Params);
            bIssuedDraws = true;
        }

        if (bIssuedDraws)
        {
            Cmd.Flush();
        }
    }
}
