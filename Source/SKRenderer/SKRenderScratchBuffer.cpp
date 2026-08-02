#include "SKRenderer/SKRenderScratchBuffer.h"

namespace Skylark
{
    FSKIndexedTriangleScratch& FSKRenderScratchBuffer::GetTriangleScratch()
    {
        static thread_local FSKIndexedTriangleScratch GScratch;
        return GScratch;
    }
}


namespace Skylark
{
    FSKLineScratch& FSKRenderScratchBuffer::GetLineScratch()
    {
        static thread_local FSKLineScratch GLineScratch;
        return GLineScratch;
    }
}
