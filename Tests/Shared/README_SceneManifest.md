# Shared Scene Manifest Options

Both `SkylarkCadEditorLab` and `SkylarkCadSceneTechniques` support a shared scene manifest.

Supported options:
- `--scene-manifest=<path>`
- `--scene-index=<int>`
- `--list-scenes`
- `--no-list-scenes`

Environment variables:
- `SKYLARK_SCENE_MANIFEST`
- `SKYLARK_SCENE_INDEX`
- `SKYLARK_LIST_SCENES`

Manifest file format:
- one scene per line
- empty lines ignored
- `#` / `;` comments supported
- optional label:
  - `Engine Cover|Tests/SkylarkCadSceneTechniques/Assets/geforce.csf.gz`
  - `Engine Cover=Tests/SkylarkCadSceneTechniques/Assets/geforce.csf.gz`

Relative scene paths are resolved relative to the manifest file location.
