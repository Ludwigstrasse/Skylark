# Format Technology Selection

## STL

Use the in-house `FSKStlFastReader` for the commercial fast path. STL is simple enough that a general-purpose scene importer adds avoidable memory and conversion overhead.

M1 supports strict binary STL. M2 should add memory-mapped chunk reading and optional ASCII STL streaming.

## OBJ / PLY / glTF

Use adapter boundaries so lightweight readers can be swapped without changing the rest of the product:

- OBJ: tinyobjloader or Assimp adapter
- PLY: tinyply or Assimp adapter
- glTF: tinygltf or fastgltf adapter

For very large industrial files, prefer a direct adapter that emits `FSKVolumeStudioMeshAsset` chunks rather than first building a third-party scene graph.

## STEP / IGES / BREP

Use OCCT for topology-bearing CAD formats. These formats should not be reduced to triangle soup when the downstream task needs topology, faces, tolerances, CAM, CAE or healing.

## JT / 3DXML

Do not pretend full JT/3DXML support is solved by open-source libraries. The architecture must expose optional adapter boundaries for commercial SDKs or controlled conversion pipelines. The core import registry should report capability status explicitly.
