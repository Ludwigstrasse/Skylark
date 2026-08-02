#pragma once

#include <cstdint>
#include <limits>
#include <string>
#include <string_view>
#include <vector>

namespace skylark::cam
{
struct Vec3d
{
    double x{0.0};
    double y{0.0};
    double z{0.0};
};

Vec3d operator+(const Vec3d& lhs, const Vec3d& rhs) noexcept;
Vec3d operator-(const Vec3d& lhs, const Vec3d& rhs) noexcept;
Vec3d operator*(const Vec3d& value, double scale) noexcept;
double Dot(const Vec3d& lhs, const Vec3d& rhs) noexcept;
Vec3d Cross(const Vec3d& lhs, const Vec3d& rhs) noexcept;
double Length(const Vec3d& value) noexcept;
Vec3d Normalize(const Vec3d& value) noexcept;

struct BoundingBox3d
{
    Vec3d min{std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max()};
    Vec3d max{-std::numeric_limits<double>::max(), -std::numeric_limits<double>::max(), -std::numeric_limits<double>::max()};
    void Expand(const Vec3d& p) noexcept;
    void Expand(const BoundingBox3d& box) noexcept;
    bool IsValid() const noexcept;
    Vec3d Center() const noexcept;
    Vec3d Size() const noexcept;
    double DiagonalLength() const noexcept;
};

struct Ray3d { Vec3d origin{}; Vec3d direction{0.0, 0.0, 1.0}; };
struct Plane3d { Vec3d origin{}; Vec3d normal{0.0, 0.0, 1.0}; };
struct Polyline3d { std::vector<Vec3d> points; };

enum class CamGeometryKind { Unknown, BRep, Mesh, Assembly, Lightweight };
enum class CamOperationType { Roughing3D, ZLevelFinishing, ParallelFinishing, Projection, Pencil, Drilling, Contour2D, Pocket2D };
enum class OperationState { Draft, Valid, Dirty, Calculating, Failed, Calculated, Suppressed };

struct CamIntersectionResult
{
    bool hit{false};
    double distance{0.0};
    Vec3d point{};
    Vec3d normal{0.0, 0.0, 1.0};
    std::uint32_t primitiveId{0};
};

std::string_view GetWorkbenchName() noexcept;
std::string_view GetWorkbenchMilestone() noexcept;
std::string_view GetWorkbenchVersionText() noexcept;
std::string ToString(CamGeometryKind kind);
std::string ToString(CamOperationType type);
std::string ToString(OperationState state);
}
