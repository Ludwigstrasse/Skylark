#include "SKLineRender/SKLineRender.h"

#include "SKCore/SKAssert.h"
#include "SKRenderer/SKEdgeDrawPacket.h"
#include "SKRenderer/SKGeometryResidencyCache.h"
#include "SKRenderer/SKRenderScratchBuffer.h"
#include "SKRenderer/SKGpuResidentBufferCache.h"
#include "SKRenderer/SKSceneUploadQueue.h"
#include "SKRenderer/SKRenderBatching.h"

namespace Skylark
{
	FSKVector4f FSKLineRenderer::TransformToClip(const FSKVector3f& PWorld, const FSKViewMatrices& Matrices)
	{
		const FSKVector4f W(PWorld.X, PWorld.Y, PWorld.Z, 1.0f);
		const FSKVector4f V = SKTransformVector4(Matrices.View, W);
		const FSKVector4f C = SKTransformVector4(Matrices.Projection, V);
		return C;
	}

    namespace
    {
        static void AppendPacketLines(const FSKEdgeSectionDrawPacket& Section,
            const TArray<FSKEdgePacketLine>& Lines,
            const FSKMatrix4f& LocalToWorld,
            const FSKViewMatrices& Matrices,
            uint32 ColorRGBA8,
            TArray<FSKRHILineVertex>& Out)
        {
            if (Lines.empty())
            {
                return;
            }

            Out.reserve(Out.size() + Lines.size() * 2);
            for (const auto& Line : Lines)
            {
                const FSKVector3f P0W = SKTransformPoint(LocalToWorld, Line.P0Local);
                const FSKVector3f P1W = SKTransformPoint(LocalToWorld, Line.P1Local);

                const auto C0 = FSKLineRenderer::TransformToClip(P0W, Matrices);
                const auto C1 = FSKLineRenderer::TransformToClip(P1W, Matrices);

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

        static const ISKRHIResidentLineBuffer* ResolveResidentEdgeBuffer(const FSKResidentEdgeSectionBuffers& Section, FSKEdgeRenderSettings::EDisplay Display)
        {
            switch (Display)
            {
            case FSKEdgeRenderSettings::EDisplay::Boundary:
                if (Section.Boundary) return Section.Boundary.get();
                break;
            case FSKEdgeRenderSettings::EDisplay::Sharp:
                if (Section.Sharp) return Section.Sharp.get();
                break;
            case FSKEdgeRenderSettings::EDisplay::All:
            default:
                if (Section.All) return Section.All.get();
                break;
            }

            if (Section.All) return Section.All.get();
            if (Section.Sharp) return Section.Sharp.get();
            if (Section.Boundary) return Section.Boundary.get();
            return nullptr;
        }

        static FSKRHIResidentLineDrawParams MakeResidentLineParams(const FSKMatrix4f& LocalToWorld, const FSKViewInfo& View, const FSKLineStyle& Style, bool bDepthTest, uint32 ColorRGBA8)
        {
            FSKRHIResidentLineDrawParams Params;
            Params.LocalToWorld = LocalToWorld;
            Params.View = View.Matrices.View;
            Params.Projection = View.Matrices.Projection;
            Params.bDepthTest = bDepthTest;
            Params.Width = Style.Width;
            Params.bUseOverrideColor = true;
            Params.OverrideColorRGBA8 = ColorRGBA8;
            return Params;
        }


        static bool ShouldDrawSection(SIZE_T SectionIndex, int32 RequestedSectionIndex)
        {
            return RequestedSectionIndex < 0 || static_cast<SIZE_T>(RequestedSectionIndex) == SectionIndex;
        }
    }

	void FSKLineRenderer::BuildEdgeLines(
		const FSKGeometryData& Geo,
		const FSKMatrix4f& LocalToWorld,
		const FSKEdgeRenderSettings& EdgeSettings,
		const FSKViewMatrices& Matrices,
		uint32 ColorRGBA8,
		TArray<FSKRHILineVertex>& Out,
		int32 SectionIndex)
	{
        const FSKEdgeDrawPacket& Packet = FSKGeometryResidencyCache::Get().GetOrBuildEdgePacket(Geo);
        for (SIZE_T PacketSectionIndex = 0; PacketSectionIndex < Packet.Sections.size(); ++PacketSectionIndex)
        {
            if (!ShouldDrawSection(PacketSectionIndex, SectionIndex))
            {
                continue;
            }

            const auto& Section = Packet.Sections[PacketSectionIndex];
            const auto& Lines = FSKEdgeDrawPacketBuilder::GetLines(Section, EdgeSettings.Display);
            AppendPacketLines(Section, Lines, LocalToWorld, Matrices, ColorRGBA8, Out);
        }
	}


void FSKLineRenderer::DrawEdges(ISKRHICommandList& Cmd, const FSKLineBuildInput& In)
{
    if (!In.SceneView || !In.View || !In.SceneView->GeometryRegistry)
    {
        return;
    }

    auto& UploadQueue = FSKSceneUploadQueue::Get();
    UploadQueue.Reset();
    UploadQueue.QueueVisibleScene(*In.SceneView, SK_SceneUpload_Edge);
    UploadQueue.Resolve(Cmd, *In.SceneView);

    TArray<FSKGeometryDrawBatch> GeometryBatches;
    FSKRenderBatching::BuildGeometryBatches(*In.SceneView, GeometryBatches);

    auto& Scratch = FSKRenderScratchBuffer::GetLineScratch();
    Scratch.Reset();
    auto& Vertices = Scratch.Vertices;
    Vertices.reserve(4096);

    bool bIssuedResidentDraws = false;
    auto& GpuResident = FSKGpuResidentBufferCache::Get();

    for (const auto& GeometryBatch : GeometryBatches)
    {
        const FSKGeometryData* Geo = In.SceneView->GeometryRegistry->Find(GeometryBatch.GeometryKey);
        if (!Geo)
        {
            continue;
        }

        const auto& Resident = GpuResident.GetOrCreateEdgeBuffers(Cmd, *Geo);
        for (const auto& Item : GeometryBatch.Items)
        {
            if (!Item.LocalToWorld)
            {
                continue;
            }

            if (Resident.bResidentSupported)
            {
                for (SIZE_T SectionIndex = 0; SectionIndex < Resident.Sections.size(); ++SectionIndex)
                {
                    if (!ShouldDrawSection(SectionIndex, Item.SectionIndex))
                    {
                        continue;
                    }

                    const auto& Section = Resident.Sections[SectionIndex];
                    if (const ISKRHIResidentLineBuffer* Buffer = ResolveResidentEdgeBuffer(Section, In.EdgeSettings.Display))
                    {
                        Cmd.DrawResidentLineBuffer(*Buffer, MakeResidentLineParams(*Item.LocalToWorld, *In.View, In.VisibleStyle, !In.EdgeSettings.bOverlay, In.VisibleStyle.ColorRGBA8));
                        bIssuedResidentDraws = true;
                    }
                }
                continue;
            }

            BuildEdgeLines(*Geo, *Item.LocalToWorld, In.EdgeSettings, In.View->Matrices, In.VisibleStyle.ColorRGBA8, Vertices, Item.SectionIndex);
        }
    }

    if (!Vertices.empty())
    {
        FSKRHILineDrawParams Params;
        Params.bDepthTest = !In.EdgeSettings.bOverlay;
        Params.Width = In.VisibleStyle.Width;
        Params.PatternMask = In.VisibleStyle.PatternMask;
        Params.PatternScale = In.VisibleStyle.PatternScale;

        Cmd.DrawLineList(Vertices.data(), (uint32)Vertices.size(), Params);
        bIssuedResidentDraws = true;
    }

    if (bIssuedResidentDraws)
    {
        Cmd.Flush();
    }
}

void FSKLineRenderer::DrawEdgesForHitProxies(ISKRHICommandList& Cmd, const FSKLineBuildInput& In)
	{
		if (!In.SceneView || !In.View || !In.SceneView->GeometryRegistry)
		{
			return;
		}

		auto& Scratch = FSKRenderScratchBuffer::GetLineScratch();
        Scratch.Reset();
        auto& Vertices = Scratch.Vertices;
        Vertices.reserve(4096);

		auto EncodeColor = [](uint32 HitProxyId) -> uint32
		{
			const uint8 R = (uint8)((HitProxyId >> 0) & 0xFF);
			const uint8 G = (uint8)((HitProxyId >> 8) & 0xFF);
			const uint8 B = (uint8)((HitProxyId >> 16) & 0xFF);
			const uint8 A = (uint8)((HitProxyId >> 24) & 0xFF);
			return SKPackRGBA8(R, G, B, A);
		};

        bool bIssuedResidentDraws = false;
        auto& GpuResident = FSKGpuResidentBufferCache::Get();

		for (const auto& Proxy : In.SceneView->VisibleProxies)
		{
			const FSKGeometryData* Geo = In.SceneView->GeometryRegistry->Find(Proxy.GeometryKey);
			if (!Geo)
			{
				continue;
			}

            const auto& Resident = GpuResident.GetOrCreateEdgeBuffers(Cmd, *Geo);
            if (Resident.bResidentSupported)
            {
                for (SIZE_T SectionIndex = 0; SectionIndex < Resident.Sections.size(); ++SectionIndex)
                {
                    if (!ShouldDrawSection(SectionIndex, Proxy.SectionIndex))
                    {
                        continue;
                    }

                    const auto& Section = Resident.Sections[SectionIndex];
                    if (const ISKRHIResidentLineBuffer* Buffer = ResolveResidentEdgeBuffer(Section, In.EdgeSettings.Display))
                    {
                        auto Params = MakeResidentLineParams(Proxy.LocalToWorld, *In.View, In.VisibleStyle, false, EncodeColor(Proxy.HitProxyId));
                        Params.Width = 1.0f;
                        Cmd.DrawResidentLineBuffer(*Buffer, Params);
                        bIssuedResidentDraws = true;
                    }
                }
                continue;
            }

			BuildEdgeLines(*Geo, Proxy.LocalToWorld, In.EdgeSettings, In.View->Matrices, EncodeColor(Proxy.HitProxyId), Vertices, Proxy.SectionIndex);
		}

		for (const auto& Batch : In.SceneView->VisibleInstances)
		{
			const FSKGeometryData* Geo = In.SceneView->GeometryRegistry->Find(Batch.GeometryKey);
			if (!Geo)
			{
				continue;
			}

            const auto& Resident = GpuResident.GetOrCreateEdgeBuffers(Cmd, *Geo);
            if (Resident.bResidentSupported)
            {
                for (SIZE_T i = 0; i < Batch.InstanceLocalToWorld.size(); ++i)
                {
                    const uint32 HitProxyId = (i < Batch.InstanceHitProxyId.size()) ? Batch.InstanceHitProxyId[i] : 0;
                    for (SIZE_T SectionIndex = 0; SectionIndex < Resident.Sections.size(); ++SectionIndex)
                    {
                        if (!ShouldDrawSection(SectionIndex, Batch.SectionIndex))
                        {
                            continue;
                        }

                        const auto& Section = Resident.Sections[SectionIndex];
                        if (const ISKRHIResidentLineBuffer* Buffer = ResolveResidentEdgeBuffer(Section, In.EdgeSettings.Display))
                        {
                            auto Params = MakeResidentLineParams(Batch.InstanceLocalToWorld[i], *In.View, In.VisibleStyle, false, EncodeColor(HitProxyId));
                            Params.Width = 1.0f;
                            Cmd.DrawResidentLineBuffer(*Buffer, Params);
                            bIssuedResidentDraws = true;
                        }
                    }
                }
                continue;
            }

			for (SIZE_T i = 0; i < Batch.InstanceLocalToWorld.size(); ++i)
			{
				const uint32 HitProxyId = (i < Batch.InstanceHitProxyId.size()) ? Batch.InstanceHitProxyId[i] : 0;
				BuildEdgeLines(*Geo, Batch.InstanceLocalToWorld[i], In.EdgeSettings, In.View->Matrices, EncodeColor(HitProxyId), Vertices, Batch.SectionIndex);
			}
		}

		if (!Vertices.empty())
		{
			FSKRHILineDrawParams Params;
			Params.bDepthTest = false;
			Params.Width = 1.0f;
			Params.PatternMask = 0xFFFFFFFFu;
			Params.PatternScale = 1.0f;

			Cmd.DrawLineList(Vertices.data(), (uint32)Vertices.size(), Params);
            bIssuedResidentDraws = true;
		}

        if (bIssuedResidentDraws)
        {
		    Cmd.Flush();
        }
	}
}