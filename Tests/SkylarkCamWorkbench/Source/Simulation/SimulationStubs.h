#pragma once

#include "CAM/CamDomain.h"
#include "Common/CamCore.h"
#include "Model/CamProject.h"

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace skylark::cam
{
struct SimulationCapability
{
    std::string name;
    bool availableInM0{false};
};

struct ToolAssembly
{
    ToolDefinition cutter;
    double holderDiameter{32.0};
    double holderLength{70.0};
    double shankDiameter{12.0};
    double holderSafeClearance{2.0};
};

struct ToolPose
{
    std::size_t moveIndex{0};
    Vec3d tip{};
    Vec3d axis{0.0, 0.0, 1.0};
    ToolpathMoveKind moveKind{ToolpathMoveKind::Rapid};
    double feedRate{0.0};
};

struct ToolpathPlaybackSnapshot
{
    bool loaded{false};
    bool finished{false};
    std::size_t currentMoveIndex{0};
    std::size_t totalMoves{0};
    double normalizedTime{0.0};
    ToolPose currentPose;
};

class ToolpathPlayback
{
public:
    void Load(const ToolpathDocument& document);
    void Reset();
    bool StepForward(std::size_t moveCount = 1U);
    bool SeekNormalized(double t);
    ToolpathPlaybackSnapshot CreateSnapshot() const;

private:
    const ToolpathDocument* m_document{nullptr};
    std::size_t m_currentMoveIndex{0};
};

struct CollisionEvent
{
    std::size_t moveIndex{0};
    std::string objectName;
    Vec3d point{};
    double penetration{0.0};
    std::string message;
};

struct CollisionCheckResult
{
    bool executed{false};
    bool collisionFree{true};
    std::uint64_t checkedPoses{0};
    std::vector<CollisionEvent> events;
};

class CutterCollisionChecker
{
public:
    CollisionCheckResult CheckToolpath(
        const CamSetup& setup,
        const ToolAssembly& toolAssembly,
        const ToolpathDocument& document) const;
};

struct StockRemovalReport
{
    bool executed{false};
    std::string modelKind;
    std::uint32_t resolutionX{0};
    std::uint32_t resolutionY{0};
    std::uint64_t visitedCells{0};
    std::uint64_t removedCells{0};
    double estimatedRemovedVolume{0.0};
    double minRemainingHeight{0.0};
    double maxRemainingHeight{0.0};
};

class ZMapStockModel
{
public:
    void Initialize(const StockDefinition& stock, std::uint32_t resolutionX, std::uint32_t resolutionY);
    StockRemovalReport ApplyToolpath(const ToolAssembly& toolAssembly, const ToolpathDocument& document);
    double QueryTopHeight(std::uint32_t x, std::uint32_t y) const;

private:
    StockDefinition m_stock;
    std::uint32_t m_resolutionX{0};
    std::uint32_t m_resolutionY{0};
    std::vector<double> m_topHeights;
};

class DexelStockModel
{
public:
    void Initialize(const StockDefinition& stock, std::uint32_t resolutionX, std::uint32_t resolutionY);
    StockRemovalReport ApplyToolpath(const ToolAssembly& toolAssembly, const ToolpathDocument& document);

private:
    StockDefinition m_stock;
    std::uint32_t m_resolutionX{0};
    std::uint32_t m_resolutionY{0};
    std::vector<double> m_topHeights;
};

struct VerificationReport
{
    bool generated{false};
    bool passed{false};
    std::vector<std::string> lines;
    std::string text;
};

struct SimulationResult
{
    bool success{false};
    ToolAssembly toolAssembly;
    ToolpathPlaybackSnapshot playback;
    CollisionCheckResult collision;
    StockRemovalReport zmapRemoval;
    StockRemovalReport dexelRemoval;
    VerificationReport report;
};

ToolAssembly CreateDefaultToolAssembly(const ToolLibrary& toolLibrary);
SimulationResult RunM5ToolpathSimulation(const CamProject& project);
std::vector<SimulationCapability> CreateM0SimulationCapabilities();
std::vector<SimulationCapability> CreateM5SimulationCapabilities();
std::string BuildVerificationReportText(const SimulationResult& result);
}
