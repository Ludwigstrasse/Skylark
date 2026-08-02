#include "CAM/BRepCamGeometryProvider.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <limits>
#include <sstream>
#include <utility>

namespace skylark::cam
{
namespace
{
bool ContainsCaseInsensitive(const std::string& text, const std::string& token)
{
    auto it = std::search(
        text.begin(), text.end(),
        token.begin(), token.end(),
        [](char lhs, char rhs) { return std::toupper(static_cast<unsigned char>(lhs)) == std::toupper(static_cast<unsigned char>(rhs)); });
    return it != text.end();
}

std::vector<double> ExtractNumbers(const std::string& line)
{
    std::vector<double> values;
    const char* current = line.c_str();
    char* end = nullptr;
    while (*current != '\0')
    {
        if (*current == '+' || *current == '-' || *current == '.' || std::isdigit(static_cast<unsigned char>(*current)))
        {
            const double value = std::strtod(current, &end);
            if (end != current)
            {
                values.push_back(value);
                current = end;
                continue;
            }
        }
        ++current;
    }
    return values;
}

BoundingBox3d NormalizeBounds(BoundingBox3d bounds)
{
    if (!bounds.IsValid())
    {
        bounds.Expand(Vec3d{-60.0, -40.0, 0.0});
        bounds.Expand(Vec3d{60.0, 40.0, 30.0});
        return bounds;
    }

    Vec3d size = bounds.Size();
    if (size.x < 1.0) { bounds.min.x -= 50.0; bounds.max.x += 50.0; }
    if (size.y < 1.0) { bounds.min.y -= 30.0; bounds.max.y += 30.0; }
    if (size.z < 1.0) { bounds.min.z = std::min(bounds.min.z, 0.0); bounds.max.z = bounds.min.z + 25.0; }
    return bounds;
}

void AddBoxEdges(BrepModel& model, const BoundingBox3d& b)
{
    const Vec3d p000{b.min.x, b.min.y, b.min.z};
    const Vec3d p100{b.max.x, b.min.y, b.min.z};
    const Vec3d p110{b.max.x, b.max.y, b.min.z};
    const Vec3d p010{b.min.x, b.max.y, b.min.z};
    const Vec3d p001{b.min.x, b.min.y, b.max.z};
    const Vec3d p101{b.max.x, b.min.y, b.max.z};
    const Vec3d p111{b.max.x, b.max.y, b.max.z};
    const Vec3d p011{b.min.x, b.max.y, b.max.z};
    const std::pair<Vec3d, Vec3d> edges[] = {
        {p000, p100}, {p100, p110}, {p110, p010}, {p010, p000},
        {p001, p101}, {p101, p111}, {p111, p011}, {p011, p001},
        {p000, p001}, {p100, p101}, {p110, p111}, {p010, p011}};
    std::uint32_t id = 1U;
    for (const auto& edge : edges)
    {
        BrepEdge e;
        e.id = id;
        e.name = "E" + std::to_string(id);
        e.kind = BrepEdgeKind::Line;
        e.start = edge.first;
        e.end = edge.second;
        model.AddEdge(std::move(e));
        ++id;
    }
}

void AddBoxFaces(BrepModel& model, const BoundingBox3d& b)
{
    struct FaceSeed { const char* name; Vec3d normal; Vec3d origin; BoundingBox3d bounds; };
    BoundingBox3d bottom; bottom.Expand(Vec3d{b.min.x, b.min.y, b.min.z}); bottom.Expand(Vec3d{b.max.x, b.max.y, b.min.z});
    BoundingBox3d top; top.Expand(Vec3d{b.min.x, b.min.y, b.max.z}); top.Expand(Vec3d{b.max.x, b.max.y, b.max.z});
    BoundingBox3d left; left.Expand(Vec3d{b.min.x, b.min.y, b.min.z}); left.Expand(Vec3d{b.min.x, b.max.y, b.max.z});
    BoundingBox3d right; right.Expand(Vec3d{b.max.x, b.min.y, b.min.z}); right.Expand(Vec3d{b.max.x, b.max.y, b.max.z});
    BoundingBox3d front; front.Expand(Vec3d{b.min.x, b.min.y, b.min.z}); front.Expand(Vec3d{b.max.x, b.min.y, b.max.z});
    BoundingBox3d back; back.Expand(Vec3d{b.min.x, b.max.y, b.min.z}); back.Expand(Vec3d{b.max.x, b.max.y, b.max.z});
    const FaceSeed seeds[] = {
        {"Bottom Face", {0.0, 0.0, -1.0}, {0.0, 0.0, b.min.z}, bottom},
        {"Top Planar Face", {0.0, 0.0, 1.0}, {0.0, 0.0, b.max.z}, top},
        {"Left Face", {-1.0, 0.0, 0.0}, {b.min.x, 0.0, 0.0}, left},
        {"Right Face", {1.0, 0.0, 0.0}, {b.max.x, 0.0, 0.0}, right},
        {"Front Face", {0.0, -1.0, 0.0}, {0.0, b.min.y, 0.0}, front},
        {"Back Face", {0.0, 1.0, 0.0}, {0.0, b.max.y, 0.0}, back}};
    for (std::uint32_t i = 0U; i < 6U; ++i)
    {
        BrepFace face;
        face.id = i + 1U;
        face.name = seeds[i].name;
        face.kind = BrepFaceKind::Planar;
        face.normal = seeds[i].normal;
        face.origin = seeds[i].origin;
        face.bounds = seeds[i].bounds;
        model.AddFace(std::move(face));
    }
}

bool RayBoxIntersect(const Ray3d& ray, const BoundingBox3d& box, double& tHit)
{
    if (!box.IsValid()) { return false; }
    double tMin = 0.0;
    double tMax = std::numeric_limits<double>::max();
    const double origin[3] = {ray.origin.x, ray.origin.y, ray.origin.z};
    const double direction[3] = {ray.direction.x, ray.direction.y, ray.direction.z};
    const double minValue[3] = {box.min.x, box.min.y, box.min.z};
    const double maxValue[3] = {box.max.x, box.max.y, box.max.z};
    for (int axis = 0; axis < 3; ++axis)
    {
        if (std::abs(direction[axis]) < 1.0e-12)
        {
            if (origin[axis] < minValue[axis] || origin[axis] > maxValue[axis]) { return false; }
            continue;
        }
        const double inv = 1.0 / direction[axis];
        double t0 = (minValue[axis] - origin[axis]) * inv;
        double t1 = (maxValue[axis] - origin[axis]) * inv;
        if (t0 > t1) { std::swap(t0, t1); }
        tMin = std::max(tMin, t0);
        tMax = std::min(tMax, t1);
        if (tMin > tMax) { return false; }
    }
    tHit = tMin;
    return true;
}

Polyline3d RectangleAtZ(const BoundingBox3d& b, double z)
{
    Polyline3d polyline;
    polyline.points = {
        {b.min.x, b.min.y, z},
        {b.max.x, b.min.y, z},
        {b.max.x, b.max.y, z},
        {b.min.x, b.max.y, z},
        {b.min.x, b.min.y, z}};
    return polyline;
}
}

void BrepModel::SetName(std::string name) { m_name = std::move(name); }
void BrepModel::SetSourcePath(std::string sourcePath) { m_sourcePath = std::move(sourcePath); }
void BrepModel::SetBounds(BoundingBox3d bounds) { m_bounds = bounds; }
void BrepModel::AddFace(BrepFace face) { m_faces.push_back(std::move(face)); }
void BrepModel::AddEdge(BrepEdge edge) { m_edges.push_back(std::move(edge)); }
void BrepModel::AddHole(BrepHoleFeature hole) { m_holes.push_back(std::move(hole)); }
void BrepModel::AddPocket(BrepPocketFeature pocket) { m_pockets.push_back(std::move(pocket)); }
void BrepModel::SetStepEntityCounters(std::uint32_t pointCount, std::uint32_t advancedFaceCount, std::uint32_t edgeCurveCount, std::uint32_t circleCount)
{
    m_stepPointCount = pointCount;
    m_stepAdvancedFaceCount = advancedFaceCount;
    m_stepEdgeCurveCount = edgeCurveCount;
    m_stepCircleCount = circleCount;
}
const std::string& BrepModel::GetName() const noexcept { return m_name; }
const std::string& BrepModel::GetSourcePath() const noexcept { return m_sourcePath; }
const BoundingBox3d& BrepModel::GetBounds() const noexcept { return m_bounds; }
const std::vector<BrepFace>& BrepModel::GetFaces() const noexcept { return m_faces; }
const std::vector<BrepEdge>& BrepModel::GetEdges() const noexcept { return m_edges; }
const std::vector<BrepHoleFeature>& BrepModel::GetHoles() const noexcept { return m_holes; }
const std::vector<BrepPocketFeature>& BrepModel::GetPockets() const noexcept { return m_pockets; }
std::uint32_t BrepModel::GetStepPointCount() const noexcept { return m_stepPointCount; }
std::uint32_t BrepModel::GetStepAdvancedFaceCount() const noexcept { return m_stepAdvancedFaceCount; }
std::uint32_t BrepModel::GetStepEdgeCurveCount() const noexcept { return m_stepEdgeCurveCount; }
std::uint32_t BrepModel::GetStepCircleCount() const noexcept { return m_stepCircleCount; }

BrepModel CreateAnalyticalBoxBrepModel(std::string name, BoundingBox3d bounds)
{
    bounds = NormalizeBounds(bounds);
    BrepModel model;
    model.SetName(std::move(name));
    model.SetBounds(bounds);
    AddBoxFaces(model, bounds);
    AddBoxEdges(model, bounds);

    const Vec3d center = bounds.Center();
    const Vec3d size = bounds.Size();
    BrepPocketFeature pocket;
    pocket.id = 1U;
    pocket.name = "Top rectangular pocket candidate";
    pocket.bounds.Expand(Vec3d{center.x - size.x * 0.22, center.y - size.y * 0.18, bounds.max.z});
    pocket.bounds.Expand(Vec3d{center.x + size.x * 0.22, center.y + size.y * 0.18, bounds.max.z});
    pocket.depth = std::max(size.z * 0.18, 2.0);
    model.AddPocket(std::move(pocket));

    const double holeDiameter = std::max(std::min(size.x, size.y) * 0.08, 3.0);
    const Vec3d holeCenters[] = {
        {center.x - size.x * 0.25, center.y - size.y * 0.20, bounds.max.z},
        {center.x + size.x * 0.25, center.y - size.y * 0.20, bounds.max.z},
        {center.x - size.x * 0.25, center.y + size.y * 0.20, bounds.max.z},
        {center.x + size.x * 0.25, center.y + size.y * 0.20, bounds.max.z}};
    for (std::uint32_t i = 0U; i < 4U; ++i)
    {
        BrepHoleFeature hole;
        hole.id = i + 1U;
        hole.name = "Hole candidate " + std::to_string(i + 1U);
        hole.center = holeCenters[i];
        hole.diameter = holeDiameter;
        hole.depth = std::max(size.z * 0.55, 5.0);
        model.AddHole(std::move(hole));
    }
    return model;
}

BrepModel CreateM7SampleBrepModel()
{
    BoundingBox3d bounds;
    bounds.Expand(Vec3d{-60.0, -40.0, 0.0});
    bounds.Expand(Vec3d{60.0, 40.0, 32.0});
    BrepModel model = CreateAnalyticalBoxBrepModel("M7 Synthetic STEP/BRep Plate", bounds);
    model.SetSourcePath("synthetic://m7-step-brep-plate");
    model.SetStepEntityCounters(8U, 6U, 12U, 4U);
    return model;
}

BrepFeatureDiagnostics BuildBrepFeatureDiagnostics(const BrepModel& model)
{
    BrepFeatureDiagnostics diagnostics;
    diagnostics.valid = model.GetBounds().IsValid() && !model.GetFaces().empty() && !model.GetEdges().empty();
    diagnostics.bodyCount = diagnostics.valid ? 1U : 0U;
    diagnostics.faceCount = static_cast<std::uint32_t>(model.GetFaces().size());
    diagnostics.edgeCount = static_cast<std::uint32_t>(model.GetEdges().size());
    diagnostics.holeCount = static_cast<std::uint32_t>(model.GetHoles().size());
    diagnostics.pocketCount = static_cast<std::uint32_t>(model.GetPockets().size());
    diagnostics.bounds = model.GetBounds();
    for (const BrepFace& face : model.GetFaces())
    {
        if (face.kind == BrepFaceKind::Planar) { ++diagnostics.planarFaceCount; }
        else if (face.kind == BrepFaceKind::CylindricalStub) { ++diagnostics.cylindricalFaceCount; }
    }
    if (model.GetStepAdvancedFaceCount() > 0U && model.GetStepAdvancedFaceCount() != diagnostics.faceCount)
    {
        diagnostics.warnings.push_back("STEP entity face count differs from analytical M7 topology proxy; exact OCCT topology binding is a later enhancement.");
    }
    if (!diagnostics.valid)
    {
        diagnostics.warnings.push_back("BRep diagnostics failed: missing valid bounds, faces or edges.");
    }
    std::ostringstream out;
    out << "BRep diagnostics: bodies=" << diagnostics.bodyCount
        << ", faces=" << diagnostics.faceCount
        << ", edges=" << diagnostics.edgeCount
        << ", planarFaces=" << diagnostics.planarFaceCount
        << ", holes=" << diagnostics.holeCount
        << ", pockets=" << diagnostics.pocketCount;
    diagnostics.summary = out.str();
    return diagnostics;
}

StepBrepImportResult StepBrepImporter::ImportStepFile(const std::filesystem::path& path) const
{
    StepBrepImportResult result;
    std::ifstream input(path);
    if (!input)
    {
        result.messages.push_back("STEP import failed: cannot open " + path.string());
        result.diagnostics = BuildBrepFeatureDiagnostics(result.model);
        return result;
    }

    BoundingBox3d bounds;
    std::uint32_t pointCount = 0U;
    std::uint32_t advancedFaceCount = 0U;
    std::uint32_t edgeCurveCount = 0U;
    std::uint32_t circleCount = 0U;
    std::string line;
    while (std::getline(input, line))
    {
        if (ContainsCaseInsensitive(line, "CARTESIAN_POINT"))
        {
            const std::vector<double> numbers = ExtractNumbers(line);
            if (numbers.size() >= 4U)
            {
                bounds.Expand(Vec3d{numbers[numbers.size() - 3U], numbers[numbers.size() - 2U], numbers[numbers.size() - 1U]});
                ++pointCount;
            }
        }
        if (ContainsCaseInsensitive(line, "ADVANCED_FACE")) { ++advancedFaceCount; }
        if (ContainsCaseInsensitive(line, "EDGE_CURVE")) { ++edgeCurveCount; }
        if (ContainsCaseInsensitive(line, "CIRCLE")) { ++circleCount; }
    }

    const std::string fileName = path.filename().string().empty() ? std::string("Imported STEP BRep") : path.filename().string();
    result.model = CreateAnalyticalBoxBrepModel(fileName, bounds);
    result.model.SetSourcePath(path.string());
    result.model.SetStepEntityCounters(pointCount, advancedFaceCount, edgeCurveCount, circleCount);
    result.diagnostics = BuildBrepFeatureDiagnostics(result.model);
    result.success = result.diagnostics.valid;
    std::ostringstream message;
    message << "Imported STEP as BRep CAM analytical topology proxy: points=" << pointCount
            << ", advancedFaces=" << advancedFaceCount
            << ", edgeCurves=" << edgeCurveCount
            << ", circles=" << circleCount;
    result.messages.push_back(message.str());
    for (const std::string& warning : result.diagnostics.warnings)
    {
        result.messages.push_back("warning: " + warning);
    }
    return result;
}

BRepCamGeometryProvider::BRepCamGeometryProvider(BrepModel model) : m_model(std::move(model)), m_diagnostics(BuildBrepFeatureDiagnostics(m_model)) {}
CamGeometryKind BRepCamGeometryProvider::GetKind() const { return CamGeometryKind::BRep; }
BoundingBox3d BRepCamGeometryProvider::GetBoundingBox() const { return m_model.GetBounds(); }

bool BRepCamGeometryProvider::RayIntersect(const Ray3d& ray, CamIntersectionResult& result) const
{
    double t = 0.0;
    if (!RayBoxIntersect(ray, m_model.GetBounds(), t)) { return false; }
    result.hit = true;
    result.distance = t;
    result.point = ray.origin + ray.direction * t;
    result.normal = {0.0, 0.0, ray.direction.z < 0.0 ? 1.0 : -1.0};
    result.primitiveId = 1U;
    return true;
}

bool BRepCamGeometryProvider::IntersectPlane(const Plane3d& plane, std::vector<Polyline3d>& sectionCurves) const
{
    const BoundingBox3d& b = m_model.GetBounds();
    if (!b.IsValid()) { return false; }
    const Vec3d normal = Normalize(plane.normal);
    if (std::abs(normal.z) < 0.9) { return false; }
    const double z = plane.origin.z;
    if (z < b.min.z - 1.0e-7 || z > b.max.z + 1.0e-7) { return false; }
    sectionCurves.push_back(RectangleAtZ(b, z));
    return true;
}

bool BRepCamGeometryProvider::QueryHeightAlongZ(double x, double y, double& z) const
{
    const BoundingBox3d& b = m_model.GetBounds();
    if (!b.IsValid() || x < b.min.x || x > b.max.x || y < b.min.y || y > b.max.y) { return false; }
    z = b.max.z;
    return true;
}

bool BRepCamGeometryProvider::QueryTrianglesInBox(const BoundingBox3d&, std::vector<std::uint32_t>& triangleIds) const
{
    triangleIds.clear();
    return false;
}

const BrepModel& BRepCamGeometryProvider::GetModel() const noexcept { return m_model; }
const BrepFeatureDiagnostics& BRepCamGeometryProvider::GetDiagnostics() const noexcept { return m_diagnostics; }

bool BRepCamGeometryProvider::SelectFace(std::uint32_t faceId)
{
    const auto it = std::find_if(m_model.GetFaces().begin(), m_model.GetFaces().end(), [&](const BrepFace& face) { return face.id == faceId && face.selectable; });
    if (it == m_model.GetFaces().end()) { return false; }
    if (std::find(m_selection.faceIds.begin(), m_selection.faceIds.end(), faceId) == m_selection.faceIds.end()) { m_selection.faceIds.push_back(faceId); }
    return true;
}

bool BRepCamGeometryProvider::SelectEdge(std::uint32_t edgeId)
{
    const auto it = std::find_if(m_model.GetEdges().begin(), m_model.GetEdges().end(), [&](const BrepEdge& edge) { return edge.id == edgeId && edge.selectable; });
    if (it == m_model.GetEdges().end()) { return false; }
    if (std::find(m_selection.edgeIds.begin(), m_selection.edgeIds.end(), edgeId) == m_selection.edgeIds.end()) { m_selection.edgeIds.push_back(edgeId); }
    return true;
}

void BRepCamGeometryProvider::ClearSelection()
{
    m_selection.faceIds.clear();
    m_selection.edgeIds.clear();
}

const BrepSelectionState& BRepCamGeometryProvider::GetSelection() const noexcept { return m_selection; }

std::string ToString(BrepFaceKind kind)
{
    switch (kind)
    {
    case BrepFaceKind::Planar: return "Planar";
    case BrepFaceKind::CylindricalStub: return "CylindricalStub";
    default: return "Unknown";
    }
}

std::string ToString(BrepEdgeKind kind)
{
    switch (kind)
    {
    case BrepEdgeKind::Line: return "Line";
    case BrepEdgeKind::CircleStub: return "CircleStub";
    default: return "Unknown";
    }
}
}
