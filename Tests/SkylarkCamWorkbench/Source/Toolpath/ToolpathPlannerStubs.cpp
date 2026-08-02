#include "Toolpath/ToolpathPlannerStubs.h"

#include <algorithm>
#include <cmath>
#include <sstream>
#include <utility>

namespace skylark::cam
{
namespace
{
double ClampPositive(double value, double fallback) noexcept
{
    return value > 1.0e-9 ? value : fallback;
}

Vec3d MidPoint(const Vec3d& a, const Vec3d& b) noexcept
{
    return {(a.x + b.x) * 0.5, (a.y + b.y) * 0.5, (a.z + b.z) * 0.5};
}

void AppendPolylineMoves(ToolpathDocument& document, const Polyline3d& polyline, double safeZ, double feedRate, double rapidFeedRate)
{
    if (polyline.points.size() < 2U) { return; }
    const Vec3d start = polyline.points.front();
    document.moves.push_back({ToolpathMoveKind::Rapid, {start.x, start.y, safeZ}, rapidFeedRate});
    document.moves.push_back({ToolpathMoveKind::LeadIn, start, feedRate});
    for (std::size_t i = 1; i < polyline.points.size(); ++i)
    {
        document.moves.push_back({ToolpathMoveKind::Feed, polyline.points[i], feedRate});
    }
    const Vec3d end = polyline.points.back();
    document.moves.push_back({ToolpathMoveKind::LeadOut, end, feedRate});
    document.moves.push_back({ToolpathMoveKind::Retract, {end.x, end.y, safeZ}, rapidFeedRate});
}

MeshToolpathOptions NormalizeOptions(const BoundingBox3d& bounds, MeshToolpathOptions options)
{
    const Vec3d size = bounds.Size();
    options.zStep = ClampPositive(options.zStep, std::max(size.z / 12.0, 1.0));
    options.stepover = ClampPositive(options.stepover, std::max(std::max(size.x, size.y) / 40.0, 1.0));
    options.sampleStep = ClampPositive(options.sampleStep, std::max(std::max(size.x, size.y) / 80.0, 1.0));
    options.safeZ = std::max(options.safeZ, bounds.max.z + std::max(options.clearanceOffset, 5.0));
    options.maxScanLines = std::max<std::uint32_t>(options.maxScanLines, 2U);
    options.maxSamplesPerLine = std::max<std::uint32_t>(options.maxSamplesPerLine, 2U);
    return options;
}
}

ToolpathResult MeshZLevelToolpathPlanner::Calculate(const ICamGeometryProvider& geometry, const CamSetup&, const MeshToolpathOptions& rawOptions) const
{
    ToolpathResult result;
    if (geometry.GetKind() != CamGeometryKind::Mesh)
    {
        result.warnings.push_back("Z-Level requires MeshCamGeometryProvider.");
        return result;
    }

    const BoundingBox3d bounds = geometry.GetBoundingBox();
    if (!bounds.IsValid())
    {
        result.warnings.push_back("Z-Level failed: invalid mesh bounding box.");
        return result;
    }

    const MeshToolpathOptions options = NormalizeOptions(bounds, rawOptions);
    ToolpathDocument document;
    document.id = 4101ULL;
    document.name = "M4 Z-Level Toolpath - Mesh First Version";
    document.visible = true;

    std::uint32_t levelCount = 0U;
    for (double z = bounds.max.z - options.stockToLeave; z >= bounds.min.z - 1.0e-7; z -= options.zStep)
    {
        Plane3d plane;
        plane.origin = {0.0, 0.0, z};
        plane.normal = {0.0, 0.0, 1.0};
        std::vector<Polyline3d> sections;
        if (geometry.IntersectPlane(plane, sections))
        {
            std::sort(sections.begin(), sections.end(), [](const Polyline3d& a, const Polyline3d& b) {
                return MidPoint(a.points.front(), a.points.back()).x < MidPoint(b.points.front(), b.points.back()).x;
            });
            for (const Polyline3d& section : sections)
            {
                AppendPolylineMoves(document, section, options.safeZ, options.feedRate, options.rapidFeedRate);
            }
        }
        ++levelCount;
        if (levelCount >= options.maxScanLines) { break; }
    }

    if (document.moves.empty())
    {
        result.warnings.push_back("Z-Level produced no section moves. Check mesh preview payload and z step.");
        return result;
    }

    std::ostringstream warning;
    warning << "M4 Z-Level uses MeshCamGeometryProvider plane slicing; levels=" << levelCount << ", moves=" << document.moves.size();
    result.warnings.push_back(warning.str());
    result.document = std::move(document);
    result.success = true;
    return result;
}

ToolpathResult MeshParallelFinishingPlanner::Calculate(const ICamGeometryProvider& geometry, const CamSetup&, const MeshToolpathOptions& rawOptions) const
{
    ToolpathResult result;
    if (geometry.GetKind() != CamGeometryKind::Mesh)
    {
        result.warnings.push_back("Parallel Finishing requires MeshCamGeometryProvider.");
        return result;
    }

    const BoundingBox3d bounds = geometry.GetBoundingBox();
    if (!bounds.IsValid())
    {
        result.warnings.push_back("Parallel Finishing failed: invalid mesh bounding box.");
        return result;
    }

    const MeshToolpathOptions options = NormalizeOptions(bounds, rawOptions);
    ToolpathDocument document;
    document.id = 4102ULL;
    document.name = "M4 Parallel Finishing Toolpath - Mesh First Version";
    document.visible = true;

    const double widthY = std::max(bounds.max.y - bounds.min.y, options.stepover);
    const std::uint32_t lineCount = std::min<std::uint32_t>(options.maxScanLines, static_cast<std::uint32_t>(std::floor(widthY / options.stepover)) + 1U);
    const double widthX = std::max(bounds.max.x - bounds.min.x, options.sampleStep);
    const std::uint32_t sampleCount = std::min<std::uint32_t>(options.maxSamplesPerLine, static_cast<std::uint32_t>(std::floor(widthX / options.sampleStep)) + 1U);

    for (std::uint32_t line = 0U; line < lineCount; ++line)
    {
        const double y = lineCount <= 1U ? bounds.Center().y : bounds.min.y + (static_cast<double>(line) / static_cast<double>(lineCount - 1U)) * (bounds.max.y - bounds.min.y);
        Polyline3d polyline;
        polyline.points.reserve(sampleCount);
        const bool reverse = (line % 2U) != 0U;
        for (std::uint32_t sample = 0U; sample < sampleCount; ++sample)
        {
            const std::uint32_t logicalSample = reverse ? (sampleCount - 1U - sample) : sample;
            const double x = sampleCount <= 1U ? bounds.Center().x : bounds.min.x + (static_cast<double>(logicalSample) / static_cast<double>(sampleCount - 1U)) * (bounds.max.x - bounds.min.x);
            double z = 0.0;
            if (geometry.QueryHeightAlongZ(x, y, z))
            {
                polyline.points.push_back({x, y, z + options.stockToLeave});
            }
            else if (!polyline.points.empty())
            {
                AppendPolylineMoves(document, polyline, options.safeZ, options.feedRate, options.rapidFeedRate);
                polyline.points.clear();
            }
        }
        AppendPolylineMoves(document, polyline, options.safeZ, options.feedRate, options.rapidFeedRate);
    }

    if (document.moves.empty())
    {
        result.warnings.push_back("Parallel Finishing produced no moves. Check XY coverage and mesh preview payload.");
        return result;
    }

    std::ostringstream warning;
    warning << "M4 Parallel Finishing uses QueryHeightAlongZ; scanLines=" << lineCount << ", samplesPerLine=" << sampleCount << ", moves=" << document.moves.size();
    result.warnings.push_back(warning.str());
    result.document = std::move(document);
    result.success = true;
    return result;
}

std::vector<PlannerDescriptor> CreateM0PlannerMatrix()
{
    return {
        {CamOperationType::Roughing3D, "Mesh 3D Roughing Planner", false},
        {CamOperationType::ZLevelFinishing, "Z-Level Finishing Planner", false},
        {CamOperationType::ParallelFinishing, "Parallel Finishing Planner", false},
        {CamOperationType::Projection, "Projection Toolpath Planner", false},
        {CamOperationType::Pencil, "Pencil Cleanup Planner", false},
        {CamOperationType::Drilling, "Drilling Planner Stub", false}};
}

std::vector<PlannerDescriptor> CreateM4PlannerMatrix()
{
    return {
        {CamOperationType::Roughing3D, "Mesh 3D Roughing Planner Stub", false},
        {CamOperationType::ZLevelFinishing, "Z-Level Finishing Planner V1", true},
        {CamOperationType::ParallelFinishing, "Parallel Finishing Planner V1", true},
        {CamOperationType::Projection, "Projection Toolpath Planner Stub", false},
        {CamOperationType::Pencil, "Pencil Cleanup Planner Stub", false},
        {CamOperationType::Drilling, "Drilling Planner Stub", false}};
}
}

// M7 BRep CAM planners keep the toolpath layer independent from STEP files and OCCT internals.
namespace skylark::cam
{
namespace
{
void AppendM7PolylineMoves(ToolpathDocument& document, const Polyline3d& polyline, double safeZ, double feedRate, double rapidFeedRate)
{
    if (polyline.points.size() < 2U) { return; }
    const Vec3d start = polyline.points.front();
    document.moves.push_back({ToolpathMoveKind::Rapid, {start.x, start.y, safeZ}, rapidFeedRate});
    document.moves.push_back({ToolpathMoveKind::LeadIn, start, feedRate});
    for (std::size_t i = 1U; i < polyline.points.size(); ++i)
    {
        document.moves.push_back({ToolpathMoveKind::Feed, polyline.points[i], feedRate});
    }
    const Vec3d end = polyline.points.back();
    document.moves.push_back({ToolpathMoveKind::LeadOut, end, feedRate});
    document.moves.push_back({ToolpathMoveKind::Retract, {end.x, end.y, safeZ}, rapidFeedRate});
}

Polyline3d ShrinkRectangle(const BoundingBox3d& bounds, double offset, double z)
{
    Polyline3d p;
    const double maxOffset = std::max(0.0, std::min(bounds.Size().x, bounds.Size().y) * 0.45);
    const double o = std::min(std::max(0.0, offset), maxOffset);
    p.points = {
        {bounds.min.x + o, bounds.min.y + o, z},
        {bounds.max.x - o, bounds.min.y + o, z},
        {bounds.max.x - o, bounds.max.y - o, z},
        {bounds.min.x + o, bounds.max.y - o, z},
        {bounds.min.x + o, bounds.min.y + o, z}};
    return p;
}
}

ToolpathResult Brep25DContourPlanner::Calculate(const BRepCamGeometryProvider& geometry, const CamSetup&, const BrepToolpathOptions& rawOptions) const
{
    ToolpathResult result;
    const BoundingBox3d bounds = geometry.GetBoundingBox();
    if (!bounds.IsValid())
    {
        result.warnings.push_back("M7 2.5D contour failed: invalid BRep bounds.");
        return result;
    }

    BrepToolpathOptions options = rawOptions;
    options.safeZ = std::max(options.safeZ, bounds.max.z + 10.0);
    const double z = rawOptions.contourZ == 0.0 ? bounds.max.z : rawOptions.contourZ;
    Plane3d plane;
    plane.origin = {0.0, 0.0, z};
    plane.normal = {0.0, 0.0, 1.0};
    std::vector<Polyline3d> sections;
    if (!geometry.IntersectPlane(plane, sections))
    {
        result.warnings.push_back("M7 2.5D contour failed: no planar section found.");
        return result;
    }

    ToolpathDocument document;
    document.id = 7101ULL;
    document.name = "M7 2.5D Contour - BRep V1";
    document.visible = true;
    for (const Polyline3d& section : sections)
    {
        AppendM7PolylineMoves(document, section, options.safeZ, options.feedRate, options.rapidFeedRate);
    }
    result.success = !document.moves.empty();
    result.document = std::move(document);
    result.warnings.push_back("M7 2.5D contour generated from BRepCamGeometryProvider plane section.");
    return result;
}

ToolpathResult BrepPocketPlannerStub::Calculate(const BRepCamGeometryProvider& geometry, const CamSetup&, const BrepToolpathOptions& rawOptions) const
{
    ToolpathResult result;
    const BoundingBox3d bounds = geometry.GetBoundingBox();
    if (!bounds.IsValid())
    {
        result.warnings.push_back("M7 pocket stub failed: invalid BRep bounds.");
        return result;
    }

    BrepToolpathOptions options = rawOptions;
    options.safeZ = std::max(options.safeZ, bounds.max.z + 10.0);
    const double depth = std::max(options.pocketDepth, 1.0);
    const double stepdown = std::max(options.pocketStepdown, 0.5);
    const double stepover = std::max(options.stepover, 1.0);

    ToolpathDocument document;
    document.id = 7102ULL;
    document.name = "M7 Pocket Stub - BRep V1";
    document.visible = true;
    std::uint32_t level = 0U;
    for (double z = bounds.max.z - stepdown; z >= bounds.max.z - depth - 1.0e-7; z -= stepdown)
    {
        const double levelOffset = static_cast<double>(level) * stepover * 0.25;
        for (double offset = stepover + levelOffset; offset < std::min(bounds.Size().x, bounds.Size().y) * 0.42; offset += stepover)
        {
            AppendM7PolylineMoves(document, ShrinkRectangle(bounds, offset, z), options.safeZ, options.feedRate, options.rapidFeedRate);
        }
        ++level;
        if (level > 16U) { break; }
    }
    result.success = !document.moves.empty();
    if (!result.success)
    {
        result.warnings.push_back("M7 pocket stub produced no moves; check stock size and stepover.");
        return result;
    }
    result.document = std::move(document);
    result.warnings.push_back("M7 pocket stub generated rectangular offset passes from BRep top face diagnostics.");
    return result;
}

ToolpathResult BrepDrillingPlannerStub::Calculate(const BRepCamGeometryProvider& geometry, const CamSetup&, const BrepToolpathOptions& rawOptions) const
{
    ToolpathResult result;
    const BoundingBox3d bounds = geometry.GetBoundingBox();
    if (!bounds.IsValid())
    {
        result.warnings.push_back("M7 drilling stub failed: invalid BRep bounds.");
        return result;
    }

    BrepToolpathOptions options = rawOptions;
    options.safeZ = std::max(options.safeZ, bounds.max.z + 10.0);
    ToolpathDocument document;
    document.id = 7103ULL;
    document.name = "M7 Drilling Stub - BRep Hole Candidates";
    document.visible = true;

    const std::vector<BrepHoleFeature>& holes = geometry.GetModel().GetHoles();
    for (const BrepHoleFeature& hole : holes)
    {
        const Vec3d top = {hole.center.x, hole.center.y, bounds.max.z};
        const Vec3d bottom = {hole.center.x, hole.center.y, std::max(bounds.min.z, bounds.max.z - hole.depth)};
        document.moves.push_back({ToolpathMoveKind::Rapid, {top.x, top.y, options.safeZ}, options.rapidFeedRate});
        document.moves.push_back({ToolpathMoveKind::LeadIn, top, options.feedRate});
        document.moves.push_back({ToolpathMoveKind::Feed, bottom, options.feedRate});
        document.moves.push_back({ToolpathMoveKind::Retract, {top.x, top.y, options.safeZ}, options.rapidFeedRate});
    }

    result.success = !document.moves.empty();
    if (!result.success)
    {
        result.warnings.push_back("M7 drilling stub produced no moves; no hole candidates were found.");
        return result;
    }
    result.document = std::move(document);
    result.warnings.push_back("M7 drilling stub generated drill cycles as polyline moves from BRep hole diagnostics.");
    return result;
}

std::vector<PlannerDescriptor> CreateM7PlannerMatrix()
{
    std::vector<PlannerDescriptor> matrix = CreateM4PlannerMatrix();
    matrix.push_back({CamOperationType::Contour2D, "BRep 2.5D Contour Planner V1", true});
    matrix.push_back({CamOperationType::Pocket2D, "BRep Pocket Planner Stub", true});
    matrix.push_back({CamOperationType::Drilling, "BRep Drilling Planner Stub", true});
    return matrix;
}
}
