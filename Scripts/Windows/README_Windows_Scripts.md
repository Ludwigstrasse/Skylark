# Skylark Windows Scripts

这套脚本分成四类：

- `Common`：公共脚本，不建议直接双击
- `Build`：构建脚本
- `Run`：运行脚本
- `Tools`：清理、打开日志、打开 VS 解决方案

## 常用入口

### Ninja 版本
```bat
Scripts\Windows\Build\Build_Windows_CadScene_Ninja_Debug.bat
Scripts\Windows\Run\Run_Windows_CadScene_Ninja_Debug.bat
```

特点：
- 生成器：`Ninja Multi-Config`
- 输出目录：`out\build\windows-cadscene-ninja`
- 不会生成 `.sln`

### VS2022 版本
```bat
Scripts\Windows\Build\Build_Windows_CadScene_VS2022_Debug.bat
Scripts\Windows\Tools\Open_Windows_CadScene_VS2022_Sln.bat
Scripts\Windows\Run\Run_Windows_CadScene_VS2022_Debug.bat
```

特点：
- 生成器：`Visual Studio 17 2022`
- 输出目录：`out\build\windows-cadscene-vs2022`
- 会生成 `.sln` 和 `.vcxproj`

## 其他入口

```bat
Scripts\Windows\Build\Build_Windows_Msvc_Debug.bat
Scripts\Windows\Build\Build_Windows_OpenGL_Debug.bat
Scripts\Windows\Tools\OpenLastLog.bat
Scripts\Windows\Tools\CleanBuild.bat
```

## 约定

- 仓库根目录需要存在：
  - `CMakeLists.txt`
  - `vcpkg.json`
- vcpkg 会固定到：
  - `third_party\vcpkg`
- 日志会输出到：
  - `out\logs`


## P14 Added Backend Matrix

CadEditorLab runtime sample now has explicit Windows scripts for all three main backends:

- D3D11
  - `Build_Windows_CadEditorLab_D3D11_VS2022_Debug.bat`
  - `Run_Windows_CadEditorLab_D3D11_VS2022_Debug.bat`
- Vulkan
  - `Build_Windows_CadEditorLab_Vulkan_VS2022_Debug.bat`
  - `Run_Windows_CadEditorLab_Vulkan_VS2022_Debug.bat`
- OpenGL
  - `Build_Windows_CadEditorLab_OpenGL_VS2022_Debug.bat`
  - `Run_Windows_CadEditorLab_OpenGL_VS2022_Debug.bat`

CadSceneTechniques remains the OpenGL benchmark viewer:

- `Build_Windows_CadSceneTechniques_OpenGL_VS2022_Debug.bat`
- `Run_Windows_CadSceneTechniques_OpenGL_VS2022_Debug.bat`


## P15 Added Full Backend Matrix and Runtime Options

CadEditorLab 现在补齐了更完整的 Windows 启动矩阵：

- D3D11
  - `Build_Windows_CadEditorLab_D3D11_Ninja_Debug.bat`
  - `Run_Windows_CadEditorLab_D3D11_Ninja_Debug.bat`
  - `Build_Windows_CadEditorLab_D3D11_VS2022_Debug.bat`
  - `Run_Windows_CadEditorLab_D3D11_VS2022_Debug.bat`
- D3D12
  - `Build_Windows_CadEditorLab_D3D12_VS2022_Debug.bat`
  - `Run_Windows_CadEditorLab_D3D12_VS2022_Debug.bat`
- Vulkan
  - `Build_Windows_CadEditorLab_Vulkan_Ninja_Debug.bat`
  - `Run_Windows_CadEditorLab_Vulkan_Ninja_Debug.bat`
  - `Build_Windows_CadEditorLab_Vulkan_VS2022_Debug.bat`
  - `Run_Windows_CadEditorLab_Vulkan_VS2022_Debug.bat`
- OpenGL
  - `Build_Windows_CadEditorLab_OpenGL_Ninja_Debug.bat`
  - `Run_Windows_CadEditorLab_OpenGL_Ninja_Debug.bat`
  - `Build_Windows_CadEditorLab_OpenGL_VS2022_Debug.bat`
  - `Run_Windows_CadEditorLab_OpenGL_VS2022_Debug.bat`

CadEditorLab 运行参数也扩展为：

```bat
SkylarkCadEditorLab.exe --rhi=d3d11 --scene=... --size=1920x1080 --vsync=0 --debug-rhi --render-thread
```

也支持环境变量：
- `SKYLARK_RHI`
- `SKYLARK_SCENE`
- `SKYLARK_SIZE`
- `SKYLARK_WIDTH`
- `SKYLARK_HEIGHT`
- `SKYLARK_VSYNC`
- `SKYLARK_RHI_DEBUG`
- `SKYLARK_RENDER_THREAD`
