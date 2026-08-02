#!/usr/bin/env bash
set -u
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/../../.." && pwd)"
BUILD_DIR="${REPO_ROOT}/out/build/linux-volumestudio-debug"
PACKAGE_DIR="${REPO_ROOT}/out/package/linux-volumestudio"
LOG_DIR="${REPO_ROOT}/out/logs"
PACKAGE_LOG="${LOG_DIR}/package_linux_volumestudio.log"

mkdir -p "${PACKAGE_DIR}/bin" "${PACKAGE_DIR}/docs" "${LOG_DIR}"

echo "============================================================"
echo "[SkylarkVolumeStudio] Linux Package"
echo "[ROOT   ] ${REPO_ROOT}"
echo "[BUILD  ] ${BUILD_DIR}"
echo "[PACKAGE] ${PACKAGE_DIR}"
echo "============================================================"

VALIDATION_EXE="${BUILD_DIR}/Tests/SkylarkVolumeStudio/SkylarkVolumeStudioValidation"
if [[ ! -x "${VALIDATION_EXE}" ]]; then
  VALIDATION_EXE="${BUILD_DIR}/SkylarkVolumeStudioValidation"
fi
if [[ ! -x "${VALIDATION_EXE}" ]]; then
  echo "[FAIL] SkylarkVolumeStudioValidation was not found. Run build_linux.sh first."
  exit 1
fi

cp -f "${VALIDATION_EXE}" "${PACKAGE_DIR}/bin/" > "${PACKAGE_LOG}" 2>&1
if [[ -d "${REPO_ROOT}/Tests/SkylarkVolumeStudio/docs" ]]; then
  cp -R "${REPO_ROOT}/Tests/SkylarkVolumeStudio/docs/." "${PACKAGE_DIR}/docs/" >> "${PACKAGE_LOG}" 2>&1
fi

echo "[OK] Linux package staged at ${PACKAGE_DIR}"
