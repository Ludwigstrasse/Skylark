#pragma once

#include "CAM/CamDomain.h"
#include "Common/CamCore.h"

#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

namespace skylark::cam
{
enum class BrepFaceKind { Planar, CylindricalStub, Unknown };
enum class BrepEdgeKind { Line, CircleStub, Unknown };

struct BrepFace
{
    std::uint32_t id{0};
    std::string name;
    BrepFaceKind kind{BrepFaceKind::Planar};
    Vec3d origin{};
    Vec3d normal{0.0, 0.0, 1.0};
    BoundingBox3d bounds;
    bool selectable{true};
};

struct BrepEdge
{
    std::uint32_t id{0};
    std::string name;
    BrepEdgeKind kind{BrepEdgeKind::Line};
    Vec3d start{};
    Vec3d end{};
    std::vector<std::uint32_t> adjacentFaceIds;
    bool selectable{true};
};

struct BrepHoleFeature
{
    std::uint32_t id{0};
    std::string name;
    Vec3d center{};
    double diameter{6.0};
    double depth{12.0};
};

struct BrepPocketFeature
{
    std::uint32_t id{0};
    std::string name;
    BoundingBox3d bounds;
    double depth{4.0};
};

struct BrepSelectionState
{
    std::vector<std::uint32_t> faceIds;
    std::vector<std::uint32_t> edgeIds;
};

struct BrepFeatureDiagnostics
{
    bool valid{false};
    std::uint32_t bodyCount{0};
    std::uint32_t faceCount{0};
    std::uint32_t edgeCount{0};
    std::uint32_t planarFaceCount{0};
    std::uint32_t cylindricalFaceCount{0};
    std::uint32_t holeCount{0};
    std::uint32_t pocketCount{0};
    BoundingBox3d bounds;
    std::vector<std::string> warnings;
    std::string summary;
};

class BrepModel
{
public:
    void SetName(std::string name);
    void SetSourcePath(std::string sourcePath);
    void SetBounds(BoundingBox3d bounds);
    void AddFace(BrepFace face);
    void AddEdge(BrepEdge edge);
    void AddHole(BrepHoleFeature hole);
    void AddPocket(BrepPocketFeature pocket);
    void SetStepEntityCounters(std::uint32_t pointCount, std::uint32_t advancedFaceCount, std::uint32_t edgeCurveCount, std::uint32_t circleCount);

    const std::string& GetName() const noexcept;
    const std::string& GetSourcePath() const noexcept;
    const BoundingBox3d& GetBounds() const noexcept;
    const std::vector<BrepFace>& GetFaces() const noexcept;
    const std::vector<BrepEdge>& GetEdges() const noexcept;
    const std::vector<BrepHoleFeature>& GetHoles() const noexcept;
    const std::vector<BrepPocketFeature>& GetPockets() const noexcept;
    std::uint32_t GetStepPointCount() const noexcept;
    std::uint32_t GetStepAdvancedFaceCount() const noexcept;
    std::uint32_t GetStepEdgeCurveCount() const noexcept;
    std::uint32_t GetStepCircleCount() const noexcept;

private:
    std::string m_name{"BRep Model"};
    std::string m_sourcePath;
    BoundingBox3d m_bounds;
    std::vector<BrepFace> m_faces;
    std::vector<BrepEdge> m_edges;
    std::vector<BrepHoleFeature> m_holes;
    std::vector<BrepPocketFeature> m_pockets;
    std::uint32_t m_stepPointCount{0};
    std::uint32_t m_stepAdvancedFaceCount{0};
    std::uint32_t m_stepEdgeCurveCount{0};
    std::uint32_t m_stepCircleCount{0};
};

struct StepBrepImportResult
{
    bool success{false};
    BrepModel model;
    BrepFeatureDiagnostics diagnostics;
    std::vector<std::string> messages;
};

class StepBrepImporter
{
public:
    StepBrepImportResult ImportStepFile(const std::filesystem::path& path) const;
};

class BRepCamGeometryProvider final : public ICamGeometryProvider
{
public:
    explicit BRepCamGeometryProvider(BrepModel model);

    CamGeometryKind GetKind() const override;
    BoundingBox3d GetBoundingBox() const override;
    bool RayIntersect(const Ray3d& ray, CamIntersectionResult& result) const override;
    bool IntersectPlane(const Plane3d& plane, std::vector<Polyline3d>& sectionCurves) const override;
    bool QueryHeightAlongZ(double x, double y, double& z) const override;
    bool QueryTrianglesInBox(const BoundingBox3d& box, std::vector<std::uint32_t>& triangleIds) const override;

    const BrepModel& GetModel() const noexcept;
    const BrepFeatureDiagnostics& GetDiagnostics() const noexcept;
    bool SelectFace(std::uint32_t faceId);
    bool SelectEdge(std::uint32_t edgeId);
    void ClearSelection();
    const BrepSelectionState& GetSelection() const noexcept;

private:
    BrepModel m_model;
    BrepFeatureDiagnostics m_diagnostics;
    BrepSelectionState m_selection;
};

BrepModel CreateAnalyticalBoxBrepModel(std::string name, BoundingBox3d bounds);
BrepModel CreateM7SampleBrepModel();
BrepFeatureDiagnostics BuildBrepFeatureDiagnostics(const BrepModel& model);
std::string ToString(BrepFaceKind kind);
std::string ToString(BrepEdgeKind kind);
}
