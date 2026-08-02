#!/usr/bin/env bash
set -u

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "${SCRIPT_DIR}/../../.." && pwd)"
BUILD_DIR="${ROOT}/out/build/linux-skylark-cam-workbench-headless"
LOG_DIR="${ROOT}/out/logs"
EXE="${BUILD_DIR}/Tests/SkylarkCamWorkbench/SkylarkCamWorkbenchHeadless"

mkdir -p "${LOG_DIR}"

echo "============================================================"
echo "[SkylarkCamWorkbench] M5 Simulation Validation"
echo "[ROOT ] ${ROOT}"
echo "[EXE  ] ${EXE}"
echo "============================================================"

if [[ ! -x "${EXE}" ]]; then
  echo "[FAIL] Headless executable not found. Build first:"
  echo "       Scripts/Linux/Build/Build_Linux_SkylarkCamWorkbench_Headless.sh"
  exit 1
fi

"${EXE}" > "${LOG_DIR}/run_linux_skylark_cam_workbench_m5_simulation_validation.log" 2>&1
RC=$?
cat "${LOG_DIR}/run_linux_skylark_cam_workbench_m5_simulation_validation.log"
if [[ ${RC} -ne 0 ]]; then
  echo "[FAILED] M5 Simulation validation failed."
  exit ${RC}
fi

echo "[OK] M5 Simulation validation passed."
