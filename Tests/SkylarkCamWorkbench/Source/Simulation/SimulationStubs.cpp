#include "Simulation/SimulationStubs.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <sstream>
#include <utility>

namespace skylark::cam
{
namespace
{
constexpr double kEpsilon = 1.0e-9;

bool IsCuttingMove(ToolpathMoveKind kind) noexcept
{
    return kind == ToolpathMoveKind::Feed || kind == ToolpathMoveKind::LeadIn || kind == ToolpathMoveKind::LeadOut;
}

std::size_t CellIndex(std::uint32_t x, std::uint32_t y, std::uint32_t width) noexcept
{
    return static_cast<std::size_t>(y) * static_cast<std::size_t>(width) + static_cast<std::size_t>(x);
}

void AppendLine(VerificationReport& report, std::string line)
{
    report.lines.push_back(std::move(line));
}

std::string FormatDouble(double value)
{
    std::ostringstream stream;
    stream.setf(std::ios::fixed);
    stream.precision(3);
    stream << value;
    return stream.str();
}

StockRemovalReport ApplyToolpathToHeightField(
    const char* modelKind,
    const StockDefinition& stock,
    std::uint32_t resolutionX,
    std::uint32_t resolutionY,
    std::vector<double>& topHeights,
    const ToolAssembly& toolAssembly,
    const ToolpathDocument& document)
{
    StockRemovalReport report;
    report.executed = true;
    report.modelKind = modelKind;
    report.resolutionX = resolutionX;
    report.resolutionY = resolutionY;
    report.maxRemainingHeight = stock.sizeZ;
    report.minRemainingHeight = stock.sizeZ;

    if (resolutionX == 0U || resolutionY == 0U || topHeights.empty() || document.moves.empty())
    {
        return report;
    }

    const double toolRadius = std::max(toolAssembly.cutter.diameter * 0.5, 0.5);
    const double halfX = stock.sizeX * 0.5;
    const double halfY = stock.sizeY * 0.5;
    const double dx = stock.sizeX / static_cast<double>(resolutionX);
    const double dy = stock.sizeY / static_cast<double>(resolutionY);
    const double cellArea = dx * dy;

    for (const ToolpathMove& move : document.moves)
    {
        if (!IsCuttingMove(move.kind)) { continue; }
        const double cx = move.point.x;
        const double cy = move.point.y;
        const double cutTop = std::clamp(move.point.z, 0.0, stock.sizeZ);

        const int minX = std::max(0, static_cast<int>(std::floor((cx - toolRadius + halfX) / dx)));
        const int maxX = std::min(static_cast<int>(resolutionX) - 1, static_cast<int>(std::ceil((cx + toolRadius + halfX) / dx)));
        const int minY = std::max(0, static_cast<int>(std::floor((cy - toolRadius + halfY) / dy)));
        const int maxY = std::min(static_cast<int>(resolutionY) - 1, static_cast<int>(std::ceil((cy + toolRadius + halfY) / dy)));

        for (int y = minY; y <= maxY; ++y)
        {
            for (int x = minX; x <= maxX; ++x)
            {
                const double px = -halfX + (static_cast<double>(x) + 0.5) * dx;
                const double py = -halfY + (static_cast<double>(y) + 0.5) * dy;
                const double dist2 = (px - cx) * (px - cx) + (py - cy) * (py - cy);
                if (dist2 > toolRadius * toolRadius) { continue; }

                const std::size_t index = CellIndex(static_cast<std::uint32_t>(x), static_cast<std::uint32_t>(y), resolutionX);
                ++report.visitedCells;
                if (cutTop + kEpsilon < topHeights[index])
                {
                    const double removedHeight = topHeights[index] - cutTop;
                    report.estimatedRemovedVolume += removedHeight * cellArea;
                    topHeights[index] = cutTop;
                    ++report.removedCells;
                }
            }
        }
    }

    for (double height : topHeights)
    {
        report.minRemainingHeight = std::min(report.minRemainingHeight, height);
        report.maxRemainingHeight = std::max(report.maxRemainingHeight, height);
    }
    return report;
}
}

void ToolpathPlayback::Load(const ToolpathDocument& document)
{
    m_document = &document;
    m_currentMoveIndex = 0U;
}

void ToolpathPlayback::Reset()
{
    m_currentMoveIndex = 0U;
}

bool ToolpathPlayback::StepForward(std::size_t moveCount)
{
    if (m_document == nullptr || m_document->moves.empty()) { return false; }
    m_currentMoveIndex = std::min(m_currentMoveIndex + moveCount, m_document->moves.size() - 1U);
    return m_currentMoveIndex + 1U >= m_document->moves.size();
}

bool ToolpathPlayback::SeekNormalized(double t)
{
    if (m_document == nullptr || m_document->moves.empty()) { return false; }
    const double clamped = std::clamp(t, 0.0, 1.0);
    m_currentMoveIndex = static_cast<std::size_t>(std::round(clamped * static_cast<double>(m_document->moves.size() - 1U)));
    return true;
}

ToolpathPlaybackSnapshot ToolpathPlayback::CreateSnapshot() const
{
    ToolpathPlaybackSnapshot snapshot;
    if (m_document == nullptr || m_document->moves.empty()) { return snapshot; }
    snapshot.loaded = true;
    snapshot.totalMoves = m_document->moves.size();
    snapshot.currentMoveIndex = std::min(m_currentMoveIndex, m_document->moves.size() - 1U);
    snapshot.finished = snapshot.currentMoveIndex + 1U >= snapshot.totalMoves;
    snapshot.normalizedTime = snapshot.totalMoves <= 1U ? 1.0 : static_cast<double>(snapshot.currentMoveIndex) / static_cast<double>(snapshot.totalMoves - 1U);
    const ToolpathMove& move = m_document->moves[snapshot.currentMoveIndex];
    snapshot.currentPose.moveIndex = snapshot.currentMoveIndex;
    snapshot.currentPose.tip = move.point;
    snapshot.currentPose.axis = {0.0, 0.0, 1.0};
    snapshot.currentPose.moveKind = move.kind;
    snapshot.currentPose.feedRate = move.feedRate;
    return snapshot;
}

CollisionCheckResult CutterCollisionChecker::CheckToolpath(
    const CamSetup& setup,
    const ToolAssembly& toolAssembly,
    const ToolpathDocument& document) const
{
    CollisionCheckResult result;
    result.executed = true;
    const double halfX = setup.stock.sizeX * 0.5 + setup.stock.offset;
    const double halfY = setup.stock.sizeY * 0.5 + setup.stock.offset;
    const double cutterRadius = std::max(toolAssembly.cutter.diameter * 0.5, 0.5);
    const double machineTravelLimit = 100000.0;

    for (std::size_t i = 0; i < document.moves.size(); ++i)
    {
        const ToolpathMove& move = document.moves[i];
        ++result.checkedPoses;

        if (std::abs(move.point.x) > machineTravelLimit || std::abs(move.point.y) > machineTravelLimit || std::abs(move.point.z) > machineTravelLimit)
        {
            result.collisionFree = false;
            result.events.push_back({i, "Machine Travel Envelope", move.point, 0.0, "Tool pose exceeds the generic machine travel envelope."});
        }

        if (move.point.z < -1.0)
        {
            result.collisionFree = false;
            result.events.push_back({i, "Machine Table / Fixture", move.point, -move.point.z, "Cutter tip is below the setup table plane."});
        }

        if (setup.fixture.collisionEnabled && IsCuttingMove(move.kind))
        {
            const bool insideStockFootprint = std::abs(move.point.x) <= halfX && std::abs(move.point.y) <= halfY;
            const double holderBottom = move.point.z + toolAssembly.cutter.fluteLength;
            if (!insideStockFootprint && holderBottom < setup.stock.sizeZ + toolAssembly.holderSafeClearance && cutterRadius > 0.0)
            {
                result.collisionFree = false;
                result.events.push_back({i, setup.fixture.name, move.point, setup.stock.sizeZ + toolAssembly.holderSafeClearance - holderBottom, "Potential holder/fixture collision outside stock footprint."});
            }
        }
    }
    return result;
}

void ZMapStockModel::Initialize(const StockDefinition& stock, std::uint32_t resolutionX, std::uint32_t resolutionY)
{
    m_stock = stock;
    m_resolutionX = std::max(1U, resolutionX);
    m_resolutionY = std::max(1U, resolutionY);
    m_topHeights.assign(static_cast<std::size_t>(m_resolutionX) * static_cast<std::size_t>(m_resolutionY), stock.sizeZ);
}

StockRemovalReport ZMapStockModel::ApplyToolpath(const ToolAssembly& toolAssembly, const ToolpathDocument& document)
{
    return ApplyToolpathToHeightField("ZMap", m_stock, m_resolutionX, m_resolutionY, m_topHeights, toolAssembly, document);
}

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 26446)
#endif

double ZMapStockModel::QueryTopHeight(std::uint32_t x, std::uint32_t y) const
{
    if (m_resolutionX == 0U || m_resolutionY == 0U || m_topHeights.empty()) { return 0.0; }
    const std::uint32_t cx = std::min(x, m_resolutionX - 1U);
    const std::uint32_t cy = std::min(y, m_resolutionY - 1U);
    return m_topHeights[CellIndex(cx, cy, m_resolutionX)];
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

void DexelStockModel::Initialize(const StockDefinition& stock, std::uint32_t resolutionX, std::uint32_t resolutionY)
{
    m_stock = stock;
    m_resolutionX = std::max(1U, resolutionX);
    m_resolutionY = std::max(1U, resolutionY);
    m_topHeights.assign(static_cast<std::size_t>(m_resolutionX) * static_cast<std::size_t>(m_resolutionY), stock.sizeZ);
}

StockRemovalReport DexelStockModel::ApplyToolpath(const ToolAssembly& toolAssembly, const ToolpathDocument& document)
{
    return ApplyToolpathToHeightField("Dexel", m_stock, m_resolutionX, m_resolutionY, m_topHeights, toolAssembly, document);
}

ToolAssembly CreateDefaultToolAssembly(const ToolLibrary& toolLibrary)
{
    ToolAssembly assembly;
    if (!toolLibrary.GetTools().empty())
    {
        assembly.cutter = toolLibrary.GetTools().front();
    }
    else
    {
        assembly.cutter = {1, "T01 Flat End Mill D10", CutterKind::FlatEndMill, 10.0, 0.0, 32.0, 65.0};
    }
    assembly.shankDiameter = std::max(assembly.cutter.diameter, 6.0);
    assembly.holderDiameter = std::max(assembly.cutter.diameter * 2.8, 24.0);
    assembly.holderLength = std::max(assembly.cutter.gaugeLength - assembly.cutter.fluteLength, 35.0);
    assembly.holderSafeClearance = 2.0;
    return assembly;
}

SimulationResult RunM5ToolpathSimulation(const CamProject& project)
{
    SimulationResult result;
    if (project.GetSetups().empty() || project.GetToolpathDocuments().empty())
    {
        result.report.generated = true;
        result.report.passed = false;
        AppendLine(result.report, "M5 simulation failed: project has no setup or no toolpath document.");
        result.report.text = BuildVerificationReportText(result);
        return result;
    }

    const CamSetup& setup = project.GetSetups().back();
    const ToolpathDocument& document = project.GetToolpathDocuments().back();
    result.toolAssembly = CreateDefaultToolAssembly(project.GetToolLibrary());

    ToolpathPlayback playback;
    playback.Load(document);
    playback.SeekNormalized(0.35);
    result.playback = playback.CreateSnapshot();

    CutterCollisionChecker checker;
    result.collision = checker.CheckToolpath(setup, result.toolAssembly, document);

    ZMapStockModel zmap;
    zmap.Initialize(setup.stock, 96U, 64U);
    result.zmapRemoval = zmap.ApplyToolpath(result.toolAssembly, document);

    DexelStockModel dexel;
    dexel.Initialize(setup.stock, 48U, 32U);
    result.dexelRemoval = dexel.ApplyToolpath(result.toolAssembly, document);

    result.report.generated = true;
    result.report.passed = result.collision.executed && result.zmapRemoval.executed && result.dexelRemoval.executed;
    result.success = result.report.generated && result.report.passed;

    AppendLine(result.report, "M5 Toolpath Playback: loaded=" + std::string(result.playback.loaded ? "yes" : "no")
        + ", move=" + std::to_string(result.playback.currentMoveIndex) + "/" + std::to_string(result.playback.totalMoves));
    AppendLine(result.report, "M5 Tool Assembly: cutter=" + result.toolAssembly.cutter.name
        + ", diameter=" + FormatDouble(result.toolAssembly.cutter.diameter)
        + ", holderDiameter=" + FormatDouble(result.toolAssembly.holderDiameter));
    AppendLine(result.report, "M5 Collision Check: poses=" + std::to_string(result.collision.checkedPoses)
        + ", collisions=" + std::to_string(result.collision.events.size()));
    AppendLine(result.report, "M5 ZMap Removal: cells=" + std::to_string(result.zmapRemoval.removedCells)
        + ", removedVolume=" + FormatDouble(result.zmapRemoval.estimatedRemovedVolume));
    AppendLine(result.report, "M5 Dexel Removal: cells=" + std::to_string(result.dexelRemoval.removedCells)
        + ", removedVolume=" + FormatDouble(result.dexelRemoval.estimatedRemovedVolume));
    result.report.text = BuildVerificationReportText(result);
    return result;
}

std::vector<SimulationCapability> CreateM0SimulationCapabilities()
{
    return {{"Toolpath Playback", false}, {"Cutter Collision Checker", false}, {"ZMap Stock Removal", false}, {"Dexel Stock Model", false}, {"Verification Report", false}};
}

std::vector<SimulationCapability> CreateM5SimulationCapabilities()
{
    return {{"Toolpath Playback", true}, {"Cutter Entity Display", true}, {"Holder Entity Display", true}, {"Basic Collision Check", true}, {"ZMap Stock Removal V1", true}, {"Dexel Stock Model V1", true}, {"Verification Report", true}};
}

std::string BuildVerificationReportText(const SimulationResult& result)
{
    std::ostringstream stream;
    stream << "SkylarkCamWorkbench M5 Verification Report\n";
    stream << "Status: " << (result.report.passed ? "PASSED" : "FAILED") << "\n";
    stream << "Playback loaded: " << (result.playback.loaded ? "yes" : "no") << "\n";
    stream << "Playback move: " << result.playback.currentMoveIndex << "/" << result.playback.totalMoves << "\n";
    stream << "Tool: " << result.toolAssembly.cutter.name << "\n";
    stream << "Collision executed: " << (result.collision.executed ? "yes" : "no") << "\n";
    stream << "Collision free: " << (result.collision.collisionFree ? "yes" : "no") << "\n";
    stream << "Collision events: " << result.collision.events.size() << "\n";
    stream << "ZMap removed cells: " << result.zmapRemoval.removedCells << "\n";
    stream << "ZMap estimated removed volume: " << result.zmapRemoval.estimatedRemovedVolume << "\n";
    stream << "Dexel removed cells: " << result.dexelRemoval.removedCells << "\n";
    stream << "Dexel estimated removed volume: " << result.dexelRemoval.estimatedRemovedVolume << "\n";
    for (const std::string& line : result.report.lines)
    {
        stream << "- " << line << "\n";
    }
    for (const CollisionEvent& event : result.collision.events)
    {
        stream << "! Collision at move " << event.moveIndex << " against " << event.objectName << ": " << event.message << "\n";
    }
    return stream.str();
}
}
