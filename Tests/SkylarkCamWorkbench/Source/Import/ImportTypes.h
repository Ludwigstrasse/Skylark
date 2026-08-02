#pragma once
#include "Core/MemoryInfo.h"
#include "Model/ModelTypes.h"

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace Skylark::FormatWorkbench
{
    enum class EImportSeverity
    {
        Info,
        Warning,
        Error
    };

    struct FwImportMessage final
    {
        EImportSeverity Severity = EImportSeverity::Info;
        std::string Text;
    };

    struct FwImportOptions final
    {
        std::uint64_t MaxTriangles = 0;                // 0 = unlimited
        std::uint32_t TrianglesPerBlock = 1'000'000;   // keeps blocks GPU/upload friendly
        bool StoreAttributeBytes = false;
        bool RecomputeInvalidNormals = true;
        bool BuildSkylarkPreview = true;
        std::uint64_t MaxSkylarkPreviewTriangles = 1'000'000;
    };

    struct FwImportReport final
    {
        bool Succeeded = false;
        EModelFileFormat Format = EModelFileFormat::Unknown;
        std::uint64_t FileBytes = 0;
        std::uint64_t Triangles = 0;
        std::uint64_t Vertices = 0;
        std::uint64_t MeshBlocks = 0;
        std::uint64_t CompactCpuBytes = 0;
        double ImportMilliseconds = 0.0;
        FwProcessMemorySnapshot MemoryBefore;
        FwProcessMemorySnapshot MemoryAfter;
        std::vector<FwImportMessage> Messages;

        void AddInfo(std::string Text);
        void AddWarning(std::string Text);
        void AddError(std::string Text);
    };

    struct FwImportResult final
    {
        std::unique_ptr<FwModelDocument> Document;
        FwImportReport Report;
    };

    using FwProgressCallback = std::function<bool(double Percent, const char* Stage)>;
}
