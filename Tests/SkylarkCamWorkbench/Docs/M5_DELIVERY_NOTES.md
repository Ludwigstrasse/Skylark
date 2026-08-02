# SkylarkCamWorkbench M5 - Toolpath Simulation

## Scope

M5 implements the first verification layer above M4 Mesh CAM toolpaths:

- Toolpath playback state
- Cutter entity and holder entity data model
- Basic collision checker
- ZMap stock removal model V1
- Dexel stock removal model V1
- Verification report generation
- Qt viewport overlay for tool + holder
- Headless validation entry for CI / Windows scripts

## Architecture Boundary

M5 does not read STL directly and does not convert STL to BRep. Simulation consumes manufacturing semantics:

```text
CamProject / CamSetup / ToolLibrary / ToolpathDocument
    -> ToolpathPlayback
    -> CutterCollisionChecker
    -> ZMapStockModel / DexelStockModel
    -> VerificationReport
    -> SkylarkViewportWidget overlay
```

## Validation

Windows:

```bat
Scripts\Windows\Run\Run_Windows_SkylarkCamWorkbench_M5_Simulation_Validation.bat
```

Linux:

```bash
bash Scripts/Linux/Run/Run_Linux_SkylarkCamWorkbench_M5_Simulation_Validation.sh
```
