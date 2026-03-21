# Skylark Build System

This repository now uses a target-oriented CMake layout with optional backend modules and vcpkg manifest integration.

## What changed

- Replaced recursive, catch-all module globbing with explicit target definitions.
- Split the portable modern graph from the legacy SKEngine2 path.
- Added platform compile definitions at the project interface layer.
- Added optional backend toggles for D3D11, D3D12, OpenGL, and Vulkan.
- Added install/export rules so Skylark libraries can be consumed as exported CMake targets.
- Added `vcpkg.json` and `CMakePresets.json` for reproducible Windows/Linux workflows.
- Added an optional smoke test executable (`SkylarkSmoke`) to verify the link graph.

## Build requirements

- CMake 3.23 or newer
- A C++20-capable compiler
- Ninja recommended
- vcpkg recommended for dependency-managed builds

## vcpkg setup

Set `VCPKG_ROOT` before using the vcpkg presets.

### Linux

```bash
export VCPKG_ROOT=/path/to/vcpkg
cmake --preset linux-debug
cmake --build --preset linux-debug
```

### Windows

```powershell
$env:VCPKG_ROOT="C:\dev\vcpkg"
cmake --preset windows-msvc
cmake --build --preset windows-msvc-debug
```

## Presets

Bootstrap presets without a vcpkg toolchain:

- `dev-linux-debug`
- `dev-linux-release`

vcpkg-backed Linux presets:

- `linux-debug`
- `linux-release`
- `linux-vulkan-debug`
- `linux-opengl-debug`

vcpkg-backed Windows presets:

- `windows-msvc`
- `windows-d3d11`
- `windows-d3d12`
- `windows-opengl`
- `windows-vulkan`

## Backend options

- `SK_ENABLE_D3D11`
- `SK_ENABLE_D3D12`
- `SK_ENABLE_OPENGL`
- `SK_ENABLE_VULKAN`
- `SK_BUILD_LEGACY`
- `SK_BUILD_SMOKE_TEST`

The default configuration is intentionally conservative: portable modern modules are enabled, legacy SKEngine2 is disabled, and graphics backends are opt-in.
