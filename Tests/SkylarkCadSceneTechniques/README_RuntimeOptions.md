SkylarkCadSceneTechniques command line
=====================================

Supported options:
- `--scene=<path>` or positional `<path>`
- `--width=<pixels>`
- `--height=<pixels>`
- `--size=<width>x<height>`
- `--vsync=<0|1|on|off>`
- `--help`

Environment fallback:
- `SKYLARK_SCENE`
- `SKYLARK_SIZE`
- `SKYLARK_WIDTH`
- `SKYLARK_HEIGHT`
- `SKYLARK_VSYNC`

Example:

```bat
SkylarkCadSceneTechniques.exe --scene=Tests/SkylarkCadSceneTechniques/Assets/geforce.csf.gz --size=1920x1080 --vsync=off
```

## Runtime stats

- `--title-stats` / `--no-title-stats`
- `--print-stats` / `--no-print-stats`
- `--stats-interval=<seconds>`
- `--title-stats-every=<frames>`
