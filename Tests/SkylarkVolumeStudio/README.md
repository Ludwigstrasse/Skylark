# SkylarkVolumeStudio

`SkylarkVolumeStudio` is hosted under `SkylarkEngine/Tests/SkylarkVolumeStudio` as a commercial validation workbench for large model import, volume/voxel preprocessing, CAM-oriented mesh workflows, and future Skylark native rendering integration.

Current executable targets:

- `SkylarkVolumeStudioValidation`: Linux/Windows headless core validation.
- `SkylarkVolumeStudio`: Qt5 commercial workbench shell.

Current implemented scope:

- Binary STL fast import.
- Packed float triangle mesh payload.
- Conservative voxel grid generation.
- Hexahedral mesh generation.
- Commercial Qt workbench layout with ribbon, manufacturing viewport, project tree, operation browser, properties/statistics, tool library, job queue, and diagnostics console.

Important boundary:

- Large STL is intentionally not converted to OCCT BRep.
- STL follows the large mesh payload / mesh CAM route.
- STEP/BRep precise CAM remains a separate future route through kernel-level precise geometry.

Build from repository root:

```bat
build_windows_vs2022.bat
```

```bash
bash build_linux.sh
```
