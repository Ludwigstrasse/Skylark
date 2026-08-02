#pragma once

#include "CAM/CamDomain.h"
#include "CAM/BRepCamGeometryProvider.h"
#include "Common/CamCore.h"

#include <cstdint>
#include <string>
#include <vector>

namespace skylark::cam
{
struct PlannerDescriptor
{
    CamOperationType type{CamOperationType::Roughing3D};
    std::string name;
    bool availableInM0{false};
};

struct MeshToolpathOptions
{
    double safeZ{60.0};
    double clearanceOffset{8.0};
    double zStep{2.0};
    double stepover{6.0};
    double sampleStep{4.0};
    double stockToLeave{0.0};
    double feedRate{1200.0};
    double rapidFeedRate{6000.0};
    std::uint32_t maxScanLines{160};
    std::uint32_t maxSamplesPerLine{400};
};

class MeshZLevelToolpathPlanner
{
public:
    ToolpathResult Calculate(const ICamGeometryProvider& geometry, const CamSetup& setup, const MeshToolpathOptions& options = {}) const;
};

class MeshParallelFinishingPlanner
{
public:
    ToolpathResult Calculate(const ICamGeometryProvider& geometry, const CamSetup& setup, const MeshToolpathOptions& options = {}) const;
};

std::vector<PlannerDescriptor> CreateM0PlannerMatrix();
std::vector<PlannerDescriptor> CreateM4PlannerMatrix();
std::vector<PlannerDescriptor> CreateM7PlannerMatrix();

struct BrepToolpathOptions
{
    double safeZ{60.0};
    double contourZ{0.0};
    double pocketDepth{4.0};
    double pocketStepdown{2.0};
    double stepover{5.0};
    double feedRate{1000.0};
    double rapidFeedRate{6000.0};
};

class Brep25DContourPlanner
{
public:
    ToolpathResult Calculate(const BRepCamGeometryProvider& geometry, const CamSetup& setup, const BrepToolpathOptions& options = {}) const;
};

class BrepPocketPlannerStub
{
public:
    ToolpathResult Calculate(const BRepCamGeometryProvider& geometry, const CamSetup& setup, const BrepToolpathOptions& options = {}) const;
};

class BrepDrillingPlannerStub
{
public:
    ToolpathResult Calculate(const BRepCamGeometryProvider& geometry, const CamSetup& setup, const BrepToolpathOptions& options = {}) const;
};
}
