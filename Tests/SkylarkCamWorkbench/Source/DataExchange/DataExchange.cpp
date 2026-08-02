#include "DataExchange/DataExchange.h"

#include <algorithm>
#include <utility>

namespace skylark::cam
{
void ImportDiagnostics::AddInfo(std::string text) { messages.push_back({"Info", std::move(text)}); }
void ImportDiagnostics::AddWarning(std::string text) { messages.push_back({"Warning", std::move(text)}); }
FormatRegistry::FormatRegistry()
{
    m_formats.push_back({"step", "STEP / STP BRep CAM", {"step", "stp"}, true, false});
    m_formats.push_back({"stl", "STL large mesh", {"stl"}, true, false});
    m_formats.push_back({"obj", "OBJ mesh", {"obj"}, false, false});
    m_formats.push_back({"ply", "PLY mesh", {"ply"}, false, false});
    m_formats.push_back({"gltf", "glTF lightweight assembly", {"gltf", "glb"}, false, false});
    m_formats.push_back({"ifc", "IFC BIM geometry", {"ifc"}, false, false});
    m_formats.push_back({"jt", "JT commercial bridge slot", {"jt"}, false, true});
    m_formats.push_back({"3dxml", "3DXML commercial bridge slot", {"3dxml"}, false, true});
}
const std::vector<FormatDescriptor>& FormatRegistry::GetFormats() const noexcept { return m_formats; }
const FormatDescriptor* FormatRegistry::FindByExtension(const std::string& ext) const noexcept
{
    for (const auto& f : m_formats) if (std::find(f.extensions.begin(), f.extensions.end(), ext) != f.extensions.end()) return &f;
    return nullptr;
}
ExchangeScene::ExchangeScene() { m_root.name = "ExchangeScene"; m_root.kind = CamGeometryKind::Assembly; }
const ExchangeNode& ExchangeScene::GetRoot() const noexcept { return m_root; }
void ExchangeScene::SetRoot(ExchangeNode root) { m_root = std::move(root); }
ImportSession::ImportSession(const FormatRegistry& registry) : m_registry(registry) {}
ImportSessionResult ImportSession::CreateM0DryRun(const std::filesystem::path& sourcePath) const
{
    ImportSessionResult r; r.diagnostics.success = false; r.diagnostics.format = sourcePath.extension().string();
    r.diagnostics.AddInfo("M0 dry-run only: real STL/STEP/IFC/glTF importers start in later milestones.");
    const std::string ext = sourcePath.has_extension() ? sourcePath.extension().string().substr(1) : std::string{};
    const FormatDescriptor* d = m_registry.FindByExtension(ext);
    if (d == nullptr) r.diagnostics.AddWarning("No registered exchange format matched the extension.");
    else if (d->commercialBridge) r.diagnostics.AddWarning("Commercial bridge registered, but no licensed reader is linked in M0.");
    ExchangeNode root; root.name = "M0 Import Dry Run"; root.kind = CamGeometryKind::Assembly; r.scene.SetRoot(std::move(root));
    return r;
}
}
