#pragma once

#include "Render/RenderCore.h"

#include <cstdint>
#include <string>
#include <vector>

namespace skylark::cam
{
class LargeMeshAsset;

struct BenchmarkCounter
{
    std::string name;
    std::string value;
};

struct BenchmarkReport
{
    std::vector<BenchmarkCounter> counters;
    void AddCounter(std::string name, std::string value);
};

BenchmarkReport CreateM0BenchmarkReport();
BenchmarkReport CreateM1BenchmarkReport(const LargeMeshAsset* activeMesh);
BenchmarkReport CreateM2BenchmarkReport(const LargeMeshAsset* activeMesh, const GpuUploadQueueSnapshot& uploadQueue, std::uint32_t pendingAccelerationBuilds);
}
