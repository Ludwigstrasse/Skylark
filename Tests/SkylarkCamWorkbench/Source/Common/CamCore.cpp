#include "Common/CamCore.h"

#include <algorithm>
#include <cmath>

namespace skylark::cam
{
Vec3d operator+(const Vec3d& lhs, const Vec3d& rhs) noexcept
{
    return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
}

Vec3d operator-(const Vec3d& lhs, const Vec3d& rhs) noexcept
{
    return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}

Vec3d operator*(const Vec3d& value, double scale) noexcept
{
    return {value.x * scale, value.y * scale, value.z * scale};
}

double Dot(const Vec3d& lhs, const Vec3d& rhs) noexcept
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

Vec3d Cross(const Vec3d& lhs, const Vec3d& rhs) noexcept
{
    return {
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x};
}

double Length(const Vec3d& value) noexcept
{
    return std::sqrt(Dot(value, value));
}

Vec3d Normalize(const Vec3d& value) noexcept
{
    const double length = Length(value);
    if (length <= 1.0e-12) { return {0.0, 0.0, 0.0}; }
    return {value.x / length, value.y / length, value.z / length};
}

void BoundingBox3d::Expand(const Vec3d& p) noexcept
{
    min.x = std::min(min.x, p.x);
    min.y = std::min(min.y, p.y);
    min.z = std::min(min.z, p.z);
    max.x = std::max(max.x, p.x);
    max.y = std::max(max.y, p.y);
    max.z = std::max(max.z, p.z);
}

void BoundingBox3d::Expand(const BoundingBox3d& box) noexcept
{
    if (!box.IsValid()) { return; }
    Expand(box.min);
    Expand(box.max);
}

bool BoundingBox3d::IsValid() const noexcept
{
    return min.x <= max.x && min.y <= max.y && min.z <= max.z;
}

Vec3d BoundingBox3d::Center() const noexcept
{
    if (!IsValid()) { return {}; }
    return {(min.x + max.x) * 0.5, (min.y + max.y) * 0.5, (min.z + max.z) * 0.5};
}

Vec3d BoundingBox3d::Size() const noexcept
{
    if (!IsValid()) { return {}; }
    return {max.x - min.x, max.y - min.y, max.z - min.z};
}

double BoundingBox3d::DiagonalLength() const noexcept
{
    return Length(Size());
}

std::string_view GetWorkbenchName() noexcept { return "SkylarkCamWorkbench"; }
std::string_view GetWorkbenchMilestone() noexcept { return "M7"; }
std::string_view GetWorkbenchVersionText() noexcept { return "SkylarkCamWorkbench M7 - BRep CAM Channel"; }

std::string ToString(CamGeometryKind kind)
{
    switch (kind)
    {
    case CamGeometryKind::BRep: return "BRep";
    case CamGeometryKind::Mesh: return "Mesh";
    case CamGeometryKind::Assembly: return "Assembly";
    case CamGeometryKind::Lightweight: return "Lightweight";
    default: return "Unknown";
    }
}

std::string ToString(CamOperationType type)
{
    switch (type)
    {
    case CamOperationType::Roughing3D: return "3D Roughing";
    case CamOperationType::ZLevelFinishing: return "Z-Level Finishing";
    case CamOperationType::ParallelFinishing: return "Parallel Finishing";
    case CamOperationType::Projection: return "Projection";
    case CamOperationType::Pencil: return "Pencil";
    case CamOperationType::Drilling: return "Drilling";
    case CamOperationType::Contour2D: return "2.5D Contour";
    case CamOperationType::Pocket2D: return "2.5D Pocket";
    default: return "Unknown";
    }
}

std::string ToString(OperationState state)
{
    switch (state)
    {
    case OperationState::Draft: return "Draft";
    case OperationState::Valid: return "Valid";
    case OperationState::Dirty: return "Dirty";
    case OperationState::Calculating: return "Calculating";
    case OperationState::Failed: return "Failed";
    case OperationState::Calculated: return "Calculated";
    case OperationState::Suppressed: return "Suppressed";
    default: return "Unknown";
    }
}
}
