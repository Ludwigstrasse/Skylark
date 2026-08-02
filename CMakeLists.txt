cmake_minimum_required(VERSION 3.23)

project(Skylark VERSION 0.1.0 LANGUAGES CXX)

include(GNUInstallDirs)
include(CMakeDependentOption)

option(SK_BUILD_LEGACY "Build legacy SKEngine2 modules" OFF)
option(SK_BUILD_INSTALL "Enable install/export rules" ON)
option(SK_BUILD_SHARED "Build Skylark libraries as shared libraries" OFF)
option(SK_BUILD_SMOKE_TEST "Build a tiny host-side link smoke test" OFF)
option(SK_BUILD_CADSCENE_TEST_APP "Build the Skylark CAD scene techniques OpenGL test application" OFF)
option(SK_BUILD_CAD_EDITOR_LAB "Build the Skylark CAD editor lab runtime test application" OFF)
option(SK_BUILD_FORMAT_WORKBENCH "Build the Skylark format workbench commercial test application" OFF)
option(SK_BUILD_CAM_WORKBENCH "Build the Skylark CAM workbench commercial test application" OFF)
option(SK_BUILD_VOLUME_STUDIO "Build the Skylark Volume Studio commercial validation application under Tests" OFF)
option(SK_BUILD_VOLUME_STUDIO_QT "Build the Qt desktop shell for Skylark Volume Studio" OFF)
option(SK_ENABLE_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)
option(SK_ENABLE_OPENGL "Enable OpenGL RHI backend" OFF)
option(SK_ENABLE_VULKAN "Enable Vulkan RHI backend" OFF)
cmake_dependent_option(SK_ENABLE_D3D11 "Enable D3D11 RHI backend" OFF "WIN32" OFF)
cmake_dependent_option(SK_ENABLE_D3D12 "Enable D3D12 RHI backend" OFF "WIN32" OFF)

set(BUILD_SHARED_LIBS ${SK_BUILD_SHARED})
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_library(skylark_project_options INTERFACE)
add_library(skylark_project_warnings INTERFACE)

target_compile_features(skylark_project_options INTERFACE cxx_std_20)

if(WIN32)
  target_compile_definitions(skylark_project_options INTERFACE
    SK_PLATFORM_WINDOWS=1
    SK_PLATFORM_LINUX=0
    SK_PLATFORM_MAC=0
    WIN32_LEAN_AND_MEAN
    NOMINMAX)
elseif(APPLE)
  target_compile_definitions(skylark_project_options INTERFACE SK_PLATFORM_WINDOWS=0 SK_PLATFORM_LINUX=0 SK_PLATFORM_MAC=1)
else()
  target_compile_definitions(skylark_project_options INTERFACE SK_PLATFORM_WINDOWS=0 SK_PLATFORM_LINUX=1 SK_PLATFORM_MAC=0)
endif()

target_compile_definitions(skylark_project_options INTERFACE
  $<$<BOOL:${SK_BUILD_LEGACY}>:SK_RUNTIME_WITH_LEGACY=1>
  $<$<NOT:$<BOOL:${SK_BUILD_LEGACY}>>:SK_RUNTIME_WITH_LEGACY=0>
  $<$<BOOL:${SK_ENABLE_D3D11}>:SK_WITH_D3D11_RHI=1>
  $<$<NOT:$<BOOL:${SK_ENABLE_D3D11}>>:SK_WITH_D3D11_RHI=0>
  $<$<BOOL:${SK_ENABLE_D3D12}>:SK_WITH_D3D12_RHI=1>
  $<$<NOT:$<BOOL:${SK_ENABLE_D3D12}>>:SK_WITH_D3D12_RHI=0>
  $<$<BOOL:${SK_ENABLE_OPENGL}>:SK_WITH_OPENGL_RHI=1>
  $<$<NOT:$<BOOL:${SK_ENABLE_OPENGL}>>:SK_WITH_OPENGL_RHI=0>
  $<$<BOOL:${SK_ENABLE_VULKAN}>:SK_WITH_VULKAN_RHI=1>
  $<$<NOT:$<BOOL:${SK_ENABLE_VULKAN}>>:SK_WITH_VULKAN_RHI=0>
)

if(MSVC)
  target_compile_options(skylark_project_options INTERFACE /permissive- /Zc:__cplusplus /EHsc /utf-8)
  target_compile_options(skylark_project_warnings INTERFACE /W4)
  if(SK_ENABLE_WARNINGS_AS_ERRORS)
    target_compile_options(skylark_project_warnings INTERFACE /WX)
  endif()
else()
  target_compile_options(skylark_project_warnings INTERFACE -Wall -Wextra -Wpedantic)
  if(SK_ENABLE_WARNINGS_AS_ERRORS)
    target_compile_options(skylark_project_warnings INTERFACE -Werror)
  endif()
endif()

set_target_properties(skylark_project_options skylark_project_warnings PROPERTIES FOLDER "Skylark/Internal")

if(SK_BUILD_INSTALL)
  install(TARGETS skylark_project_options skylark_project_warnings EXPORT SkylarkTargets)
endif()

function(sk_add_module target)
  set(options)
  set(oneValueArgs)
  set(multiValueArgs SOURCES PUBLIC_DEPS PRIVATE_DEPS PUBLIC_INCLUDE_DIRS PRIVATE_INCLUDE_DIRS PUBLIC_DEFINES PRIVATE_DEFINES LINK_LIBS)
  cmake_parse_arguments(SK "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  if(NOT SK_SOURCES)
    add_library(${target} INTERFACE)
  else()
    add_library(${target} ${SK_SOURCES})
  endif()

  add_library(Skylark::${target} ALIAS ${target})

  if(SK_SOURCES)
    target_link_libraries(${target}
      PUBLIC skylark_project_options
      PRIVATE skylark_project_warnings
      PUBLIC ${SK_PUBLIC_DEPS}
      PRIVATE ${SK_PRIVATE_DEPS}
      PRIVATE ${SK_LINK_LIBS}
    )
    target_include_directories(${target}
      PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Source>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
        ${SK_PUBLIC_INCLUDE_DIRS}
      PRIVATE
        ${SK_PRIVATE_INCLUDE_DIRS}
    )
    if(SK_PUBLIC_DEFINES)
      target_compile_definitions(${target} PUBLIC ${SK_PUBLIC_DEFINES})
    endif()
    if(SK_PRIVATE_DEFINES)
      target_compile_definitions(${target} PRIVATE ${SK_PRIVATE_DEFINES})
    endif()
  else()
    target_link_libraries(${target} INTERFACE skylark_project_options ${SK_PUBLIC_DEPS})
    target_include_directories(${target} INTERFACE
      "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Source>"
      "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
      ${SK_PUBLIC_INCLUDE_DIRS})
    if(SK_PUBLIC_DEFINES)
      target_compile_definitions(${target} INTERFACE ${SK_PUBLIC_DEFINES})
    endif()
  endif()

  set_target_properties(${target} PROPERTIES FOLDER "Skylark")
endfunction()

function(sk_install_target target)
  if(NOT SK_BUILD_INSTALL)
    return()
  endif()
  install(TARGETS ${target}
    EXPORT SkylarkTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endfunction()

if(SK_ENABLE_OPENGL)
  find_package(OpenGL QUIET)
endif()
if(SK_ENABLE_VULKAN)
  find_package(Vulkan QUIET)
endif()

sk_add_module(SKCore SOURCES Source/SKCore/SKCore.cpp)
sk_add_module(SKLWC SOURCES Source/SKLWC/SKLWC.cpp PUBLIC_DEPS SKCore)
sk_add_module(SKProfiler SOURCES Source/SKProfiler/SKProfiler.cpp PUBLIC_DEPS SKCore)
sk_add_module(SKGeometry SOURCES Source/SKGeometry/SKGeometryRegistry.cpp Source/SKGeometry/SKMeshBuilder.cpp PUBLIC_DEPS SKCore)
sk_add_module(SKField SOURCES Source/SKField/SKField.cpp PUBLIC_DEPS SKCore)
sk_add_module(SKPlatform
  SOURCES
    Source/SKPlatform/SKPlatform.cpp
    $<$<PLATFORM_ID:Windows>:Source/SKPlatform/SKWindowsPlatform.cpp>
    $<$<PLATFORM_ID:Linux>:Source/SKPlatform/SKLinuxPlatform.cpp>
    $<$<PLATFORM_ID:Darwin>:Source/SKPlatform/SKMacPlatform.cpp>
  PUBLIC_DEPS SKCore)
sk_add_module(SKScene SOURCES Source/SKScene/SKScene.cpp Source/SKScene/SKSceneGraph.cpp Source/SKScene/SKPagedLOD.cpp PUBLIC_DEPS SKCore SKLWC)
sk_add_module(SKRHI SOURCES Source/SKRHI/SKRHIFactory.cpp Source/SKRHI/SKRHIRuntimeConfig.cpp Source/SKRHI/SKNullRHI/SKNullRHI.cpp PUBLIC_DEPS SKCore SKPlatform)
sk_add_module(SKRenderGraph SOURCES Source/SKRenderGraph/SKRenderGraph.cpp PUBLIC_DEPS SKCore SKProfiler SKRHI)
sk_add_module(SKAnnotation SOURCES Source/SKAnnotation/SKAnnotation.cpp PUBLIC_DEPS SKCore SKScene)
sk_add_module(SKCAD SOURCES Source/SKCAD/SKCadImporter.cpp PUBLIC_DEPS SKCore SKGeometry)
sk_add_module(SKInteraction SOURCES Source/SKInteraction/SKInteraction.cpp Source/SKInteraction/SKHitProxyTable.cpp Source/SKInteraction/SKGpuPicker.cpp PUBLIC_DEPS SKCore SKCAD SKRHI)
sk_add_module(SKBIM SOURCES Source/SKBIM/SKBIM.cpp Source/SKBIM/SKBIMVisibility.cpp PUBLIC_DEPS SKCore SKScene)
sk_add_module(SKCAE SOURCES Source/SKCAE/SKCAE.cpp PUBLIC_DEPS SKCore)
sk_add_module(SKStreaming SOURCES Source/SKStreaming/SKStreaming.cpp Source/SKStreaming/SKDatabasePager.cpp PUBLIC_DEPS SKCore SKScene)
sk_add_module(SKRenderThread SOURCES Source/SKRenderThread/SKRenderThread.cpp PUBLIC_DEPS SKCore)
sk_add_module(SKLineRender SOURCES Source/SKLineRender/SKLineRender.cpp PUBLIC_DEPS SKCore SKGeometry SKRHI SKScene)
sk_add_module(SKFieldViz SOURCES Source/SKFieldViz/SKFieldViz.cpp Source/SKFieldViz/SKIsoSurface.cpp PUBLIC_DEPS SKCore SKField SKGeometry SKLineRender SKRHI SKScene)
sk_add_module(SKRenderer
  SOURCES
    Source/SKRenderer/SKRenderer.cpp
    Source/SKRenderer/SKRenderPipeline.cpp
    Source/SKRenderer/SKDefaultPasses.cpp
    Source/SKRenderer/SKTriangleSceneRenderer.cpp
    Source/SKRenderer/SKMeshRenderer.cpp
    Source/SKRenderer/SKSelectionRenderer.cpp
    Source/SKRenderer/SKSelectionOverlayRenderer.cpp
    Source/SKRenderer/SKMeshDrawPacket.cpp
    Source/SKRenderer/SKPacketBuildUtils.cpp
    Source/SKRenderer/SKRenderScratchBuffer.cpp
    Source/SKRenderer/SKEdgeDrawPacket.cpp
    Source/SKRenderer/SKGeometryResidencyCache.cpp
    Source/SKRenderer/SKGpuResidentBufferCache.cpp
    Source/SKRenderer/SKSceneUploadQueue.cpp
    Source/SKRenderer/SKRenderBatching.cpp
    Source/SKRenderer/SKInstanceDrawPacket.cpp
    Source/SKRenderer/SKIndirectReadyPacket.cpp
  PUBLIC_DEPS SKCore SKPlatform SKProfiler SKRHI SKRenderGraph SKScene SKInteraction SKLineRender SKAnnotation SKFieldViz)
sk_add_module(SKShaderCore SOURCES Source/SKShaderCore/SKShaderCore.cpp PUBLIC_DEPS SKCore)
sk_add_module(SKShaderCompiler SOURCES Source/SKShaderCompiler/SKShaderCompiler.cpp PUBLIC_DEPS SKCore SKShaderCore)
sk_add_module(SKPipelineCache SOURCES Source/SKPipelineCache/SKPipelineCache.cpp PUBLIC_DEPS SKCore SKShaderCore)
sk_add_module(SKHZB SOURCES Source/SKHZB/SKHZB.cpp PUBLIC_DEPS SKCore SKRHI)
sk_add_module(SKGpuCulling SOURCES Source/SKGpuCulling/SKGpuCulling.cpp PUBLIC_DEPS SKCore SKHZB SKScene)
sk_add_module(SKMeasurement SOURCES Source/SKMeasurement/SKMeasurement.cpp PUBLIC_DEPS SKCore)
sk_add_module(SKPMI SOURCES Source/SKPMI/SKPMI.cpp PUBLIC_DEPS SKCore)
sk_add_module(SKTaskGraph SOURCES Source/SKTaskGraph/SKTaskGraph.cpp PUBLIC_DEPS SKCore)
sk_add_module(SKRuntime SOURCES Source/SKRuntime/SKRuntimeEngine.cpp Source/SKRuntime/SKLegacyEngine2Context.cpp PUBLIC_DEPS SKCore SKGeometry SKField SKAnnotation SKRenderer SKRHI SKScene SKBIM SKStreaming SKRenderThread)
sk_add_module(SKEngine SOURCES Source/SKEngine/SKEngine.cpp PUBLIC_DEPS SKCore SKRenderer SKRHI SKRuntime)

if(SK_ENABLE_OPENGL)
  sk_add_module(SKOpenGLRHI SOURCES Source/SKOpenGLRHI/SKOpenGLRHI.cpp PUBLIC_DEPS SKCore SKRHI)
  if(TARGET OpenGL::GL)
    target_link_libraries(SKOpenGLRHI PRIVATE OpenGL::GL)
  elseif(WIN32)
    target_link_libraries(SKOpenGLRHI PRIVATE opengl32)
  endif()
  target_link_libraries(SKRHI PUBLIC SKOpenGLRHI)
  sk_install_target(SKOpenGLRHI)
endif()

if(SK_ENABLE_VULKAN)
  sk_add_module(SKVulkanRHI SOURCES Source/SKVulkanRHI/SKVulkanRHI.cpp PUBLIC_DEPS SKCore SKRHI)
  if(TARGET Vulkan::Vulkan)
    target_link_libraries(SKVulkanRHI PRIVATE Vulkan::Vulkan)
  elseif(UNIX AND NOT APPLE)
    target_link_libraries(SKVulkanRHI PRIVATE ${CMAKE_DL_LIBS})
  endif()
  target_link_libraries(SKRHI PUBLIC SKVulkanRHI)
  sk_install_target(SKVulkanRHI)
endif()

if(SK_ENABLE_D3D11)
  sk_add_module(SKD3D11RHI SOURCES Source/SKD3D11RHI/SKD3D11RHI.cpp PUBLIC_DEPS SKCore SKRHI)
  target_link_libraries(SKD3D11RHI PRIVATE d3d11 dxgi d3dcompiler)
  target_link_libraries(SKRHI PUBLIC SKD3D11RHI)
  sk_install_target(SKD3D11RHI)
endif()

if(SK_ENABLE_D3D12)
  sk_add_module(SKD3D12RHI SOURCES Source/SKD3D12RHI/SKD3D12RHI.cpp PUBLIC_DEPS SKCore SKRHI SKShaderCompiler SKPipelineCache)
  target_link_libraries(SKD3D12RHI PRIVATE d3d12 dxgi d3dcompiler)
  target_link_libraries(SKRHI PUBLIC SKD3D12RHI)
  sk_install_target(SKD3D12RHI)
endif()

if(SK_BUILD_LEGACY)
  message(STATUS "Legacy SKEngine2 build path is enabled. This path remains Windows-only and is intentionally isolated from the default portable graph.")
endif()

foreach(target SKCore SKLWC SKProfiler SKGeometry SKField SKPlatform SKScene SKRHI SKRenderGraph SKAnnotation SKCAD SKInteraction SKBIM SKCAE SKStreaming SKRenderThread SKLineRender SKFieldViz SKRenderer SKShaderCore SKShaderCompiler SKPipelineCache SKHZB SKGpuCulling SKMeasurement SKPMI SKTaskGraph SKEngine SKRuntime)
  sk_install_target(${target})
endforeach()

if(SK_BUILD_SMOKE_TEST)
  add_executable(SkylarkSmoke Tests/SkylarkSmokeMain.cpp)
  target_link_libraries(SkylarkSmoke PRIVATE SKEngine)
  target_link_libraries(SkylarkSmoke PRIVATE skylark_project_options skylark_project_warnings)
  set_target_properties(SkylarkSmoke PROPERTIES FOLDER "Skylark/Tests")
endif()

if(SK_BUILD_CADSCENE_TEST_APP)
  find_package(OpenGL REQUIRED)
  find_package(glfw3 CONFIG REQUIRED)
  find_package(GLEW REQUIRED)
  find_package(ZLIB REQUIRED)

  set(SK_CADSCENE_TEST_SOURCES
  Tests/Shared/SkSampleAppOptions.cpp
  Tests/Shared/SkSampleAppOptions.h
  Tests/Shared/SkSampleAppHost.cpp
  Tests/Shared/SkSampleAppHost.h
  Tests/Shared/SkSampleCameraOptions.cpp
  Tests/Shared/SkSampleCameraOptions.h
  Tests/Shared/SkSampleRuntimeStats.cpp
  Tests/Shared/SkSampleRuntimeStats.h
  Tests/Shared/SkSampleBenchmarkOptions.cpp
  Tests/Shared/SkSampleBenchmarkOptions.h
  Tests/Shared/SkSampleRunReport.cpp
  Tests/Shared/SkSampleRunReport.h
  Tests/Shared/SkSampleSceneManifest.cpp
  Tests/Shared/SkSampleSceneManifest.h
  Tests/Shared/SkSampleOutputLayout.cpp
  Tests/Shared/SkSampleOutputLayout.h
  Tests/Shared/SkSampleControls.cpp
  Tests/Shared/SkSampleControls.h
  Tests/Shared/SkCadSceneDiagnostics.cpp
  Tests/Shared/SkCadSceneDiagnostics.h
  Tests/SkylarkCadSceneTechniques/Source/SkCadSceneApp.cpp
  Tests/SkylarkCadSceneTechniques/Source/SkCadSceneImporter.cpp
  Tests/SkylarkCadSceneTechniques/Source/SkCadSceneImporter.h
  Tests/SkylarkCadSceneTechniques/Source/SkCadSceneFile.cpp
  Tests/SkylarkCadSceneTechniques/Source/SkCadSceneFile.h
)

  add_executable(SkylarkCadSceneTechniques ${SK_CADSCENE_TEST_SOURCES})
  target_link_libraries(SkylarkCadSceneTechniques PRIVATE SKCore SKGeometry skylark_project_options skylark_project_warnings ZLIB::ZLIB GLEW::GLEW OpenGL::GL)

  if(TARGET glfw)
    target_link_libraries(SkylarkCadSceneTechniques PRIVATE glfw)
  elseif(TARGET glfw3::glfw)
    target_link_libraries(SkylarkCadSceneTechniques PRIVATE glfw3::glfw)
  else()
    message(FATAL_ERROR "Could not find an imported GLFW target from glfw3 package.")
  endif()

  target_compile_definitions(SkylarkCadSceneTechniques PRIVATE CSF_ZIP_SUPPORT=1)
  target_include_directories(SkylarkCadSceneTechniques PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Tests/SkylarkCadSceneTechniques/Source)
  set_target_properties(SkylarkCadSceneTechniques PROPERTIES
    FOLDER "Skylark/Tests"
    VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:SkylarkCadSceneTechniques>")

  add_custom_command(TARGET SkylarkCadSceneTechniques POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:SkylarkCadSceneTechniques>/Tests/SkylarkCadSceneTechniques/Assets
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      ${CMAKE_CURRENT_SOURCE_DIR}/Tests/SkylarkCadSceneTechniques/Assets/geforce.csf.gz
      $<TARGET_FILE_DIR:SkylarkCadSceneTechniques>/Tests/SkylarkCadSceneTechniques/Assets/geforce.csf.gz)
endif()


if(SK_BUILD_CAD_EDITOR_LAB)
  find_package(glfw3 CONFIG REQUIRED)
  find_package(ZLIB REQUIRED)

  set(SK_CAD_EDITOR_LAB_SOURCES
  Tests/Shared/SkSampleAppOptions.cpp
  Tests/Shared/SkSampleAppOptions.h
  Tests/Shared/SkSampleAppHost.cpp
  Tests/Shared/SkSampleAppHost.h
  Tests/Shared/SkSampleCameraOptions.cpp
  Tests/Shared/SkSampleCameraOptions.h
  Tests/Shared/SkSampleRuntimeStats.cpp
  Tests/Shared/SkSampleRuntimeStats.h
  Tests/Shared/SkSampleBenchmarkOptions.cpp
  Tests/Shared/SkSampleBenchmarkOptions.h
  Tests/Shared/SkSampleRunReport.cpp
  Tests/Shared/SkSampleRunReport.h
  Tests/Shared/SkSampleSceneManifest.cpp
  Tests/Shared/SkSampleSceneManifest.h
  Tests/Shared/SkSampleOutputLayout.cpp
  Tests/Shared/SkSampleOutputLayout.h
  Tests/Shared/SkSampleControls.cpp
  Tests/Shared/SkSampleControls.h
  Tests/Shared/SkCadSceneDiagnostics.cpp
  Tests/Shared/SkCadSceneDiagnostics.h
  Tests/SkylarkCadEditorLab/Source/SkCadEditorLab.cpp
  Tests/SkylarkCadSceneTechniques/Source/SkCadSceneImporter.cpp
  Tests/SkylarkCadSceneTechniques/Source/SkCadSceneImporter.h
  Tests/SkylarkCadSceneTechniques/Source/SkCadSceneFile.cpp
  Tests/SkylarkCadSceneTechniques/Source/SkCadSceneFile.h
)

  set(SK_CAD_EDITOR_LAB_DEFAULT_RHI "auto")
  if(SK_ENABLE_D3D12 AND NOT SK_ENABLE_D3D11 AND NOT SK_ENABLE_OPENGL AND NOT SK_ENABLE_VULKAN)
    set(SK_CAD_EDITOR_LAB_DEFAULT_RHI "d3d12")
  elseif(SK_ENABLE_D3D11 AND NOT SK_ENABLE_D3D12 AND NOT SK_ENABLE_OPENGL AND NOT SK_ENABLE_VULKAN)
    set(SK_CAD_EDITOR_LAB_DEFAULT_RHI "d3d11")
  elseif(SK_ENABLE_VULKAN AND NOT SK_ENABLE_D3D11 AND NOT SK_ENABLE_D3D12 AND NOT SK_ENABLE_OPENGL)
    set(SK_CAD_EDITOR_LAB_DEFAULT_RHI "vulkan")
  elseif(SK_ENABLE_OPENGL AND NOT SK_ENABLE_D3D11 AND NOT SK_ENABLE_D3D12 AND NOT SK_ENABLE_VULKAN)
    set(SK_CAD_EDITOR_LAB_DEFAULT_RHI "opengl")
  endif()

  add_executable(SkylarkCadEditorLab ${SK_CAD_EDITOR_LAB_SOURCES})
  target_link_libraries(SkylarkCadEditorLab PRIVATE
    SKCore SKGeometry SKScene SKRHI SKInteraction SKRenderer SKRuntime SKEngine
    skylark_project_options skylark_project_warnings ZLIB::ZLIB)

  if(TARGET glfw)
    target_link_libraries(SkylarkCadEditorLab PRIVATE glfw)
  elseif(TARGET glfw3::glfw)
    target_link_libraries(SkylarkCadEditorLab PRIVATE glfw3::glfw)
  else()
    message(FATAL_ERROR "Could not find an imported GLFW target from glfw3 package.")
  endif()

  target_compile_definitions(SkylarkCadEditorLab PRIVATE
    CSF_ZIP_SUPPORT=1
    SK_CAD_EDITOR_LAB_DEFAULT_RHI="${SK_CAD_EDITOR_LAB_DEFAULT_RHI}")
  target_include_directories(SkylarkCadEditorLab PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Tests/SkylarkCadEditorLab/Source
    ${CMAKE_CURRENT_SOURCE_DIR}/Tests/SkylarkCadSceneTechniques/Source)
  set_target_properties(SkylarkCadEditorLab PROPERTIES
    FOLDER "Skylark/Tests"
    VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:SkylarkCadEditorLab>")

  add_custom_command(TARGET SkylarkCadEditorLab POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:SkylarkCadEditorLab>/Tests/SkylarkCadEditorLab/Assets
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:SkylarkCadEditorLab>/Tests/SkylarkCadSceneTechniques/Assets
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      ${CMAKE_CURRENT_SOURCE_DIR}/Tests/SkylarkCadSceneTechniques/Assets/geforce.csf.gz
      $<TARGET_FILE_DIR:SkylarkCadEditorLab>/Tests/SkylarkCadEditorLab/Assets/geforce.csf.gz
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      ${CMAKE_CURRENT_SOURCE_DIR}/Tests/SkylarkCadSceneTechniques/Assets/geforce.csf.gz
      $<TARGET_FILE_DIR:SkylarkCadEditorLab>/Tests/SkylarkCadSceneTechniques/Assets/geforce.csf.gz)
endif()

if(SK_BUILD_FORMAT_WORKBENCH)
  add_subdirectory(Tests/SkylarkFormatWorkbench)
endif()
if(SK_BUILD_CAM_WORKBENCH)
  add_subdirectory(Tests/SkylarkCamWorkbench)
endif()
if(SK_BUILD_VOLUME_STUDIO)
  add_subdirectory(Tests/SkylarkVolumeStudio)
endif()
if(SK_BUILD_INSTALL)
  install(DIRECTORY Source/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp" PATTERN "*.inl")
  install(EXPORT SkylarkTargets FILE SkylarkTargets.cmake NAMESPACE Skylark:: DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Skylark)
endif()
