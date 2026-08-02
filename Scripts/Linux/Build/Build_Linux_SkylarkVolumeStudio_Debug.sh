#!/usr/bin/env bash
set -u

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/../../.." && pwd)"
BUILD_DIR="${REPO_ROOT}/out/build/linux-volumestudio-debug"
LOG_DIR="${REPO_ROOT}/out/logs"
CONFIGURE_LOG="${LOG_DIR}/configure_linux_volumestudio_debug.log"
BUILD_LOG="${LOG_DIR}/build_linux_volumestudio_debug.log"
RUN_LOG="${LOG_DIR}/run_linux_volumestudio_validation.log"
JOBS="${PARALLEL_JOBS:-4}"

mkdir -p "${BUILD_DIR}" "${LOG_DIR}"

print_header() {
  echo "============================================================"
  echo "[SkylarkVolumeStudio] Linux Debug Build"
  echo "[ROOT ] ${REPO_ROOT}"
  echo "[BUILD] ${BUILD_DIR}"
  echo "[LOGS ] ${LOG_DIR}"
  echo "[JOBS ] ${JOBS}"
  echo "============================================================"
}

fail_with_log_tail() {
  local message="$1"
  local log_file="$2"
  echo "[FAIL] ${message}"
  if [[ -f "${log_file}" ]]; then
    echo "------------------------------------------------------------"
    echo "Last 120 lines of ${log_file}"
    echo "------------------------------------------------------------"
    tail -120 "${log_file}"
  fi
  exit 1
}

resolve_executable() {
  local target_name="$1"
  local candidate
  for candidate in \
    "${BUILD_DIR}/bin/Debug/${target_name}" \
    "${BUILD_DIR}/bin/${target_name}" \
    "${BUILD_DIR}/Tests/SkylarkVolumeStudio/${target_name}" \
    "${BUILD_DIR}/${target_name}"; do
    if [[ -x "${candidate}" ]]; then
      echo "${candidate}"
      return 0
    fi
  done
  return 1
}

print_header

if ! command -v cmake >/dev/null 2>&1; then
  echo "[FAIL] cmake was not found in PATH."
  exit 1
fi

CMAKE_GENERATOR="Ninja"
if ! command -v ninja >/dev/null 2>&1; then
  CMAKE_GENERATOR="Unix Makefiles"
fi

CXX_COMPILER="${CXX:-}"
if [[ -z "${CXX_COMPILER}" ]]; then
  if command -v g++ >/dev/null 2>&1; then
    CXX_COMPILER="$(command -v g++)"
  elif command -v c++ >/dev/null 2>&1; then
    CXX_COMPILER="$(command -v c++)"
  elif command -v clang++ >/dev/null 2>&1; then
    CXX_COMPILER="$(command -v clang++)"
  else
    echo "[FAIL] no C++ compiler was found in PATH."
    exit 1
  fi
fi

# Do not force lld by default. Users can opt in explicitly with:
#   SVE_LINUX_LINKER_FLAGS=-fuse-ld=lld bash build_linux.sh
LINKER_FLAGS="${SVE_LINUX_LINKER_FLAGS:-}"
echo "[STEP] configure"
echo "[CXX ] ${CXX_COMPILER}"
echo "[GEN ] ${CMAKE_GENERATOR}"
echo "[LDF ] ${LINKER_FLAGS}"
cmake -S "${REPO_ROOT}" -B "${BUILD_DIR}" -G "${CMAKE_GENERATOR}" \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY \
  -DCMAKE_CXX_COMPILER="${CXX_COMPILER}" \
  -DCMAKE_EXE_LINKER_FLAGS="${LINKER_FLAGS}" \
  -DCMAKE_SHARED_LINKER_FLAGS="${LINKER_FLAGS}" \
  -DSK_BUILD_VOLUME_STUDIO=ON \
  -DSK_BUILD_VOLUME_STUDIO_QT=OFF \
  -DSK_BUILD_SMOKE_TEST=OFF \
  -DSK_BUILD_CADSCENE_TEST_APP=OFF \
  -DSK_BUILD_CAD_EDITOR_LAB=OFF \
  -DSK_ENABLE_OPENGL=OFF \
  -DSK_ENABLE_VULKAN=OFF \
  -DSK_BUILD_INSTALL=ON \
  > "${CONFIGURE_LOG}" 2>&1
if [[ $? -ne 0 ]]; then
  fail_with_log_tail "configure failed" "${CONFIGURE_LOG}"
fi

if [[ "${CMAKE_GENERATOR}" == "Ninja" ]]; then
  echo "[STEP] build validation target with ninja"
  ninja -C "${BUILD_DIR}" SkylarkVolumeStudioValidation -j "${JOBS}" > "${BUILD_LOG}" 2>&1
else
  echo "[STEP] build validation target with cmake --build"
  cmake --build "${BUILD_DIR}" --target SkylarkVolumeStudioValidation --parallel "${JOBS}" > "${BUILD_LOG}" 2>&1
fi
if [[ $? -ne 0 ]]; then
  fail_with_log_tail "build failed" "${BUILD_LOG}"
fi

echo "[STEP] run validation"
VALIDATION_EXE="$(resolve_executable SkylarkVolumeStudioValidation || true)"
if [[ -z "${VALIDATION_EXE}" ]]; then
  fail_with_log_tail "validation executable was not found" "${BUILD_LOG}"
fi
echo "[EXE ] ${VALIDATION_EXE}"
"${VALIDATION_EXE}" > "${RUN_LOG}" 2>&1
if [[ $? -ne 0 ]]; then
  fail_with_log_tail "validation failed" "${RUN_LOG}"
fi
cat "${RUN_LOG}"

echo "[OK] Linux build and validation completed."
echo "[BIN] ${BUILD_DIR}/bin"
