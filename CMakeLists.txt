cmake_minimum_required(VERSION 3.20)
project(SkylarkEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(SK_BUILD_SAMPLES "Build Skylark samples" ON)
option(SK_BUILD_TOOLS   "Build Skylark tools"   ON)

# Modern RHI backend modules
option(SK_WITH_D3D11_RHI "Build D3D11 RHI backend (modern, not legacy renderer)" OFF)

# Legacy (VSEngine2 / SKEngine2) modules are heavy and Windows-only.
# Keep them buildable for migration, but do NOT require them for the modern embedded runtime.
option(SK_WITH_LEGACY_ENGINE2 "Build legacy SKEngine2 modules (RenderCore/Application/LegacyRHI)" OFF)

# ---- Helpers ----
function(sk_collect_sources out_var dir)
  file(GLOB_RECURSE _src
    "${dir}/*.c" "${dir}/*.cc" "${dir}/*.cpp" "${dir}/*.cxx"
    "${dir}/*.h" "${dir}/*.hpp" "${dir}/*.hh" "${dir}/*.inl"
    "${dir}/*.rc" "${dir}/*.def"
  )
  set(${out_var} ${_src} PARENT_SCOPE)
endfunction()

function(sk_add_module target dir)
  if(NOT EXISTS "${dir}")
    message(FATAL_ERROR "Module dir not found: ${dir}")
  endif()
  sk_collect_sources(_src "${dir}")
  add_library(${target} STATIC ${_src})
  target_include_directories(${target} PUBLIC "${CMAKE_CURRENT_LIST_DIR}/Source")
  target_compile_definitions(${target} PRIVATE SK_MODULE_NAME="${target}")
  if(MSVC)
    target_compile_options(${target} PRIVATE /W4 /permissive-)
  else()
    target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
  endif()
endfunction()

# ---- Core (always) ----
sk_add_module(SKCore        "${CMAKE_CURRENT_LIST_DIR}/Source/SKCore")
sk_add_module(SKMath        "${CMAKE_CURRENT_LIST_DIR}/Source/SKMath")
sk_add_module(SKContainers  "${CMAKE_CURRENT_LIST_DIR}/Source/SKContainers")
target_link_libraries(SKMath       PUBLIC SKCore)
target_link_libraries(SKContainers PUBLIC SKCore)

# ---- Platform (always) ----
sk_add_module(SKPlatform    "${CMAKE_CURRENT_LIST_DIR}/Source/SKPlatform")
target_link_libraries(SKPlatform PUBLIC SKCore)

# ---- Modern RHI / RenderGraph / Renderer / Runtime / Engine (always) ----
sk_add_module(SKRHI         "${CMAKE_CURRENT_LIST_DIR}/Source/SKRHI")
sk_add_module(SKRenderGraph "${CMAKE_CURRENT_LIST_DIR}/Source/SKRenderGraph")
sk_add_module(SKRenderer    "${CMAKE_CURRENT_LIST_DIR}/Source/SKRenderer")
sk_add_module(SKRuntime     "${CMAKE_CURRENT_LIST_DIR}/Source/SKRuntime")
sk_add_module(SKEngine      "${CMAKE_CURRENT_LIST_DIR}/Source/SKEngine")

target_link_libraries(SKRHI         PUBLIC SKCore SKPlatform)
target_link_libraries(SKRenderGraph PUBLIC SKCore SKRHI)
target_link_libraries(SKRenderer    PUBLIC SKCore SKPlatform SKRHI SKRenderGraph SKScene SKInteraction)
target_link_libraries(SKRuntime     PUBLIC SKCore SKPlatform SKRHI SKRenderer SKScene)
target_link_libraries(SKEngine      PUBLIC SKCore SKRenderer SKRuntime)

# ---- Domain extension points (always) ----
sk_add_module(SKScene       "${CMAKE_CURRENT_LIST_DIR}/Source/SKScene")
sk_add_module(SKCAD         "${CMAKE_CURRENT_LIST_DIR}/Source/SKCAD")
sk_add_module(SKInteraction "${CMAKE_CURRENT_LIST_DIR}/Source/SKInteraction")
sk_add_module(SKBIM         "${CMAKE_CURRENT_LIST_DIR}/Source/SKBIM")
sk_add_module(SKCAE         "${CMAKE_CURRENT_LIST_DIR}/Source/SKCAE")

target_link_libraries(SKScene       PUBLIC SKCore)
target_link_libraries(SKCAD         PUBLIC SKCore)
target_link_libraries(SKInteraction PUBLIC SKCore SKCAD SKRHI)
target_link_libraries(SKBIM         PUBLIC SKCore)
target_link_libraries(SKCAE         PUBLIC SKCore)

# ---- Legacy (optional) ----
if(SK_WITH_LEGACY_ENGINE2)
  if(WIN32)
    sk_add_module(SKInput       "${CMAKE_CURRENT_LIST_DIR}/Source/SKInput")
    sk_add_module(SKLegacyRHI   "${CMAKE_CURRENT_LIST_DIR}/Source/SKLegacyRHI")
    sk_add_module(SKRenderCore  "${CMAKE_CURRENT_LIST_DIR}/Source/SKRenderCore")
    sk_add_module(SKApplication "${CMAKE_CURRENT_LIST_DIR}/Source/SKApplication")

    target_link_libraries(SKInput       PUBLIC SKCore)
    target_link_libraries(SKLegacyRHI   PUBLIC SKCore)
    target_include_directories(SKLegacyRHI PUBLIC
      "${CMAKE_CURRENT_LIST_DIR}/Source/SKLegacyRHI/SKD3D11"
      "${CMAKE_CURRENT_LIST_DIR}/Source/SKLegacyRHI/SKD3D9"
    )
    target_link_libraries(SKRenderCore  PUBLIC SKCore SKMath SKContainers)
    target_include_directories(SKRenderCore PUBLIC
      "${CMAKE_CURRENT_LIST_DIR}/Source/SKRenderCore"
      "${CMAKE_CURRENT_LIST_DIR}/Source/SKMath"
      "${CMAKE_CURRENT_LIST_DIR}/Source/SKContainers"
      "${CMAKE_CURRENT_LIST_DIR}/Source/SKCore"
    )
    target_link_libraries(SKApplication PUBLIC SKCore SKMath SKContainers SKInput SKRenderCore SKLegacyRHI)
    target_include_directories(SKApplication PUBLIC
      "${CMAKE_CURRENT_LIST_DIR}/Source/SKApplication"
      "${CMAKE_CURRENT_LIST_DIR}/Source/SKRenderCore"
      "${CMAKE_CURRENT_LIST_DIR}/Source/SKInput"
      "${CMAKE_CURRENT_LIST_DIR}/Source/SKLegacyRHI/SKD3D11"
      "${CMAKE_CURRENT_LIST_DIR}/Source/SKLegacyRHI/SKD3D9"
    )

    # Runtime legacy bridge deps
    target_link_libraries(SKRuntime PUBLIC SKMath SKContainers SKRenderCore SKLegacyRHI)
    target_compile_definitions(SKRuntime PRIVATE SK_RUNTIME_WITH_LEGACY=1)
  else()
    message(WARNING "SK_WITH_LEGACY_ENGINE2 is ON but not on Windows; legacy modules will be skipped.")
  endif()
else()
  target_compile_definitions(SKRuntime PRIVATE SK_RUNTIME_WITH_LEGACY=0)
endif()

# ---- Modern backend modules (optional) ----
if(SK_WITH_D3D11_RHI)
  if(WIN32)
    sk_add_module(SKD3D11RHI "${CMAKE_CURRENT_LIST_DIR}/Source/SKD3D11RHI")
    target_link_libraries(SKD3D11RHI PUBLIC SKCore SKPlatform)
    target_link_libraries(SKD3D11RHI PUBLIC d3d11 dxgi)
    target_link_libraries(SKRHI PUBLIC SKD3D11RHI)
    target_compile_definitions(SKRHI PRIVATE SK_WITH_D3D11_RHI=1)
  else()
    message(WARNING "SK_WITH_D3D11_RHI is ON but not on Windows; will be disabled.")
    target_compile_definitions(SKRHI PRIVATE SK_WITH_D3D11_RHI=0)
  endif()
else()
  target_compile_definitions(SKRHI PRIVATE SK_WITH_D3D11_RHI=0)
endif()

# ---- Tools ----
if(SK_BUILD_TOOLS)
  if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/Tools")
    file(GLOB _tool_dirs LIST_DIRECTORIES true "${CMAKE_CURRENT_LIST_DIR}/Tools/*")
    foreach(_d ${_tool_dirs})
      if(IS_DIRECTORY "${_d}")
        get_filename_component(_name "${_d}" NAME)
        string(REPLACE " " "_" _name "${_name}")
        sk_add_module(${_name} "${_d}")
      endif()
    endforeach()
  endif()
endif()

# ---- Samples ----
if(SK_BUILD_SAMPLES)
  if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/Samples")
    # NOTE: VSEngine2 demos are numerous and may require platform SDKs.
    # You can add a small host sample here later.
  endif()
endif()
