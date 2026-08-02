#!/usr/bin/env bash
set -u

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR=""
CANDIDATE="$SCRIPT_DIR"
for _ in 1 2 3 4 5 6 7 8 9; do
    if [ -f "$CANDIDATE/Source/SKCore/SKCore.cpp" ] && [ -f "$CANDIDATE/Tests/SkylarkFormatWorkbench/CMakeLists.txt" ]; then
        ROOT_DIR="$(cd "$CANDIDATE" && pwd)"
        break
    fi
    CANDIDATE="$CANDIDATE/.."
done

if [ -z "$ROOT_DIR" ]; then
    echo "[FAIL] Cannot locate Skylark repository root from $SCRIPT_DIR"
    exit 2
fi

"$ROOT_DIR/Scripts/Linux/Build/Build_Linux_FormatWorkbench_Headless.sh"
if [ $? -ne 0 ]; then
    exit 10
fi

BUILD_DIR="$ROOT_DIR/out/build/linux-format-workbench-headless"
LOG_DIR="$ROOT_DIR/out/logs"
TEST_DIR="$ROOT_DIR/out/format_workbench_test"
mkdir -p "$TEST_DIR" "$LOG_DIR"

EXE="$BUILD_DIR/Tests/SkylarkFormatWorkbench/SkylarkFormatWorkbenchCli"
if [ ! -x "$EXE" ]; then
    echo "[FAIL] Executable not found: $EXE"
    exit 20
fi

"$EXE" --generate-binary-stl "$TEST_DIR/smoke_1000.stl" --triangles 1000 \
    > "$LOG_DIR/run_linux_format_workbench_generate.log" 2>&1 || exit 30
"$EXE" --input "$TEST_DIR/smoke_1000.stl" --report "$TEST_DIR/smoke_report.json" --max-preview-triangles 1000 \
    > "$LOG_DIR/run_linux_format_workbench_import.log" 2>&1 || exit 40

grep -q '"triangles": 1000' "$TEST_DIR/smoke_report.json" || exit 50

echo "[OK] Linux FormatWorkbench headless configure/build/run passed."
