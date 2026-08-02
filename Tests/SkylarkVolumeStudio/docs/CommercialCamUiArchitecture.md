# SkylarkVolumeStudio 商业 CAM / Volume Rendering 工作台界面架构

## 1. 本轮重构目标

上一版界面只有两个按钮和一个日志框，只能证明 STL reader 与 voxelizer 可以被调用，不能作为商业 CAM / 大模型仿真软件的产品界面。本轮将 `SkylarkVolumeStudio` 从验证窗口重构为 **商业制造工作台雏形**，目标是对标专业 CAM / CAE 前处理软件的布局和工作流。

本轮仍然遵守一个硬边界：不伪装成已经完成完整 CAM 内核。当前实现的是可运行的商业工作台框架，真实已完成能力包括 binary STL 导入、mesh payload 统计、hex voxel mesh 构建、项目树/操作树/属性统计/工具库/任务队列/诊断控制台联动。

## 2. 界面分区

```text
SkylarkVolumeStudio MainWindow
├── MenuBar
│   ├── File
│   ├── Exchange
│   ├── CAM
│   └── View
├── Ribbon Tabs
│   ├── Home
│   ├── Exchange
│   └── CAM / Simulation
├── Central Manufacturing Viewport
│   ├── metric cards: triangles / CPU mesh budget / voxel grid / hex cells
│   ├── dark CAD/CAM viewport placeholder
│   ├── manufacturing overlay
│   └── axis gizmo
├── Left Docks
│   ├── Project Navigator
│   └── CAM Operation Browser
├── Right Docks
│   ├── Properties / Statistics
│   └── Manufacturing Setup
├── Bottom Dock
│   └── Diagnostics Console
└── StatusBar
    ├── current workflow status
    ├── backend route
    └── target memory/performance note
```

## 3. 当前真实功能

| 功能 | 当前状态 | 说明 |
|---|---:|---|
| Binary STL import | 已实现 | 调用 `SKStlFastReader`，默认不保存 facet normal，降低 CPU 内存 |
| Mesh payload statistics | 已实现 | 三角数、估算 CPU mesh bytes、bounds、payload policy |
| Hex voxel mesh build | 已实现 | 调用 `SKSurfaceVoxelizer` 和 `SKHexahedralMeshBuilder` |
| Project tree | 已实现 | 显示外部源、payload、CAM setup、simulation mesh |
| Operation browser | 已实现 | Import / Voxelization / Stock / Toolpath / Simulation 阶段状态 |
| Tool library panel | 已实现 | 为后续 CAM operation 提供 UI 落点 |
| Manufacturing job queue | 已实现 | 为后续 NC pipeline / package pipeline 提供 UI 落点 |
| Backend selector | 已实现 UI | Skylark large mesh / VTK bridge / OCCViewer precise CAD 三条路线 |
| Commercial viewport | 已实现 UI | 当前为 Qt painter 预览；后续替换为 Skylark/VTK native viewport |

## 4. 平台架构边界

### 4.1 大 STL 路线

大 STL 不能进入 OCCT BRep，也不能进入小对象级 `ViewPrimitiveRecord.sampleXs/sampleYs/sampleZs`。正确路线是：

```text
Binary STL
  -> SKStlFastReader
  -> LargeMeshPayload / SKVolumeStudioMeshAsset
  -> Project Navigator / Properties / Statistics
  -> Skylark Large Mesh Backend
  -> optional voxelization / CAM mesh provider
```

### 4.2 CAM / 仿真路线

STL 作为三角网格，只适合 Mesh CAM / Voxel / Dexel / BVH / collision 等路线；精确孔、槽、倒角、圆柱面等特征驱动 CAM 仍需要 STEP/BRep/Parasolid 等精确几何路线。

```text
Imported STL Mesh
  -> Mesh CAM Provider
  -> Mesh topology / BVH / voxel / dexel
  -> roughing / finishing / collision / stock simulation

Imported STEP/BRep
  -> Kernel / OCCT precise geometry
  -> BRep CAM Provider
  -> feature-based machining
```

## 5. 后续里程碑

### M2.1 商业 UI 骨架完成

验收：

1. Windows Qt 应用不再是 demo 窗口；具备 ribbon、viewport、dock panels、属性统计、工具库、任务队列。
2. STL 导入后，项目树、统计表、视口 overlay、任务队列同步更新。
3. voxel mesh 构建后，simulation mesh 节点和 hex mesh 统计同步更新。
4. Linux core/headless validation 继续可编译运行。

### M2.2 Skylark native viewport 接入

验收：

1. central viewport 替换为 Skylark QWidget/native window bridge。
2. 支持 GPU vertex/index buffer 上传。
3. 支持 shaded display、fit all、orbit/pan/zoom。
4. 支持 600 万 STL 首帧性能统计。

### M2.3 LargeMesh payload 与 render resource 拆分

验收：

1. 解析 payload、display cache、GPU buffer 三层拆分。
2. 支持 chunk metadata、bounds、memory budget。
3. 支持后台加载和 UI 进度更新。

### M2.4 CAM mesh provider

验收：

1. Mesh topology 重建。
2. Mesh BVH / triangle query。
3. plane slicing / height query 原型。
4. 与 operation browser 连接。

