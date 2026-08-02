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

EXE="$ROOT_DIR/out/build/linux-format-workbench-headless/Tests/SkylarkFormatWorkbench/SkylarkFormatWorkbenchCli"
if [ ! -x "$EXE" ]; then
    echo "[FAIL] Executable not found: $EXE"
    echo "[ACTION] Run Scripts/Linux/Build/Build_Linux_FormatWorkbench_Headless.sh first."
    exit 3
fi

"$EXE" "$@"
