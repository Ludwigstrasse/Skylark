# Shared benchmark options

This shared layer adds batch-friendly runtime options for sample/test applications.

## Command line

- `--benchmark`
- `--no-benchmark`
- `--warmup-frames=<int>`
- `--max-frames=<int>`
- `--max-seconds=<float>`
- `--benchmark-csv=<path>`
- `--append-benchmark-csv`
- `--print-benchmark-summary`
- `--no-benchmark-summary`

## Environment variables

- `SKYLARK_BENCHMARK`
- `SKYLARK_WARMUP_FRAMES`
- `SKYLARK_MAX_FRAMES`
- `SKYLARK_MAX_SECONDS`
- `SKYLARK_BENCHMARK_CSV`
- `SKYLARK_BENCHMARK_APPEND_CSV`
- `SKYLARK_BENCHMARK_SUMMARY`

## Current behavior

- Warmup frames are excluded from measured frame/seconds counters.
- If `--max-frames` or `--max-seconds` is set, the sample auto-exits when the measured limit is reached.
- If `--benchmark-csv` is set, the application appends one summary row on exit.
