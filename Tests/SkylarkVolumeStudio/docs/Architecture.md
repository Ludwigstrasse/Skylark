# SkylarkVolumeStudio Architecture

## 1. Product positioning

`SkylarkVolumeStudio` is a commercial validation application inside `SkylarkEngine/Tests/SkylarkVolumeStudio`. Its first milestone validates the following production data path:

```text
3D file reader
  -> compact CPU mesh asset
  -> Skylark preview bridge
  -> conservative voxel grid
  -> hexahedral cell mesh
  -> later: volume rendering / simulation / CAM / CAE pipeline
```

The implementation is deliberately hosted under Skylark's top-level CMake graph. It links to `SKCore`, `SKGeometry` and `SKScene` instead of creating an isolated application framework.

## 2. Module layout

```text
Tests/SkylarkVolumeStudio
├── Source/Core
│   ├── SKVolumeStudioTypes.h
│   └── SKVolumeStudioMeshAsset.*
├── Source/ModelIO
│   └── SKStlFastReader.*
├── Source/Voxel
│   ├── SKVolumeVoxelGrid.*
│   └── SKHexahedralMesh.*
├── Source/SkylarkBridge
│   └── SKVolumeStudioSkylarkBridge.*
├── Source/AppQt
│   └── SkylarkVolumeStudioMain.cpp
└── Source/Validation
    └── SKVolumeStudioValidationMain.cpp
```

## 3. Current M1 data model

### 3.1 Compact mesh asset

`FSKVolumeStudioMeshAsset` stores STL triangle soup as a packed float32 XYZ buffer:

```text
triangle count = PositionBuffer.size() / 9
memory = triangleCount * 3 vertices * 3 floats * 4 bytes
```

For 20 million STL triangles this raw position buffer is approximately:

```text
20,000,000 * 9 * 4 = 720,000,000 bytes ≈ 686 MiB
```

Facet normals are optional and disabled by default. This is the correct first constraint for an 800 MB-level memory target.

### 3.2 Binary STL reader

`FSKStlFastReader` currently accepts strict binary STL layout:

```text
80 byte header
4 byte uint32 triangle count
N * 50 byte facets
```

The reader rejects corrupt size/count combinations and non-finite coordinate data. ASCII STL and mixed/vendor-specific STL extensions are intentionally not accepted in M1 because they cannot meet the same predictable memory and throughput profile without a separate streaming parser.

### 3.3 Voxel grid and hexahedral mesh

`FSKSurfaceVoxelizer` creates a conservative boundary voxel grid and performs a simple parity interior fill. `FSKHexahedralMeshBuilder` converts occupied voxels into node-deduplicated 8-node hexahedral cells.

M1 voxelization is a correctness and integration baseline, not the final high-performance kernel. The commercial target requires the M2/M3 upgrades below.

## 4. Performance roadmap for 600 MB / 20M STL triangles

### M1: verified engine-hosted core

- strict binary STL reader
- packed triangle buffer
- conservative voxel baseline
- hexahedral mesh baseline
- Linux headless build validation
- Windows VS2022 + Qt build script

### M2: large STL import path

- chunked file reading
- memory mapped reader on Windows/Linux
- parallel facet decode
- async progress reporting
- model-space bounds pass + payload pass
- cancellation and error recovery

### M3: commercial voxelization kernel

- triangle-AABB binning
- exact triangle-box overlap test
- sparse brick grid
- multithreaded boundary marking
- watertightness diagnostics
- separate solid fill algorithm for closed shell and open shell modes

### M4: Skylark rendering integration

- GPU-resident triangle buffer
- chunk culling
- LOD and out-of-core streaming
- volume rendering pass integration
- selection/probing bridge

### M5: multi-format import architecture

- STL: in-house fast path
- OBJ/PLY/glTF: lightweight open-source reader adapters
- STEP/IGES/BREP: OCCT adapter
- JT/3DXML: optional commercial SDK adapter boundary, no fake open-source claim

## 5. Non-goals in M1

M1 does not claim final ParaView/VTK-grade volume rendering or final exact voxel meshing. It establishes a clean, compiled, engine-hosted data path that can be hardened milestone by milestone without throwing away the structure.
