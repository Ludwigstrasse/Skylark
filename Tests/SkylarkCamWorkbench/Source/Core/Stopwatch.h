#pragma once
#include <chrono>
#include <cstdint>

namespace Skylark::FormatWorkbench
{
    class FwStopwatch final
    {
    public:
        FwStopwatch();
        void Restart();
        [[nodiscard]] double ElapsedMilliseconds() const;

    private:
        std::chrono::steady_clock::time_point StartTime;
    };
}
