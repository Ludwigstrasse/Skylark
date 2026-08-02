#pragma once

#include "Common/CamCore.h"

#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

namespace skylark::cam
{
struct ImportDiagnosticMessage { std::string severity; std::string text; };
struct ImportDiagnostics
{
    bool success{false};
    std::string format;
    std::uint64_t importedTriangleCount{0};
    std::uint64_t importedBodyCount{0};
    double importTimeSeconds{0.0};
    std::uint64_t estimatedMemoryBytes{0};
    std::vector<ImportDiagnosticMessage> messages;
    void AddInfo(std::string text);
    void AddWarning(std::string text);
};
struct FormatDescriptor { std::string id; std::string displayName; std::vector<std::string> extensions; bool availableInM0{false}; bool commercialBridge{false}; };
class FormatRegistry
{
public:
    FormatRegistry();
    const std::vector<FormatDescriptor>& GetFormats() const noexcept;
    const FormatDescriptor* FindByExtension(const std::string& extension) const noexcept;
private:
    std::vector<FormatDescriptor> m_formats;
};
struct ExchangeNode { std::string name; CamGeometryKind kind{CamGeometryKind::Unknown}; BoundingBox3d bounds; std::vector<ExchangeNode> children; };
class ExchangeScene
{
public:
    ExchangeScene();
    const ExchangeNode& GetRoot() const noexcept;
    void SetRoot(ExchangeNode root);
private:
    ExchangeNode m_root;
};
struct ImportSessionResult { ImportDiagnostics diagnostics; ExchangeScene scene; };
class ImportSession
{
public:
    explicit ImportSession(const FormatRegistry& registry);
    ImportSessionResult CreateM0DryRun(const std::filesystem::path& sourcePath) const;
private:
    const FormatRegistry& m_registry;
};
}
