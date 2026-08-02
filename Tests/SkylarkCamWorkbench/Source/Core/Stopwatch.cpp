#include "Core/Stopwatch.h"

namespace Skylark::FormatWorkbench
{
    FwStopwatch::FwStopwatch()
        : StartTime(std::chrono::steady_clock::now())
    {
    }

    void FwStopwatch::Restart()
    {
        StartTime = std::chrono::steady_clock::now();
    }

    double FwStopwatch::ElapsedMilliseconds() const
    {
        const auto Now = std::chrono::steady_clock::now();
        return std::chrono::duration<double, std::milli>(Now - StartTime).count();
    }
}
