#!/usr/bin/env bash
set -u
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "${SCRIPT_DIR}/../../.." && pwd)"
BUILD_DIR="${ROOT_DIR}/out/build/linux-skylark-cam-workbench-headless"
LOG_DIR="${ROOT_DIR}/out/logs"
CFG_LOG="${LOG_DIR}/configure_linux_skylark_cam_workbench_headless.log"
BLD_LOG="${LOG_DIR}/build_linux_skylark_cam_workbench_headless.log"
RUN_LOG="${LOG_DIR}/run_linux_skylark_cam_workbench_headless.log"
mkdir -p "${BUILD_DIR}" "${LOG_DIR}"

echo "============================================================"
echo "[SkylarkCamWorkbench] Linux Headless Build"
echo "[ROOT ] ${ROOT_DIR}"
echo "[BUILD] ${BUILD_DIR}"
echo "============================================================"

if [ ! -f "${ROOT_DIR}/Tests/SkylarkCamWorkbench/CMakeLists.txt" ]; then
  echo "[FAIL] Tests/SkylarkCamWorkbench/CMakeLists.txt was not found."
  exit 2
fi

cmake -S "${ROOT_DIR}" -B "${BUILD_DIR}" -G Ninja \
  -DCMAKE_BUILD_TYPE=Debug \
  -DSK_BUILD_INSTALL=OFF \
  -DSK_BUILD_CAM_WORKBENCH=ON \
  -DSKCAM_BUILD_HEADLESS=ON \
  -DSKCAM_BUILD_QT_APP=OFF > "${CFG_LOG}" 2>&1
if [ $? -ne 0 ]; then
  echo "[FAIL] configure failed"
  tail -160 "${CFG_LOG}"
  exit 10
fi

cmake --build "${BUILD_DIR}" --target SkylarkCamWorkbenchHeadless -j 2 > "${BLD_LOG}" 2>&1
if [ $? -ne 0 ]; then
  echo "[FAIL] build failed"
  tail -220 "${BLD_LOG}"
  exit 20
fi

EXE="${BUILD_DIR}/Tests/SkylarkCamWorkbench/SkylarkCamWorkbenchHeadless"
if [ ! -x "${EXE}" ]; then
  EXE="${BUILD_DIR}/SkylarkCamWorkbenchHeadless"
fi
"${EXE}" > "${RUN_LOG}" 2>&1
if [ $? -ne 0 ]; then
  echo "[FAIL] headless validation failed"
  cat "${RUN_LOG}"
  exit 30
fi

cat "${RUN_LOG}"
echo "[OK] Linux SkylarkCamWorkbench headless build succeeded."
