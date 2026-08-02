#pragma once

#include "CAM/CamDomain.h"
#include "Common/CamCore.h"

#include <cstdint>
#include <string>
#include <vector>

namespace skylark::cam
{
using ImportedModelId = std::uint64_t;

struct SourceEntityMapEntry { std::string sourcePath; std::string sourceEntityId; std::string workbenchEntityId; };
struct ImportedMeshPart
{
    std::string name;
    std::uint64_t triangleCount{0};
    std::uint32_t chunkCount{0};
    bool bvhReady{false};
    bool lodReady{false};
    std::uint32_t bvhNodeCount{0};
    std::uint32_t lodLevelCount{0};
    BoundingBox3d bounds;
};
struct ImportedBrepPart { std::string name; std::uint32_t bodyCount{0}; std::uint32_t faceCount{0}; std::uint32_t edgeCount{0}; BoundingBox3d bounds; };
struct AssemblyNode { std::string name; std::vector<AssemblyNode> children; };
struct ImportedModel
{
    ImportedModelId id{0};
    std::string name;
    std::string sourcePath;
    CamGeometryKind geometryKind{CamGeometryKind::Unknown};
    ImportedMeshPart meshPart;
    ImportedBrepPart brepPart;
    AssemblyNode assemblyRoot;
    std::vector<SourceEntityMapEntry> sourceEntityMap;
    bool placeholder{false};
};
struct PostProcessorConfig { std::string activePost{"Generic ISO 3-Axis"}; std::string ncExtension{".nc"}; };
struct SimulationSettings { bool enableToolAnimation{true}; bool enableCollisionCheck{true}; bool enableStockRemoval{false}; };

class CamProject
{
public:
    CamProject();
    const std::string& GetName() const noexcept;
    void SetName(std::string name);
    const std::vector<ImportedModel>& GetImportedModels() const noexcept;
    std::vector<ImportedModel>& GetImportedModels() noexcept;
    const std::vector<CamSetup>& GetSetups() const noexcept;
    std::vector<CamSetup>& GetSetups() noexcept;
    const ToolLibrary& GetToolLibrary() const noexcept;
    ToolLibrary& GetToolLibrary() noexcept;
    const OperationTree& GetOperationTree() const noexcept;
    OperationTree& GetOperationTree() noexcept;
    const std::vector<ToolpathDocument>& GetToolpathDocuments() const noexcept;
    std::vector<ToolpathDocument>& GetToolpathDocuments() noexcept;
    const PostProcessorConfig& GetPostConfig() const noexcept;
    void SetPostConfig(PostProcessorConfig config);
    const SimulationSettings& GetSimulationSettings() const noexcept;
    void SetSimulationSettings(SimulationSettings settings);
    void AddImportedModel(ImportedModel model);
    void ClearImportedModels();
    void RemovePlaceholderModels();
    void AddSetup(CamSetup setup);
    void ClearSetups();
    void AddToolpathDocument(ToolpathDocument document);
    void ClearToolpathDocuments();
    bool UpdateImportedMeshAccelerationStatus(const std::string& modelName, bool bvhReady, bool lodReady, std::uint32_t bvhNodeCount, std::uint32_t lodLevelCount);
    bool UpdateImportedBrepDiagnostics(const std::string& modelName, std::uint32_t bodyCount, std::uint32_t faceCount, std::uint32_t edgeCount, BoundingBox3d bounds);
private:
    std::string m_name{"Untitled CAM Project"};
    std::vector<ImportedModel> m_importedModels;
    std::vector<CamSetup> m_setups;
    ToolLibrary m_toolLibrary;
    OperationTree m_operationTree;
    std::vector<ToolpathDocument> m_toolpathDocuments;
    PostProcessorConfig m_postConfig;
    SimulationSettings m_simulationSettings;
};

ImportedModel CreatePlaceholderImportedModel();
CamProject CreateM0CamProject();
CamProject CreateM1CamProject();
CamProject CreateM2CamProject();
CamProject CreateM3CamProject();
CamProject CreateM4CamProject();
CamProject CreateM5CamProject();
CamProject CreateM6CamProject();
CamProject CreateM7CamProject();
}
