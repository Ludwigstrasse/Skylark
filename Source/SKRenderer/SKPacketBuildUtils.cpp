#include "SKRenderer/SKPacketBuildUtils.h"

#include "SKLineRender/SKLineRender.h"

#include <algorithm>
#include <cmath>

namespace Skylark
{
    namespace
    {
        SK_FORCEINLINE FSKVector3f Scale3(const FSKVector3f& V, float S)
        {
            return FSKVector3f(V.X * S, V.Y * S, V.Z * S);
        }

        SK_FORCEINLINE float Dot3(const FSKVector3f& A, const FSKVector3f& B)
        {
            return A.X * B.X + A.Y * B.Y + A.Z * B.Z;
        }

        SK_FORCEINLINE float Length3(const FSKVector3f& V)
        {
            return std::sqrt(Dot3(V, V));
        }

        SK_FORCEINLINE FSKVector3f Normalize3(const FSKVector3f& V, const FSKVector3f& Fallback = FSKVector3f(0.0f, 0.0f, 1.0f))
        {
            const float Len = Length3(V);
            if (Len <= 1.0e-6f)
            {
                return Fallback;
            }
            return Scale3(V, 1.0f / Len);
        }

        SK_FORCEINLINE FSKVector3f TransformVector3NoTranslation(const FSKMatrix4f& M, const FSKVector3f& V)
        {
            return FSKVector3f(
                M.M[0][0] * V.X + M.M[0][1] * V.Y + M.M[0][2] * V.Z,
                M.M[1][0] * V.X + M.M[1][1] * V.Y + M.M[1][2] * V.Z,
                M.M[2][0] * V.X + M.M[2][1] * V.Y + M.M[2][2] * V.Z);
        }

        static FSKVector3f ShadeColor(const FSKVector3f& BaseColor, const FSKVector3f& WorldNormal)
        {
            const FSKVector3f LightDir = Normalize3(FSKVector3f(-0.35f, -0.45f, 0.82f));
            const float NoL = std::max(0.0f, Dot3(Normalize3(WorldNormal), LightDir));
            const float Ambient = 0.30f;
            const float Diffuse = 0.70f * NoL;
            return Scale3(BaseColor, Ambient + Diffuse);
        }

        static uint32 EncodeHitProxyColor(uint32 HitProxyId)
        {
            const uint8 R = static_cast<uint8>((HitProxyId >> 0u) & 0xFFu);
            const uint8 G = static_cast<uint8>((HitProxyId >> 8u) & 0xFFu);
            const uint8 B = static_cast<uint8>((HitProxyId >> 16u) & 0xFFu);
            const uint8 A = static_cast<uint8>((HitProxyId >> 24u) & 0xFFu);
            return static_cast<uint32>(R) | (static_cast<uint32>(G) << 8u) | (static_cast<uint32>(B) << 16u) | (static_cast<uint32>(A) << 24u);
        }

        static bool IsSectionPickable(const FSKMeshSectionDrawPacket& Section, const FSKViewInfo& View)
        {
            if (!Section.OwnerId.IsValid())
            {
                return true;
            }
            return SKSelectionMaskMatches(Section.OwnerId.Type, View.Selection.Style.PickEntityMask);
        }
    }

    uint32 FSKPacketBuildUtils::PackColor(float R, float G, float B, float A)
    {
        auto ClampU8 = [](float V) -> uint8
        {
            const float C = std::clamp(V, 0.0f, 1.0f);
            return static_cast<uint8>(C * 255.0f + 0.5f);
        };
        return (uint32)ClampU8(R) | ((uint32)ClampU8(G) << 8u) | ((uint32)ClampU8(B) << 16u) | ((uint32)ClampU8(A) << 24u);
    }

    void FSKPacketBuildUtils::AppendShadedSectionIndexed(const FSKMeshSectionDrawPacket& Section, const FSKMatrix4f& LocalToWorld, const FSKViewInfo& View, TArray<FSKRHITriangleVertex>& OutVertices, TArray<uint32>& OutIndices)
    {
        if (Section.Vertices.empty() || Section.Indices.empty())
        {
            return;
        }

        const uint32 BaseVertex = static_cast<uint32>(OutVertices.size());
        const float Alpha = (View.ViewMode == ESKViewMode::XRay) ? 0.35f : 1.0f;
        OutVertices.reserve(OutVertices.size() + Section.Vertices.size());
        OutIndices.reserve(OutIndices.size() + Section.Indices.size());

        for (const auto& PacketVertex : Section.Vertices)
        {
            const FSKVector3f PositionWorld = SKTransformPoint(LocalToWorld, PacketVertex.PositionLocal);
            const FSKVector3f NormalWorld = Normalize3(TransformVector3NoTranslation(LocalToWorld, PacketVertex.NormalLocal));
            const FSKVector3f Color = ShadeColor(Section.BaseColor, NormalWorld);
            const auto Clip = FSKLineRenderer::TransformToClip(PositionWorld, View.Matrices);

            FSKRHITriangleVertex V;
            V.X = Clip.X; V.Y = Clip.Y; V.Z = Clip.Z; V.W = Clip.W;
            V.ColorRGBA8 = PackColor(Color.X, Color.Y, Color.Z, Alpha);
            OutVertices.push_back(V);
        }

        for (const uint32 LocalIndex : Section.Indices)
        {
            OutIndices.push_back(BaseVertex + LocalIndex);
        }
    }

    void FSKPacketBuildUtils::AppendSelectionSectionIndexed(const FSKMeshSectionDrawPacket& Section, const FSKMatrix4f& LocalToWorld, uint32 FallbackHitProxyId, const FSKViewInfo& View, TArray<FSKRHITriangleVertex>& OutVertices, TArray<uint32>& OutIndices)
    {
        if (Section.Vertices.empty() || Section.Indices.empty() || !IsSectionPickable(Section, View))
        {
            return;
        }

        const uint32 EffectiveHitProxyId = (Section.SectionHitProxyId != 0u) ? Section.SectionHitProxyId : FallbackHitProxyId;
        if (EffectiveHitProxyId == 0u)
        {
            return;
        }

        const uint32 BaseVertex = static_cast<uint32>(OutVertices.size());
        const uint32 EncodedColor = EncodeHitProxyColor(EffectiveHitProxyId);
        OutVertices.reserve(OutVertices.size() + Section.Vertices.size());
        OutIndices.reserve(OutIndices.size() + Section.Indices.size());

        for (const auto& PacketVertex : Section.Vertices)
        {
            const FSKVector3f PositionWorld = SKTransformPoint(LocalToWorld, PacketVertex.PositionLocal);
            const auto Clip = FSKLineRenderer::TransformToClip(PositionWorld, View.Matrices);
            FSKRHITriangleVertex V;
            V.X = Clip.X; V.Y = Clip.Y; V.Z = Clip.Z; V.W = Clip.W;
            V.ColorRGBA8 = EncodedColor;
            OutVertices.push_back(V);
        }

        for (const uint32 LocalIndex : Section.Indices)
        {
            OutIndices.push_back(BaseVertex + LocalIndex);
        }
    }

    void FSKPacketBuildUtils::AppendTintSectionIndexed(const FSKMeshSectionDrawPacket& Section, const FSKMatrix4f& LocalToWorld, const FSKViewInfo& View, uint32 TintColorRGBA8, TArray<FSKRHITriangleVertex>& OutVertices, TArray<uint32>& OutIndices)
    {
        if (Section.Vertices.empty() || Section.Indices.empty())
        {
            return;
        }

        const uint32 BaseVertex = static_cast<uint32>(OutVertices.size());
        OutVertices.reserve(OutVertices.size() + Section.Vertices.size());
        OutIndices.reserve(OutIndices.size() + Section.Indices.size());

        for (const auto& PacketVertex : Section.Vertices)
        {
            const FSKVector3f PositionWorld = SKTransformPoint(LocalToWorld, PacketVertex.PositionLocal);
            const auto Clip = FSKLineRenderer::TransformToClip(PositionWorld, View.Matrices);
            FSKRHITriangleVertex V;
            V.X = Clip.X; V.Y = Clip.Y; V.Z = Clip.Z; V.W = Clip.W;
            V.ColorRGBA8 = TintColorRGBA8;
            OutVertices.push_back(V);
        }

        for (const uint32 LocalIndex : Section.Indices)
        {
            OutIndices.push_back(BaseVertex + LocalIndex);
        }
    }
}
