#include "Core/MemoryInfo.h"

#include <cstdio>
#include <cstring>
#include <fstream>
#include <string>

#if defined(_WIN32)
    #define NOMINMAX
    #include <windows.h>
    #include <psapi.h>
#endif

namespace Skylark::FormatWorkbench
{
    namespace
    {
        static std::uint64_t KilobytesToBytes(std::uint64_t Value)
        {
            return Value * 1024ull;
        }

#if !defined(_WIN32)
        static std::uint64_t ReadStatusValueKb(const char* Key)
        {
            std::ifstream Input("/proc/self/status");
            std::string Name;
            while (Input >> Name)
            {
                if (Name == Key)
                {
                    std::uint64_t Value = 0;
                    std::string Unit;
                    Input >> Value >> Unit;
                    return KilobytesToBytes(Value);
                }

                std::string Remainder;
                std::getline(Input, Remainder);
            }
            return 0;
        }
#endif
    }

    FwProcessMemorySnapshot QueryProcessMemorySnapshot()
    {
        FwProcessMemorySnapshot Snapshot{};
#if defined(_WIN32)
        PROCESS_MEMORY_COUNTERS_EX Counters{};
        Counters.cb = sizeof(Counters);
        if (GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&Counters), sizeof(Counters)))
        {
            Snapshot.ResidentBytes = static_cast<std::uint64_t>(Counters.WorkingSetSize);
            Snapshot.PeakResidentBytes = static_cast<std::uint64_t>(Counters.PeakWorkingSetSize);
            Snapshot.VirtualBytes = static_cast<std::uint64_t>(Counters.PrivateUsage);
        }
#else
        Snapshot.ResidentBytes = ReadStatusValueKb("VmRSS:");
        Snapshot.PeakResidentBytes = ReadStatusValueKb("VmHWM:");
        Snapshot.VirtualBytes = ReadStatusValueKb("VmSize:");
#endif
        return Snapshot;
    }

    const char* FormatBytesHumanReadable(std::uint64_t Bytes, char* Buffer, unsigned BufferSize)
    {
        const char* Units[] = {"B", "KB", "MB", "GB", "TB"};
        double Value = static_cast<double>(Bytes);
        unsigned UnitIndex = 0;
        while (Value >= 1024.0 && UnitIndex + 1 < sizeof(Units) / sizeof(Units[0]))
        {
            Value /= 1024.0;
            ++UnitIndex;
        }
        std::snprintf(Buffer, BufferSize, "%.2f %s", Value, Units[UnitIndex]);
        return Buffer;
    }
}
