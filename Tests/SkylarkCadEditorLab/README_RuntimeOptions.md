# SkylarkCadEditorLab Runtime Options

Examples:

```bat
SkylarkCadEditorLab.exe --rhi=d3d11 --scene=Tests/SkylarkCadSceneTechniques/Assets/geforce.csf.gz
SkylarkCadEditorLab.exe --backend=vulkan --size=1920x1080 --vsync=0
SkylarkCadEditorLab.exe --rhi=opengl --debug-rhi --render-thread
```

Environment-variable fallback:

- `SKYLARK_RHI`
- `SKYLARK_SCENE`
- `SKYLARK_SIZE`
- `SKYLARK_WIDTH`
- `SKYLARK_HEIGHT`
- `SKYLARK_VSYNC`
- `SKYLARK_RHI_DEBUG`
- `SKYLARK_RENDER_THREAD`

## Runtime stats

- `--title-stats` / `--no-title-stats`
- `--print-stats` / `--no-print-stats`
- `--stats-interval=<seconds>`
- `--title-stats-every=<frames>`
