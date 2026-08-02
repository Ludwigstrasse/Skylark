#!/usr/bin/env bash
set -u
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "${SCRIPT_DIR}/../../.." && pwd)"
EXE="${ROOT_DIR}/out/build/linux-skylark-cam-workbench-headless/Tests/SkylarkCamWorkbench/SkylarkCamWorkbenchHeadless"
if [ ! -x "${EXE}" ]; then
  EXE="${ROOT_DIR}/out/build/linux-skylark-cam-workbench-headless/SkylarkCamWorkbenchHeadless"
fi
if [ ! -x "${EXE}" ]; then
  echo "[FAIL] Executable not found. Run Scripts/Linux/Build/Build_Linux_SkylarkCamWorkbench_Headless.sh first."
  exit 1
fi
echo "============================================================"
echo "[SkylarkCamWorkbench] M3 Project JSON Validation"
echo "[EXE] ${EXE}"
echo "============================================================"
"${EXE}"
