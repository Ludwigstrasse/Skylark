#include "CAM/MeshCamGeometryProvider.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <utility>

namespace skylark::cam
{
namespace
{
constexpr double kEpsilon = 1.0e-9;

BoundingBox3d ComputeTriangleBounds(const MeshTriangle3d& triangle) noexcept
{
    BoundingBox3d box;
    box.Expand(triangle.p0);
    box.Expand(triangle.p1);
    box.Expand(triangle.p2);
    return box;
}

bool BoxesIntersect(const BoundingBox3d& a, const BoundingBox3d& b) noexcept
{
    if (!a.IsValid() || !b.IsValid()) { return false; }
    return !(a.max.x < b.min.x || a.min.x > b.max.x ||
             a.max.y < b.min.y || a.min.y > b.max.y ||
             a.max.z < b.min.z || a.min.z > b.max.z);
}

bool IntersectRayTriangle(const Ray3d& ray, const MeshTriangle3d& triangle, double& distance) noexcept
{
    const Vec3d edge1 = triangle.p1 - triangle.p0;
    const Vec3d edge2 = triangle.p2 - triangle.p0;
    const Vec3d pvec = Cross(ray.direction, edge2);
    const double det = Dot(edge1, pvec);
    if (std::abs(det) < kEpsilon) { return false; }

    const double invDet = 1.0 / det;
    const Vec3d tvec = ray.origin - triangle.p0;
    const double u = Dot(tvec, pvec) * invDet;
    if (u < -kEpsilon || u > 1.0 + kEpsilon) { return false; }

    const Vec3d qvec = Cross(tvec, edge1);
    const double v = Dot(ray.direction, qvec) * invDet;
    if (v < -kEpsilon || u + v > 1.0 + kEpsilon) { return false; }

    distance = Dot(edge2, qvec) * invDet;
    return distance >= 0.0;
}

bool AddPlaneEdgeIntersection(const Vec3d& a, const Vec3d& b, double da, double db, std::vector<Vec3d>& points)
{
    if (std::abs(da) <= kEpsilon && std::abs(db) <= kEpsilon)
    {
        points.push_back(a);
        points.push_back(b);
        return true;
    }
    if (std::abs(da) <= kEpsilon)
    {
        points.push_back(a);
        return true;
    }
    if (std::abs(db) <= kEpsilon)
    {
        points.push_back(b);
        return true;
    }
    if ((da < 0.0 && db > 0.0) || (da > 0.0 && db < 0.0))
    {
        const double t = da / (da - db);
        points.push_back(a + ((b - a) * t));
        return true;
    }
    return false;
}

bool IsSamePoint(const Vec3d& a, const Vec3d& b) noexcept
{
    return Length(a - b) <= 1.0e-7;
}

void UniquePoints(std::vector<Vec3d>& points)
{
    std::vector<Vec3d> unique;
    for (const Vec3d& point : points)
    {
        const bool exists = std::any_of(unique.begin(), unique.end(), [&](const Vec3d& p) { return IsSamePoint(p, point); });
        if (!exists) { unique.push_back(point); }
    }
    points = std::move(unique);
}

bool PointInProjectedTriangleXY(const MeshTriangle3d& triangle, double x, double y, double& z) noexcept
{
    const double x0 = triangle.p0.x;
    const double y0 = triangle.p0.y;
    const double x1 = triangle.p1.x;
    const double y1 = triangle.p1.y;
    const double x2 = triangle.p2.x;
    const double y2 = triangle.p2.y;
    const double denom = (y1 - y2) * (x0 - x2) + (x2 - x1) * (y0 - y2);
    if (std::abs(denom) < kEpsilon) { return false; }

    const double a = ((y1 - y2) * (x - x2) + (x2 - x1) * (y - y2)) / denom;
    const double b = ((y2 - y0) * (x - x2) + (x0 - x2) * (y - y2)) / denom;
    const double c = 1.0 - a - b;
    if (a < -1.0e-7 || b < -1.0e-7 || c < -1.0e-7) { return false; }

    z = a * triangle.p0.z + b * triangle.p1.z + c * triangle.p2.z;
    return true;
}
}

MeshCamGeometryProvider::MeshCamGeometryProvider(const LargeMeshAsset& asset) : m_asset(asset) {}

CamGeometryKind MeshCamGeometryProvider::GetKind() const { return CamGeometryKind::Mesh; }

BoundingBox3d MeshCamGeometryProvider::GetBoundingBox() const { return m_asset.GetBounds(); }

bool MeshCamGeometryProvider::RayIntersect(const Ray3d& ray, CamIntersectionResult& result) const
{
    const std::vector<MeshTriangle3d>& triangles = m_asset.GetPreviewTriangles();
    bool found = false;
    double bestDistance = std::numeric_limits<double>::max();
    CamIntersectionResult best;
    for (std::uint32_t i = 0; i < static_cast<std::uint32_t>(triangles.size()); ++i)
    {
        double distance = 0.0;
        if (IntersectRayTriangle(ray, triangles[i], distance) && distance < bestDistance)
        {
            bestDistance = distance;
            best.hit = true;
            best.distance = distance;
            best.point = ray.origin + (ray.direction * distance);
            best.normal = Normalize(triangles[i].normal);
            if (Length(best.normal) <= kEpsilon)
            {
                best.normal = Normalize(Cross(triangles[i].p1 - triangles[i].p0, triangles[i].p2 - triangles[i].p0));
            }
            best.primitiveId = i;
            found = true;
        }
    }
    result = best;
    return found;
}

bool MeshCamGeometryProvider::IntersectPlane(const Plane3d& plane, std::vector<Polyline3d>& sectionCurves) const
{
    sectionCurves.clear();
    const Vec3d normal = Normalize(plane.normal);
    if (Length(normal) <= kEpsilon) { return false; }

    for (const MeshTriangle3d& triangle : m_asset.GetPreviewTriangles())
    {
        const double d0 = Dot(triangle.p0 - plane.origin, normal);
        const double d1 = Dot(triangle.p1 - plane.origin, normal);
        const double d2 = Dot(triangle.p2 - plane.origin, normal);
        std::vector<Vec3d> points;
        points.reserve(4U);
        AddPlaneEdgeIntersection(triangle.p0, triangle.p1, d0, d1, points);
        AddPlaneEdgeIntersection(triangle.p1, triangle.p2, d1, d2, points);
        AddPlaneEdgeIntersection(triangle.p2, triangle.p0, d2, d0, points);
        UniquePoints(points);
        if (points.size() >= 2U)
        {
            Polyline3d segment;
            segment.points.push_back(points[0]);
            segment.points.push_back(points[1]);
            sectionCurves.push_back(std::move(segment));
        }
    }
    return !sectionCurves.empty();
}

bool MeshCamGeometryProvider::QueryHeightAlongZ(double x, double y, double& z) const
{
    bool found = false;
    double bestZ = -std::numeric_limits<double>::max();
    for (const MeshTriangle3d& triangle : m_asset.GetPreviewTriangles())
    {
        double candidateZ = 0.0;
        if (PointInProjectedTriangleXY(triangle, x, y, candidateZ) && candidateZ > bestZ)
        {
            bestZ = candidateZ;
            found = true;
        }
    }
    if (found) { z = bestZ; }
    return found;
}

bool MeshCamGeometryProvider::QueryTrianglesInBox(const BoundingBox3d& box, std::vector<std::uint32_t>& triangleIds) const
{
    triangleIds.clear();
    const std::vector<MeshTriangle3d>& triangles = m_asset.GetPreviewTriangles();
    for (std::uint32_t i = 0; i < static_cast<std::uint32_t>(triangles.size()); ++i)
    {
        if (BoxesIntersect(ComputeTriangleBounds(triangles[i]), box))
        {
            triangleIds.push_back(i);
        }
    }
    return !triangleIds.empty();
}

MeshCamGeometryStatistics MeshCamGeometryProvider::GetStatistics() const
{
    MeshCamGeometryStatistics stats;
    stats.sourceTriangleCount = m_asset.GetTriangleCount();
    stats.queryTriangleCount = static_cast<std::uint32_t>(m_asset.GetPreviewTriangles().size());
    stats.chunkCount = static_cast<std::uint32_t>(m_asset.GetChunks().size());
    stats.bounds = m_asset.GetBounds();
    stats.usesPreviewPayload = true;
    return stats;
}
}
