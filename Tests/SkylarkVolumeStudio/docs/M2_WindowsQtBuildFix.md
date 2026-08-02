# M2 Windows Qt Build Fix

## Root cause

The Windows Qt target failed because `SkylarkVolumeStudioMain.cpp` used an obsolete type name:

```cpp
FSKVoxelGrid
```

The actual voxel grid class declared by `Source/Voxel/SKVolumeVoxelGrid.h` is:

```cpp
FSKVolumeVoxelGrid
```

MSVC therefore reported missing type declarations around `SetVoxelState`, `CurrentVoxelGrid`, and the reset expression in `OpenBinaryStl()`.

## Fix

All `FSKVoxelGrid` usages in the Qt application were replaced with `FSKVolumeVoxelGrid`.

Affected file:

```text
Tests/SkylarkVolumeStudio/Source/AppQt/SkylarkVolumeStudioMain.cpp
```

The headless validation path is not changed by this patch.
