#pragma once

#include "SKRenderer/SKMeshDrawPacket.h"
#include "SKRenderer/SKRenderer.h"
#include "SKRHI/SKRHI.h"

namespace Skylark
{
    class FSKPacketBuildUtils final
    {
    public:
        static uint32 PackColor(float R, float G, float B, float A);

        static void AppendShadedSectionIndexed(
            const FSKMeshSectionDrawPacket& Section,
            const FSKMatrix4f& LocalToWorld,
            const FSKViewInfo& View,
            TArray<FSKRHITriangleVertex>& OutVertices,
            TArray<uint32>& OutIndices);

        static void AppendSelectionSectionIndexed(
            const FSKMeshSectionDrawPacket& Section,
            const FSKMatrix4f& LocalToWorld,
            uint32 FallbackHitProxyId,
            const FSKViewInfo& View,
            TArray<FSKRHITriangleVertex>& OutVertices,
            TArray<uint32>& OutIndices);

        static void AppendTintSectionIndexed(
            const FSKMeshSectionDrawPacket& Section,
            const FSKMatrix4f& LocalToWorld,
            const FSKViewInfo& View,
            uint32 TintColorRGBA8,
            TArray<FSKRHITriangleVertex>& OutVertices,
            TArray<uint32>& OutIndices);
    };
}
