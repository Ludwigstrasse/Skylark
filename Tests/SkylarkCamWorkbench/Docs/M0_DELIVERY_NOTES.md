# M0 Delivery Notes

M0 adds `Tests/SkylarkCamWorkbench` as the CAM platform validator skeleton.

Acceptance mapping:

- Windows VS2022 Debug entry: `Scripts/Windows/Build/Build_Windows_SkylarkCamWorkbench_VS2022_Debug.bat`
- Linux headless entry: `Scripts/Linux/Build/Build_Linux_SkylarkCamWorkbench_Headless.sh`
- Qt desktop app target: `SkylarkCamWorkbench`
- Headless validation target: `SkylarkCamWorkbenchHeadless`
- UI modules: Project Tree, SkylarkViewportWidget, Operation Parameters, bottom Log/Warnings/NC/Timeline/Benchmark tabs
- Domain skeleton: DataExchange, Model, LargeMesh, Render, CAM, Toolpath, Simulation, Post, Diagnostics

This milestone intentionally keeps real import, real GPU upload, toolpath generation, simulation and post-processing behind stable module interfaces for M1-M6.
