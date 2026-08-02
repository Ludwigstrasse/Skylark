# SkylarkCamWorkbench

SkylarkCamWorkbench is the CAM platform validator under `Tests/SkylarkCamWorkbench`.

Milestones delivered so far:

- M0: CAM workbench shell, project tree, operation panel, log/benchmark tabs.
- M1: real Binary STL import and interactive 3D viewport preview.
- M2: Chunked Mesh Asset, LargeMeshRenderProxy, GPU Upload Queue, background BVH/LOD, 20M STL pressure entry.

The design rule remains strict: STL is a mesh CAM source. It is not converted to BRep or millions of OCCT TopoDS faces for CAM validation.


## M3: CAM Project / Setup / Tool Library

M3 turns the workbench project model into a persistable manufacturing document. It adds explicit APIs and UI commands for creating CAM projects, creating setups, setting WCS, defining box stock, defining fixture stubs, adding tools and saving/loading `.skcam.json` project files.

Validation script:

```bat
Scripts\Windows\Run\Run_Windows_SkylarkCamWorkbench_M3_ProjectJson_Validation.bat
```


## M7

BRep CAM channel: STEP import proxy, BRepCamGeometryProvider, 2.5D contour, pocket/drilling stubs, selection and diagnostics.
