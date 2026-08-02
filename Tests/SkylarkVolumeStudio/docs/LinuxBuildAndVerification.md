# Linux Build and Verification

Run from the Skylark repository root:

```bash
bash Scripts/Linux/Build/Build_Linux_SkylarkVolumeStudio_Debug.sh
```

The script performs:

```text
configure -> build SkylarkVolumeStudioValidation -> run validation executable
```

It does not require Qt, OpenGL, Vulkan, VTK, OCCT or vcpkg. This is intentional: the Linux gate validates the commercial core data path first.

The script writes logs to:

```text
out/logs/configure_linux_volumestudio_debug.log
out/logs/build_linux_volumestudio_debug.log
out/logs/run_linux_volumestudio_validation.log
```

Qt can be enabled manually if a Linux Qt 5.15 SDK is installed:

```bash
cmake -S . -B out/build/linux-volumestudio-qt-debug -G Ninja \
  -DCMAKE_BUILD_TYPE=Debug \
  -DSK_BUILD_VOLUME_STUDIO=ON \
  -DSK_BUILD_VOLUME_STUDIO_QT=ON \
  -DCMAKE_PREFIX_PATH=/path/to/Qt/5.15.x/gcc_64
cmake --build out/build/linux-volumestudio-qt-debug --target SkylarkVolumeStudio
```
