#pragma once

#include "Common/CamCore.h"

#include <cstdint>
#include <string>
#include <vector>

namespace skylark::cam
{
using ToolId = std::uint64_t;
using CamOperationId = std::uint64_t;
using ToolpathId = std::uint64_t;

enum class CutterKind { FlatEndMill, BallEndMill, BullNoseEndMill, Drill, ChamferMill };
enum class ToolpathMoveKind { Rapid, Feed, LeadIn, LeadOut, Retract };

struct WorkCoordinateSystem { std::string name{"G54 Top"}; Vec3d origin{0.0, 0.0, 30.0}; Vec3d xAxis{1.0,0.0,0.0}; Vec3d yAxis{0.0,1.0,0.0}; Vec3d zAxis{0.0,0.0,1.0}; };
struct StockDefinition { std::string name{"Box Stock"}; double sizeX{120.0}; double sizeY{80.0}; double sizeZ{40.0}; double offset{2.0}; };
struct FixtureDefinition { std::string name{"Vise Fixture Stub"}; bool collisionEnabled{true}; };
struct MachineDefinition { std::string name{"Generic 3-Axis Vertical Mill"}; int axisCount{3}; double maxFeed{8000.0}; double maxSpindleRpm{12000.0}; };
struct CamSetup { std::string name{"Setup 1"}; WorkCoordinateSystem wcs; StockDefinition stock; FixtureDefinition fixture; MachineDefinition machine; };
struct ToolDefinition { ToolId id{0}; std::string name; CutterKind kind{CutterKind::FlatEndMill}; double diameter{10.0}; double cornerRadius{0.0}; double fluteLength{30.0}; double gaugeLength{60.0}; };
struct OperationNode { CamOperationId id{0}; std::string name; CamOperationType type{CamOperationType::Roughing3D}; OperationState state{OperationState::Draft}; bool suppressed{false}; };
struct ToolpathMove { ToolpathMoveKind kind{ToolpathMoveKind::Feed}; Vec3d point{}; double feedRate{1000.0}; };
struct ToolpathDocument { ToolpathId id{0}; std::string name; std::vector<ToolpathMove> moves; bool visible{true}; };

class ToolLibrary
{
public:
    void AddTool(ToolDefinition tool);
    void Clear();
    ToolId GetNextAvailableToolId() const noexcept;
    const std::vector<ToolDefinition>& GetTools() const noexcept;
private:
    std::vector<ToolDefinition> m_tools;
};

class OperationTree
{
public:
    void AddOperation(OperationNode op);
    void Clear();
    CamOperationId GetNextAvailableOperationId() const noexcept;
    const std::vector<OperationNode>& GetOperations() const noexcept;
private:
    std::vector<OperationNode> m_operations;
};

class ICamGeometryProvider
{
public:
    virtual ~ICamGeometryProvider() = default;
    virtual CamGeometryKind GetKind() const = 0;
    virtual BoundingBox3d GetBoundingBox() const = 0;
    virtual bool RayIntersect(const Ray3d& ray, CamIntersectionResult& result) const = 0;
    virtual bool IntersectPlane(const Plane3d& plane, std::vector<Polyline3d>& sectionCurves) const = 0;
    virtual bool QueryHeightAlongZ(double x, double y, double& z) const = 0;
    virtual bool QueryTrianglesInBox(const BoundingBox3d& box, std::vector<std::uint32_t>& triangleIds) const = 0;
};

struct OperationValidationReport { bool valid{false}; std::vector<std::string> messages; };
struct ToolpathResult { bool success{false}; ToolpathDocument document; std::vector<std::string> warnings; };
class IProgressReporter { public: virtual ~IProgressReporter() = default; virtual void ReportProgress(double progress, const std::string& message) = 0; };
class ICancellationToken { public: virtual ~ICancellationToken() = default; virtual bool IsCancellationRequested() const = 0; };
class ICamOperation
{
public:
    virtual ~ICamOperation() = default;
    virtual CamOperationType GetType() const = 0;
    virtual OperationState GetState() const = 0;
    virtual bool Validate(const class CamProject& project, OperationValidationReport& report) const = 0;
    virtual ToolpathResult Calculate(const ICamGeometryProvider& geometry, const CamSetup& setup, const ToolLibrary& tools, IProgressReporter& progress, ICancellationToken& cancellation) = 0;
};

CamSetup CreateDefaultSetup();
ToolLibrary CreateDefaultToolLibrary();
OperationTree CreateDefaultOperationTree();
ToolpathDocument CreatePlaceholderToolpathDocument();
std::string ToString(CutterKind kind);
std::string ToString(ToolpathMoveKind kind);
}
