# SkylarkVolumeStudio Source Delivery Plan

## Mandatory delivery rule

Every delivery after M1 must satisfy both conditions:

1. The project remains under `SkylarkEngine/Tests/SkylarkVolumeStudio`.
2. `Scripts/Linux/Build/Build_Linux_SkylarkVolumeStudio_Debug.sh` completes configure, build and validation successfully.

## M1 acceptance criteria

- Top-level CMake option `SK_BUILD_VOLUME_STUDIO` exists.
- Linux headless validation target builds without Qt/OpenGL/Vulkan dependencies.
- `SkylarkVolumeStudioValidation` generates a binary STL cube, reads it, voxelizes it, builds hexahedral cells and validates the Skylark preview bridge.
- Windows VS2022 build script exists and uses Qt path `C:\Qt\Qt5.15.14\5.15.14\msvc2019_64` by default.

## M2 acceptance criteria

- Binary STL reader has a chunked two-pass path.
- Reader reports progress and supports cancellation.
- Reader supports memory-mapped file mode on Windows and Linux.
- 600 MB binary STL read path avoids storing facet normals by default.
- Benchmark target records elapsed time, memory estimate and triangle count.

## M3 acceptance criteria

- Conservative voxel marking is replaced by exact triangle-box overlap for boundary cells.
- Sparse brick storage is introduced.
- Multi-threaded voxel marking exists.
- Closed shell fill and open shell diagnostic modes are separated.

## M4 acceptance criteria

- Skylark GPU buffer upload path is connected.
- Large surface preview avoids creating one monolithic `FSKMeshData` section for 20M triangles.
- View culling and chunk metadata are connected to the Skylark scene/render graph.

## M5 acceptance criteria

- Format adapter registry exists.
- STL/OBJ/PLY/glTF/STEP adapters are wired behind one import interface.
- JT/3DXML are represented by explicit optional adapter boundaries, not fake built-in support.
