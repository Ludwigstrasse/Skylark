# SkylarkCamWorkbench M6 - Post Processing

## Scope

M6 implements the first real CAM post-processing milestone for SkylarkCamWorkbench.

## Acceptance Items

- Generic ISO G-code
- FANUC 3-axis post V1
- NC Preview
- Export `.nc`
- Operation Sheet

## Architecture Constraints

- Post processing consumes `CamProject`, `CamSetup`, `ToolLibrary`, and `ToolpathDocument`.
- The post processor does not read STL files directly.
- STL remains in the Mesh CAM channel and is not converted to BRep.
- M6 generated NC is 3-axis milling output only; full machine kinematics, cycle expansion, rotary axis support, and customized post templates belong to later milestones.

## Main Classes

- `IPostProcessor`
- `GenericIsoPost`
- `Fanuc3AxisPost`
- `GCodeProgram`
- `OperationSheet`
- `ProjectSession::GenerateM6GenericIsoProgram()`
- `ProjectSession::GenerateM6FanucProgram()`
- `ProjectSession::ExportM6NcFile()`
- `ProjectSession::GenerateM6OperationSheet()`

## Validation Scripts

Windows:

```bat
Scripts\Windows\Run\Run_Windows_SkylarkCamWorkbench_M6_Post_Validation.bat
```

Linux:

```bash
bash Scripts/Linux/Run/Run_Linux_SkylarkCamWorkbench_M6_Post_Validation.sh
```
