# Shared Output Layout Options

Both sample applications support a shared output layout helper.

Supported options:
- `--output-dir=<path>`
- `--session-tag=<text>`
- `--auto-report-files`
- `--no-auto-report-files`

Environment variables:
- `SKYLARK_OUTPUT_DIR`
- `SKYLARK_SESSION_TAG`
- `SKYLARK_AUTO_REPORT_FILES`

When auto-report-files is enabled, the sample will populate default paths for:
- benchmark CSV
- run report text

Generated filenames use:
- app name
- backend name
- scene stem
- optional session tag
