#pragma once

#include "CAM/CamDomain.h"
#include "LargeMesh/LargeMesh.h"

#include <cstdint>
#include <vector>

namespace skylark::cam
{
struct MeshCamGeometryStatistics
{
    std::uint64_t sourceTriangleCount{0};
    std::uint32_t queryTriangleCount{0};
    std::uint32_t chunkCount{0};
    BoundingBox3d bounds;
    bool usesPreviewPayload{true};
};

class MeshCamGeometryProvider final : public ICamGeometryProvider
{
public:
    explicit MeshCamGeometryProvider(const LargeMeshAsset& asset);

    CamGeometryKind GetKind() const override;
    BoundingBox3d GetBoundingBox() const override;
    bool RayIntersect(const Ray3d& ray, CamIntersectionResult& result) const override;
    bool IntersectPlane(const Plane3d& plane, std::vector<Polyline3d>& sectionCurves) const override;
    bool QueryHeightAlongZ(double x, double y, double& z) const override;
    bool QueryTrianglesInBox(const BoundingBox3d& box, std::vector<std::uint32_t>& triangleIds) const override;

    MeshCamGeometryStatistics GetStatistics() const;

private:
    const LargeMeshAsset& m_asset;
};
}
