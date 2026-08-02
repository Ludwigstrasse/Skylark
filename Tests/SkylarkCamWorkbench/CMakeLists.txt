cmake_minimum_required(VERSION 3.23)

project(SkylarkCamWorkbench LANGUAGES CXX)

option(SKCAM_BUILD_HEADLESS "Build Linux/CI headless validator" ON)
option(SKCAM_BUILD_QT_APP "Build Qt desktop shell" ON)
option(SKCAM_REQUIRE_QT "Fail when Qt Widgets is unavailable" OFF)

set(SKCAM_CORE_SOURCES
  Source/Common/CamCore.cpp
  Source/App/ProjectSession.cpp
  Source/App/CommandRegistry.cpp
  Source/DataExchange/DataExchange.cpp
  Source/Model/CamProject.cpp
  Source/Model/CamProjectJson.cpp
  Source/LargeMesh/LargeMesh.cpp
  Source/Render/RenderCore.cpp
  Source/CAM/CamDomain.cpp
  Source/CAM/MeshCamGeometryProvider.cpp
  Source/CAM/BRepCamGeometryProvider.cpp
  Source/Toolpath/ToolpathPlannerStubs.cpp
  Source/Simulation/SimulationStubs.cpp
  Source/Post/PostProcessor.cpp
  Source/Diagnostics/BenchmarkReport.cpp
)

add_library(SkylarkCamWorkbenchCore STATIC ${SKCAM_CORE_SOURCES})
target_include_directories(SkylarkCamWorkbenchCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Source)
target_link_libraries(SkylarkCamWorkbenchCore PUBLIC skylark_project_options PRIVATE skylark_project_warnings)
target_compile_definitions(SkylarkCamWorkbenchCore PUBLIC SK_CAM_WORKBENCH_M0=1 SK_CAM_WORKBENCH_M1=1 SK_CAM_WORKBENCH_M2=1 SK_CAM_WORKBENCH_M3=1 SK_CAM_WORKBENCH_M4=1 SK_CAM_WORKBENCH_M5=1 SK_CAM_WORKBENCH_M6=1 SK_CAM_WORKBENCH_M7=1)
set_target_properties(SkylarkCamWorkbenchCore PROPERTIES FOLDER "Skylark/Tests/SkylarkCamWorkbench")

if(SKCAM_BUILD_HEADLESS)
  add_executable(SkylarkCamWorkbenchHeadless Source/App/HeadlessMain.cpp)
  target_link_libraries(SkylarkCamWorkbenchHeadless PRIVATE SkylarkCamWorkbenchCore skylark_project_options skylark_project_warnings)
  set_target_properties(SkylarkCamWorkbenchHeadless PROPERTIES FOLDER "Skylark/Tests/SkylarkCamWorkbench")
endif()

if(SKCAM_BUILD_QT_APP)
  find_package(Qt5 COMPONENTS Widgets QUIET)
  if(NOT Qt5_FOUND)
    find_package(Qt6 COMPONENTS Widgets QUIET)
  endif()

  if(Qt5_FOUND OR Qt6_FOUND)
    add_executable(SkylarkCamWorkbench WIN32
      Source/App/QtMain.cpp
      Source/App/MainWindow.cpp
      Source/App/MainWindow.h
      Source/Render/SkylarkViewportWidget.cpp
      Source/Render/SkylarkViewportWidget.h
    )
    target_link_libraries(SkylarkCamWorkbench PRIVATE SkylarkCamWorkbenchCore skylark_project_options skylark_project_warnings)
    if(Qt5_FOUND)
      target_link_libraries(SkylarkCamWorkbench PRIVATE Qt5::Widgets)
    else()
      target_link_libraries(SkylarkCamWorkbench PRIVATE Qt6::Widgets)
    endif()
    set_target_properties(SkylarkCamWorkbench PROPERTIES
      FOLDER "Skylark/Tests/SkylarkCamWorkbench"
      VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:SkylarkCamWorkbench>")
  else()
    if(SKCAM_REQUIRE_QT)
      message(FATAL_ERROR "Qt Widgets not found. Set Qt5_DIR or CMAKE_PREFIX_PATH, or turn SKCAM_BUILD_QT_APP off.")
    else()
      message(WARNING "Qt Widgets not found; only SkylarkCamWorkbenchHeadless will be built.")
    endif()
  endif()
endif()
