# SkylarkCamWorkbench M1 Delivery Notes

## Scope

M1 upgrades the M0 CAM shell into a usable large-STL validation workstation:

- Real Binary STL import path.
- Streaming triangle scan without STL -> BRep conversion.
- Chunk table generation for LargeMeshAsset.
- Decimated viewport preview payload for large meshes.
- Interactive 3D viewport: fit, rotate, pan, zoom.
- CAM project tree refresh after import.
- Operation parameter panel imports mesh diagnostics.
- Log / warnings / benchmark tabs report triangle count, chunk count, import time, file size, estimated GPU payload and preview stride.

## M1 Import Pipeline

```text
Binary STL file
  -> BinaryStlFastReader
  -> LargeMeshAsset
  -> CamProject ImportedModel
  -> SkylarkSceneAdapter / RenderProxyDescriptor
  -> SkylarkViewportWidget preview draw
```

## Explicit Architecture Boundary

M1 still does not build BVH, LOD, GPU upload queue, collision, material removal or G-code. Those remain M2-M6 tasks. M1 also does not convert STL triangles into OCCT TopoDS_Face/Edge/Vertex, because mesh CAM must stay on the MeshCamGeometryProvider path.

## Acceptance

- Windows VS2022 Debug target: `SkylarkCamWorkbench`.
- Linux headless target: `SkylarkCamWorkbenchHeadless`.
- Qt main window has Project Tree / 3D viewport / Operation Panel / Log-Benchmark tabs.
- `File -> Import STL` loads Binary STL and updates scene diagnostics.
- Viewport supports left-drag rotate, middle-drag pan, wheel zoom and `F` fit-all.
