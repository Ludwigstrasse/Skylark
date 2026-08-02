# SkylarkCadEditorLab

Boundary in this delivery:

- `SkylarkCadEditorLab` does **not** use OpenGL API.
- Window/input host: GLFW (no OpenGL context)
- Rendering path: `SKCreateEngine -> SKRuntime -> SKRenderer -> SKRHI(D3D11)`
- Model import path: shared CSF importer from `SkylarkCadSceneTechniques`

Controls:
- Left click: pick
- Right drag: orbit
- Middle drag: pan
- Mouse wheel: zoom
- F: fit view
- [ / ]: explode amount
- H / I / U: hide / isolate / show all
- W S A D Q E: move selected item
- R: reset selected offset
- Ctrl+C or C: clone selected item
- 1 / 2 / 3 / 4: shaded+edges / shaded / wireframe / hidden-line
