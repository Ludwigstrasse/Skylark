#pragma once

#include "SKCore/SKCoreMinimal.h"
#include "SKRHI/SKRHI.h"

namespace Skylark
{
    struct FSKIndexedTriangleScratch
    {
        TArray<FSKRHITriangleVertex> Vertices;
        TArray<uint32> Indices;

        void Reset()
        {
            Vertices.clear();
            Indices.clear();
        }
    };

    struct FSKLineScratch
    {
        TArray<FSKRHILineVertex> Vertices;

        void Reset()
        {
            Vertices.clear();
        }
    };

    class FSKRenderScratchBuffer final
    {
    public:
        static FSKIndexedTriangleScratch& GetTriangleScratch();
        static FSKLineScratch& GetLineScratch();
    };
}
