# SkylarkCamWorkbench M7 - BRep CAM Channel

## Scope

M7 adds the exact-geometry CAM channel requested in the architecture plan:

- STEP import entry point
- `BRepCamGeometryProvider`
- 2.5D contour planner V1
- pocket planner stub
- drilling planner stub
- face / edge selection state
- BRep feature diagnostics

The Mesh CAM path from M1-M6 is kept intact. STL is still imported through the LargeMesh pipeline and is not converted to BRep.

## Architecture boundaries

- UI does not read STEP or build toolpaths directly.
- Toolpath planners consume `BRepCamGeometryProvider` / `ICamGeometryProvider` semantics.
- The M7 STEP reader is a lightweight STEP topology proxy parser for CAM workflow validation. Exact OCCT STEP topology binding can replace `StepBrepImporter` later without changing the CAM operation API.
- BRep selection is represented by stable face/edge ids and `SourceEntityMap`, not hard-coded UI strings.

## Validation

Windows:

```bat
Scripts\Windows\Build\Build_Windows_SkylarkCamWorkbench_VS2022_Debug.bat
Scripts\Windows\Run\Run_Windows_SkylarkCamWorkbench_M7_BRepCAM_Validation.bat
```

Linux:

```bash
bash Scripts/Linux/Build/Build_Linux_SkylarkCamWorkbench_Headless.sh
bash Scripts/Linux/Run/Run_Linux_SkylarkCamWorkbench_M7_BRepCAM_Validation.sh
```
