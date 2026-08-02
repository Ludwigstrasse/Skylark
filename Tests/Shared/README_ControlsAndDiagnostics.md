# Shared Controls and CAD Scene Diagnostics

This shared layer is used by both:
- `SkylarkCadEditorLab`
- `SkylarkCadSceneTechniques`

## Controls text

`SkSampleControls.h/.cpp` centralizes control help text so the two applications do not keep duplicating key bindings in their `main` loops and `--help` flows.

## CAD scene diagnostics

`SkCadSceneDiagnostics.h/.cpp` centralizes scene summary printing for `FSkCadSceneDocument`.

Supported command-line switches:
- `--print-scene-summary`
- `--no-scene-summary`
- `--print-scene-bounds`
- `--no-scene-bounds`

Supported environment variables:
- `SKYLARK_PRINT_SCENE_SUMMARY`
- `SKYLARK_PRINT_SCENE_BOUNDS`
