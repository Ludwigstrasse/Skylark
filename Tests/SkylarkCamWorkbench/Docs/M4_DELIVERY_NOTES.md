# SkylarkCamWorkbench M4 Delivery Notes

## Scope

M4 delivers the first Mesh CAM operation layer on top of the M1/M2 LargeMesh pipeline and the M3 CAM project model.

## Acceptance Items

- MeshCamGeometryProvider
- Ray query
- Plane slicing
- Z height query
- Z-Level toolpath first version
- Parallel Finishing first version
- Toolpath polyline rendering in SkylarkViewportWidget

## Architecture Rules

- STL is still not converted to BRep.
- M4 uses LargeMeshAsset preview/query payload for the first Mesh CAM algorithms.
- Full-resolution CAM acceleration remains a future MeshPayloadStore/BVH query extension.
- Toolpath generation depends on ICamGeometryProvider, not direct STL file IO.
- UI calls ProjectSession, not planner implementation classes directly.

## Validation

Run:

```bat
Scripts\Windows\Build\Build_Windows_SkylarkCamWorkbench_VS2022_Debug.bat
Scripts\Windows\Run\Run_Windows_SkylarkCamWorkbench_M4_MeshCAM_Validation.bat
```

Linux headless:

```bash
Scripts/Linux/Build/Build_Linux_SkylarkCamWorkbench_Headless.sh
Scripts/Linux/Run/Run_Linux_SkylarkCamWorkbench_M4_MeshCAM_Validation.sh
```
