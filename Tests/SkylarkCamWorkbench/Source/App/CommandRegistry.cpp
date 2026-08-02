#include "App/CommandRegistry.h"

#include <algorithm>
#include <iterator>
#include <utility>

namespace skylark::cam
{
void CommandRegistry::RegisterCommand(CommandDescriptor descriptor, Handler handler)
{
    m_handlers[descriptor.id] = std::move(handler);
    auto it = std::find_if(m_commands.begin(), m_commands.end(), [&](const CommandDescriptor& c) { return c.id == descriptor.id; });
    if (it == m_commands.end()) { m_commands.push_back(std::move(descriptor)); } else { *it = std::move(descriptor); }
}
bool CommandRegistry::Execute(const std::string& commandId) const
{
    auto it = m_handlers.find(commandId);
    if (it == m_handlers.end()) { return false; }
    it->second();
    return true;
}
const std::vector<CommandDescriptor>& CommandRegistry::GetCommands() const noexcept { return m_commands; }
std::vector<CommandDescriptor> CommandRegistry::GetCommandsForTab(const std::string& tab) const
{
    std::vector<CommandDescriptor> result;
    std::copy_if(m_commands.begin(), m_commands.end(), std::back_inserter(result), [&](const CommandDescriptor& c) { return c.tab == tab; });
    return result;
}
CommandRegistry CreateDefaultCommandRegistry()
{
    CommandRegistry r;
    auto add = [&](const char* id, const char* title, const char* tab, const char* group) { r.RegisterCommand({id, title, tab, group, true}, []{}); };
    add("file.newProject", "New CAM Project", "File", "Project"); add("file.openProject", "Open Project", "File", "Project"); add("file.saveProject", "Save Project", "File", "Project");
    add("file.importStep", "Import STEP", "File", "Import"); add("file.syntheticBrep", "Synthetic BRep", "File", "Import"); add("file.importStl", "Import STL", "File", "Import"); add("file.synthetic20m", "20M STL Pressure", "File", "Benchmark"); add("file.importObj", "Import OBJ", "File", "Import"); add("file.importPly", "Import PLY", "File", "Import"); add("file.importGltf", "Import glTF", "File", "Import"); add("file.importIfc", "Import IFC", "File", "Import");
    add("model.meshStats", "Mesh Statistics", "Model", "Inspect"); add("model.brepSelect", "Select BRep Face/Edge", "Model", "BRep Inspect"); add("model.brepDiagnostics", "BRep Diagnostics", "Model", "BRep Inspect"); add("model.buildBvh", "Build BVH", "Model", "Large Mesh"); add("model.buildLod", "Build LOD", "Model", "Large Mesh"); add("model.gpuUpload", "Pump GPU Upload", "Model", "Large Mesh"); add("model.section", "Section View", "Model", "Inspect");
    add("setup.create", "Create Setup", "Setup", "Setup"); add("setup.wcs", "Set WCS", "Setup", "Coordinate"); add("setup.stock", "Define Stock", "Setup", "Manufacturing"); add("setup.fixture", "Define Fixture", "Setup", "Manufacturing"); add("setup.addTool", "Add Tool", "Setup", "Tool Library"); add("setup.machine", "Select Machine", "Setup", "Machine");
    add("toolpath.roughing", "3D Roughing", "Toolpath", "3 Axis Mesh CAM"); add("toolpath.zlevel", "Z-Level", "Toolpath", "3 Axis Mesh CAM"); add("toolpath.parallel", "Parallel Finish", "Toolpath", "3 Axis Mesh CAM"); add("toolpath.projection", "Projection", "Toolpath", "3 Axis Mesh CAM"); add("toolpath.contour2d", "2.5D Contour", "Toolpath", "BRep CAM"); add("toolpath.pocket2d", "Pocket Stub", "Toolpath", "BRep CAM"); add("toolpath.drilling", "Drilling Stub", "Toolpath", "BRep CAM"); add("toolpath.regenerate", "Regenerate All", "Toolpath", "Calculation");
    add("simulation.play", "Play", "Simulation", "Playback"); add("simulation.collision", "Collision Check", "Simulation", "Verify"); add("simulation.stock", "Material Removal", "Simulation", "Verify"); add("simulation.report", "Generate Report", "Simulation", "Report");
    add("machine.library", "Machine Library", "Machine", "Machine"); add("machine.post", "Post Processor", "Machine", "Post"); add("machine.ncPreview", "NC Preview", "Machine", "NC"); add("machine.fanucPreview", "FANUC Preview", "Machine", "NC"); add("machine.exportGcode", "Export G-code", "Machine", "NC"); add("machine.operationSheet", "Operation Sheet", "Machine", "Documentation");
    add("view.fit", "Fit All", "View", "Camera"); add("view.shaded", "Shaded", "View", "Style"); add("view.wireframe", "Wireframe", "View", "Style"); add("view.toolpath", "Toolpath", "View", "Overlay"); add("view.stockTransparent", "Stock Transparent", "View", "Overlay"); add("view.wcs", "Show WCS", "View", "Overlay"); add("view.benchmark", "Show FPS / Memory", "View", "Diagnostics");
    return r;
}
}
