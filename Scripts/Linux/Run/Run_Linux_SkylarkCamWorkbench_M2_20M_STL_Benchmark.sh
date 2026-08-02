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
if [ $# -eq 0 ]; then
  echo "[INFO] No STL path passed. Running synthetic 20M chunk/render/upload/BVH/LOD pressure descriptor."
  "${EXE}" --m2-pressure20m
else
  echo "[INFO] Running real Binary STL pressure file: $1"
  "${EXE}" --m2-pressure20m "$1"
fi
