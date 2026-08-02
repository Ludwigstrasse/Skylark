# SkylarkVolumeStudio is intentionally hosted under SkylarkEngine/Tests.
# It is a commercial validation application for high-volume model import,
# voxelization and future volume rendering integration, not a standalone sample.

set(SK_VOLUME_STUDIO_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(SK_VOLUME_STUDIO_SOURCE ${SK_VOLUME_STUDIO_ROOT}/Source)
set(SK_VOLUME_STUDIO_OUTPUT_ROOT "${CMAKE_BINARY_DIR}/bin")

function(sk_volume_studio_apply_runtime_layout target_name)
    set_target_properties(${target_name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${SK_VOLUME_STUDIO_OUTPUT_ROOT}"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${SK_VOLUME_STUDIO_OUTPUT_ROOT}/Debug"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${SK_VOLUME_STUDIO_OUTPUT_ROOT}/Release"
        RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${SK_VOLUME_STUDIO_OUTPUT_ROOT}/RelWithDebInfo"
        RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${SK_VOLUME_STUDIO_OUTPUT_ROOT}/MinSizeRel")
endfunction()

add_library(SKVolumeStudioCore
    Source/Core/SKVolumeStudioTypes.h
    Source/Core/SKVolumeStudioMeshAsset.h
    Source/Core/SKVolumeStudioMeshAsset.cpp)
add_library(Skylark::SKVolumeStudioCore ALIAS SKVolumeStudioCore)
target_include_directories(SKVolumeStudioCore
    PUBLIC
        "$<BUILD_INTERFACE:${SK_VOLUME_STUDIO_SOURCE}>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/SkylarkVolumeStudio>")
target_link_libraries(SKVolumeStudioCore
    PUBLIC
        SKCore
        SKGeometry
        skylark_project_options
    PRIVATE
        skylark_project_warnings)
set_target_properties(SKVolumeStudioCore PROPERTIES FOLDER "Skylark/Tests/SkylarkVolumeStudio")

add_library(SKVolumeStudioModelIO
    Source/ModelIO/SKStlFastReader.h
    Source/ModelIO/SKStlFastReader.cpp)
add_library(Skylark::SKVolumeStudioModelIO ALIAS SKVolumeStudioModelIO)
target_link_libraries(SKVolumeStudioModelIO
    PUBLIC
        SKVolumeStudioCore
    PRIVATE
        skylark_project_warnings)
set_target_properties(SKVolumeStudioModelIO PROPERTIES FOLDER "Skylark/Tests/SkylarkVolumeStudio")

add_library(SKVolumeStudioVoxel
    Source/Voxel/SKVolumeVoxelGrid.h
    Source/Voxel/SKVolumeVoxelGrid.cpp
    Source/Voxel/SKHexahedralMesh.h
    Source/Voxel/SKHexahedralMesh.cpp)
add_library(Skylark::SKVolumeStudioVoxel ALIAS SKVolumeStudioVoxel)
target_link_libraries(SKVolumeStudioVoxel
    PUBLIC
        SKVolumeStudioCore
    PRIVATE
        skylark_project_warnings)
set_target_properties(SKVolumeStudioVoxel PROPERTIES FOLDER "Skylark/Tests/SkylarkVolumeStudio")

add_library(SKVolumeStudioBridge
    Source/SkylarkBridge/SKVolumeStudioSkylarkBridge.h
    Source/SkylarkBridge/SKVolumeStudioSkylarkBridge.cpp)
add_library(Skylark::SKVolumeStudioBridge ALIAS SKVolumeStudioBridge)
target_link_libraries(SKVolumeStudioBridge
    PUBLIC
        SKVolumeStudioCore
        SKVolumeStudioVoxel
        SKGeometry
        SKScene
    PRIVATE
        skylark_project_warnings)
set_target_properties(SKVolumeStudioBridge PROPERTIES FOLDER "Skylark/Tests/SkylarkVolumeStudio")

add_executable(SkylarkVolumeStudioValidation
    Source/Validation/SKVolumeStudioValidationMain.cpp)
target_link_libraries(SkylarkVolumeStudioValidation
    PRIVATE
        SKVolumeStudioModelIO
        SKVolumeStudioVoxel
        SKVolumeStudioBridge
        skylark_project_options
        skylark_project_warnings)
set_target_properties(SkylarkVolumeStudioValidation PROPERTIES
    FOLDER "Skylark/Tests/SkylarkVolumeStudio"
    VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:SkylarkVolumeStudioValidation>")
sk_volume_studio_apply_runtime_layout(SkylarkVolumeStudioValidation)

if(SK_BUILD_VOLUME_STUDIO_QT)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_AUTOUIC ON)

    find_package(Qt5 5.15 COMPONENTS Widgets CONFIG REQUIRED)

    add_executable(SkylarkVolumeStudio
        Source/AppQt/SkylarkVolumeStudioMain.cpp)
    target_link_libraries(SkylarkVolumeStudio
        PRIVATE
            SKVolumeStudioModelIO
            SKVolumeStudioVoxel
            SKVolumeStudioBridge
            Qt5::Widgets
            skylark_project_options
            skylark_project_warnings)
    set_target_properties(SkylarkVolumeStudio PROPERTIES
        FOLDER "Skylark/Tests/SkylarkVolumeStudio"
        VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:SkylarkVolumeStudio>")
    sk_volume_studio_apply_runtime_layout(SkylarkVolumeStudio)
endif()

if(SK_BUILD_INSTALL)
    install(TARGETS
        SKVolumeStudioCore
        SKVolumeStudioModelIO
        SKVolumeStudioVoxel
        SKVolumeStudioBridge
        SkylarkVolumeStudioValidation
        EXPORT SkylarkTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

    if(SK_BUILD_VOLUME_STUDIO_QT)
        install(TARGETS SkylarkVolumeStudio
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
    endif()
endif()
