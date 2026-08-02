# SkylarkCamWorkbench M3 Delivery Notes

## Milestone

M3: CAM Project / Setup / Tool Library

## Acceptance Matrix

- Create `CamProject`
- Create `CamSetup`
- Set WCS
- Define Box Stock
- Define Fixture Stub
- Add tools to `ToolLibrary`
- Save project JSON
- Load project JSON

## New Core Files

```text
Source/Model/CamProjectJson.h
Source/Model/CamProjectJson.cpp
```

## New ProjectSession API

```cpp
void CreateNewCamProject(std::string name);
std::size_t CreateSetup(std::string name);
bool SetActiveSetupWcs(WorkCoordinateSystem wcs);
bool DefineActiveSetupBoxStock(StockDefinition stock);
bool DefineActiveSetupFixtureStub(FixtureDefinition fixture);
ToolId AddToolToLibrary(ToolDefinition tool);
bool SaveProjectJson(const std::filesystem::path& path);
bool LoadProjectJson(const std::filesystem::path& path);
```

## JSON Scope

The M3 JSON stores manufacturing project semantics:

- Project name
- Imported model metadata
- Setup / WCS / Stock / Fixture / Machine
- Tool library
- Operation tree
- Toolpath document metadata and demo moves
- Post configuration
- Simulation settings

Runtime caches are intentionally not serialized in M3:

- GPU upload queue
- BVH nodes
- LOD payloads
- preview triangle buffers

Those caches belong to M2 runtime acceleration and later M4/M5/M6 project cache work.

## Validation Entry

Windows:

```bat
Scripts\Windows\Run\Run_Windows_SkylarkCamWorkbench_M3_ProjectJson_Validation.bat
```

Linux:

```bash
bash Scripts/Linux/Run/Run_Linux_SkylarkCamWorkbench_M3_ProjectJson_Validation.sh
```
