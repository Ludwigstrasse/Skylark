#pragma once
#include "Import/ImportTypes.h"

#include <cstdint>
#include <string>

#if SKFW_WITH_SKYLARK
    #include "SKGeometry/SKGeometryRegistry.h"
#endif

namespace Skylark::FormatWorkbench
{
    struct FwSkylarkPreviewOptions final
    {
        std::uint64_t MaxTriangles = 1'000'000;
        bool BuildWireframe = false;
        bool BuildEdgeCategories = false;
    };

    struct FwSkylarkPreviewResult final
    {
        bool Succeeded = false;
        std::uint64_t GeometryKey = 0;
        std::uint64_t UploadedTriangles = 0;
        std::string Message;
    };

#if SKFW_WITH_SKYLARK
    FwSkylarkPreviewResult RegisterPreviewMeshInSkylark(const FwModelDocument& Document, Skylark::FSKGeometryRegistry& Registry, const FwSkylarkPreviewOptions& Options);
#else
    FwSkylarkPreviewResult RegisterPreviewMeshInSkylark(const FwModelDocument& Document, void* Registry, const FwSkylarkPreviewOptions& Options);
#endif
}
