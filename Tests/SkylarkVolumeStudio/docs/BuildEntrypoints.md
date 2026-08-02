# SkylarkVolumeStudio Build Entrypoints

SkylarkVolumeStudio is hosted under `Tests/SkylarkVolumeStudio`, but the normal commercial build entrypoints are placed at the Skylark repository root.

## Windows VS2022 Debug

```bat
cd /d D:\WorkSpaceForSolidDesigner\Skylark
build_windows_vs2022.bat
```

Equivalent internal script:

```bat
Scripts\Windows\Build\Build_Windows_SkylarkVolumeStudio_VS2022_Debug.bat
```

The script intentionally normalizes `REPO_ROOT` without a trailing slash before calling CMake. Do not change this back to `..\..\..\`; a trailing backslash inside a quoted CMake argument can corrupt `-S`, `-B`, and `-G` parsing on Windows.

## Windows package

```bat
package_windows.bat
```

## Linux Debug

```bash
cd /path/to/Skylark
bash build_linux.sh
```

Equivalent internal script:

```bash
bash Scripts/Linux/Build/Build_Linux_SkylarkVolumeStudio_Debug.sh
```

## Linux package

```bash
bash package_linux.sh
```
