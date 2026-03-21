# Skylark CAD Scene Techniques Test

This test project ports the uploaded `gl_cadscene_rendertechniques` sample into a Skylark-owned sample/test entry.

## What is included

- `Source/SkCadSceneFile.*`
  - migrated CSF file loader from the uploaded project
- `Source/SkCadSceneImporter.*`
  - CSF -> Skylark geometry import path (`FSKGeometryRegistry`, `FSKMeshBuilder`)
- `Source/SkCadSceneApp.cpp`
  - OpenGL viewer app using GLFW + GLEW
- `Assets/geforce.csf.gz`
  - default CAD scene asset
- `ThirdParty/original_sample/`
  - full snapshot of the uploaded original sample for reference and future deep porting

## Current rendering modes

- `1` shaded
- `2` shaded + edges
- `3` wireframe
- `4` xray
- `5` object-id color view
- `6` normal visualization
- `E` exploded view toggle
- `B` boundary edges toggle
- `C` sharp edges toggle
- left mouse drag orbit, wheel zoom

## Build

Enable the test app preset:

- Linux: `cmake --preset linux-cadscene-debug && cmake --build --preset linux-cadscene-debug`
- Windows: `cmake --preset windows-cadscene && cmake --build --preset windows-cadscene-debug`

## Scope note

The original sample contains several NVIDIA-extension-heavy renderers (token, NV_command_list, cullsorted, etc.).
This first Skylark port migrates the full source snapshot into the repository, but the compile-ready Skylark test app currently wires up:

- CSF loading
- scene import into Skylark geometry structures
- general OpenGL rendering effects suitable for cross-platform engine testing

This keeps the test project buildable in a standard GLFW/GLEW/OpenGL environment while preserving the original source for future staged porting.
