#include "CAM/CamDomain.h"

#include <algorithm>
#include <utility>

namespace skylark::cam
{
void ToolLibrary::AddTool(ToolDefinition tool) { m_tools.push_back(std::move(tool)); }
void ToolLibrary::Clear() { m_tools.clear(); }
ToolId ToolLibrary::GetNextAvailableToolId() const noexcept
{
    ToolId nextId = 1;
    for (const ToolDefinition& tool : m_tools) { nextId = std::max(nextId, tool.id + 1); }
    return nextId;
}
const std::vector<ToolDefinition>& ToolLibrary::GetTools() const noexcept { return m_tools; }
void OperationTree::AddOperation(OperationNode op) { m_operations.push_back(std::move(op)); }
void OperationTree::Clear() { m_operations.clear(); }
CamOperationId OperationTree::GetNextAvailableOperationId() const noexcept
{
    CamOperationId nextId = 1;
    for (const OperationNode& operation : m_operations) { nextId = std::max(nextId, operation.id + 1); }
    return nextId;
}
const std::vector<OperationNode>& OperationTree::GetOperations() const noexcept { return m_operations; }

CamSetup CreateDefaultSetup() { CamSetup setup; setup.name = "Setup 1 - Top WCS / Box Stock / 3-Axis Mill"; return setup; }
ToolLibrary CreateDefaultToolLibrary()
{
    ToolLibrary lib;
    lib.AddTool({1, "T01 Flat End Mill D10", CutterKind::FlatEndMill, 10.0, 0.0, 32.0, 65.0});
    lib.AddTool({2, "T02 Ball End Mill D6", CutterKind::BallEndMill, 6.0, 3.0, 26.0, 58.0});
    lib.AddTool({3, "T03 Drill D5", CutterKind::Drill, 5.0, 0.0, 35.0, 72.0});
    lib.AddTool({4, "T04 Chamfer Mill D8", CutterKind::ChamferMill, 8.0, 0.0, 20.0, 55.0});
    return lib;
}
OperationTree CreateDefaultOperationTree()
{
    OperationTree tree;
    tree.AddOperation({1, "01 Adaptive Roughing", CamOperationType::Roughing3D, OperationState::Draft, false});
    tree.AddOperation({2, "02 Z-Level Finish", CamOperationType::ZLevelFinishing, OperationState::Draft, false});
    tree.AddOperation({3, "03 Parallel Finish", CamOperationType::ParallelFinishing, OperationState::Draft, false});
    tree.AddOperation({4, "04 Drilling", CamOperationType::Drilling, OperationState::Draft, false});
    return tree;
}
ToolpathDocument CreatePlaceholderToolpathDocument()
{
    ToolpathDocument doc; doc.id = 1; doc.name = "M0 placeholder parallel finishing toolpath";
    doc.moves.push_back({ToolpathMoveKind::Rapid, {-50.0, -25.0, 55.0}, 6000.0});
    doc.moves.push_back({ToolpathMoveKind::LeadIn, {-50.0, -25.0, 32.0}, 1000.0});
    doc.moves.push_back({ToolpathMoveKind::Feed, {50.0, -25.0, 30.0}, 1200.0});
    doc.moves.push_back({ToolpathMoveKind::Feed, {-50.0, 0.0, 26.0}, 1200.0});
    doc.moves.push_back({ToolpathMoveKind::Feed, {50.0, 25.0, 24.0}, 1200.0});
    doc.moves.push_back({ToolpathMoveKind::Retract, {50.0, 25.0, 55.0}, 6000.0});
    return doc;
}
std::string ToString(CutterKind kind)
{
    switch (kind) { case CutterKind::FlatEndMill: return "Flat End Mill"; case CutterKind::BallEndMill: return "Ball End Mill"; case CutterKind::BullNoseEndMill: return "Bull Nose End Mill"; case CutterKind::Drill: return "Drill"; case CutterKind::ChamferMill: return "Chamfer Mill"; default: return "Unknown Cutter"; }
}
std::string ToString(ToolpathMoveKind kind)
{
    switch (kind) { case ToolpathMoveKind::Rapid: return "Rapid"; case ToolpathMoveKind::Feed: return "Feed"; case ToolpathMoveKind::LeadIn: return "Lead In"; case ToolpathMoveKind::LeadOut: return "Lead Out"; case ToolpathMoveKind::Retract: return "Retract"; default: return "Unknown Move"; }
}
}
