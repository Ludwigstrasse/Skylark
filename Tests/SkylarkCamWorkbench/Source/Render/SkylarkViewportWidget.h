#pragma once

#include "App/ProjectSession.h"
#include "Render/RenderCore.h"

#include <QPoint>
#include <QWidget>

namespace skylark::cam
{
class SkylarkViewportWidget final : public QWidget
{
public:
    explicit SkylarkViewportWidget(ProjectSession& session, QWidget* parent = nullptr);
    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;
    void FitAll();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    struct ProjectedPoint { QPoint point; double depth{0.0}; };

    ProjectedPoint ProjectPoint(const Vec3d& point) const;
    Vec3d RotatePoint(const Vec3d& point) const;
    void DrawBackground(QPainter& painter) const;
    void DrawGrid(QPainter& painter) const;
    void DrawImportedMesh(QPainter& painter) const;
    void DrawBrepModel(QPainter& painter) const;
    void DrawChunkBounds(QPainter& painter) const;
    void DrawStock(QPainter& painter) const;
    void DrawWcs(QPainter& painter) const;
    void DrawToolpath(QPainter& painter) const;
    void DrawToolAssembly(QPainter& painter) const;
    void DrawOverlay(QPainter& painter) const;

    ProjectSession& m_session;
    SkylarkSceneAdapter m_sceneAdapter;
    QPoint m_lastMousePosition;
    double m_zoom{5.0};
    double m_panX{0.0};
    double m_panY{0.0};
    double m_yawDegrees{-38.0};
    double m_pitchDegrees{24.0};
    bool m_simulationOverlay{false};
};
}
