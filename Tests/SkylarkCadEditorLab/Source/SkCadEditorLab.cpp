#include "../../SkylarkCadSceneTechniques/Source/SkCadSceneImporter.h"

#include "SKEngine/SKEngine.h"
#include "SKEngine/SKEngineScene.h"
#include "SKInteraction/SKGpuPicker.h"
#include "SKInteraction/SKHitProxyTable.h"
#include "SKRenderer/SKRenderer.h"
#include "SKRHI/SKRHIFactory.h"
#include "SKRHI/SKRHIRuntimeConfig.h"
#include "SKScene/SKSceneGraph.h"
#include "../../Shared/SkSampleAppOptions.h"
#include "../../Shared/SkSampleAppHost.h"
#include "../../Shared/SkSampleCameraOptions.h"
#include "../../Shared/SkSampleRuntimeStats.h"
#include "../../Shared/SkSampleBenchmarkOptions.h"
#include "../../Shared/SkSampleRunReport.h"
#include "../../Shared/SkSampleSceneManifest.h"
#include "../../Shared/SkSampleOutputLayout.h"
#include "../../Shared/SkSampleControls.h"
#include "../../Shared/SkCadSceneDiagnostics.h"

#include <GLFW/glfw3.h>
#if defined(_WIN32)
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif
    #define GLFW_EXPOSE_NATIVE_WIN32
    #include <windows.h>
    #include <GLFW/glfw3native.h>
    #ifdef min
        #undef min
    #endif
    #ifdef max
        #undef max
    #endif
#endif

#include <algorithm>
#include <chrono>
#include <cstdarg>
#include <exception>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <limits>
#include <memory>
#include <numbers>
#include <string>
#include <unordered_map>
#include <vector>

#if defined(_WIN32)
namespace { void SKRuntimeDiagWrite(const char* Format, ...); }
#endif

namespace Skylark::CadEditorLab
{
    namespace
    {
        static constexpr const char* GCadEditorLabRuntimeFixVersion = "CAD_EDITOR_LAB_BLANK_FREEZE_FIX3_20260428";

        static void PrintRuntimeFixStamp(const FSKRHIRuntimeOptions& Options)
        {
            std::fprintf(stdout, "[fix] CadEditorLab runtime fix=%s requestedRhi=%s\n", GCadEditorLabRuntimeFixVersion, SKRHIApiToString(Options.RequestedApi));
            std::fflush(stdout);
            std::fprintf(stderr, "[fix] CadEditorLab runtime fix=%s requestedRhi=%s\n", GCadEditorLabRuntimeFixVersion, SKRHIApiToString(Options.RequestedApi));
            std::fflush(stderr);
#if defined(_WIN32)
            ::SKRuntimeDiagWrite("[fix] CadEditorLab runtime fix=%s requestedRhi=%s", GCadEditorLabRuntimeFixVersion, SKRHIApiToString(Options.RequestedApi));
#endif
        }

        static void ApplyRuntimeBackendSafetyOverride(FSKRHIRuntimeOptions& Options)
        {
            if (Options.RequestedApi == ESKRHIApi::Vulkan)
            {
                std::fprintf(stdout, "[fix] Vulkan CAD solid rendering is not enabled in this backend; forcing D3D12 raster backend for visible CadEditorLab output.\n");
                std::fflush(stdout);
                std::fprintf(stderr, "[fix] Vulkan CAD solid rendering is not enabled in this backend; forcing D3D12 raster backend for visible CadEditorLab output.\n");
                std::fflush(stderr);
#if defined(_WIN32)
                ::SKRuntimeDiagWrite("[fix] Vulkan CAD solid rendering is not enabled; forcing D3D12 raster backend for visible CadEditorLab output.");
#endif
                Options.RequestedApi = ESKRHIApi::D3D12;
            }
        }

        struct FCameraState
        {
            float YawRadians = 0.8f;
            float PitchRadians = 0.5f;
            float Distance = 10.0f;
            FSKVector3f Target = FSKVector3f(0.0f, 0.0f, 0.0f);

            bool bRightDragging = false;
            bool bMiddleDragging = false;
            bool bLeftPressed = false;
            bool bLeftMaybeClick = false;
            double LastMouseX = 0.0;
            double LastMouseY = 0.0;
            double LeftPressX = 0.0;
            double LeftPressY = 0.0;
        };

        struct FNodeEntry
        {
            TSharedPtr<FSKGeometryNode> Node;
            FSKObjectId OwnerId{};
            uint64 GeometryKey = 0;
            uint32 HitProxyId = 0;
            FSKMatrix4f BaseTransform = FSKMatrix4f::Identity();
            FSKVector3f ExplosionDirection = FSKVector3f(0.0f, 0.0f, 1.0f);
            FSKVector3f UserOffset = FSKVector3f(0.0f, 0.0f, 0.0f);
            bool bHidden = false;
        };

        struct FAppState
        {
            GLFWwindow* Window = nullptr;
            ISKEngine* Engine = nullptr;
            ISKViewport* Viewport = nullptr;
            TSharedPtr<FSKSceneGraph> SceneGraph;
            FSKGeometryRegistry* GeometryRegistry = nullptr;
            FSKHitProxyTable HitProxyTable;

            CadSceneTest::FSkCadSceneDocument SourceDocument{};
            std::vector<FNodeEntry> Entries;
            std::unordered_map<uint64, size_t> OwnerHashToEntry;
            uint64 SelectedOwnerHash = 0;
            float ExplodeAmount = 0.0f;
            FCameraState Camera{};

            bool bRequestPick = false;
            bool bHasIsolation = false;
            bool bWireframe = false;
            bool bShadedWithEdges = false;
            bool bHiddenLine = false;

            FSKGuid DocumentGuid{ 0x11AA22BB33CC44DDull, 0x55EE66FF77889900ull };
            uint64 NextObjectSeed = 1;

            int FrameWidth = 1280;
            int FrameHeight = 720;
            ESKRHIApi ActiveApi = ESKRHIApi::Null;
            FSKString WindowTitleBase;
            std::filesystem::path LoadedScenePath;
            bool bEnableVSync = true;
            FSKSampleCameraOptions CameraOptions{};
            FSKSampleRuntimeStatsOptions RuntimeStatsOptions{};
            FSKSampleBenchmarkOptions BenchmarkOptions{};
            FSKSampleRunReportOptions RunReportOptions{};
            FSKSampleSceneManifestOptions SceneManifestOptions{};
            FSKSampleOutputLayoutOptions OutputLayoutOptions{};
            FSKCadSceneDiagnosticsOptions DiagnosticsOptions{};
            FSKSampleRuntimeStatsState RuntimeStats{};
            FSKSampleBenchmarkState BenchmarkState{};
        };

        FAppState* GApp = nullptr;

#ifndef SK_CAD_EDITOR_LAB_DEFAULT_RHI
#define SK_CAD_EDITOR_LAB_DEFAULT_RHI "auto"
#endif

        static bool HasExplicitRhiSelection(int Argc, char** Argv)
        {
            if (std::getenv("SKYLARK_RHI"))
            {
                return true;
            }

            for (int Index = 1; Index < Argc; ++Index)
            {
                const char* Arg = Argv[Index];
                if (!Arg || !*Arg)
                {
                    continue;
                }

                const std::string_view View(Arg);
                if (View == "--rhi" || View == "--backend" ||
                    View.rfind("--rhi=", 0) == 0 ||
                    View.rfind("--backend=", 0) == 0)
                {
                    return true;
                }
            }

            return false;
        }

        static void ApplyCompiledDefaultRhiIfNeeded(int Argc, char** Argv, FSKRHIRuntimeOptions& Options)
        {
            if (HasExplicitRhiSelection(Argc, Argv))
            {
                return;
            }

            constexpr const char* CompiledDefaultRhi = SK_CAD_EDITOR_LAB_DEFAULT_RHI;
            if (!CompiledDefaultRhi || !*CompiledDefaultRhi)
            {
                return;
            }

            const std::string_view DefaultView(CompiledDefaultRhi);
            if (DefaultView == "auto")
            {
                return;
            }

            ESKRHIApi Parsed = Options.RequestedApi;
            if (SKTryParseRHIApi(CompiledDefaultRhi, Parsed))
            {
                Options.RequestedApi = Parsed;
            }
        }

static void PrintCommandLineHelp()
{
    const FSKString CompiledBackends = SKBuildRHIBackendListString(false);
    const FSKString SupportedBackends = SKBuildRHIBackendListString(true);
    const FSKString BackendSummary = FSKString("Runtime backends: compiled=") + CompiledBackends + ", supported=" + SupportedBackends;
    SKPrintBasicSampleAppHelp(
        "SkylarkCadEditorLab",
        BackendSummary.c_str(),
        "  --rhi=<d3d11|d3d12|opengl|vulkan>\n"
        "  --backend=<...>                alias of --rhi\n"
        "  --debug-rhi                    enable RHI debug layer if backend supports it\n"
        "  --render-thread                enable render thread\n"
        "  --no-render-thread\n"
        "  --scene-manifest=<path>\n"
        "  --scene-index=<int>\n"
        "  --list-scenes / --no-list-scenes\n"
        "  --output-dir=<path>\n"
        "  --session-tag=<text>\n"
        "  --auto-report-files / --no-auto-report-files\n"
        "  --print-scene-summary / --no-scene-summary\n"
        "  --print-scene-bounds / --no-scene-bounds\n"
        "  --benchmark / --no-benchmark\n"
        "  --warmup-frames=<int>\n"
        "  --max-frames=<int>\n"
        "  --max-seconds=<float>\n"
        "  --benchmark-csv=<path>\n"
        "  --append-benchmark-csv\n"
        "  --print-benchmark-summary / --no-benchmark-summary\n",
        "  SKYLARK_RHI, SKYLARK_SCENE, SKYLARK_SIZE, SKYLARK_WIDTH, SKYLARK_HEIGHT,\n"
        "  SKYLARK_VSYNC, SKYLARK_RHI_DEBUG, SKYLARK_RENDER_THREAD,\n"
        "  SKYLARK_SCENE_MANIFEST, SKYLARK_SCENE_INDEX, SKYLARK_LIST_SCENES,\n"
        "  SKYLARK_OUTPUT_DIR, SKYLARK_SESSION_TAG, SKYLARK_AUTO_REPORT_FILES,\n"
        "  SKYLARK_PRINT_SCENE_SUMMARY, SKYLARK_PRINT_SCENE_BOUNDS,\n"
        "  SKYLARK_BENCHMARK, SKYLARK_WARMUP_FRAMES, SKYLARK_MAX_FRAMES,\n"
        "  SKYLARK_MAX_SECONDS, SKYLARK_BENCHMARK_CSV, SKYLARK_BENCHMARK_APPEND_CSV,\n"
        "  SKYLARK_BENCHMARK_SUMMARY\n");
    SKPrintSampleControls(ESKSampleControlProfile::CadEditorLab);
}

        static FSKVector3f MakeVec3(float X, float Y, float Z)
        {
            return FSKVector3f(X, Y, Z);
        }

        static FSKVector3f Add(const FSKVector3f& A, const FSKVector3f& B)
        {
            return FSKVector3f(A.X + B.X, A.Y + B.Y, A.Z + B.Z);
        }

        static FSKVector3f Sub(const FSKVector3f& A, const FSKVector3f& B)
        {
            return FSKVector3f(A.X - B.X, A.Y - B.Y, A.Z - B.Z);
        }

        static FSKVector3f Scale(const FSKVector3f& V, float S)
        {
            return FSKVector3f(V.X * S, V.Y * S, V.Z * S);
        }

        static float Dot(const FSKVector3f& A, const FSKVector3f& B)
        {
            return A.X * B.X + A.Y * B.Y + A.Z * B.Z;
        }

        static FSKVector3f Cross(const FSKVector3f& A, const FSKVector3f& B)
        {
            return FSKVector3f(
                A.Y * B.Z - A.Z * B.Y,
                A.Z * B.X - A.X * B.Z,
                A.X * B.Y - A.Y * B.X);
        }

        static float Length(const FSKVector3f& V)
        {
            return std::sqrt(Dot(V, V));
        }

        static FSKVector3f Normalize(const FSKVector3f& V)
        {
            const float L = Length(V);
            if (L <= 1.0e-6f)
            {
                return FSKVector3f(0.0f, 0.0f, 1.0f);
            }
            return Scale(V, 1.0f / L);
        }

        static FSKMatrix4f MakeRowMajorFromColumnMajorAppMat(const CadSceneTest::FAppMat4& InMat)
        {
            FSKMatrix4f Out{};
            for (int Row = 0; Row < 4; ++Row)
            {
                for (int Column = 0; Column < 4; ++Column)
                {
                    Out.M[Row][Column] = InMat.M[Column * 4 + Row];
                }
            }
            return Out;
        }

        static FSKMatrix4f MakePerspective(float FovYRadians, float Aspect, float ZNear, float ZFar)
        {
            const float TanHalf = std::tan(FovYRadians * 0.5f);
            FSKMatrix4f Result{};
            Result.M[0][0] = 1.0f / (Aspect * TanHalf);
            Result.M[1][1] = 1.0f / TanHalf;
            Result.M[2][2] = -(ZFar + ZNear) / (ZFar - ZNear);
            Result.M[2][3] = -(2.0f * ZFar * ZNear) / (ZFar - ZNear);
            Result.M[3][2] = -1.0f;
            return Result;
        }

        static FSKMatrix4f MakeLookAt(const FSKVector3f& Eye, const FSKVector3f& Center, const FSKVector3f& Up)
        {
            const FSKVector3f F = Normalize(Sub(Center, Eye));
            const FSKVector3f S = Normalize(Cross(F, Up));
            const FSKVector3f U = Cross(S, F);

            FSKMatrix4f Result = FSKMatrix4f::Identity();
            Result.M[0][0] = S.X;  Result.M[0][1] = S.Y;  Result.M[0][2] = S.Z;  Result.M[0][3] = -Dot(S, Eye);
            Result.M[1][0] = U.X;  Result.M[1][1] = U.Y;  Result.M[1][2] = U.Z;  Result.M[1][3] = -Dot(U, Eye);
            Result.M[2][0] = -F.X; Result.M[2][1] = -F.Y; Result.M[2][2] = -F.Z; Result.M[2][3] = Dot(F, Eye);
            Result.M[3][0] = 0.0f; Result.M[3][1] = 0.0f; Result.M[3][2] = 0.0f; Result.M[3][3] = 1.0f;
            return Result;
        }

        static FSKVector3f GetCameraForward(const FCameraState& Camera)
        {
            const float CP = std::cos(Camera.PitchRadians);
            const float SP = std::sin(Camera.PitchRadians);
            const float CY = std::cos(Camera.YawRadians);
            const float SY = std::sin(Camera.YawRadians);
            return Normalize(FSKVector3f(-CP * CY, -CP * SY, -SP));
        }

        static FSKVector3f GetCameraPosition(const FCameraState& Camera)
        {
            const float CP = std::cos(Camera.PitchRadians);
            const float SP = std::sin(Camera.PitchRadians);
            const float CY = std::cos(Camera.YawRadians);
            const float SY = std::sin(Camera.YawRadians);
            return Add(Camera.Target, FSKVector3f(Camera.Distance * CP * CY, Camera.Distance * CP * SY, Camera.Distance * SP));
        }

        static FSKObjectId MakeObjectId(FAppState& App, uint32 SubId)
        {
            FSKObjectId Id{};
            Id.DocumentGuid = App.DocumentGuid;
            Id.ObjectGuid.A = 0xCADA000000000000ull | App.NextObjectSeed;
            Id.ObjectGuid.B = 0xE011000000000000ull | (App.NextObjectSeed * 1315423911ull);
            Id.SubId = SubId;
            Id.Type = ESKObjectEntityType::Element;
            Id.LegacyValue = App.NextObjectSeed;
            ++App.NextObjectSeed;
            return Id;
        }

        
static std::filesystem::path ResolveScenePath(const FSKRHIRuntimeOptions& Options)
{
    const auto Cwd = std::filesystem::current_path();
    return SKResolveSampleScenePath(
        Options.ScenePath,
        {
            Cwd / "Tests" / "SkylarkCadEditorLab" / "Assets" / "geforce.csf.gz",
            Cwd / "Tests" / "SkylarkCadSceneTechniques" / "Assets" / "geforce.csf.gz",
            Cwd / "Debug" / "Tests" / "SkylarkCadEditorLab" / "Assets" / "geforce.csf.gz",
            Cwd / "Debug" / "Tests" / "SkylarkCadSceneTechniques" / "Assets" / "geforce.csf.gz",
            Cwd / "Release" / "Tests" / "SkylarkCadEditorLab" / "Assets" / "geforce.csf.gz",
            Cwd / "Release" / "Tests" / "SkylarkCadSceneTechniques" / "Assets" / "geforce.csf.gz",
            Cwd / "Tests" / "SkylarkCadSceneTechniques" / "ThirdParty" / "original_sample" / "geforce.csf.gz"
        },
        Cwd / "Tests" / "SkylarkCadSceneTechniques" / "ThirdParty" / "original_sample" / "geforce.csf.gz");
}

        static uint64 MakeDrawItemGeometryCacheKey(uint64 SourceGeometryKey, int32 SectionIndex)
        {
            uint64 H = SourceGeometryKey + 0x9E3779B97F4A7C15ull;
            H ^= (H >> 30);
            H *= 0xBF58476D1CE4E5B9ull;
            H ^= (H >> 27);
            H *= 0x94D049BB133111EBull;
            H ^= (H >> 31);
            H ^= static_cast<uint64>(static_cast<uint32>(SectionIndex)) + 0x9E3779B97F4A7C15ull + (H << 6) + (H >> 2);
            return H;
        }

        static FSKString MakeSectionGeometryDebugName(const FSKGeometryData& SourceGeometry, int32 SectionIndex)
        {
            return SourceGeometry.DebugName + "_Section_" + std::to_string(SectionIndex).c_str();
        }


        static void ApplyEntryTransform(FAppState& App, FNodeEntry& Entry)
        {
            const FSKVector3f Explode = Scale(Entry.ExplosionDirection, App.ExplodeAmount);
            const FSKVector3f TotalOffset = Add(Explode, Entry.UserOffset);
            const FSKMatrix4f Translation = FSKMatrix4f::Translation(TotalOffset.X, TotalOffset.Y, TotalOffset.Z);
            const FSKMatrix4f World = SKMatrixMultiply(Translation, Entry.BaseTransform);
            Entry.Node->SetLocalTransform(World);
            Entry.Node->SetVisible(!Entry.bHidden);
        }

        static void ApplyAllTransforms(FAppState& App)
        {
            for (FNodeEntry& Entry : App.Entries)
            {
                ApplyEntryTransform(App, Entry);
            }
        }

        static void RebuildOwnerIndex(FAppState& App)
        {
            App.OwnerHashToEntry.clear();
            for (size_t Index = 0; Index < App.Entries.size(); ++Index)
            {
                App.OwnerHashToEntry.emplace(App.Entries[Index].OwnerId.Hash64(), Index);
            }
            if (App.SceneGraph)
            {
                App.SceneGraph->RebuildIndex();
            }
        }

        static bool PopulateRuntimeScene(FAppState& App, FSKString& OutError)
        {
            ISKEngineSceneAccess* SceneAccess = dynamic_cast<ISKEngineSceneAccess*>(App.Engine);
            if (!SceneAccess)
            {
                OutError = "ISKEngineSceneAccess cast failed.";
                return false;
            }

            App.SceneGraph = SceneAccess->GetSceneGraph();
            App.GeometryRegistry = SceneAccess->GetGeometryRegistry();
            if (!App.SceneGraph || !App.GeometryRegistry)
            {
                OutError = "Failed to acquire scene graph or geometry registry.";
                return false;
            }

            auto Root = App.SceneGraph->GetRoot();
            if (!Root)
            {
                OutError = "Scene root is null.";
                return false;
            }

            FSKGeometryBuildSettings BuildSettings{};
            BuildSettings.bBuildWireframe = true;
            BuildSettings.bBuildEdgeCategories = true;
            BuildSettings.CreaseAngleDeg = 30.0f;
            BuildSettings.bWeldVertices = false;

            // Important engine fix:
            // The CSF document has 110 source geometries and 68k draw items.  Each draw item references
            // one source geometry plus one SectionIndex.  The previous runtime path copied a single
            // section into a brand new FSKGeometryData for every unique (GeometryKey, SectionIndex), which
            // produced 22k+ derived geometries and blocked the UI before the first frame.
            //
            // The engine now carries SectionIndex through FSKGeometryNode -> FSKRenderProxy -> renderer.
            // Therefore we register each source geometry exactly once and let the renderer draw the
            // selected section.  This preserves CSF semantics and avoids the startup freeze.
            std::unordered_map<uint64, uint64> RuntimeGeometryBySourceKey;
            RuntimeGeometryBySourceKey.reserve(App.SourceDocument.GeometryKeys.size());

            auto ResolveRuntimeGeometryKey = [&](uint64 SourceGeometryKey) -> uint64
            {
                auto CacheIt = RuntimeGeometryBySourceKey.find(SourceGeometryKey);
                if (CacheIt != RuntimeGeometryBySourceKey.end())
                {
                    return CacheIt->second;
                }

                const FSKGeometryData* SourceGeo = App.SourceDocument.GeometryRegistry.Find(SourceGeometryKey);
                if (!SourceGeo)
                {
                    return 0;
                }

                const uint64 RuntimeGeometryKey = App.GeometryRegistry->RegisterMesh(
                    SourceGeo->DebugName,
                    SourceGeo->Mesh,
                    BuildSettings);

                RuntimeGeometryBySourceKey.emplace(SourceGeometryKey, RuntimeGeometryKey);
                return RuntimeGeometryKey;
            };

            App.Entries.clear();
            App.Entries.reserve(App.SourceDocument.DrawItems.size());

            SIZE_T SkippedItems = 0;
            SIZE_T ProgressCounter = 0;
            for (const CadSceneTest::FSkCadSceneDrawItem& DrawItem : App.SourceDocument.DrawItems)
            {
                const FSKGeometryData* SourceGeo = App.SourceDocument.GeometryRegistry.Find(DrawItem.GeometryKey);
                if (!SourceGeo)
                {
                    ++SkippedItems;
                    continue;
                }

                if (DrawItem.SectionIndex < 0 || static_cast<SIZE_T>(DrawItem.SectionIndex) >= SourceGeo->Mesh.Sections.size())
                {
                    ++SkippedItems;
                    continue;
                }

                const uint64 RuntimeGeometryKey = ResolveRuntimeGeometryKey(DrawItem.GeometryKey);
                if (RuntimeGeometryKey == 0)
                {
                    ++SkippedItems;
                    continue;
                }

                auto Node = std::make_shared<FSKGeometryNode>("CadItem_" + std::to_string(DrawItem.StableId));
                Node->SetGeometryKey(RuntimeGeometryKey);
                Node->SetSectionIndex(DrawItem.SectionIndex);

                const FSKObjectId OwnerId = MakeObjectId(App, DrawItem.StableId);
                const FSKHitProxyId HitId = App.HitProxyTable.Register(OwnerId);

                Node->SetOwnerId(OwnerId);
                Node->SetHitProxyId(HitId.Value);
                Node->SetSelectable(true);

                FNodeEntry Entry{};
                Entry.Node = Node;
                Entry.OwnerId = OwnerId;
                Entry.GeometryKey = RuntimeGeometryKey;
                Entry.HitProxyId = HitId.Value;
                Entry.BaseTransform = MakeRowMajorFromColumnMajorAppMat(DrawItem.WorldMatrix);
                Entry.ExplosionDirection = Normalize(FSKVector3f(
                    DrawItem.ExplosionDirection.X,
                    DrawItem.ExplosionDirection.Y,
                    DrawItem.ExplosionDirection.Z));
                ApplyEntryTransform(App, Entry);

                Root->AddChild(Node);
                App.Entries.push_back(std::move(Entry));

                ++ProgressCounter;
                if ((ProgressCounter & 0x3FFu) == 0u)
                {
                    std::fprintf(stdout,
                        "CadEditorLab populate progress: drawItems=%llu/%llu runtimeGeometries=%llu skipped=%llu\n",
                        static_cast<unsigned long long>(ProgressCounter),
                        static_cast<unsigned long long>(App.SourceDocument.DrawItems.size()),
                        static_cast<unsigned long long>(RuntimeGeometryBySourceKey.size()),
                        static_cast<unsigned long long>(SkippedItems));
                    std::fflush(stdout);
                    ::SKRuntimeDiagWrite("[stage] populate progress drawItems=%llu/%llu runtimeGeometries=%llu skipped=%llu",
                        static_cast<unsigned long long>(ProgressCounter),
                        static_cast<unsigned long long>(App.SourceDocument.DrawItems.size()),
                        static_cast<unsigned long long>(RuntimeGeometryBySourceKey.size()),
                        static_cast<unsigned long long>(SkippedItems));
                    glfwPollEvents();
                }
            }

            RebuildOwnerIndex(App);

            std::fprintf(stdout,
                "CadEditorLab scene populated: drawItems=%llu, runtimeNodes=%llu, runtimeGeometries=%llu, skipped=%llu\n",
                static_cast<unsigned long long>(App.SourceDocument.DrawItems.size()),
                static_cast<unsigned long long>(App.Entries.size()),
                static_cast<unsigned long long>(RuntimeGeometryBySourceKey.size()),
                static_cast<unsigned long long>(SkippedItems));
            std::fflush(stdout);

            return true;
        }

        static bool ShouldEnableCadEdgeOverlay(const FAppState& App)
        {
            const char* ForceEdges = std::getenv("SKYLARK_CAD_EDGES");
            if (ForceEdges && (*ForceEdges == '1' || *ForceEdges == 't' || *ForceEdges == 'T' || *ForceEdges == 'y' || *ForceEdges == 'Y'))
            {
                return App.bShadedWithEdges;
            }

            // The lightweight sample line renderer still expands CAD edge lines on the CPU.
            // On the 68k-draw-item scene this stalls the UI before the first visible frame.
            if (App.SourceDocument.DrawItems.size() > 5000u)
            {
                return false;
            }
            return App.bShadedWithEdges;
        }

        static void UpdateViewportView(FAppState& App)
        {
            auto* View = dynamic_cast<ISKViewportView*>(App.Viewport);
            if (!View)
            {
                return;
            }

            int Width = 0;
            int Height = 0;
            glfwGetFramebufferSize(App.Window, &Width, &Height);
            App.FrameWidth = (std::max)(1, Width);
            App.FrameHeight = (std::max)(1, Height);

            const FSKVector3f Eye = GetCameraPosition(App.Camera);
            const float Aspect = static_cast<float>(App.FrameWidth) / static_cast<float>(App.FrameHeight);
            const float ZNear = (std::max)(0.05f, App.SourceDocument.SceneRadius * 0.001f);
            const float ZFar = (std::max)(100.0f, App.SourceDocument.SceneRadius * 40.0f);

            FSKViewInfo& ViewInfo = View->GetViewInfoMutable();
            ViewInfo.ViewOrigin = FSKVector3d(Eye.X, Eye.Y, Eye.Z);
            ViewInfo.Matrices.View = MakeLookAt(Eye, App.Camera.Target, FSKVector3f(0.0f, 0.0f, 1.0f));
            ViewInfo.Matrices.Projection = MakePerspective(45.0f * (std::numbers::pi_v<float> / 180.0f), Aspect, ZNear, ZFar);
            ViewInfo.EdgeSettings.Display = FSKEdgeRenderSettings::EDisplay::Sharp;
            ViewInfo.EdgeSettings.CreaseAngleDeg = 30.0f;
            ViewInfo.EdgeSettings.bOverlay = true;
            ViewInfo.VisibleLineStyle.Width = 1.25f;
            ViewInfo.VisibleLineStyle.ColorRGBA8 = 0xFF111111u;
            ViewInfo.HiddenLineStyle.Width = 1.0f;
            ViewInfo.HiddenLineStyle.ColorRGBA8 = 0x80808080u;

            const bool bEdgeOverlayEnabled = ShouldEnableCadEdgeOverlay(App);
            ViewInfo.Features.bBasePass = true;
            ViewInfo.Features.bEdges = bEdgeOverlayEnabled;
            ViewInfo.Features.bHiddenLine = App.bHiddenLine;
            ViewInfo.Features.bSelection = false;
            ViewInfo.Features.bSection = false;
            ViewInfo.Features.bFieldViz = false;
            ViewInfo.Features.bPostProcess = true;

            ViewInfo.Selection.SelectedTopoIds.clear();
            ViewInfo.Selection.HoveredTopoIds.clear();
            ViewInfo.Selection.Style.bEnable = true;
            ViewInfo.Selection.Style.bTintFill = true;
            ViewInfo.Selection.Style.bOutline = true;
            ViewInfo.Selection.Style.bShowHover = false;
            ViewInfo.Selection.Style.bShowSelection = true;
            ViewInfo.Selection.Style.SelectedFillAlpha = 0.22f;
            ViewInfo.Selection.Style.HoverFillAlpha = 0.15f;
            ViewInfo.Selection.Style.OutlineWidth = 2.5f;
            ViewInfo.Selection.Style.SelectedFillRGBA8 = 0x4234A8FFu;
            ViewInfo.Selection.Style.HoverFillRGBA8 = 0x30FFD27Au;
            ViewInfo.Selection.Style.OutlineRGBA8 = 0xFF1870FFu;
            ViewInfo.Selection.Style.PickEntityMask = SK_Select_Object | SK_Select_Face | SK_Select_Edge | SK_Select_BimElement;
            ViewInfo.Selection.Style.HighlightEntityMask = SK_Select_Object | SK_Select_Face | SK_Select_Edge | SK_Select_BimElement;

            for (const FNodeEntry& Entry : App.Entries)
            {
                if (Entry.OwnerId.Hash64() == App.SelectedOwnerHash)
                {
                    ViewInfo.Selection.SelectedTopoIds.push_back(Entry.OwnerId);
                    break;
                }
            }

            if (App.bHiddenLine)
            {
                ViewInfo.ViewMode = ESKViewMode::HiddenLine;
            }
            else if (App.bWireframe)
            {
                ViewInfo.ViewMode = ESKViewMode::Wireframe;
            }
            else
            {
                ViewInfo.ViewMode = bEdgeOverlayEnabled ? ESKViewMode::ShadedWithEdges : ESKViewMode::Shaded;
            }
        }

        static void FitView(FAppState& App)
        {
            App.Camera.Target = FSKVector3f(
                App.SourceDocument.SceneCenter.X,
                App.SourceDocument.SceneCenter.Y,
                App.SourceDocument.SceneCenter.Z);
            App.Camera.Distance = (std::max)(2.0f, App.SourceDocument.SceneRadius * 2.4f);
            App.Camera.YawRadians = 0.8f;
            App.Camera.PitchRadians = 0.5f;
        }

        static void ResizeViewportIfNeeded(FAppState& App)
        {
            int Width = 0;
            int Height = 0;
            glfwGetFramebufferSize(App.Window, &Width, &Height);
            Width = (std::max)(1, Width);
            Height = (std::max)(1, Height);

            if (Width != App.FrameWidth || Height != App.FrameHeight)
            {
                App.FrameWidth = Width;
                App.FrameHeight = Height;
                if (App.Viewport)
                {
                    App.Viewport->Resize(Width, Height);
                }
            }
        }

        static void SelectByOwnerHash(FAppState& App, uint64 OwnerHash)
        {
            App.SelectedOwnerHash = OwnerHash;
        }

        static void PerformPick(FAppState& App)
        {
            auto* View = dynamic_cast<ISKViewportView*>(App.Viewport);
            auto* ViewportRHI = dynamic_cast<ISKViewportRHI*>(App.Viewport);
            auto* SelectionRHI = dynamic_cast<ISKViewportSelectionRHI*>(App.Viewport);
            if (!View || !ViewportRHI || !SelectionRHI)
            {
                return;
            }

            double CursorX = 0.0;
            double CursorY = 0.0;
            glfwGetCursorPos(App.Window, &CursorX, &CursorY);

            int FbWidth = 0;
            int FbHeight = 0;
            int WinWidth = 0;
            int WinHeight = 0;
            glfwGetFramebufferSize(App.Window, &FbWidth, &FbHeight);
            glfwGetWindowSize(App.Window, &WinWidth, &WinHeight);
            FbWidth = (std::max)(1, FbWidth);
            FbHeight = (std::max)(1, FbHeight);
            WinWidth = (std::max)(1, WinWidth);
            WinHeight = (std::max)(1, WinHeight);

            const double ScaleX = static_cast<double>(FbWidth) / static_cast<double>(WinWidth);
            const double ScaleY = static_cast<double>(FbHeight) / static_cast<double>(WinHeight);

            int PickX = static_cast<int>(CursorX * ScaleX);
            int PickY = static_cast<int>(CursorY * ScaleY);
            PickX = std::clamp(PickX, 0, FbWidth - 1);
            PickY = std::clamp(PickY, 0, FbHeight - 1);
            PickY = (FbHeight - 1) - PickY;

            FSKViewInfo& ViewInfo = View->GetViewInfoMutable();
            ViewInfo.Features.bSelection = true;
            App.Engine->Tick(0.0f);
            ViewInfo.Features.bSelection = false;

            FSKGpuPicker Picker(ViewportRHI->GetDevice(), SelectionRHI->GetSelectionTexture(), &App.HitProxyTable);
            const FSKPickResult Result = Picker.Pick(PickX, PickY);
            SelectByOwnerHash(App, Result.ObjectId);
        }

        static bool IsSelected(const FAppState& App, const FNodeEntry& Entry)
        {
            return Entry.OwnerId.Hash64() == App.SelectedOwnerHash;
        }

        static void MoveSelected(FAppState& App, const FSKVector3f& Delta)
        {
            for (FNodeEntry& Entry : App.Entries)
            {
                if (!IsSelected(App, Entry))
                {
                    continue;
                }
                Entry.UserOffset = Add(Entry.UserOffset, Delta);
                ApplyEntryTransform(App, Entry);
                break;
            }
        }

        static void ResetSelectedOffset(FAppState& App)
        {
            for (FNodeEntry& Entry : App.Entries)
            {
                if (!IsSelected(App, Entry))
                {
                    continue;
                }
                Entry.UserOffset = FSKVector3f(0.0f, 0.0f, 0.0f);
                ApplyEntryTransform(App, Entry);
                break;
            }
        }

        static void HideSelected(FAppState& App)
        {
            for (FNodeEntry& Entry : App.Entries)
            {
                if (!IsSelected(App, Entry))
                {
                    continue;
                }
                Entry.bHidden = true;
                ApplyEntryTransform(App, Entry);
                break;
            }
        }

        static void ShowAll(FAppState& App)
        {
            App.bHasIsolation = false;
            for (FNodeEntry& Entry : App.Entries)
            {
                Entry.bHidden = false;
                ApplyEntryTransform(App, Entry);
            }
        }

        static void IsolateSelected(FAppState& App)
        {
            if (App.SelectedOwnerHash == 0)
            {
                return;
            }

            App.bHasIsolation = true;
            for (FNodeEntry& Entry : App.Entries)
            {
                const bool bKeep = IsSelected(App, Entry);
                Entry.Node->SetVisible(bKeep && !Entry.bHidden);
            }
        }

        static void CloneSelected(FAppState& App)
        {
            size_t SelectedIndex = static_cast<size_t>(-1);
            for (size_t Index = 0; Index < App.Entries.size(); ++Index)
            {
                if (IsSelected(App, App.Entries[Index]))
                {
                    SelectedIndex = Index;
                    break;
                }
            }
            if (SelectedIndex == static_cast<size_t>(-1))
            {
                return;
            }

            const FNodeEntry& Source = App.Entries[SelectedIndex];
            auto CloneNode = std::make_shared<FSKGeometryNode>("Clone_" + std::to_string(App.NextObjectSeed));
            CloneNode->SetGeometryKey(Source.GeometryKey);
            CloneNode->SetSectionIndex(Source.Node ? Source.Node->GetSectionIndex() : -1);

            FNodeEntry Clone{};
            Clone.Node = CloneNode;
            Clone.OwnerId = MakeObjectId(App, 0);
            Clone.HitProxyId = App.HitProxyTable.Register(Clone.OwnerId).Value;
            Clone.GeometryKey = Source.GeometryKey;
            Clone.BaseTransform = Source.BaseTransform;
            Clone.ExplosionDirection = Source.ExplosionDirection;
            Clone.UserOffset = Add(Source.UserOffset, MakeVec3(App.SourceDocument.SceneRadius * 0.10f, 0.0f, 0.0f));
            Clone.bHidden = false;

            CloneNode->SetOwnerId(Clone.OwnerId);
            CloneNode->SetHitProxyId(Clone.HitProxyId);
            CloneNode->SetSelectable(true);
            ApplyEntryTransform(App, Clone);

            App.SceneGraph->GetRoot()->AddChild(CloneNode);
            App.Entries.push_back(std::move(Clone));
            RebuildOwnerIndex(App);
            App.SelectedOwnerHash = App.Entries.back().OwnerId.Hash64();
        }

        static void UpdateWindowTitle(FAppState& App)
        {
            FSKString Dynamic = FSKString("explode=") + std::to_string(App.ExplodeAmount).c_str() +
                "  selected=" + std::to_string(static_cast<unsigned long long>(App.SelectedOwnerHash)).c_str();
            const FSKString Stats = SKBuildSampleRuntimeStatsSuffix(App.RuntimeStats, App.RuntimeStatsOptions);
            if (!Stats.empty())
            {
                Dynamic += "  ";
                Dynamic += Stats;
            }
            const FSKString Title = SKBuildSampleWindowTitle(
                FSKSampleHostInfo{
                    "SkylarkCadEditorLab",
                    FSKString("Runtime/") + SKRHIApiToString(App.ActiveApi),
                    App.LoadedScenePath,
                    App.FrameWidth,
                    App.FrameHeight,
                    App.bEnableVSync },
                Dynamic);
            glfwSetWindowTitle(App.Window, Title.c_str());
        }

        static void OnKey(FAppState& App, int Key, int Action, int Mods)
        {
            if (!(Action == GLFW_PRESS || Action == GLFW_REPEAT))
            {
                return;
            }

            const float LinearStep = (std::max)(0.01f, App.SourceDocument.SceneRadius * 0.025f);
            const FSKVector3f Forward = GetCameraForward(App.Camera);
            const FSKVector3f WorldUp = FSKVector3f(0.0f, 0.0f, 1.0f);
            const FSKVector3f Right = Normalize(Cross(Forward, WorldUp));
            const FSKVector3f PlanarForward = Normalize(FSKVector3f(Forward.X, Forward.Y, 0.0f));

            switch (Key)
            {
            case GLFW_KEY_F:
                FitView(App);
                break;
            case GLFW_KEY_LEFT_BRACKET:
                App.ExplodeAmount = (std::max)(0.0f, App.ExplodeAmount - LinearStep);
                ApplyAllTransforms(App);
                break;
            case GLFW_KEY_RIGHT_BRACKET:
                App.ExplodeAmount += LinearStep;
                ApplyAllTransforms(App);
                break;
            case GLFW_KEY_H:
                HideSelected(App);
                break;
            case GLFW_KEY_I:
                IsolateSelected(App);
                break;
            case GLFW_KEY_U:
                ShowAll(App);
                break;
            case GLFW_KEY_R:
                ResetSelectedOffset(App);
                break;
            case GLFW_KEY_DELETE:
            case GLFW_KEY_BACKSPACE:
                HideSelected(App);
                break;
            case GLFW_KEY_1:
                App.bHiddenLine = false;
                App.bWireframe = false;
                App.bShadedWithEdges = true;
                break;
            case GLFW_KEY_2:
                App.bHiddenLine = false;
                App.bWireframe = false;
                App.bShadedWithEdges = false;
                break;
            case GLFW_KEY_3:
                App.bHiddenLine = false;
                App.bWireframe = true;
                App.bShadedWithEdges = false;
                break;
            case GLFW_KEY_4:
                App.bHiddenLine = true;
                App.bWireframe = false;
                App.bShadedWithEdges = false;
                break;
            case GLFW_KEY_C:
                if ((Mods & GLFW_MOD_CONTROL) != 0 || Action == GLFW_PRESS)
                {
                    CloneSelected(App);
                }
                break;
            case GLFW_KEY_W:
                MoveSelected(App, Scale(PlanarForward, LinearStep));
                break;
            case GLFW_KEY_S:
                MoveSelected(App, Scale(PlanarForward, -LinearStep));
                break;
            case GLFW_KEY_A:
                MoveSelected(App, Scale(Right, -LinearStep));
                break;
            case GLFW_KEY_D:
                MoveSelected(App, Scale(Right, LinearStep));
                break;
            case GLFW_KEY_Q:
                MoveSelected(App, MakeVec3(0.0f, 0.0f, LinearStep));
                break;
            case GLFW_KEY_E:
                MoveSelected(App, MakeVec3(0.0f, 0.0f, -LinearStep));
                break;
            default:
                break;
            }
        }

        static void KeyCallback(GLFWwindow*, int Key, int, int Action, int Mods)
        {
            if (GApp)
            {
                OnKey(*GApp, Key, Action, Mods);
            }
        }

        static void MouseButtonCallback(GLFWwindow* Window, int Button, int Action, int)
        {
            if (!GApp)
            {
                return;
            }

            FAppState& App = *GApp;
            double X = 0.0;
            double Y = 0.0;
            glfwGetCursorPos(Window, &X, &Y);

            if (Button == GLFW_MOUSE_BUTTON_RIGHT)
            {
                App.Camera.bRightDragging = (Action == GLFW_PRESS);
                App.Camera.LastMouseX = X;
                App.Camera.LastMouseY = Y;
            }
            else if (Button == GLFW_MOUSE_BUTTON_MIDDLE)
            {
                App.Camera.bMiddleDragging = (Action == GLFW_PRESS);
                App.Camera.LastMouseX = X;
                App.Camera.LastMouseY = Y;
            }
            else if (Button == GLFW_MOUSE_BUTTON_LEFT)
            {
                if (Action == GLFW_PRESS)
                {
                    App.Camera.bLeftPressed = true;
                    App.Camera.bLeftMaybeClick = true;
                    App.Camera.LeftPressX = X;
                    App.Camera.LeftPressY = Y;
                    App.Camera.LastMouseX = X;
                    App.Camera.LastMouseY = Y;
                }
                else if (Action == GLFW_RELEASE)
                {
                    App.Camera.bLeftPressed = false;
                    if (App.Camera.bLeftMaybeClick)
                    {
                        App.bRequestPick = true;
                    }
                }
            }
        }

        static void CursorPosCallback(GLFWwindow*, double X, double Y)
        {
            if (!GApp)
            {
                return;
            }

            FAppState& App = *GApp;
            const double Dx = X - App.Camera.LastMouseX;
            const double Dy = Y - App.Camera.LastMouseY;
            App.Camera.LastMouseX = X;
            App.Camera.LastMouseY = Y;

            if (App.Camera.bLeftPressed)
            {
                const double MoveDist = std::abs(X - App.Camera.LeftPressX) + std::abs(Y - App.Camera.LeftPressY);
                if (MoveDist > 3.0)
                {
                    App.Camera.bLeftMaybeClick = false;
                }
            }

            if (App.Camera.bRightDragging)
            {
                const float OrbitScale = 0.008f * App.CameraOptions.OrbitScale;
                const float PitchSign = App.CameraOptions.bInvertOrbitY ? 1.0f : -1.0f;
                App.Camera.YawRadians += static_cast<float>(Dx) * OrbitScale;
                App.Camera.PitchRadians += static_cast<float>(Dy) * OrbitScale * PitchSign;
                App.Camera.PitchRadians = std::clamp(App.Camera.PitchRadians, -1.55f, 1.55f);
            }
            else if (App.Camera.bMiddleDragging)
            {
                const FSKVector3f Eye = GetCameraPosition(App.Camera);
                const FSKVector3f Forward = Normalize(Sub(App.Camera.Target, Eye));
                const FSKVector3f Right = Normalize(Cross(Forward, FSKVector3f(0.0f, 0.0f, 1.0f)));
                const FSKVector3f Up = Normalize(Cross(Right, Forward));
                const float PanScale = (std::max)(0.001f, App.Camera.Distance * 0.0015f * App.CameraOptions.PanScale);
                App.Camera.Target = Add(App.Camera.Target, Add(Scale(Right, static_cast<float>(-Dx) * PanScale), Scale(Up, static_cast<float>(Dy) * PanScale)));
            }
        }

        static void ScrollCallback(GLFWwindow*, double, double YOffset)
        {
            if (!GApp)
            {
                return;
            }

            FAppState& App = *GApp;
            const float BaseScale = (YOffset > 0.0) ? 0.90f : 1.10f;
            const float ZoomScale = std::pow(BaseScale, App.CameraOptions.ZoomScale);
            App.Camera.Distance = std::clamp(App.Camera.Distance * ZoomScale, (std::max)(0.10f, App.SourceDocument.SceneRadius * 0.02f), App.SourceDocument.SceneRadius * 20.0f);
        }

        static void FramebufferSizeCallback(GLFWwindow*, int Width, int Height)
        {
            if (!GApp)
            {
                return;
            }

            Width = (std::max)(1, Width);
            Height = (std::max)(1, Height);
            GApp->FrameWidth = Width;
            GApp->FrameHeight = Height;

            if (GApp->Viewport)
            {
                GApp->Viewport->Resize(Width, Height);
            }
        }

        static bool InitWindow(FAppState& App, const FSKRHIRuntimeOptions& Options)
        {
            if (!glfwInit())
            {
                std::fprintf(stderr, "glfwInit failed.\n");
                return false;
            }

            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
            // The HWND must be visible before the first swap-chain/render tick.
            // Hidden swap-chain creation can leave the sample looking like a console-only app.
            glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);

            std::string WindowTitle = std::string("SkylarkCadEditorLab [Runtime/") + SKRHIApiToString(Options.RequestedApi) + "]";
            App.Window = glfwCreateWindow((std::max)(1, Options.Width), (std::max)(1, Options.Height), WindowTitle.c_str(), nullptr, nullptr);
            if (!App.Window)
            {
                std::fprintf(stderr, "glfwCreateWindow failed.\n");
                glfwTerminate();
                return false;
            }

            glfwSetKeyCallback(App.Window, &KeyCallback);
            glfwSetMouseButtonCallback(App.Window, &MouseButtonCallback);
            glfwSetCursorPosCallback(App.Window, &CursorPosCallback);
            glfwSetScrollCallback(App.Window, &ScrollCallback);
            glfwSetFramebufferSizeCallback(App.Window, &FramebufferSizeCallback);

            return true;
        }

        static bool InitEngine(FAppState& App, const FSKRHIRuntimeOptions& Options, FSKString& OutError)
        {
            App.Engine = SKCreateEngine();
            if (!App.Engine)
            {
                OutError = "SKCreateEngine failed.";
                return false;
            }

            int Width = (std::max)(1, Options.Width);
            int Height = (std::max)(1, Options.Height);
            glfwGetFramebufferSize(App.Window, &Width, &Height);

            FSKEngineInitParams InitParams{};
            InitParams.NativeWindowHandle = nullptr;
            InitParams.BackBufferWidth = (std::max)(1, Width);
            InitParams.BackBufferHeight = (std::max)(1, Height);
            InitParams.bEnableVSync = Options.bEnableVSync;
            InitParams.bUseRenderThread = Options.bUseRenderThread;
            InitParams.RhiApi = Options.RequestedApi;
            InitParams.bEnableRHIDebug = Options.bEnableDebugLayer;

            std::fprintf(stdout, "[stage] Engine->Init begin\n");
            std::fflush(stdout);
            if (!App.Engine->Init(InitParams))
            {
                OutError = "Skylark engine init failed.";
                return false;
            }
            App.ActiveApi = App.Engine->GetCurrentRHIApi();

#if defined(_WIN32)
            FSKViewportDesc ViewportDesc{};
            ViewportDesc.Window.Handle = glfwGetWin32Window(App.Window);
            ViewportDesc.Width = (std::max)(1, Width);
            ViewportDesc.Height = (std::max)(1, Height);
            ViewportDesc.bVSync = Options.bEnableVSync;
            std::fprintf(stdout, "[stage] Engine CreateViewport begin\n");
            std::fflush(stdout);
            App.Viewport = App.Engine->CreateViewport(ViewportDesc);
#else
            OutError = "CadEditorLab runtime host is currently implemented for Windows HWND only.";
            return false;
#endif

            if (!App.Viewport)
            {
                OutError = "CreateViewport failed.";
                return false;
            }

            std::fprintf(stdout, "[stage] Engine CreateViewport done\n");
            std::fflush(stdout);

            App.FrameWidth = (std::max)(1, Width);
            App.FrameHeight = (std::max)(1, Height);
            return true;
        }

        static void Shutdown(FAppState& App)
        {
            GApp = nullptr;

            if (App.Engine && App.Viewport)
            {
                App.Engine->DestroyViewport(App.Viewport);
                App.Viewport = nullptr;
            }

            if (App.Engine)
            {
                App.Engine->Shutdown();
                SKDestroyEngine(App.Engine);
                App.Engine = nullptr;
            }

            if (App.Window)
            {
                glfwDestroyWindow(App.Window);
                App.Window = nullptr;
            }
            glfwTerminate();
        }
    }

    int Run(int Argc, char** Argv)
    {
#if !defined(_WIN32)
        std::fprintf(stderr, "SkylarkCadEditorLab runtime sample is Windows-only in this delivery.\n");
        return 1;
#else
        FAppState App{};
        GApp = &App;

        FSKRHIRuntimeOptions RuntimeOptions{};
        FSKString RuntimeWarnings;
        SKParseRHIRuntimeOptions(Argc, Argv, RuntimeOptions, &RuntimeWarnings);
        if (!RuntimeWarnings.empty())
        {
            std::fprintf(stderr, "%s\n", RuntimeWarnings.c_str());
        }
        ApplyCompiledDefaultRhiIfNeeded(Argc, Argv, RuntimeOptions);
        PrintRuntimeFixStamp(RuntimeOptions);
        ApplyRuntimeBackendSafetyOverride(RuntimeOptions);
        PrintRuntimeFixStamp(RuntimeOptions);

        FSKString CameraWarnings;
        SKParseSampleCameraOptions(Argc, Argv, App.CameraOptions, &CameraWarnings);
        if (!CameraWarnings.empty())
        {
            std::fprintf(stderr, "%s\n", CameraWarnings.c_str());
        }

        FSKString RuntimeStatWarnings;
        SKParseSampleRuntimeStatsOptions(Argc, Argv, App.RuntimeStatsOptions, &RuntimeStatWarnings);
        if (!RuntimeStatWarnings.empty())
        {
            std::fprintf(stderr, "%s\n", RuntimeStatWarnings.c_str());
        }

        FSKString BenchmarkWarnings;
        SKParseSampleBenchmarkOptions(Argc, Argv, App.BenchmarkOptions, &BenchmarkWarnings);
        if (!BenchmarkWarnings.empty())
        {
            std::fprintf(stderr, "%s\n", BenchmarkWarnings.c_str());
        }

        FSKString RunReportWarnings;
        SKParseSampleRunReportOptions(Argc, Argv, App.RunReportOptions, &RunReportWarnings);
        if (!RunReportWarnings.empty())
        {
            std::fprintf(stderr, "%s\n", RunReportWarnings.c_str());
        }

        FSKString SceneManifestWarnings;
        SKParseSampleSceneManifestOptions(Argc, Argv, App.SceneManifestOptions, &SceneManifestWarnings);
        if (!SceneManifestWarnings.empty())
        {
            std::fprintf(stderr, "%s\n", SceneManifestWarnings.c_str());
        }

        FSKString OutputLayoutWarnings;
        SKParseSampleOutputLayoutOptions(Argc, Argv, App.OutputLayoutOptions, &OutputLayoutWarnings);
        if (!OutputLayoutWarnings.empty())
        {
            std::fprintf(stderr, "%s\n", OutputLayoutWarnings.c_str());
        }

        FSKCadSceneDiagnosticsOptions DiagnosticsOptions{};
        FSKString DiagnosticsWarnings;
        SKParseCadSceneDiagnosticsOptions(Argc, Argv, DiagnosticsOptions, &DiagnosticsWarnings);
        if (!DiagnosticsWarnings.empty())
        {
            std::fprintf(stderr, "%s\n", DiagnosticsWarnings.c_str());
        }

        if (RuntimeOptions.bShowHelp)
        {
            PrintCommandLineHelp();
            return 0;
        }

if (App.SceneManifestOptions.ManifestPath.empty() == false)
        {
            std::vector<FSKSampleSceneManifestEntry> ManifestEntries;
            FSKString ManifestError;
            if (!SKLoadSampleSceneManifest(App.SceneManifestOptions, ManifestEntries, &ManifestError))
            {
                std::fprintf(stderr, "%s\n", ManifestError.c_str());
                return 1;
            }
            if (App.SceneManifestOptions.bListScenesOnly)
            {
                SKPrintSampleSceneManifestEntries(ManifestEntries);
                return 0;
            }
            if (RuntimeOptions.ScenePath.empty())
            {
                const std::filesystem::path ManifestScene = SKResolveSampleSceneFromManifest(App.SceneManifestOptions, ManifestEntries, &ManifestError);
                if (ManifestScene.empty())
                {
                    std::fprintf(stderr, "%s\n", ManifestError.c_str());
                    return 1;
                }
                RuntimeOptions.ScenePath = ManifestScene.string().c_str();
            }
        }

        const std::filesystem::path ScenePath = ResolveScenePath(RuntimeOptions);
        App.LoadedScenePath = ScenePath;
        ::SKRuntimeDiagWrite("[stage] resolved scene path=%s", ScenePath.string().c_str());
        ::SKRuntimeDiagWrite("[stage] InitWindow begin requestedRhi=%s size=%dx%d", SKRHIApiToString(RuntimeOptions.RequestedApi), RuntimeOptions.Width, RuntimeOptions.Height);
        if (!InitWindow(App, RuntimeOptions))
        {
            ::SKRuntimeDiagWrite("[stage] InitWindow failed");
            return 1;
        }
        ::SKRuntimeDiagWrite("[stage] InitWindow done");
        glfwShowWindow(App.Window);
        glfwPollEvents();
        glfwSetWindowTitle(App.Window, "SkylarkCadEditorLab - loading scene...");
        SKPrintSampleLaunchSummary(FSKSampleHostInfo{ "SkylarkCadEditorLab", FSKString("Runtime/") + SKRHIApiToString(RuntimeOptions.RequestedApi), ScenePath, RuntimeOptions.Width, RuntimeOptions.Height, RuntimeOptions.bEnableVSync });
        std::fprintf(stdout, "SkylarkCadEditorLab using RHI: %s\n", SKRHIApiToString(RuntimeOptions.RequestedApi));
        App.DiagnosticsOptions = DiagnosticsOptions;
        FSKString OutputApplyWarnings;
        SKApplySampleOutputLayoutDefaults(App.OutputLayoutOptions, "SkylarkCadEditorLab", (FSKString("Runtime/") + SKRHIApiToString(RuntimeOptions.RequestedApi)).c_str(), ScenePath, App.BenchmarkOptions, App.RunReportOptions, &OutputApplyWarnings);
        if (!OutputApplyWarnings.empty())
        {
            std::fprintf(stderr, "%s\n", OutputApplyWarnings.c_str());
        }
        SKPrintSampleCameraOptionSummary(App.CameraOptions);
        SKPrintSampleRuntimeStatsOptionSummary(App.RuntimeStatsOptions);
        SKPrintSampleBenchmarkOptionSummary(App.BenchmarkOptions);
        SKPrintSampleRunReportOptionSummary(App.RunReportOptions);
        SKPrintSampleOutputLayoutOptionSummary(App.OutputLayoutOptions);
        SKPrintCadSceneDiagnosticsOptionSummary(App.DiagnosticsOptions);
        FSKString Error;
        ::SKRuntimeDiagWrite("[stage] LoadCadSceneDocument begin");
        if (!CadSceneTest::LoadCadSceneDocument(ScenePath.string().c_str(), App.SourceDocument, Error))
        {
            std::fprintf(stderr, "LoadCadSceneDocument failed: %s\n", Error.c_str());
            Shutdown(App);
            return 1;
        }
        ::SKRuntimeDiagWrite("[stage] LoadCadSceneDocument done drawItems=%llu geometries=%llu radius=%.3f",
            static_cast<unsigned long long>(App.SourceDocument.DrawItems.size()),
            static_cast<unsigned long long>(App.SourceDocument.GeometryKeys.size()),
            static_cast<double>(App.SourceDocument.SceneRadius));

        SKPrintCadSceneDocumentDiagnostics(App.SourceDocument, App.DiagnosticsOptions);

        if (App.CameraOptions.bFitOnStartup)
        {
            FitView(App);
        }

        std::fprintf(stdout, "[stage] InitEngine begin\n");
        std::fflush(stdout);
        ::SKRuntimeDiagWrite("[stage] InitEngine begin requestedRhi=%s", SKRHIApiToString(RuntimeOptions.RequestedApi));
        if (!InitEngine(App, RuntimeOptions, Error))
        {
            std::fprintf(stderr, "InitEngine failed: %s\n", Error.c_str());
            Shutdown(App);
            return 1;
        }

        std::fprintf(stdout, "[stage] InitEngine done\n");
        ::SKRuntimeDiagWrite("[stage] InitEngine done activeRhi=%s", SKRHIApiToString(App.ActiveApi));
        std::fprintf(stdout, "[fix] CadEditorLab activeRhi=%s\n", SKRHIApiToString(App.ActiveApi));
        std::fflush(stdout);
        std::fprintf(stdout, "[stage] PopulateRuntimeScene begin\n");
        std::fflush(stdout);
        ::SKRuntimeDiagWrite("[stage] PopulateRuntimeScene begin");
        if (!PopulateRuntimeScene(App, Error))
        {
            std::fprintf(stderr, "PopulateRuntimeScene failed: %s\n", Error.c_str());
            Shutdown(App);
            return 1;
        }

        std::fprintf(stdout, "[stage] PopulateRuntimeScene done\n");
        std::fflush(stdout);
        ::SKRuntimeDiagWrite("[stage] PopulateRuntimeScene done entries=%llu", static_cast<unsigned long long>(App.Entries.size()));

        ResizeViewportIfNeeded(App);
        UpdateViewportView(App);
        glfwShowWindow(App.Window);
        glfwFocusWindow(App.Window);
        glfwPollEvents();
        std::fprintf(stdout, "CadEditorLab entering render loop.\n");
        ::SKRuntimeDiagWrite("[stage] entering render loop edges=%s entries=%llu", ShouldEnableCadEdgeOverlay(App) ? "on" : "off", static_cast<unsigned long long>(App.Entries.size()));

        if (App.CameraOptions.bPrintControls)
        {
            SKPrintSampleControls(ESKSampleControlProfile::CadEditorLab);
        }

        SKResetSampleRuntimeStats(App.RuntimeStats);
        SKResetSampleBenchmarkState(App.BenchmarkState);
        auto LastTick = std::chrono::steady_clock::now();
        uint64 FrameIndex = 0;
        while (!glfwWindowShouldClose(App.Window))
        {
            glfwPollEvents();

            ResizeViewportIfNeeded(App);
            UpdateViewportView(App);

            if (App.bRequestPick)
            {
                PerformPick(App);
                App.bRequestPick = false;
            }

            const auto Now = std::chrono::steady_clock::now();
            const std::chrono::duration<float> Delta = Now - LastTick;
            LastTick = Now;

            SKUpdateSampleRuntimeStats(App.RuntimeStats, App.RuntimeStatsOptions, Delta.count());
            SKUpdateSampleBenchmarkState(App.BenchmarkState, App.BenchmarkOptions, Delta.count());

            if (FrameIndex == 0) { ::SKRuntimeDiagWrite("[stage] first frame Tick begin"); }
            App.Engine->Tick(Delta.count());
            if (FrameIndex == 0) { ::SKRuntimeDiagWrite("[stage] first frame Tick done"); }
            ++FrameIndex;

            FSKString StatsLine;
            if (SKTryConsumeSampleRuntimeStatsPrint(App.RuntimeStats, App.RuntimeStatsOptions, StatsLine))
            {
                std::fprintf(stdout, "%s\n", StatsLine.c_str());
            }

            if (SKShouldRefreshSampleRuntimeTitle(App.RuntimeStats, App.RuntimeStatsOptions))
            {
                UpdateWindowTitle(App);
            }

            if (SKShouldExitSampleBenchmark(App.BenchmarkState, App.BenchmarkOptions))
            {
                glfwSetWindowShouldClose(App.Window, GLFW_TRUE);
            }
        }

        if (App.BenchmarkOptions.bPrintSummaryOnExit)
        {
            const FSKString SummaryLine = SKBuildSampleBenchmarkSummaryLine(
                "SkylarkCadEditorLab",
                (FSKString("Runtime/") + SKRHIApiToString(RuntimeOptions.RequestedApi)).c_str(),
                App.LoadedScenePath,
                App.BenchmarkState,
                App.RuntimeStats);
            std::fprintf(stdout, "%s\n", SummaryLine.c_str());
        }
        if (!App.BenchmarkOptions.CsvPath.empty())
        {
            FSKString CsvError;
            if (!SKWriteSampleBenchmarkSummaryCsv(
                    std::filesystem::path(App.BenchmarkOptions.CsvPath),
                    App.BenchmarkOptions.bAppendCsv,
                    "SkylarkCadEditorLab",
                    (FSKString("Runtime/") + SKRHIApiToString(RuntimeOptions.RequestedApi)).c_str(),
                    App.LoadedScenePath,
                    App.BenchmarkState,
                    App.RuntimeStats,
                    &CsvError))
            {
                std::fprintf(stderr, "%s\n", CsvError.c_str());
            }
        }
        if (!App.RunReportOptions.ReportPath.empty())
        {
            FSKString ReportError;
            std::filesystem::path WrittenPath;
            if (!SKWriteSampleRunReport(
                    App.RunReportOptions,
                    FSKSampleRunReportInput{
                        "SkylarkCadEditorLab",
                        FSKString("Runtime/") + SKRHIApiToString(RuntimeOptions.RequestedApi),
                        App.LoadedScenePath,
                        App.FrameWidth,
                        App.FrameHeight,
                        App.bEnableVSync,
                        &App.RuntimeStats,
                        &App.BenchmarkState,
                        &App.SourceDocument },
                    &ReportError,
                    &WrittenPath))
            {
                std::fprintf(stderr, "%s\n", ReportError.c_str());
            }
            else if (App.RunReportOptions.bPrintPathOnWrite)
            {
                std::fprintf(stdout, "Run report: %s\n", WrittenPath.string().c_str());
            }
        }

        Shutdown(App);
        return 0;
#endif
    }
}

#if defined(_WIN32)
namespace
{
    std::filesystem::path GSkCadEditorLabRuntimeLogPath;
    FILE* GSkCadEditorLabRuntimeLogFile = nullptr;

    std::filesystem::path SKFindRepoRootFromExecutable()
    {
        wchar_t ModulePath[MAX_PATH]{};
        const DWORD Length = ::GetModuleFileNameW(nullptr, ModulePath, MAX_PATH);
        std::filesystem::path Cursor = (Length > 0) ? std::filesystem::path(ModulePath).parent_path() : std::filesystem::current_path();
        for (int Depth = 0; Depth < 12 && !Cursor.empty(); ++Depth)
        {
            if (std::filesystem::exists(Cursor / "CMakeLists.txt") && std::filesystem::exists(Cursor / "Source"))
            {
                return Cursor;
            }
            Cursor = Cursor.parent_path();
        }
        return std::filesystem::current_path();
    }

    void SKRuntimeDiagWrite(const char* Format, ...)
    {
        char Buffer[2048]{};
        va_list Args;
        va_start(Args, Format);
        vsnprintf(Buffer, sizeof(Buffer), Format, Args);
        va_end(Args);

        if (GSkCadEditorLabRuntimeLogFile)
        {
            std::fprintf(GSkCadEditorLabRuntimeLogFile, "%s\n", Buffer);
            std::fflush(GSkCadEditorLabRuntimeLogFile);
        }
        std::fprintf(stderr, "%s\n", Buffer);
        std::fflush(stderr);
        ::OutputDebugStringA(Buffer);
        ::OutputDebugStringA("\n");
    }

    LONG WINAPI SKCadEditorLabUnhandledExceptionFilter(EXCEPTION_POINTERS* ExceptionInfo)
    {
        const DWORD Code = ExceptionInfo && ExceptionInfo->ExceptionRecord ? ExceptionInfo->ExceptionRecord->ExceptionCode : 0u;
        const void* Address = ExceptionInfo && ExceptionInfo->ExceptionRecord ? ExceptionInfo->ExceptionRecord->ExceptionAddress : nullptr;
        SKRuntimeDiagWrite("[FATAL] Unhandled SEH exception. code=0x%08lX address=%p", static_cast<unsigned long>(Code), Address);
        if (!GSkCadEditorLabRuntimeLogPath.empty())
        {
            const std::string Message = std::string("SkylarkCadEditorLab crashed.\nRuntime log:\n") + GSkCadEditorLabRuntimeLogPath.string();
            ::MessageBoxA(nullptr, Message.c_str(), "SkylarkCadEditorLab runtime crash", MB_OK | MB_ICONERROR);
        }
        return EXCEPTION_EXECUTE_HANDLER;
    }

    void SKInstallCadEditorLabRuntimeDiagnostics(int argc, char** argv)
    {
        ::SetUnhandledExceptionFilter(&SKCadEditorLabUnhandledExceptionFilter);

        const std::filesystem::path RepoRoot = SKFindRepoRootFromExecutable();
        const std::filesystem::path LogDir = RepoRoot / "out" / "logs";
        std::error_code Ec;
        std::filesystem::create_directories(LogDir, Ec);

        SYSTEMTIME St{};
        ::GetLocalTime(&St);
        char FileName[256]{};
        std::snprintf(
            FileName,
            sizeof(FileName),
            "runtime-windows-cadeditor-self-%04u%02u%02u-%02u%02u%02u-%lu.log",
            St.wYear,
            St.wMonth,
            St.wDay,
            St.wHour,
            St.wMinute,
            St.wSecond,
            static_cast<unsigned long>(::GetCurrentProcessId()));
        GSkCadEditorLabRuntimeLogPath = LogDir / FileName;

        FILE* F = nullptr;
        fopen_s(&F, GSkCadEditorLabRuntimeLogPath.string().c_str(), "w");
        GSkCadEditorLabRuntimeLogFile = F;
        if (F)
        {
            setvbuf(F, nullptr, _IONBF, 0);
        }

        SKRuntimeDiagWrite("============================================================");
        SKRuntimeDiagWrite("SkylarkCadEditorLab self runtime diagnostics");
        SKRuntimeDiagWrite("============================================================");
        SKRuntimeDiagWrite("[repo] %s", RepoRoot.string().c_str());
        SKRuntimeDiagWrite("[log ] %s", GSkCadEditorLabRuntimeLogPath.string().c_str());
        SKRuntimeDiagWrite("[argc] %d", argc);
        for (int I = 0; I < argc; ++I)
        {
            SKRuntimeDiagWrite("[argv%d] %s", I, argv && argv[I] ? argv[I] : "<null>");
        }
    }
}
#endif

int main(int argc, char** argv)
{
#if defined(_WIN32)
    SKInstallCadEditorLabRuntimeDiagnostics(argc, argv);
#endif
    try
    {
#if defined(_WIN32)
        SKRuntimeDiagWrite("[stage] entering Skylark::CadEditorLab::Run");
#endif
        const int Result = Skylark::CadEditorLab::Run(argc, argv);
#if defined(_WIN32)
        SKRuntimeDiagWrite("[stage] leaving Skylark::CadEditorLab::Run result=%d", Result);
#endif
        return Result;
    }
    catch (const std::exception& Ex)
    {
#if defined(_WIN32)
        SKRuntimeDiagWrite("[FATAL] C++ exception: %s", Ex.what());
        if (!GSkCadEditorLabRuntimeLogPath.empty())
        {
            const std::string Message = std::string("SkylarkCadEditorLab failed with C++ exception.\nRuntime log:\n") + GSkCadEditorLabRuntimeLogPath.string();
            ::MessageBoxA(nullptr, Message.c_str(), "SkylarkCadEditorLab runtime exception", MB_OK | MB_ICONERROR);
        }
#else
        std::fprintf(stderr, "SkylarkCadEditorLab fatal C++ exception: %s\n", Ex.what());
#endif
        return 1001;
    }
    catch (...)
    {
#if defined(_WIN32)
        SKRuntimeDiagWrite("[FATAL] Unknown C++ exception.");
        if (!GSkCadEditorLabRuntimeLogPath.empty())
        {
            const std::string Message = std::string("SkylarkCadEditorLab failed with unknown C++ exception.\nRuntime log:\n") + GSkCadEditorLabRuntimeLogPath.string();
            ::MessageBoxA(nullptr, Message.c_str(), "SkylarkCadEditorLab runtime exception", MB_OK | MB_ICONERROR);
        }
#else
        std::fprintf(stderr, "SkylarkCadEditorLab fatal unknown C++ exception.\n");
#endif
        return 1002;
    }
}
