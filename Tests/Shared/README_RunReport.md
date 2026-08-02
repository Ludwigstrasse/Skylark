# Shared Run Report Options

Both `SkylarkCadEditorLab` and `SkylarkCadSceneTechniques` support a shared run-report layer.

Supported options:

- `--run-report=<path>`
- `--append-run-report`
- `--print-run-report`
- `--no-print-run-report`

Environment fallback:

- `SKYLARK_RUN_REPORT`
- `SKYLARK_APPEND_RUN_REPORT`
- `SKYLARK_PRINT_RUN_REPORT`

Behavior:

- If `--run-report` is set, the app writes a plain-text key/value report on exit.
- The report includes app/backend/scene/window/vsync, runtime stats, benchmark stats, and scene diagnostics.
- `--append-run-report` appends a new report block instead of replacing the file.
