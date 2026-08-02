# SkylarkCamWorkbench M2 Delivery Notes

## Scope

M2 implements the LargeMesh + Skylark RenderProxy milestone:

- STL remains in the Mesh CAM path and is not converted to BRep / TopoDS_Shape.
- The importer does not allocate one CAM object per triangle.
- LargeMeshAsset owns chunk descriptors, global bounds, decimated preview payload and acceleration status.
- LargeMeshRenderProxy maps LargeMeshAsset chunks to render chunk proxies.
- GpuUploadQueue provides the upload scheduling boundary for Skylark/RHI integration.
- MeshBvhBuilder builds a chunk-level BVH from chunk bounds.
- MeshLodBuilder builds LOD descriptors from the LargeMeshAsset without expanding triangles.
- LargeMeshAsyncBuilder schedules BVH/LOD through std::async background jobs.
- Headless and UI expose a 20M STL pressure entry.

## Validation Entries

Windows build:

```bat
Scripts\Windows\Build\Build_Windows_SkylarkCamWorkbench_VS2022_Debug.bat
```

Windows 20M pressure entry:

```bat
Scripts\Windows\Run\Run_Windows_SkylarkCamWorkbench_M2_20M_STL_Benchmark.bat
Scripts\Windows\Run\Run_Windows_SkylarkCamWorkbench_M2_20M_STL_Benchmark.bat D:\Models\turbine_20m.stl
```

Linux headless:

```bash
bash Scripts/Linux/Build/Build_Linux_SkylarkCamWorkbench_Headless.sh
bash Scripts/Linux/Run/Run_Linux_SkylarkCamWorkbench_M2_20M_STL_Benchmark.sh
bash Scripts/Linux/Run/Run_Linux_SkylarkCamWorkbench_M2_20M_STL_Benchmark.sh /data/turbine_20m.stl
```

## Architecture Notes

The current M2 GPU upload queue is a render-system boundary object, not a real graphics API uploader. The real backend integration belongs to the Skylark render bridge. This is intentional: CAM and LargeMesh must not directly own OpenGL/Vulkan/D3D resources.
