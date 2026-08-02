#pragma once
#include <cstdint>

namespace Skylark::FormatWorkbench
{
    struct FwProcessMemorySnapshot
    {
        std::uint64_t ResidentBytes = 0;
        std::uint64_t PeakResidentBytes = 0;
        std::uint64_t VirtualBytes = 0;
    };

    FwProcessMemorySnapshot QueryProcessMemorySnapshot();
    const char* FormatBytesHumanReadable(std::uint64_t Bytes, char* Buffer, unsigned BufferSize);
}
