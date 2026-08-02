#!/usr/bin/env bash
set -u

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR=""
CANDIDATE="$SCRIPT_DIR"
for _ in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16; do
    CLEAN="$(cd "$CANDIDATE" 2>/dev/null && pwd || true)"
    if [ -n "$CLEAN" ] && [ -f "$CLEAN/Source/SKCore/SKCore.cpp" ] && [ -f "$CLEAN/Tests/SkylarkFormatWorkbench/CMakeLists.txt" ]; then
        ROOT_DIR="$CLEAN"
        break
    fi
    CANDIDATE="$CANDIDATE/.."
done

if [ -z "$ROOT_DIR" ]; then
    echo "[FAIL] Cannot locate Skylark repository root from $SCRIPT_DIR"
    echo "[HINT] Expected: <SkylarkRoot>/Source/SKCore/SKCore.cpp"
    echo "[HINT] Expected: <SkylarkRoot>/Tests/SkylarkFormatWorkbench/CMakeLists.txt"
    exit 2
fi

BUILD_DIR="$ROOT_DIR/out/build/linux-format-workbench-headless"
LOG_DIR="$ROOT_DIR/out/logs"
mkdir -p "$LOG_DIR"

echo "============================================================"
echo "[SkylarkFormatWorkbench] Linux Headless Build"
echo "[ROOT ] $ROOT_DIR"
echo "[BUILD] $BUILD_DIR"
echo "============================================================"

if ! grep -q "SK_BUILD_FORMAT_WORKBENCH" "$ROOT_DIR/CMakeLists.txt"; then
    echo "[FAIL] Root CMakeLists.txt does not contain SK_BUILD_FORMAT_WORKBENCH."
    exit 3
fi

rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"

(
    cd "$ROOT_DIR" || exit 4
    cmake -S . -B "$BUILD_DIR" -G Ninja \
        -DCMAKE_BUILD_TYPE=Release \
        -DSK_BUILD_INSTALL=OFF \
        -DSK_BUILD_FORMAT_WORKBENCH=ON \
        -DSKFW_BUILD_CLI=ON \
        -DSKFW_BUILD_QT_APP=OFF \
        -DSKFW_LINK_SKYLARK=ON
) > "$LOG_DIR/configure_linux_format_workbench.log" 2>&1
if [ $? -ne 0 ]; then
    echo "[FAIL] configure failed"
    tail -160 "$LOG_DIR/configure_linux_format_workbench.log"
    exit 10
fi

cmake --build "$BUILD_DIR" --target SkylarkFormatWorkbenchCli -j 2 \
    > "$LOG_DIR/build_linux_format_workbench.log" 2>&1
if [ $? -ne 0 ]; then
    echo "[FAIL] build failed"
    tail -200 "$LOG_DIR/build_linux_format_workbench.log"
    exit 20
fi

echo "[OK] Linux FormatWorkbench headless build succeeded."
