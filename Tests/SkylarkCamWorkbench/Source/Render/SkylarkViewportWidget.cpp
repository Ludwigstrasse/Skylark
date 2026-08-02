#include "Render/SkylarkViewportWidget.h"

#include "LargeMesh/LargeMesh.h"
#include "CAM/BRepCamGeometryProvider.h"

#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QWheelEvent>
#include <algorithm>
#include <cmath>

namespace skylark::cam
{
namespace
{
constexpr double kPi = 3.1415926535897932384626433832795;
constexpr std::size_t kMaxPaintTriangles = 65000U;

double DegToRad(double degrees) noexcept
{
    return degrees * kPi / 180.0;
}

QColor ShadeColor(double intensity)
{
    const int v = static_cast<int>(std::clamp(80.0 + intensity * 140.0, 50.0, 235.0));
    return QColor(v, v + 6 > 255 ? 255 : v + 6, v + 14 > 255 ? 255 : v + 14, 210);
}
}

SkylarkViewportWidget::SkylarkViewportWidget(ProjectSession& session, QWidget* parent) : QWidget(parent), m_session(session)
{
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
}

QSize SkylarkViewportWidget::minimumSizeHint() const { return QSize(520, 390); }
QSize SkylarkViewportWidget::sizeHint() const { return QSize(1040, 760); }

void SkylarkViewportWidget::FitAll()
{
    const LargeMeshAsset* asset = m_session.GetActiveLargeMeshAsset();
    if (asset != nullptr && asset->GetBounds().IsValid())
    {
        const double diagonal = std::max(asset->GetBounds().DiagonalLength(), 1.0);
        const double viewSize = static_cast<double>(std::max(1, std::min(width(), height())));
        m_zoom = std::clamp(viewSize / diagonal * 0.92, 0.002, 20000.0);
    }
    else if (const BrepModel* brep = m_session.GetActiveBrepModel(); brep != nullptr && brep->GetBounds().IsValid())
    {
        const double diagonal = std::max(brep->GetBounds().DiagonalLength(), 1.0);
        const double viewSize = static_cast<double>(std::max(1, std::min(width(), height())));
        m_zoom = std::clamp(viewSize / diagonal * 0.92, 0.002, 20000.0);
    }
    else
    {
        m_zoom = 5.0;
    }
    m_panX = 0.0;
    m_panY = 0.0;
    update();
}

void SkylarkViewportWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    DrawBackground(painter);
    DrawGrid(painter);
    DrawImportedMesh(painter);
    DrawBrepModel(painter);
    DrawChunkBounds(painter);
    DrawStock(painter);
    DrawToolpath(painter);
    DrawToolAssembly(painter);
    DrawWcs(painter);
    DrawOverlay(painter);
}

void SkylarkViewportWidget::mousePressEvent(QMouseEvent* event)
{
    m_lastMousePosition = event->pos();
    setFocus(Qt::MouseFocusReason);
}

void SkylarkViewportWidget::mouseMoveEvent(QMouseEvent* event)
{
    const QPoint delta = event->pos() - m_lastMousePosition;
    if (event->buttons() & Qt::MiddleButton)
    {
        m_panX += static_cast<double>(delta.x());
        m_panY += static_cast<double>(delta.y());
    }
    else if (event->buttons() & Qt::LeftButton)
    {
        m_yawDegrees += static_cast<double>(delta.x()) * 0.45;
        m_pitchDegrees = std::clamp(m_pitchDegrees + static_cast<double>(delta.y()) * 0.35, -85.0, 85.0);
    }
    m_lastMousePosition = event->pos();
    update();
}

void SkylarkViewportWidget::wheelEvent(QWheelEvent* event)
{
    const double steps = static_cast<double>(event->angleDelta().y()) / 120.0;
    m_zoom = std::clamp(m_zoom * std::pow(1.12, steps), 0.002, 20000.0);
    update();
}

void SkylarkViewportWidget::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_F)
    {
        FitAll();
        return;
    }
    if (event->key() == Qt::Key_Space)
    {
        m_simulationOverlay = !m_simulationOverlay;
        update();
        return;
    }
    QWidget::keyPressEvent(event);
}

Vec3d SkylarkViewportWidget::RotatePoint(const Vec3d& point) const
{
    Vec3d centered = point;
    const LargeMeshAsset* asset = m_session.GetActiveLargeMeshAsset();
    if (asset != nullptr && asset->GetBounds().IsValid())
    {
        centered = centered - asset->GetBounds().Center();
    }
    else if (const BrepModel* brep = m_session.GetActiveBrepModel(); brep != nullptr && brep->GetBounds().IsValid())
    {
        centered = centered - brep->GetBounds().Center();
    }

    const double yaw = DegToRad(m_yawDegrees);
    const double pitch = DegToRad(m_pitchDegrees);
    const double cy = std::cos(yaw);
    const double sy = std::sin(yaw);
    const double cp = std::cos(pitch);
    const double sp = std::sin(pitch);

    const double x1 = centered.x * cy - centered.y * sy;
    const double y1 = centered.x * sy + centered.y * cy;
    const double z1 = centered.z;

    const double x2 = x1;
    const double y2 = y1 * cp - z1 * sp;
    const double z2 = y1 * sp + z1 * cp;
    return {x2, y2, z2};
}

SkylarkViewportWidget::ProjectedPoint SkylarkViewportWidget::ProjectPoint(const Vec3d& point) const
{
    const Vec3d rotated = RotatePoint(point);
    const double cx = width() * 0.5 + m_panX;
    const double cy = height() * 0.52 + m_panY;
    return {QPoint(static_cast<int>(cx + rotated.x * m_zoom), static_cast<int>(cy - rotated.y * m_zoom)), rotated.z};
}

void SkylarkViewportWidget::DrawBackground(QPainter& painter) const
{
    painter.fillRect(rect(), QColor(24, 28, 33));
    painter.setPen(QPen(QColor(36, 42, 49), 1));
    for (int y = 0; y < height(); y += 28) { painter.drawLine(0, y, width(), y); }
    for (int x = 0; x < width(); x += 28) { painter.drawLine(x, 0, x, height()); }
}

void SkylarkViewportWidget::DrawGrid(QPainter& painter) const
{
    painter.setPen(QPen(QColor(64, 72, 82), 1));
    for (int i = -10; i <= 10; ++i)
    {
        painter.drawLine(ProjectPoint({-100.0, static_cast<double>(i) * 10.0, 0.0}).point, ProjectPoint({100.0, static_cast<double>(i) * 10.0, 0.0}).point);
        painter.drawLine(ProjectPoint({static_cast<double>(i) * 10.0, -100.0, 0.0}).point, ProjectPoint({static_cast<double>(i) * 10.0, 100.0, 0.0}).point);
    }
}

void SkylarkViewportWidget::DrawImportedMesh(QPainter& painter) const
{
    const LargeMeshAsset* asset = m_session.GetActiveLargeMeshAsset();
    if (asset == nullptr || asset->GetPreviewTriangles().empty()) { return; }

    const std::vector<MeshTriangle3d>& triangles = asset->GetPreviewTriangles();
    const std::size_t stride = std::max<std::size_t>(1U, (triangles.size() + kMaxPaintTriangles - 1U) / kMaxPaintTriangles);
    painter.setPen(QPen(QColor(35, 42, 48), 1));

    for (std::size_t i = 0; i < triangles.size(); i += stride)
    {
        const MeshTriangle3d& triangle = triangles[i];
        const ProjectedPoint p0 = ProjectPoint(triangle.p0);
        const ProjectedPoint p1 = ProjectPoint(triangle.p1);
        const ProjectedPoint p2 = ProjectPoint(triangle.p2);
        const double intensity = std::abs(triangle.normal.z);
        painter.setBrush(ShadeColor(intensity));
        painter.drawPolygon(QPolygon({p0.point, p1.point, p2.point}));
    }
}



void SkylarkViewportWidget::DrawBrepModel(QPainter& painter) const
{
    const BrepModel* brep = m_session.GetActiveBrepModel();
    if (brep == nullptr || !brep->GetBounds().IsValid()) { return; }

    painter.setPen(QPen(QColor(90, 210, 235), 3));
    for (const BrepEdge& edge : brep->GetEdges())
    {
        painter.drawLine(ProjectPoint(edge.start).point, ProjectPoint(edge.end).point);
    }

    painter.setPen(QPen(QColor(255, 235, 120), 2));
    painter.setBrush(QColor(255, 235, 120, 90));
    for (const BrepHoleFeature& hole : brep->GetHoles())
    {
        const QPoint p = ProjectPoint(hole.center).point;
        const int r = static_cast<int>(std::max(4.0, hole.diameter * m_zoom * 0.12));
        painter.drawEllipse(p, r, r);
    }

    painter.setPen(QPen(QColor(170, 120, 255), 2, Qt::DashLine));
    for (const BrepPocketFeature& pocket : brep->GetPockets())
    {
        const BoundingBox3d& b = pocket.bounds;
        const QPoint p0 = ProjectPoint({b.min.x, b.min.y, b.max.z}).point;
        const QPoint p1 = ProjectPoint({b.max.x, b.min.y, b.max.z}).point;
        const QPoint p2 = ProjectPoint({b.max.x, b.max.y, b.max.z}).point;
        const QPoint p3 = ProjectPoint({b.min.x, b.max.y, b.max.z}).point;
        painter.drawPolygon(QPolygon({p0, p1, p2, p3}));
    }

    const BrepSelectionState& selection = m_session.GetBrepSelection();
    painter.setPen(QPen(QColor(255, 105, 85), 4));
    for (std::uint32_t edgeId : selection.edgeIds)
    {
        const auto it = std::find_if(brep->GetEdges().begin(), brep->GetEdges().end(), [&](const BrepEdge& edge) { return edge.id == edgeId; });
        if (it != brep->GetEdges().end()) { painter.drawLine(ProjectPoint(it->start).point, ProjectPoint(it->end).point); }
    }
}

void SkylarkViewportWidget::DrawChunkBounds(QPainter& painter) const
{
    const LargeMeshAsset* asset = m_session.GetActiveLargeMeshAsset();
    if (asset == nullptr || asset->GetChunks().empty()) { return; }

    painter.setPen(QPen(QColor(255, 210, 105, 90), 1, Qt::DashLine));
    const std::vector<MeshChunkDescriptor>& chunks = asset->GetChunks();
    const std::size_t stride = std::max<std::size_t>(1U, (chunks.size() + 64U - 1U) / 64U);
    for (std::size_t i = 0; i < chunks.size(); i += stride)
    {
        const BoundingBox3d& b = chunks[i].bounds;
        if (!b.IsValid()) { continue; }
        const QPoint p0 = ProjectPoint({b.min.x, b.min.y, b.min.z}).point;
        const QPoint p1 = ProjectPoint({b.max.x, b.min.y, b.min.z}).point;
        const QPoint p2 = ProjectPoint({b.max.x, b.max.y, b.min.z}).point;
        const QPoint p3 = ProjectPoint({b.min.x, b.max.y, b.min.z}).point;
        const QPoint p4 = ProjectPoint({b.min.x, b.min.y, b.max.z}).point;
        const QPoint p5 = ProjectPoint({b.max.x, b.min.y, b.max.z}).point;
        const QPoint p6 = ProjectPoint({b.max.x, b.max.y, b.max.z}).point;
        const QPoint p7 = ProjectPoint({b.min.x, b.max.y, b.max.z}).point;
        painter.drawLine(p0, p1); painter.drawLine(p1, p2); painter.drawLine(p2, p3); painter.drawLine(p3, p0);
        painter.drawLine(p4, p5); painter.drawLine(p5, p6); painter.drawLine(p6, p7); painter.drawLine(p7, p4);
        painter.drawLine(p0, p4); painter.drawLine(p1, p5); painter.drawLine(p2, p6); painter.drawLine(p3, p7);
    }
}

void SkylarkViewportWidget::DrawStock(QPainter& painter) const
{
    const auto& setups = m_session.GetProject().GetSetups();
    if (setups.empty()) { return; }
    const StockDefinition& stock = setups.front().stock;
    const double hx = stock.sizeX * 0.5;
    const double hy = stock.sizeY * 0.5;
    const double hz = stock.sizeZ;
    const QPoint p0 = ProjectPoint({-hx, -hy, 0.0}).point;
    const QPoint p1 = ProjectPoint({hx, -hy, 0.0}).point;
    const QPoint p2 = ProjectPoint({hx, hy, 0.0}).point;
    const QPoint p3 = ProjectPoint({-hx, hy, 0.0}).point;
    const QPoint p4 = ProjectPoint({-hx, -hy, hz}).point;
    const QPoint p5 = ProjectPoint({hx, -hy, hz}).point;
    const QPoint p6 = ProjectPoint({hx, hy, hz}).point;
    const QPoint p7 = ProjectPoint({-hx, hy, hz}).point;
    painter.setPen(QPen(QColor(110, 155, 195), 2));
    painter.setBrush(QColor(84, 110, 140, 34));
    painter.drawPolygon(QPolygon({p4, p5, p6, p7}));
    painter.drawLine(p0, p1); painter.drawLine(p1, p2); painter.drawLine(p2, p3); painter.drawLine(p3, p0);
    painter.drawLine(p4, p5); painter.drawLine(p5, p6); painter.drawLine(p6, p7); painter.drawLine(p7, p4);
    painter.drawLine(p0, p4); painter.drawLine(p1, p5); painter.drawLine(p2, p6); painter.drawLine(p3, p7);
}

void SkylarkViewportWidget::DrawWcs(QPainter& painter) const
{
    const QPoint origin = ProjectPoint({0.0, 0.0, 45.0}).point;
    const QPoint xAxis = ProjectPoint({40.0, 0.0, 45.0}).point;
    const QPoint yAxis = ProjectPoint({0.0, 40.0, 45.0}).point;
    const QPoint zAxis = ProjectPoint({0.0, 0.0, 85.0}).point;
    painter.setPen(QPen(QColor(230, 80, 80), 3)); painter.drawLine(origin, xAxis); painter.drawText(xAxis + QPoint(6, -4), QStringLiteral("X"));
    painter.setPen(QPen(QColor(80, 205, 120), 3)); painter.drawLine(origin, yAxis); painter.drawText(yAxis + QPoint(6, -4), QStringLiteral("Y"));
    painter.setPen(QPen(QColor(90, 150, 240), 3)); painter.drawLine(origin, zAxis); painter.drawText(zAxis + QPoint(6, -4), QStringLiteral("Z"));
}

void SkylarkViewportWidget::DrawToolpath(QPainter& painter) const
{
    const auto& toolpaths = m_session.GetProject().GetToolpathDocuments();
    if (toolpaths.empty()) { return; }
    for (const ToolpathDocument& document : toolpaths)
    {
        if (!document.visible || document.moves.size() < 2U) { continue; }
        const auto& moves = document.moves;
        for (std::size_t i = 1; i < moves.size(); ++i)
        {
            QColor color(240, 205, 90);
            if (moves[i].kind == ToolpathMoveKind::Rapid || moves[i].kind == ToolpathMoveKind::Retract) { color = QColor(95, 180, 255); }
            else if (moves[i].kind == ToolpathMoveKind::LeadIn || moves[i].kind == ToolpathMoveKind::LeadOut) { color = QColor(160, 230, 120); }
            painter.setPen(QPen(color, 3));
            painter.drawLine(ProjectPoint(moves[i - 1U].point).point, ProjectPoint(moves[i].point).point);
        }
        if (m_simulationOverlay && moves.size() > 2U)
        {
            painter.setPen(QPen(QColor(255, 245, 180), 2));
            painter.setBrush(QColor(255, 245, 180, 130));
            const SimulationResult& sim = m_session.GetLastSimulationResult();
            const Vec3d markerPoint = m_session.HasSimulationResult() && sim.playback.loaded ? sim.playback.currentPose.tip : moves[2].point;
            painter.drawEllipse(ProjectPoint(markerPoint).point, 9, 9);
        }
    }
}

void SkylarkViewportWidget::DrawToolAssembly(QPainter& painter) const
{
    if (!m_session.HasSimulationResult()) { return; }
    const SimulationResult& result = m_session.GetLastSimulationResult();
    if (!result.playback.loaded) { return; }

    const Vec3d tip = result.playback.currentPose.tip;
    const Vec3d fluteTop = tip + Vec3d{0.0, 0.0, result.toolAssembly.cutter.fluteLength};
    const Vec3d holderTop = fluteTop + Vec3d{0.0, 0.0, result.toolAssembly.holderLength};
    const double cutterRadius = std::max(result.toolAssembly.cutter.diameter * 0.5, 1.0);
    const double holderRadius = std::max(result.toolAssembly.holderDiameter * 0.5, cutterRadius + 1.0);

    const QPoint tipPoint = ProjectPoint(tip).point;
    const QPoint fluteTopPoint = ProjectPoint(fluteTop).point;
    const QPoint holderTopPoint = ProjectPoint(holderTop).point;

    painter.setPen(QPen(QColor(250, 250, 245), 3));
    painter.drawLine(tipPoint, fluteTopPoint);
    painter.setBrush(QColor(245, 245, 235, 180));
    painter.drawEllipse(tipPoint, static_cast<int>(std::max(4.0, cutterRadius * m_zoom * 0.18)), static_cast<int>(std::max(4.0, cutterRadius * m_zoom * 0.18)));

    painter.setPen(QPen(QColor(180, 190, 205), 5));
    painter.drawLine(fluteTopPoint, holderTopPoint);
    painter.setBrush(QColor(135, 145, 160, 150));
    painter.drawEllipse(fluteTopPoint, static_cast<int>(std::max(7.0, holderRadius * m_zoom * 0.16)), static_cast<int>(std::max(7.0, holderRadius * m_zoom * 0.16)));

    painter.setPen(QColor(240, 245, 250));
    painter.drawText(holderTopPoint + QPoint(8, -8), QStringLiteral("Tool + Holder"));
}

void SkylarkViewportWidget::DrawOverlay(QPainter& painter) const
{
    const SceneAdapterSnapshot snapshot = m_sceneAdapter.CreateSnapshot(m_session.GetProject(), m_session.GetLargeMeshAssets(), m_session.GetGpuUploadQueue());
    const LargeMeshAsset* asset = m_session.GetActiveLargeMeshAsset();
    painter.setPen(QColor(225, 235, 245));
    painter.drawText(18, 28, QStringLiteral("SkylarkViewportWidget M7 - Mesh + BRep CAM View"));
    painter.drawText(18, 50, QStringLiteral("LMB: Rotate | MMB: Pan | Wheel: Zoom | F: Fit All | Space: Simulation Marker / Tool"));
    painter.drawText(18, 72, QStringLiteral("Render proxies: %1 | LargeMesh proxies: %2 | Yaw: %3 | Pitch: %4")
        .arg(static_cast<int>(snapshot.proxies.size()))
        .arg(static_cast<int>(snapshot.largeMeshProxies.size()))
        .arg(m_yawDegrees, 0, 'f', 1)
        .arg(m_pitchDegrees, 0, 'f', 1));
    if (asset != nullptr)
    {
        painter.drawText(18, 94, QStringLiteral("STL: %1 | Triangles: %2 | Chunks: %3 | Preview: %4")
            .arg(QString::fromStdString(asset->GetName()))
            .arg(static_cast<qulonglong>(asset->GetTriangleCount()))
            .arg(static_cast<int>(asset->GetChunks().size()))
            .arg(static_cast<int>(asset->GetPreviewTriangles().size())));
        painter.drawText(18, 116, QStringLiteral("BVH: %1 nodes=%2 | LOD: %3 levels=%4 | GPU uploaded=%5/%6")
            .arg(QString::fromStdString(ToString(asset->GetBvhState())))
            .arg(asset->GetBvhNodeCount())
            .arg(QString::fromStdString(ToString(asset->GetLodState())))
            .arg(asset->GetLodLevelCount())
            .arg(static_cast<qulonglong>(snapshot.uploadQueue.uploadedRequests))
            .arg(static_cast<qulonglong>(snapshot.uploadQueue.totalRequests)));
        if (m_session.HasSimulationResult())
        {
            const SimulationResult& sim = m_session.GetLastSimulationResult();
            painter.drawText(18, 138, QStringLiteral("Simulation: move %1/%2 | collisions=%3 | zmapRemoved=%4 | dexelRemoved=%5")
                .arg(static_cast<qulonglong>(sim.playback.currentMoveIndex))
                .arg(static_cast<qulonglong>(sim.playback.totalMoves))
                .arg(static_cast<int>(sim.collision.events.size()))
                .arg(static_cast<qulonglong>(sim.zmapRemoval.removedCells))
                .arg(static_cast<qulonglong>(sim.dexelRemoval.removedCells)));
        }
    }
    else if (const BrepModel* brep = m_session.GetActiveBrepModel(); brep != nullptr)
    {
        painter.drawText(18, 94, QStringLiteral("STEP/BRep: %1 | Faces: %2 | Edges: %3 | Holes: %4")
            .arg(QString::fromStdString(brep->GetName()))
            .arg(static_cast<int>(brep->GetFaces().size()))
            .arg(static_cast<int>(brep->GetEdges().size()))
            .arg(static_cast<int>(brep->GetHoles().size())));
        if (m_session.HasBrepDiagnostics())
        {
            const BrepFeatureDiagnostics& d = m_session.GetLastBrepDiagnostics();
            painter.drawText(18, 116, QStringLiteral("BRep diagnostics: bodies=%1 planarFaces=%2 pockets=%3 selectedFaces=%4 selectedEdges=%5")
                .arg(d.bodyCount)
                .arg(d.planarFaceCount)
                .arg(d.pocketCount)
                .arg(static_cast<int>(m_session.GetBrepSelection().faceIds.size()))
                .arg(static_cast<int>(m_session.GetBrepSelection().edgeIds.size())));
        }
    }
    else
    {
        painter.drawText(18, 94, QStringLiteral("No STL/STEP loaded. Use File / Import STL or Import STEP."));
    }
}
}
