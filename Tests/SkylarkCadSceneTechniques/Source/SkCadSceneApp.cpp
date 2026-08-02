
#include "SkCadSceneImporter.h"
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

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <map>
#include <string>
#include <vector>

namespace Skylark::CadSceneTest
{
    namespace
    {
        enum class EViewMode : int
        {
            Shaded = 0,
            ShadedEdges,
            Wireframe,
            XRay,
            ObjectId,
            Normals,
        };

        enum class EEdgeMode : int
        {
            All = 0,
            Boundary,
            Sharp,
        };

        struct FSectionGpuBuffers
        {
            GLuint Vao = 0;
            GLuint Vbo = 0;
            GLuint EboSolid = 0;
            GLuint EboAllEdges = 0;
            GLuint EboBoundaryEdges = 0;
            GLuint EboSharpEdges = 0;
            GLsizei SolidIndexCount = 0;
            GLsizei AllEdgeIndexCount = 0;
            GLsizei BoundaryEdgeIndexCount = 0;
            GLsizei SharpEdgeIndexCount = 0;
        };

        struct FGeometryGpuBuffers
        {
            std::vector<FSectionGpuBuffers> Sections;
        };

        struct FCameraState
        {
            float YawRadians = 0.8f;
            float PitchRadians = 0.45f;
            float Distance = 10.0f;
            FAppVec3 Target{};
            bool bOrbiting = false;
            bool bPanning = false;
            bool bLeftPressed = false;
            bool bLeftDragExceeded = false;
            double LastMouseX = 0.0;
            double LastMouseY = 0.0;
            double PressMouseX = 0.0;
            double PressMouseY = 0.0;
        };

        struct FSolidUniforms
        {
            GLint Model = -1;
            GLint View = -1;
            GLint Proj = -1;
            GLint BaseColor = -1;
            GLint CameraPos = -1;
            GLint ShadingMode = -1;
            GLint SelectionTint = -1;
        };

        struct FLineUniforms
        {
            GLint Model = -1;
            GLint View = -1;
            GLint Proj = -1;
            GLint LineColor = -1;
        };

        struct FPickUniforms
        {
            GLint Model = -1;
            GLint View = -1;
            GLint Proj = -1;
            GLint ObjectColor = -1;
        };

        struct FPickResources
        {
            GLuint Fbo = 0;
            GLuint ColorTexture = 0;
            GLuint DepthRenderbuffer = 0;
            int Width = 0;
            int Height = 0;
        };

        struct FAppState
        {
            GLFWwindow* pWindow = nullptr;
            FSkCadSceneDocument Document{};
            std::map<uint64, FGeometryGpuBuffers> GeometryGpu;
            GLuint SolidProgram = 0;
            GLuint LineProgram = 0;
            GLuint PickProgram = 0;
            FSolidUniforms SolidUniforms{};
            FLineUniforms LineUniforms{};
            FPickUniforms PickUniforms{};
            FPickResources Pick{};
            EViewMode ViewMode = EViewMode::ShadedEdges;
            EEdgeMode EdgeMode = EEdgeMode::Sharp;
            bool bDisableVsync = false;
            FCameraState Camera{};
            SIZE_T NumSectionsUploaded = 0;
            SIZE_T NumTrianglesUploaded = 0;
            float ExplodeFactor = 0.0f;
            float ExplodeTarget = 0.0f;
            double LastFrameSeconds = 0.0;
            uint32 SelectedStableId = 0;
            bool bPickRequested = false;
            double PendingPickX = 0.0;
            double PendingPickY = 0.0;
            FSKString WindowTitleBase;
            bool bEnableVsync = true;
            int WindowWidth = 1600;
            int WindowHeight = 900;
            std::filesystem::path LoadedScenePath;
            FSKSampleCameraOptions CameraOptions{};
            FSKSampleRuntimeStatsOptions RuntimeStatsOptions{};
            FSKSampleRuntimeStatsState RuntimeStats{};
            FSKSampleBenchmarkOptions BenchmarkOptions{};
            FSKSampleBenchmarkState BenchmarkState{};
            FSKSampleRunReportOptions RunReportOptions{};
            FSKCadSceneDiagnosticsOptions DiagnosticsOptions{};
            FSKSampleSceneManifestOptions SceneManifestOptions{};
            FSKSampleOutputLayoutOptions OutputLayoutOptions{};
        };

        FAppState* GApp = nullptr;

        static const char* GSolidVs = R"GLSL(
        #version 330 core
        layout(location = 0) in vec3 aPosition;
        layout(location = 1) in vec3 aNormal;

        uniform mat4 uModel;
        uniform mat4 uView;
        uniform mat4 uProj;

        out vec3 vWorldPos;
        out vec3 vWorldNormal;

        void main()
        {
            vec4 worldPos = uModel * vec4(aPosition, 1.0);
            mat3 normalMatrix = transpose(inverse(mat3(uModel)));
            vWorldPos = worldPos.xyz;
            vWorldNormal = normalize(normalMatrix * aNormal);
            gl_Position = uProj * uView * worldPos;
        }
        )GLSL";

        static const char* GSolidFs = R"GLSL(
        #version 330 core
        in vec3 vWorldPos;
        in vec3 vWorldNormal;

        uniform vec4 uBaseColor;
        uniform vec3 uCameraPos;
        uniform int uShadingMode; // 0=lit, 1=id, 2=normal
        uniform vec4 uSelectionTint;

        out vec4 FragColor;

        void main()
        {
            vec3 N = normalize(vWorldNormal);

            if (uShadingMode == 1)
            {
                FragColor = uBaseColor;
                return;
            }

            if (uShadingMode == 2)
            {
                FragColor = vec4(normalize(vWorldNormal) * 0.5 + 0.5, 1.0);
                return;
            }

            vec3 L = normalize(vec3(0.35, 0.65, 0.68));
            vec3 V = normalize(uCameraPos - vWorldPos);
            vec3 H = normalize(L + V);

            float NoL = max(dot(N, L), 0.0);
            float NoV = max(dot(N, V), 0.0);
            float NoH = max(dot(N, H), 0.0);
            float Hemi = N.z * 0.5 + 0.5;

            vec3 Sky = vec3(0.72, 0.78, 0.88);
            vec3 Ground = vec3(0.22, 0.23, 0.24);
            vec3 Ambient = mix(Ground, Sky, Hemi) * 0.42;
            float Specular = pow(NoH, 48.0) * (0.08 + 0.18 * NoL);
            float Rim = pow(1.0 - NoV, 3.0) * 0.10;

            vec3 Lit = uBaseColor.rgb * (Ambient + NoL * 0.82) + vec3(1.0) * Specular + uBaseColor.rgb * Rim;
            Lit = mix(Lit, uSelectionTint.rgb, uSelectionTint.a);
            Lit = pow(max(Lit, vec3(0.0)), vec3(1.0 / 2.2));
            FragColor = vec4(Lit, uBaseColor.a);
        }
        )GLSL";

        static const char* GLineVs = R"GLSL(
        #version 330 core
        layout(location = 0) in vec3 aPosition;
        uniform mat4 uModel;
        uniform mat4 uView;
        uniform mat4 uProj;
        void main()
        {
            gl_Position = uProj * uView * uModel * vec4(aPosition, 1.0);
        }
        )GLSL";

        static const char* GLineFs = R"GLSL(
        #version 330 core
        uniform vec4 uLineColor;
        out vec4 FragColor;
        void main()
        {
            FragColor = uLineColor;
        }
        )GLSL";

        static const char* GPickVs = R"GLSL(
        #version 330 core
        layout(location = 0) in vec3 aPosition;
        uniform mat4 uModel;
        uniform mat4 uView;
        uniform mat4 uProj;
        void main()
        {
            gl_Position = uProj * uView * uModel * vec4(aPosition, 1.0);
        }
        )GLSL";

        static const char* GPickFs = R"GLSL(
        #version 330 core
        uniform vec4 uObjectColor;
        out vec4 FragColor;
        void main()
        {
            FragColor = uObjectColor;
        }
        )GLSL";

        GLuint CompileShader(GLenum Type, const char* pSource)
        {
            const GLuint Shader = glCreateShader(Type);
            glShaderSource(Shader, 1, &pSource, nullptr);
            glCompileShader(Shader);
            GLint Status = GL_FALSE;
            glGetShaderiv(Shader, GL_COMPILE_STATUS, &Status);
            if (Status != GL_TRUE)
            {
                GLint LogLength = 0;
                glGetShaderiv(Shader, GL_INFO_LOG_LENGTH, &LogLength);
                std::string Log(LogLength > 0 ? static_cast<size_t>(LogLength) : 1u, '\0');
                glGetShaderInfoLog(Shader, LogLength, nullptr, Log.data());
                std::fprintf(stderr, "Shader compilation failed: %s\n", Log.c_str());
            }
            return Shader;
        }

        GLuint CreateProgram(const char* pVs, const char* pFs)
        {
            const GLuint Vs = CompileShader(GL_VERTEX_SHADER, pVs);
            const GLuint Fs = CompileShader(GL_FRAGMENT_SHADER, pFs);
            const GLuint Program = glCreateProgram();
            glAttachShader(Program, Vs);
            glAttachShader(Program, Fs);
            glLinkProgram(Program);
            GLint Status = GL_FALSE;
            glGetProgramiv(Program, GL_LINK_STATUS, &Status);
            if (Status != GL_TRUE)
            {
                GLint LogLength = 0;
                glGetProgramiv(Program, GL_INFO_LOG_LENGTH, &LogLength);
                std::string Log(LogLength > 0 ? static_cast<size_t>(LogLength) : 1u, '\0');
                glGetProgramInfoLog(Program, LogLength, nullptr, Log.data());
                std::fprintf(stderr, "Program link failed: %s\n", Log.c_str());
            }
            glDeleteShader(Vs);
            glDeleteShader(Fs);
            return Program;
        }

        FSolidUniforms QuerySolidUniforms(GLuint Program)
        {
            FSolidUniforms U{};
            U.Model = glGetUniformLocation(Program, "uModel");
            U.View = glGetUniformLocation(Program, "uView");
            U.Proj = glGetUniformLocation(Program, "uProj");
            U.BaseColor = glGetUniformLocation(Program, "uBaseColor");
            U.CameraPos = glGetUniformLocation(Program, "uCameraPos");
            U.ShadingMode = glGetUniformLocation(Program, "uShadingMode");
            U.SelectionTint = glGetUniformLocation(Program, "uSelectionTint");
            return U;
        }

        FLineUniforms QueryLineUniforms(GLuint Program)
        {
            FLineUniforms U{};
            U.Model = glGetUniformLocation(Program, "uModel");
            U.View = glGetUniformLocation(Program, "uView");
            U.Proj = glGetUniformLocation(Program, "uProj");
            U.LineColor = glGetUniformLocation(Program, "uLineColor");
            return U;
        }

        FPickUniforms QueryPickUniforms(GLuint Program)
        {
            FPickUniforms U{};
            U.Model = glGetUniformLocation(Program, "uModel");
            U.View = glGetUniformLocation(Program, "uView");
            U.Proj = glGetUniformLocation(Program, "uProj");
            U.ObjectColor = glGetUniformLocation(Program, "uObjectColor");
            return U;
        }

        FAppVec3 GetCameraPosition(const FAppState& App)
        {
            const float CP = std::cos(App.Camera.PitchRadians);
            const float SP = std::sin(App.Camera.PitchRadians);
            const float CY = std::cos(App.Camera.YawRadians);
            const float SY = std::sin(App.Camera.YawRadians);
            return AppVec3Add(App.Camera.Target, FAppVec3{ App.Camera.Distance * CP * CY, App.Camera.Distance * CP * SY, App.Camera.Distance * SP });
        }

        FAppVec3 GetCameraForward(const FAppState& App)
        {
            return AppVec3Normalize(AppVec3Sub(App.Camera.Target, GetCameraPosition(App)));
        }

        FAppVec3 Cross(const FAppVec3& A, const FAppVec3& B)
        {
            return FAppVec3{
                A.Y * B.Z - A.Z * B.Y,
                A.Z * B.X - A.X * B.Z,
                A.X * B.Y - A.Y * B.X
            };
        }

        FAppVec3 GetCameraRight(const FAppState& App)
        {
            return AppVec3Normalize(Cross(GetCameraForward(App), FAppVec3{0.0f, 0.0f, 1.0f}));
        }

        FAppVec3 GetCameraUp(const FAppState& App)
        {
            return AppVec3Normalize(Cross(GetCameraRight(App), GetCameraForward(App)));
        }

        FAppVec4 MakeStableColor(uint32 StableId)
        {
            const uint8 R = static_cast<uint8>((StableId * 97u) & 0xFFu);
            const uint8 G = static_cast<uint8>((StableId * 57u) & 0xFFu);
            const uint8 B = static_cast<uint8>((StableId * 17u) & 0xFFu);
            return FAppVec4{ R / 255.0f, G / 255.0f, B / 255.0f, 1.0f };
        }

        FAppVec4 MakeIdEncodeColor(uint32 StableId)
        {
            return FAppVec4{
                static_cast<float>((StableId >> 0) & 0xFFu) / 255.0f,
                static_cast<float>((StableId >> 8) & 0xFFu) / 255.0f,
                static_cast<float>((StableId >> 16) & 0xFFu) / 255.0f,
                1.0f
            };
        }

        uint32 DecodeStableId(const uint8* pPixel)
        {
            return static_cast<uint32>(pPixel[0]) |
                   (static_cast<uint32>(pPixel[1]) << 8u) |
                   (static_cast<uint32>(pPixel[2]) << 16u);
        }

        FAppVec4 GetMaterialColor(const FAppState& App, int32 MaterialIndex)
        {
            if (MaterialIndex >= 0 && static_cast<SIZE_T>(MaterialIndex) < App.Document.Materials.size())
            {
                return App.Document.Materials[static_cast<SIZE_T>(MaterialIndex)].BaseColor;
            }
            return FAppVec4{ 0.7f, 0.72f, 0.75f, 1.0f };
        }
        std::filesystem::path ResolveDefaultScenePath(const FSKSampleAppOptions& Options)
        {
            const auto Cwd = std::filesystem::current_path();
            return SKResolveSampleScenePath(
                Options.ScenePath,
                {
                    Cwd / "Tests" / "SkylarkCadSceneTechniques" / "Assets" / "geforce.csf.gz",
                    Cwd / "Debug" / "Tests" / "SkylarkCadSceneTechniques" / "Assets" / "geforce.csf.gz",
                    Cwd / "Release" / "Tests" / "SkylarkCadSceneTechniques" / "Assets" / "geforce.csf.gz",
                    Cwd / "Tests" / "SkylarkCadEditorLab" / "Assets" / "geforce.csf.gz",
                    std::filesystem::path("Tests/SkylarkCadSceneTechniques/Assets/geforce.csf.gz")
                },
                std::filesystem::path("Tests/SkylarkCadSceneTechniques/Assets/geforce.csf.gz"));
        }


        static const char* ViewModeToString(EViewMode Mode)
        {
            switch (Mode)
            {
            case EViewMode::Shaded: return "shaded";
            case EViewMode::ShadedEdges: return "shaded+edges";
            case EViewMode::Wireframe: return "wireframe";
            case EViewMode::XRay: return "xray";
            case EViewMode::ObjectId: return "object-id";
            case EViewMode::Normals: return "normals";
            default: return "unknown";
            }
        }

        static const char* EdgeModeToString(EEdgeMode Mode)
        {
            switch (Mode)
            {
            case EEdgeMode::All: return "all";
            case EEdgeMode::Boundary: return "boundary";
            case EEdgeMode::Sharp: return "sharp";
            default: return "unknown";
            }
        }

        static void UpdateWindowTitle(FAppState& App)
        {
            FSKString Dynamic = FSKString("mode=") + ViewModeToString(App.ViewMode) +
                "  edges=" + EdgeModeToString(App.EdgeMode) +
                "  explode=" + std::to_string(App.ExplodeFactor).c_str();
            const FSKString Stats = SKBuildSampleRuntimeStatsSuffix(App.RuntimeStats, App.RuntimeStatsOptions);
            if (!Stats.empty())
            {
                Dynamic += "  ";
                Dynamic += Stats;
            }
            const FSKString Title = SKBuildSampleWindowTitle(
                FSKSampleHostInfo{
                    "SkylarkCadSceneTechniques",
                    "OpenGL",
                    App.LoadedScenePath,
                    App.WindowWidth,
                    App.WindowHeight,
                    App.bEnableVsync },
                Dynamic);
            glfwSetWindowTitle(App.pWindow, Title.c_str());
        }
        void PrintCommandLineHelp()
        {
            SKPrintBasicSampleAppHelp(
                "SkylarkCadSceneTechniques",
                "OpenGL benchmark viewer",
                "  positional <scene-path> is also accepted for compatibility\n"
                "  Example: SkylarkCadSceneTechniques --scene=Tests/SkylarkCadSceneTechniques/Assets/geforce.csf.gz\n"
                "  --title-stats / --no-title-stats\n"
                "  --print-stats / --no-print-stats\n"
                "  --stats-interval=<seconds>\n"
                "  --title-stats-every=<frames>\n"
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
                "  SKYLARK_SCENE, SKYLARK_SIZE, SKYLARK_WIDTH, SKYLARK_HEIGHT, SKYLARK_VSYNC,\n"
                "  SKYLARK_TITLE_STATS, SKYLARK_PRINT_STATS, SKYLARK_STATS_INTERVAL,\n"
                "  SKYLARK_SCENE_MANIFEST, SKYLARK_SCENE_INDEX, SKYLARK_LIST_SCENES,\n"
                "  SKYLARK_OUTPUT_DIR, SKYLARK_SESSION_TAG, SKYLARK_AUTO_REPORT_FILES,\n"
                "  SKYLARK_TITLE_STATS_INTERVAL_FRAMES, SKYLARK_PRINT_SCENE_SUMMARY,\n"
                "  SKYLARK_PRINT_SCENE_BOUNDS, SKYLARK_BENCHMARK, SKYLARK_WARMUP_FRAMES,\n"
                "  SKYLARK_MAX_FRAMES, SKYLARK_MAX_SECONDS, SKYLARK_BENCHMARK_CSV,\n"
                "  SKYLARK_BENCHMARK_APPEND_CSV, SKYLARK_BENCHMARK_SUMMARY\n");
            SKPrintSampleControls(ESKSampleControlProfile::CadSceneTechniques);
        }

        FAppMat4 ComputeItemWorldMatrix(const FSkCadSceneDocument& Document, const FSkCadSceneDrawItem& Item, float ExplodeFactor)
        {
            if (ExplodeFactor <= 1e-4f)
            {
                return Item.WorldMatrix;
            }
            const float Distance = Document.SceneRadius * 0.22f * ExplodeFactor;
            const FAppVec3 Offset = AppVec3Scale(Item.ExplosionDirection, Distance);
            return AppMat4Multiply(AppMat4Translation(Offset.X, Offset.Y, Offset.Z), Item.WorldMatrix);
        }

        void BuildGpuBuffers(FAppState& App)
        {
            for (const uint64 GeometryKey : App.Document.GeometryKeys)
            {
                const FSKGeometryData* pGeometry = App.Document.GeometryRegistry.Find(GeometryKey);
                if (!pGeometry)
                {
                    continue;
                }

                FGeometryGpuBuffers GeometryGpu{};
                GeometryGpu.Sections.resize(pGeometry->Mesh.Sections.size());

                for (SIZE_T SectionIndex = 0; SectionIndex < pGeometry->Mesh.Sections.size(); ++SectionIndex)
                {
                    const FSKMeshSection& Section = pGeometry->Mesh.Sections[SectionIndex];
                    const auto& EdgeLists = pGeometry->Derived.SectionEdges[SectionIndex];
                    FSectionGpuBuffers& GpuSection = GeometryGpu.Sections[SectionIndex];

                    glGenVertexArrays(1, &GpuSection.Vao);
                    glBindVertexArray(GpuSection.Vao);

                    glGenBuffers(1, &GpuSection.Vbo);
                    glBindBuffer(GL_ARRAY_BUFFER, GpuSection.Vbo);
                    glBufferData(GL_ARRAY_BUFFER,
                        static_cast<GLsizeiptr>(Section.Vertices.size() * sizeof(FSKVertex)),
                        Section.Vertices.data(),
                        GL_STATIC_DRAW);

                    glEnableVertexAttribArray(0);
                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(FSKVertex), reinterpret_cast<void*>(offsetof(FSKVertex, Position)));
                    glEnableVertexAttribArray(1);
                    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(FSKVertex), reinterpret_cast<void*>(offsetof(FSKVertex, Normal)));

                    glGenBuffers(1, &GpuSection.EboSolid);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GpuSection.EboSolid);
                    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                        static_cast<GLsizeiptr>(Section.Indices.size() * sizeof(uint32)),
                        Section.Indices.data(),
                        GL_STATIC_DRAW);
                    GpuSection.SolidIndexCount = static_cast<GLsizei>(Section.Indices.size());

                    if (!EdgeLists.All.empty())
                    {
                        glGenBuffers(1, &GpuSection.EboAllEdges);
                        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GpuSection.EboAllEdges);
                        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                            static_cast<GLsizeiptr>(EdgeLists.All.size() * sizeof(uint32)),
                            EdgeLists.All.data(),
                            GL_STATIC_DRAW);
                        GpuSection.AllEdgeIndexCount = static_cast<GLsizei>(EdgeLists.All.size());
                    }

                    if (!EdgeLists.Boundary.empty())
                    {
                        glGenBuffers(1, &GpuSection.EboBoundaryEdges);
                        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GpuSection.EboBoundaryEdges);
                        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                            static_cast<GLsizeiptr>(EdgeLists.Boundary.size() * sizeof(uint32)),
                            EdgeLists.Boundary.data(),
                            GL_STATIC_DRAW);
                        GpuSection.BoundaryEdgeIndexCount = static_cast<GLsizei>(EdgeLists.Boundary.size());
                    }

                    if (!EdgeLists.Sharp.empty())
                    {
                        glGenBuffers(1, &GpuSection.EboSharpEdges);
                        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GpuSection.EboSharpEdges);
                        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                            static_cast<GLsizeiptr>(EdgeLists.Sharp.size() * sizeof(uint32)),
                            EdgeLists.Sharp.data(),
                            GL_STATIC_DRAW);
                        GpuSection.SharpEdgeIndexCount = static_cast<GLsizei>(EdgeLists.Sharp.size());
                    }

                    glBindVertexArray(0);
                    App.NumSectionsUploaded += 1;
                    App.NumTrianglesUploaded += Section.Indices.size() / 3;
                }

                App.GeometryGpu.emplace(GeometryKey, std::move(GeometryGpu));
            }
        }

        void DestroyGpuBuffers(FAppState& App)
        {
            for (auto& Pair : App.GeometryGpu)
            {
                for (FSectionGpuBuffers& Section : Pair.second.Sections)
                {
                    if (Section.EboSharpEdges) glDeleteBuffers(1, &Section.EboSharpEdges);
                    if (Section.EboBoundaryEdges) glDeleteBuffers(1, &Section.EboBoundaryEdges);
                    if (Section.EboAllEdges) glDeleteBuffers(1, &Section.EboAllEdges);
                    if (Section.EboSolid) glDeleteBuffers(1, &Section.EboSolid);
                    if (Section.Vbo) glDeleteBuffers(1, &Section.Vbo);
                    if (Section.Vao) glDeleteVertexArrays(1, &Section.Vao);
                }
            }
            App.GeometryGpu.clear();
        }

        void EnsurePickTargets(FAppState& App, int Width, int Height)
        {
            if (Width <= 0 || Height <= 0)
            {
                return;
            }
            if (App.Pick.Width == Width && App.Pick.Height == Height && App.Pick.Fbo != 0)
            {
                return;
            }

            if (App.Pick.DepthRenderbuffer) glDeleteRenderbuffers(1, &App.Pick.DepthRenderbuffer);
            if (App.Pick.ColorTexture) glDeleteTextures(1, &App.Pick.ColorTexture);
            if (App.Pick.Fbo) glDeleteFramebuffers(1, &App.Pick.Fbo);
            App.Pick = {};

            glGenFramebuffers(1, &App.Pick.Fbo);
            glBindFramebuffer(GL_FRAMEBUFFER, App.Pick.Fbo);

            glGenTextures(1, &App.Pick.ColorTexture);
            glBindTexture(GL_TEXTURE_2D, App.Pick.ColorTexture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, App.Pick.ColorTexture, 0);

            glGenRenderbuffers(1, &App.Pick.DepthRenderbuffer);
            glBindRenderbuffer(GL_RENDERBUFFER, App.Pick.DepthRenderbuffer);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Width, Height);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, App.Pick.DepthRenderbuffer);

            const GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
            if (Status != GL_FRAMEBUFFER_COMPLETE)
            {
                std::fprintf(stderr, "Pick framebuffer incomplete: 0x%x\n", Status);
            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            App.Pick.Width = Width;
            App.Pick.Height = Height;
        }

        void DrawEdgeSection(GLuint Program, const FLineUniforms& Uniforms, const FSectionGpuBuffers& Section, const FAppMat4& Model, const FAppMat4& View, const FAppMat4& Proj, EEdgeMode EdgeMode, const FAppVec4& Color)
        {
            GLuint Ebo = 0;
            GLsizei IndexCount = 0;
            switch (EdgeMode)
            {
            case EEdgeMode::Boundary:
                Ebo = Section.EboBoundaryEdges;
                IndexCount = Section.BoundaryEdgeIndexCount;
                break;
            case EEdgeMode::Sharp:
                Ebo = Section.EboSharpEdges;
                IndexCount = Section.SharpEdgeIndexCount;
                break;
            case EEdgeMode::All:
            default:
                Ebo = Section.EboAllEdges;
                IndexCount = Section.AllEdgeIndexCount;
                break;
            }
            if (!Ebo || IndexCount == 0)
            {
                return;
            }

            glUseProgram(Program);
            glUniformMatrix4fv(Uniforms.Model, 1, GL_FALSE, Model.M);
            glUniformMatrix4fv(Uniforms.View, 1, GL_FALSE, View.M);
            glUniformMatrix4fv(Uniforms.Proj, 1, GL_FALSE, Proj.M);
            glUniform4f(Uniforms.LineColor, Color.X, Color.Y, Color.Z, Color.W);
            glBindVertexArray(Section.Vao);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Ebo);
            glDrawElements(GL_LINES, IndexCount, GL_UNSIGNED_INT, nullptr);
        }

        void RenderDrawItems(FAppState& App, const FAppMat4& View, const FAppMat4& Proj, bool bPickPass)
        {
            const FAppVec3 CameraPos = GetCameraPosition(App);
            const bool bDrawEdges = !bPickPass && ((App.ViewMode == EViewMode::ShadedEdges) || (App.ViewMode == EViewMode::Wireframe) || (App.ViewMode == EViewMode::XRay));
            const bool bSolidPass = bPickPass || (App.ViewMode != EViewMode::Wireframe);
            const bool bObjectId = !bPickPass && App.ViewMode == EViewMode::ObjectId;
            const bool bNormals = !bPickPass && App.ViewMode == EViewMode::Normals;
            const bool bXRay = !bPickPass && App.ViewMode == EViewMode::XRay;

            glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(1.0f, 1.0f);
            glLineWidth(1.0f);

            for (const FSkCadSceneDrawItem& Item : App.Document.DrawItems)
            {
                auto GeometryIt = App.GeometryGpu.find(Item.GeometryKey);
                if (GeometryIt == App.GeometryGpu.end())
                {
                    continue;
                }
                const FGeometryGpuBuffers& GeometryGpu = GeometryIt->second;
                if (Item.SectionIndex < 0 || static_cast<SIZE_T>(Item.SectionIndex) >= GeometryGpu.Sections.size())
                {
                    continue;
                }
                const FSectionGpuBuffers& Section = GeometryGpu.Sections[static_cast<SIZE_T>(Item.SectionIndex)];
                const FAppMat4 Model = ComputeItemWorldMatrix(App.Document, Item, App.ExplodeFactor);

                if (bSolidPass)
                {
                    if (bPickPass)
                    {
                        const FAppVec4 PickColor = MakeIdEncodeColor(Item.StableId);
                        glUseProgram(App.PickProgram);
                        glUniformMatrix4fv(App.PickUniforms.Model, 1, GL_FALSE, Model.M);
                        glUniformMatrix4fv(App.PickUniforms.View, 1, GL_FALSE, View.M);
                        glUniformMatrix4fv(App.PickUniforms.Proj, 1, GL_FALSE, Proj.M);
                        glUniform4f(App.PickUniforms.ObjectColor, PickColor.X, PickColor.Y, PickColor.Z, PickColor.W);
                        glBindVertexArray(Section.Vao);
                        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Section.EboSolid);
                        glDrawElements(GL_TRIANGLES, Section.SolidIndexCount, GL_UNSIGNED_INT, nullptr);
                    }
                    else
                    {
                        const FAppVec4 BaseColor = bObjectId ? MakeStableColor(Item.StableId) : GetMaterialColor(App, Item.MaterialIndex);
                        const bool bSelected = (Item.StableId == App.SelectedStableId && App.SelectedStableId != 0);
                        glUseProgram(App.SolidProgram);
                        glUniformMatrix4fv(App.SolidUniforms.Model, 1, GL_FALSE, Model.M);
                        glUniformMatrix4fv(App.SolidUniforms.View, 1, GL_FALSE, View.M);
                        glUniformMatrix4fv(App.SolidUniforms.Proj, 1, GL_FALSE, Proj.M);
                        glUniform4f(App.SolidUniforms.BaseColor, BaseColor.X, BaseColor.Y, BaseColor.Z, BaseColor.W);
                        glUniform3f(App.SolidUniforms.CameraPos, CameraPos.X, CameraPos.Y, CameraPos.Z);
                        glUniform1i(App.SolidUniforms.ShadingMode, bObjectId ? 1 : (bNormals ? 2 : 0));
                        if (bSelected)
                        {
                            glUniform4f(App.SolidUniforms.SelectionTint, 0.95f, 0.55f, 0.12f, 0.35f);
                        }
                        else
                        {
                            glUniform4f(App.SolidUniforms.SelectionTint, 0.0f, 0.0f, 0.0f, 0.0f);
                        }
                        glBindVertexArray(Section.Vao);
                        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Section.EboSolid);
                        if (bXRay)
                        {
                            glEnable(GL_BLEND);
                            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                            glUniform4f(App.SolidUniforms.BaseColor, BaseColor.X, BaseColor.Y, BaseColor.Z, 0.34f);
                        }
                        glDrawElements(GL_TRIANGLES, Section.SolidIndexCount, GL_UNSIGNED_INT, nullptr);
                        if (bXRay)
                        {
                            glDisable(GL_BLEND);
                        }
                    }
                }

                if (bDrawEdges)
                {
                    if (App.ViewMode == EViewMode::Wireframe)
                    {
                        glDisable(GL_DEPTH_TEST);
                    }
                    else
                    {
                        glEnable(GL_DEPTH_TEST);
                    }
                    DrawEdgeSection(App.LineProgram, App.LineUniforms, Section, Model, View, Proj, App.EdgeMode, FAppVec4{0.08f, 0.08f, 0.08f, 1.0f});
                    if (Item.StableId == App.SelectedStableId && App.SelectedStableId != 0)
                    {
                        glLineWidth(2.5f);
                        DrawEdgeSection(App.LineProgram, App.LineUniforms, Section, Model, View, Proj, App.EdgeMode, FAppVec4{0.98f, 0.62f, 0.14f, 1.0f});
                        glLineWidth(1.0f);
                    }
                    glEnable(GL_DEPTH_TEST);
                }
            }

            glDisable(GL_POLYGON_OFFSET_FILL);
        }

        void RenderScene(FAppState& App, bool bPickPass)
        {
            int Width = 0;
            int Height = 0;
            glfwGetFramebufferSize(App.pWindow, &Width, &Height);
            if (Width <= 0 || Height <= 0)
            {
                return;
            }

            const FAppVec3 Eye = GetCameraPosition(App);
            const FAppMat4 View = AppMat4LookAt(Eye, App.Camera.Target, FAppVec3{0.0f, 0.0f, 1.0f});
            const FAppMat4 Proj = AppMat4Perspective(0.70f, static_cast<float>(Width) / static_cast<float>(Height), App.Document.SceneRadius * 0.002f, App.Document.SceneRadius * 8.0f);

            if (bPickPass)
            {
                EnsurePickTargets(App, Width, Height);
                glBindFramebuffer(GL_FRAMEBUFFER, App.Pick.Fbo);
                glViewport(0, 0, Width, Height);
                glDisable(GL_BLEND);
                glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                RenderDrawItems(App, View, Proj, true);
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                return;
            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glViewport(0, 0, Width, Height);
            glClearColor(0.92f, 0.94f, 0.97f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            RenderDrawItems(App, View, Proj, false);
        }

        void PerformPick(FAppState& App, double WindowX, double WindowY)
        {
            int WindowWidth = 0;
            int WindowHeight = 0;
            int FramebufferWidth = 0;
            int FramebufferHeight = 0;
            glfwGetWindowSize(App.pWindow, &WindowWidth, &WindowHeight);
            glfwGetFramebufferSize(App.pWindow, &FramebufferWidth, &FramebufferHeight);
            if (WindowWidth <= 0 || WindowHeight <= 0 || FramebufferWidth <= 0 || FramebufferHeight <= 0)
            {
                return;
            }

            const double ScaleX = static_cast<double>(FramebufferWidth) / static_cast<double>(WindowWidth);
            const double ScaleY = static_cast<double>(FramebufferHeight) / static_cast<double>(WindowHeight);
            const int PixelX = static_cast<int>(std::clamp(WindowX * ScaleX, 0.0, static_cast<double>(FramebufferWidth - 1)));
            const int PixelYFromTop = static_cast<int>(std::clamp(WindowY * ScaleY, 0.0, static_cast<double>(FramebufferHeight - 1)));
            const int PixelY = FramebufferHeight - 1 - PixelYFromTop;

            RenderScene(App, true);
            uint8 Pixel[4] = {0, 0, 0, 0};
            glBindFramebuffer(GL_FRAMEBUFFER, App.Pick.Fbo);
            glReadPixels(PixelX, PixelY, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, Pixel);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            App.SelectedStableId = DecodeStableId(Pixel);
            if (App.SelectedStableId != 0)
            {
                std::printf("Selected StableId: %u\n", App.SelectedStableId);
            }
        }

        void FitView(FAppState& App)
        {
            App.Camera.Target = App.Document.SceneCenter;
            App.Camera.Distance = App.Document.SceneRadius * 2.35f;
            App.Camera.YawRadians = 0.8f;
            App.Camera.PitchRadians = 0.45f;
        }

        void KeyCallback(GLFWwindow* pWindow, int Key, int Scancode, int Action, int Mods)
        {
            (void)pWindow; (void)Scancode; (void)Mods;
            if (!GApp || (Action != GLFW_PRESS && Action != GLFW_REPEAT))
            {
                return;
            }

            switch (Key)
            {
            case GLFW_KEY_1: GApp->ViewMode = EViewMode::Shaded; break;
            case GLFW_KEY_2: GApp->ViewMode = EViewMode::ShadedEdges; break;
            case GLFW_KEY_3: GApp->ViewMode = EViewMode::Wireframe; break;
            case GLFW_KEY_4: GApp->ViewMode = EViewMode::XRay; break;
            case GLFW_KEY_5: GApp->ViewMode = EViewMode::ObjectId; break;
            case GLFW_KEY_6: GApp->ViewMode = EViewMode::Normals; break;
            case GLFW_KEY_A: GApp->EdgeMode = EEdgeMode::All; break;
            case GLFW_KEY_B: GApp->EdgeMode = EEdgeMode::Boundary; break;
            case GLFW_KEY_C: GApp->EdgeMode = EEdgeMode::Sharp; break;
            case GLFW_KEY_E: GApp->ExplodeTarget = (GApp->ExplodeTarget < 0.5f) ? 1.0f : 0.0f; break;
            case GLFW_KEY_LEFT_BRACKET: GApp->ExplodeTarget = std::max(0.0f, GApp->ExplodeTarget - 0.1f); break;
            case GLFW_KEY_RIGHT_BRACKET: GApp->ExplodeTarget = std::min(1.75f, GApp->ExplodeTarget + 0.1f); break;
            case GLFW_KEY_F: FitView(*GApp); break;
            case GLFW_KEY_V:
                GApp->bDisableVsync = !GApp->bDisableVsync;
                glfwSwapInterval(GApp->bDisableVsync ? 0 : 1);
                std::printf("VSync: %s\n", GApp->bDisableVsync ? "OFF" : "ON");
                break;
            default:
                break;
            }
        }

        void MouseButtonCallback(GLFWwindow* pWindow, int Button, int Action, int Mods)
        {
            (void)pWindow; (void)Mods;
            if (!GApp)
            {
                return;
            }

            if (Button == GLFW_MOUSE_BUTTON_RIGHT)
            {
                GApp->Camera.bOrbiting = (Action == GLFW_PRESS);
                if (GApp->Camera.bOrbiting)
                {
                    glfwGetCursorPos(GApp->pWindow, &GApp->Camera.LastMouseX, &GApp->Camera.LastMouseY);
                }
            }
            else if (Button == GLFW_MOUSE_BUTTON_MIDDLE)
            {
                GApp->Camera.bPanning = (Action == GLFW_PRESS);
                if (GApp->Camera.bPanning)
                {
                    glfwGetCursorPos(GApp->pWindow, &GApp->Camera.LastMouseX, &GApp->Camera.LastMouseY);
                }
            }
            else if (Button == GLFW_MOUSE_BUTTON_LEFT)
            {
                if (Action == GLFW_PRESS)
                {
                    GApp->Camera.bLeftPressed = true;
                    GApp->Camera.bLeftDragExceeded = false;
                    glfwGetCursorPos(GApp->pWindow, &GApp->Camera.PressMouseX, &GApp->Camera.PressMouseY);
                    GApp->Camera.LastMouseX = GApp->Camera.PressMouseX;
                    GApp->Camera.LastMouseY = GApp->Camera.PressMouseY;
                }
                else if (Action == GLFW_RELEASE)
                {
                    GApp->Camera.bLeftPressed = false;
                    double X = 0.0;
                    double Y = 0.0;
                    glfwGetCursorPos(GApp->pWindow, &X, &Y);
                    if (!GApp->Camera.bLeftDragExceeded)
                    {
                        PerformPick(*GApp, X, Y);
                    }
                }
            }
        }

        void CursorPosCallback(GLFWwindow* pWindow, double X, double Y)
        {
            (void)pWindow;
            if (!GApp)
            {
                return;
            }

            const double DeltaX = X - GApp->Camera.LastMouseX;
            const double DeltaY = Y - GApp->Camera.LastMouseY;
            GApp->Camera.LastMouseX = X;
            GApp->Camera.LastMouseY = Y;

            if (GApp->Camera.bLeftPressed)
            {
                const double DragDx = X - GApp->Camera.PressMouseX;
                const double DragDy = Y - GApp->Camera.PressMouseY;
                if ((DragDx * DragDx + DragDy * DragDy) > 16.0)
                {
                    GApp->Camera.bLeftDragExceeded = true;
                }
            }

            if (GApp->Camera.bOrbiting)
            {
                const float OrbitScale = 0.0042f * GApp->CameraOptions.OrbitScale;
                const float PitchSign = GApp->CameraOptions.bInvertOrbitY ? 1.0f : -1.0f;
                GApp->Camera.YawRadians += static_cast<float>(DeltaX) * OrbitScale;
                GApp->Camera.PitchRadians += static_cast<float>(DeltaY) * OrbitScale * PitchSign;
                GApp->Camera.PitchRadians = std::clamp(GApp->Camera.PitchRadians, -1.4f, 1.4f);
            }
            else if (GApp->Camera.bPanning)
            {
                const float PanScale = GApp->Camera.Distance * 0.0016f * GApp->CameraOptions.PanScale;
                const FAppVec3 Right = GetCameraRight(*GApp);
                const FAppVec3 Up = GetCameraUp(*GApp);
                const FAppVec3 Offset = AppVec3Add(AppVec3Scale(Right, static_cast<float>(-DeltaX) * PanScale), AppVec3Scale(Up, static_cast<float>(DeltaY) * PanScale));
                GApp->Camera.Target = AppVec3Add(GApp->Camera.Target, Offset);
            }
        }

        void ScrollCallback(GLFWwindow* pWindow, double XOffset, double YOffset)
        {
            (void)pWindow; (void)XOffset;
            if (!GApp)
            {
                return;
            }
            const float BaseScale = (YOffset > 0.0) ? 0.90f : 1.12f;
            const float Scale = std::pow(BaseScale, GApp->CameraOptions.ZoomScale);
            GApp->Camera.Distance = std::max(GApp->Document.SceneRadius * 0.05f, GApp->Camera.Distance * Scale);
        }
    }
}


int main(int argc, char** argv)
{
    using namespace Skylark;
    using namespace Skylark::CadSceneTest;
    using FClock = std::chrono::steady_clock;

    Skylark::FSKSampleAppOptions LaunchOptions{};
    Skylark::FSKString LaunchWarnings;
    Skylark::SKParseBasicSampleAppOptions(argc, argv, LaunchOptions, &LaunchWarnings);
    if (!LaunchWarnings.empty())
    {
        std::fprintf(stderr, "%s\n", LaunchWarnings.c_str());
    }

    Skylark::FSKSampleCameraOptions CameraOptions{};
    Skylark::FSKString CameraWarnings;
    Skylark::SKParseSampleCameraOptions(argc, argv, CameraOptions, &CameraWarnings);
    if (!CameraWarnings.empty())
    {
        std::fprintf(stderr, "%s\n", CameraWarnings.c_str());
    }

    Skylark::FSKSampleRuntimeStatsOptions RuntimeStatsOptions{};
    Skylark::FSKString RuntimeStatWarnings;
    Skylark::SKParseSampleRuntimeStatsOptions(argc, argv, RuntimeStatsOptions, &RuntimeStatWarnings);
    if (!RuntimeStatWarnings.empty())
    {
        std::fprintf(stderr, "%s\n", RuntimeStatWarnings.c_str());
    }

    Skylark::FSKSampleBenchmarkOptions BenchmarkOptions{};
    Skylark::FSKString BenchmarkWarnings;
    Skylark::SKParseSampleBenchmarkOptions(argc, argv, BenchmarkOptions, &BenchmarkWarnings);
    if (!BenchmarkWarnings.empty())
    {
        std::fprintf(stderr, "%s\n", BenchmarkWarnings.c_str());
    }

    Skylark::FSKSampleRunReportOptions RunReportOptions{};
    Skylark::FSKString RunReportWarnings;
    Skylark::SKParseSampleRunReportOptions(argc, argv, RunReportOptions, &RunReportWarnings);
    if (!RunReportWarnings.empty())
    {
        std::fprintf(stderr, "%s\n", RunReportWarnings.c_str());
    }

    Skylark::FSKSampleSceneManifestOptions SceneManifestOptions{};
    Skylark::FSKString SceneManifestWarnings;
    Skylark::SKParseSampleSceneManifestOptions(argc, argv, SceneManifestOptions, &SceneManifestWarnings);
    if (!SceneManifestWarnings.empty())
    {
        std::fprintf(stderr, "%s\n", SceneManifestWarnings.c_str());
    }

    Skylark::FSKSampleOutputLayoutOptions OutputLayoutOptions{};
    Skylark::FSKString OutputLayoutWarnings;
    Skylark::SKParseSampleOutputLayoutOptions(argc, argv, OutputLayoutOptions, &OutputLayoutWarnings);
    if (!OutputLayoutWarnings.empty())
    {
        std::fprintf(stderr, "%s\n", OutputLayoutWarnings.c_str());
    }

    Skylark::FSKCadSceneDiagnosticsOptions DiagnosticsOptions{};
    Skylark::FSKString DiagnosticsWarnings;
    Skylark::SKParseCadSceneDiagnosticsOptions(argc, argv, DiagnosticsOptions, &DiagnosticsWarnings);
    if (!DiagnosticsWarnings.empty())
    {
        std::fprintf(stderr, "%s\n", DiagnosticsWarnings.c_str());
    }
    if (LaunchOptions.bShowHelp)
    {
        PrintCommandLineHelp();
        return EXIT_SUCCESS;
    }

if (SceneManifestOptions.ManifestPath.empty() == false)
    {
        std::vector<Skylark::FSKSampleSceneManifestEntry> ManifestEntries;
        Skylark::FSKString ManifestError;
        if (!Skylark::SKLoadSampleSceneManifest(SceneManifestOptions, ManifestEntries, &ManifestError))
        {
            std::fprintf(stderr, "%s\n", ManifestError.c_str());
            return EXIT_FAILURE;
        }
        if (SceneManifestOptions.bListScenesOnly)
        {
            Skylark::SKPrintSampleSceneManifestEntries(ManifestEntries);
            return EXIT_SUCCESS;
        }
        if (LaunchOptions.ScenePath.empty())
        {
            const std::filesystem::path ManifestScene = Skylark::SKResolveSampleSceneFromManifest(SceneManifestOptions, ManifestEntries, &ManifestError);
            if (ManifestScene.empty())
            {
                std::fprintf(stderr, "%s\n", ManifestError.c_str());
                return EXIT_FAILURE;
            }
            LaunchOptions.ScenePath = ManifestScene.string().c_str();
        }
    }

    const std::filesystem::path DefaultScenePath = ResolveDefaultScenePath(LaunchOptions);
    std::string SceneFileStorage = DefaultScenePath.string();
    const char* pSceneFile = SceneFileStorage.c_str();

    FAppState App{};
    App.LoadedScenePath = DefaultScenePath;
    App.WindowWidth = LaunchOptions.Width;
    App.WindowHeight = LaunchOptions.Height;
    App.bEnableVsync = LaunchOptions.bEnableVSync;
    App.CameraOptions = CameraOptions;
    App.RuntimeStatsOptions = RuntimeStatsOptions;
    App.BenchmarkOptions = BenchmarkOptions;
    App.RunReportOptions = RunReportOptions;
    App.DiagnosticsOptions = DiagnosticsOptions;
    App.SceneManifestOptions = SceneManifestOptions;
    App.OutputLayoutOptions = OutputLayoutOptions;
    Skylark::FSKString Error;

    const auto LoadBegin = FClock::now();
    if (!LoadCadSceneDocument(pSceneFile, App.Document, Error))
    {
        std::fprintf(stderr, "LoadCadSceneDocument failed: %s\n", Error.c_str());
        return EXIT_FAILURE;
    }
    const double LoadMs = std::chrono::duration<double, std::milli>(FClock::now() - LoadBegin).count();

    if (App.CameraOptions.bFitOnStartup)
    {
        FitView(App);
    }

    if (!glfwInit())
    {
        std::fprintf(stderr, "glfwInit failed.\n");
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if defined(__APPLE__)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

    const FSKString InitialTitle = SKBuildSampleWindowTitle(FSKSampleHostInfo{ "SkylarkCadSceneTechniques", "OpenGL", App.LoadedScenePath, LaunchOptions.Width, LaunchOptions.Height, LaunchOptions.bEnableVSync });
    App.pWindow = glfwCreateWindow(LaunchOptions.Width, LaunchOptions.Height, InitialTitle.c_str(), nullptr, nullptr);
    if (!App.pWindow)
    {
        std::fprintf(stderr, "glfwCreateWindow failed.\n");
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(App.pWindow);
    glfwSwapInterval(LaunchOptions.bEnableVSync ? 1 : 0);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::fprintf(stderr, "glewInit failed.\n");
        glfwDestroyWindow(App.pWindow);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    App.SolidProgram = CreateProgram(GSolidVs, GSolidFs);
    App.LineProgram = CreateProgram(GLineVs, GLineFs);
    App.PickProgram = CreateProgram(GPickVs, GPickFs);
    App.SolidUniforms = QuerySolidUniforms(App.SolidProgram);
    App.LineUniforms = QueryLineUniforms(App.LineProgram);
    App.PickUniforms = QueryPickUniforms(App.PickProgram);
    BuildGpuBuffers(App);

    GApp = &App;
    glfwSetKeyCallback(App.pWindow, KeyCallback);
    glfwSetMouseButtonCallback(App.pWindow, MouseButtonCallback);
    glfwSetCursorPosCallback(App.pWindow, CursorPosCallback);
    glfwSetScrollCallback(App.pWindow, ScrollCallback);

    SKPrintSampleLaunchSummary(FSKSampleHostInfo{ "SkylarkCadSceneTechniques", "OpenGL", DefaultScenePath, LaunchOptions.Width, LaunchOptions.Height, LaunchOptions.bEnableVSync });
    Skylark::FSKString OutputApplyWarnings;
    Skylark::SKApplySampleOutputLayoutDefaults(OutputLayoutOptions, "SkylarkCadSceneTechniques", "OpenGL", DefaultScenePath, App.BenchmarkOptions, App.RunReportOptions, &OutputApplyWarnings);
    if (!OutputApplyWarnings.empty())
    {
        std::fprintf(stderr, "%s\n", OutputApplyWarnings.c_str());
    }
    SKPrintSampleCameraOptionSummary(App.CameraOptions);
    SKPrintSampleRuntimeStatsOptionSummary(App.RuntimeStatsOptions);
    SKPrintSampleBenchmarkOptionSummary(App.BenchmarkOptions);
    SKPrintSampleRunReportOptionSummary(App.RunReportOptions);
    Skylark::SKPrintSampleSceneManifestOptionSummary(App.SceneManifestOptions);
    Skylark::SKPrintSampleOutputLayoutOptionSummary(App.OutputLayoutOptions);
    SKPrintCadSceneDiagnosticsOptionSummary(App.DiagnosticsOptions);
    std::printf("Scene file : %s\n", pSceneFile);
    std::printf("Load time  : %.2f ms\n", LoadMs);
    SKPrintCadSceneDocumentDiagnostics(App.Document, App.DiagnosticsOptions);
    if (App.CameraOptions.bPrintControls)
    {
        SKPrintSampleControls(ESKSampleControlProfile::CadSceneTechniques);
    }

    SKResetSampleRuntimeStats(App.RuntimeStats);
    SKResetSampleBenchmarkState(App.BenchmarkState);
    App.LastFrameSeconds = glfwGetTime();
    while (!glfwWindowShouldClose(App.pWindow))
    {
        glfwPollEvents();
        int TitleWidth = 0;
        int TitleHeight = 0;
        glfwGetFramebufferSize(App.pWindow, &TitleWidth, &TitleHeight);
        App.WindowWidth = (std::max)(1, TitleWidth);
        App.WindowHeight = (std::max)(1, TitleHeight);
        const double NowSeconds = glfwGetTime();
        const float DeltaSeconds = static_cast<float>(NowSeconds - App.LastFrameSeconds);
        App.LastFrameSeconds = NowSeconds;

        SKUpdateSampleRuntimeStats(App.RuntimeStats, App.RuntimeStatsOptions, DeltaSeconds);
        SKUpdateSampleBenchmarkState(App.BenchmarkState, App.BenchmarkOptions, DeltaSeconds);

        const float LerpAlpha = std::clamp(DeltaSeconds * 7.5f, 0.0f, 1.0f);
        App.ExplodeFactor += (App.ExplodeTarget - App.ExplodeFactor) * LerpAlpha;
        RenderScene(App, false);

        Skylark::FSKString StatsLine;
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
            glfwSetWindowShouldClose(App.pWindow, GLFW_TRUE);
        }

        glfwSwapBuffers(App.pWindow);
    }

    if (App.BenchmarkOptions.bPrintSummaryOnExit)
    {
        const Skylark::FSKString SummaryLine = SKBuildSampleBenchmarkSummaryLine(
            "SkylarkCadSceneTechniques",
            "OpenGL",
            App.LoadedScenePath,
            App.BenchmarkState,
            App.RuntimeStats);
        std::fprintf(stdout, "%s\n", SummaryLine.c_str());
    }
    if (!App.BenchmarkOptions.CsvPath.empty())
    {
        Skylark::FSKString CsvError;
        if (!SKWriteSampleBenchmarkSummaryCsv(
                std::filesystem::path(App.BenchmarkOptions.CsvPath),
                App.BenchmarkOptions.bAppendCsv,
                "SkylarkCadSceneTechniques",
                "OpenGL",
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
        Skylark::FSKString ReportError;
        std::filesystem::path WrittenPath;
        if (!SKWriteSampleRunReport(
                App.RunReportOptions,
                Skylark::FSKSampleRunReportInput{
                    "SkylarkCadSceneTechniques",
                    "OpenGL",
                    App.LoadedScenePath,
                    App.WindowWidth,
                    App.WindowHeight,
                    App.bEnableVsync,
                    &App.RuntimeStats,
                    &App.BenchmarkState,
                    &App.Document },
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

    if (App.Pick.DepthRenderbuffer) glDeleteRenderbuffers(1, &App.Pick.DepthRenderbuffer);
    if (App.Pick.ColorTexture) glDeleteTextures(1, &App.Pick.ColorTexture);
    if (App.Pick.Fbo) glDeleteFramebuffers(1, &App.Pick.Fbo);
    DestroyGpuBuffers(App);
    if (App.PickProgram) glDeleteProgram(App.PickProgram);
    if (App.SolidProgram) glDeleteProgram(App.SolidProgram);
    if (App.LineProgram) glDeleteProgram(App.LineProgram);

    glfwDestroyWindow(App.pWindow);
    glfwTerminate();
    return EXIT_SUCCESS;
}
