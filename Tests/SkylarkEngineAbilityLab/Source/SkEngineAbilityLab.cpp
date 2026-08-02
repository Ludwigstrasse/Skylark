#include "../../SkylarkCadSceneTechniques/Source/SkCadSceneImporter.h"

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

namespace Skylark::EngineAbilityLab
{
    namespace
    {
        enum class EDisplayMode : int
        {
            Material = 0,
            ObjectId,
            SectionTint,
        };

        struct FGeometryBuffers
        {
            GLuint Vao = 0;
            GLuint Vbo = 0;
            GLuint Ebo = 0;
            GLsizei IndexCount = 0;
        };

        struct FCameraState
        {
            float Yaw = 0.75f;
            float Pitch = 0.45f;
            float Distance = 10.0f;
            bool bDragging = false;
            double LastX = 0.0;
            double LastY = 0.0;
        };

        struct FUniforms
        {
            GLint Model = -1;
            GLint View = -1;
            GLint Proj = -1;
            GLint BaseColor = -1;
            GLint CameraPos = -1;
        };

        struct FBoundsUniforms
        {
            GLint View = -1;
            GLint Proj = -1;
            GLint LineColor = -1;
        };

        struct FApp
        {
            GLFWwindow* Window = nullptr;
            FSkCadSceneDocument Document{};
            std::map<uint64, std::vector<FGeometryBuffers>> GpuMeshes;
            GLuint SolidProgram = 0;
            GLuint BoundsProgram = 0;
            FUniforms SolidUniforms{};
            FBoundsUniforms BoundsUniforms{};
            GLuint BoundsVao = 0;
            GLuint BoundsVbo = 0;
            FCameraState Camera{};
            EDisplayMode DisplayMode = EDisplayMode::Material;
            bool bShowBounds = true;
        };

        FApp* GApp = nullptr;

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
out vec4 FragColor;
void main()
{
    vec3 N = normalize(vWorldNormal);
    vec3 L = normalize(vec3(0.4, 0.75, 0.55));
    vec3 V = normalize(uCameraPos - vWorldPos);
    vec3 H = normalize(L + V);
    float NoL = max(dot(N, L), 0.0);
    float NoH = max(dot(N, H), 0.0);
    float Hemi = N.z * 0.5 + 0.5;
    vec3 Ambient = mix(vec3(0.2), vec3(0.72, 0.78, 0.88), Hemi) * 0.40;
    vec3 Lit = uBaseColor.rgb * (Ambient + NoL * 0.85) + vec3(1.0) * (pow(NoH, 48.0) * 0.18);
    Lit = pow(max(Lit, vec3(0.0)), vec3(1.0 / 2.2));
    FragColor = vec4(Lit, 1.0);
}
)GLSL";

        static const char* GBoundsVs = R"GLSL(
#version 330 core
layout(location = 0) in vec3 aPosition;
uniform mat4 uView;
uniform mat4 uProj;
void main()
{
    gl_Position = uProj * uView * vec4(aPosition, 1.0);
}
)GLSL";

        static const char* GBoundsFs = R"GLSL(
#version 330 core
uniform vec4 uLineColor;
out vec4 FragColor;
void main()
{
    FragColor = uLineColor;
}
)GLSL";

        GLuint Compile(GLenum Type, const char* Source)
        {
            GLuint Shader = glCreateShader(Type);
            glShaderSource(Shader, 1, &Source, nullptr);
            glCompileShader(Shader);
            return Shader;
        }

        GLuint Link(const char* Vs, const char* Fs)
        {
            GLuint V = Compile(GL_VERTEX_SHADER, Vs);
            GLuint F = Compile(GL_FRAGMENT_SHADER, Fs);
            GLuint P = glCreateProgram();
            glAttachShader(P, V);
            glAttachShader(P, F);
            glLinkProgram(P);
            glDeleteShader(V);
            glDeleteShader(F);
            return P;
        }

        FAppVec3 GetCameraPosition(const FSkCadSceneDocument& Document, const FCameraState& Camera)
        {
            const float CP = std::cos(Camera.Pitch);
            const float SP = std::sin(Camera.Pitch);
            const float CY = std::cos(Camera.Yaw);
            const float SY = std::sin(Camera.Yaw);
            return AppVec3Add(Document.SceneCenter, FAppVec3{ Camera.Distance * CP * CY, Camera.Distance * CP * SY, Camera.Distance * SP });
        }

        FAppVec4 MakeStableColor(uint32 StableId)
        {
            const uint8 R = static_cast<uint8>((StableId * 97u) & 0xFFu);
            const uint8 G = static_cast<uint8>((StableId * 57u) & 0xFFu);
            const uint8 B = static_cast<uint8>((StableId * 17u) & 0xFFu);
            return FAppVec4{ R / 255.0f, G / 255.0f, B / 255.0f, 1.0f };
        }

        FAppVec4 MakeSectionColor(int SectionIndex)
        {
            static const FAppVec4 Palette[] = {
                {0.82f, 0.38f, 0.32f, 1.0f},
                {0.24f, 0.61f, 0.83f, 1.0f},
                {0.34f, 0.71f, 0.41f, 1.0f},
                {0.88f, 0.69f, 0.25f, 1.0f},
                {0.67f, 0.45f, 0.86f, 1.0f},
            };
            return Palette[static_cast<size_t>(std::abs(SectionIndex)) % (sizeof(Palette) / sizeof(Palette[0]))];
        }

        std::filesystem::path ResolveScenePath(int argc, char** argv)
        {
            if (argc > 1)
            {
                return std::filesystem::path(argv[1]);
            }
            const auto Cwd = std::filesystem::current_path();
            const std::filesystem::path Candidates[] = {
                Cwd / "Tests" / "SkylarkEngineAbilityLab" / "Assets" / "geforce.csf.gz",
                Cwd / "Tests" / "SkylarkCadSceneTechniques" / "Assets" / "geforce.csf.gz",
                Cwd / "Debug" / "Tests" / "SkylarkEngineAbilityLab" / "Assets" / "geforce.csf.gz",
                Cwd / "Debug" / "Tests" / "SkylarkCadSceneTechniques" / "Assets" / "geforce.csf.gz"
            };
            for (const auto& Candidate : Candidates)
            {
                if (std::filesystem::exists(Candidate))
                {
                    return std::filesystem::absolute(Candidate);
                }
            }
            return Candidates[0];
        }

        void BuildMeshBuffers(FApp& App)
        {
            for (const uint64 GeometryKey : App.Document.GeometryKeys)
            {
                const FSKGeometryData* Geometry = App.Document.GeometryRegistry.Find(GeometryKey);
                if (!Geometry)
                {
                    continue;
                }
                std::vector<FGeometryBuffers> Sections;
                Sections.resize(Geometry->Mesh.Sections.size());
                for (size_t i = 0; i < Geometry->Mesh.Sections.size(); ++i)
                {
                    const FSKMeshSection& Section = Geometry->Mesh.Sections[i];
                    FGeometryBuffers& Buffers = Sections[i];
                    glGenVertexArrays(1, &Buffers.Vao);
                    glBindVertexArray(Buffers.Vao);
                    glGenBuffers(1, &Buffers.Vbo);
                    glBindBuffer(GL_ARRAY_BUFFER, Buffers.Vbo);
                    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(Section.Vertices.size() * sizeof(FSKVertex)), Section.Vertices.data(), GL_STATIC_DRAW);
                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(FSKVertex), reinterpret_cast<void*>(offsetof(FSKVertex, Position)));
                    glEnableVertexAttribArray(0);
                    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(FSKVertex), reinterpret_cast<void*>(offsetof(FSKVertex, Normal)));
                    glEnableVertexAttribArray(1);
                    glGenBuffers(1, &Buffers.Ebo);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers.Ebo);
                    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(Section.Indices.size() * sizeof(uint32)), Section.Indices.data(), GL_STATIC_DRAW);
                    Buffers.IndexCount = static_cast<GLsizei>(Section.Indices.size());
                    glBindVertexArray(0);
                }
                App.GpuMeshes.emplace(GeometryKey, std::move(Sections));
            }
        }

        void AppendBoundsLines(std::vector<FAppVec3>& Out, const FAppMat4& World, const FSKAabb& Box)
        {
            const FAppVec3 LocalMin{ Box.Min.X, Box.Min.Y, Box.Min.Z };
            const FAppVec3 LocalMax{ Box.Max.X, Box.Max.Y, Box.Max.Z };
            const FAppVec3 C[8] = {
                {LocalMin.X, LocalMin.Y, LocalMin.Z}, {LocalMax.X, LocalMin.Y, LocalMin.Z},
                {LocalMin.X, LocalMax.Y, LocalMin.Z}, {LocalMax.X, LocalMax.Y, LocalMin.Z},
                {LocalMin.X, LocalMin.Y, LocalMax.Z}, {LocalMax.X, LocalMin.Y, LocalMax.Z},
                {LocalMin.X, LocalMax.Y, LocalMax.Z}, {LocalMax.X, LocalMax.Y, LocalMax.Z},
            };
            auto TP = [&](const FAppVec3& P){ return FAppVec3{
                World.M[0]*P.X + World.M[4]*P.Y + World.M[8]*P.Z + World.M[12],
                World.M[1]*P.X + World.M[5]*P.Y + World.M[9]*P.Z + World.M[13],
                World.M[2]*P.X + World.M[6]*P.Y + World.M[10]*P.Z + World.M[14]}; };
            const int E[24] = {0,1, 1,3, 3,2, 2,0, 4,5, 5,7, 7,6, 6,4, 0,4, 1,5, 2,6, 3,7};
            for (int i = 0; i < 24; i += 2)
            {
                Out.push_back(TP(C[E[i]]));
                Out.push_back(TP(C[E[i + 1]]));
            }
        }

        void BuildBoundsBuffers(FApp& App)
        {
            std::vector<FAppVec3> Lines;
            Lines.reserve(App.Document.DrawItems.size() * 24);
            for (const FSkCadSceneDrawItem& Item : App.Document.DrawItems)
            {
                const FSKGeometryData* Geometry = App.Document.GeometryRegistry.Find(Item.GeometryKey);
                if (!Geometry)
                {
                    continue;
                }
                AppendBoundsLines(Lines, Item.WorldMatrix, Geometry->Bounds);
            }

            glGenVertexArrays(1, &App.BoundsVao);
            glBindVertexArray(App.BoundsVao);
            glGenBuffers(1, &App.BoundsVbo);
            glBindBuffer(GL_ARRAY_BUFFER, App.BoundsVbo);
            glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(Lines.size() * sizeof(FAppVec3)), Lines.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(FAppVec3), reinterpret_cast<void*>(0));
            glEnableVertexAttribArray(0);
            glBindVertexArray(0);
        }

        void Render(FApp& App)
        {
            int Width = 1;
            int Height = 1;
            glfwGetFramebufferSize(App.Window, &Width, &Height);
            glViewport(0, 0, Width, Height);
            glClearColor(0.95f, 0.96f, 0.98f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);

            const FAppVec3 CameraPos = GetCameraPosition(App.Document, App.Camera);
            const FAppMat4 View = AppMat4LookAt(CameraPos, App.Document.SceneCenter, FAppVec3{0,0,1});
            const float Aspect = static_cast<float>(Width) / static_cast<float>(std::max(1, Height));
            const FAppMat4 Proj = AppMat4Perspective(45.0f * 3.1415926f / 180.0f, Aspect, std::max(0.05f, App.Document.SceneRadius * 0.001f), App.Document.SceneRadius * 30.0f);

            glUseProgram(App.SolidProgram);
            glUniformMatrix4fv(App.SolidUniforms.View, 1, GL_FALSE, View.M);
            glUniformMatrix4fv(App.SolidUniforms.Proj, 1, GL_FALSE, Proj.M);
            glUniform3f(App.SolidUniforms.CameraPos, CameraPos.X, CameraPos.Y, CameraPos.Z);

            for (const FSkCadSceneDrawItem& Item : App.Document.DrawItems)
            {
                auto It = App.GpuMeshes.find(Item.GeometryKey);
                if (It == App.GpuMeshes.end()) continue;
                if (Item.SectionIndex < 0 || static_cast<size_t>(Item.SectionIndex) >= It->second.size()) continue;

                FAppVec4 Color = {0.7f,0.7f,0.74f,1.0f};
                if (App.DisplayMode == EDisplayMode::Material && Item.MaterialIndex >= 0 && static_cast<size_t>(Item.MaterialIndex) < App.Document.Materials.size())
                {
                    Color = App.Document.Materials[static_cast<size_t>(Item.MaterialIndex)].BaseColor;
                    Color.W = 1.0f;
                }
                else if (App.DisplayMode == EDisplayMode::ObjectId)
                {
                    Color = MakeStableColor(Item.StableId);
                }
                else if (App.DisplayMode == EDisplayMode::SectionTint)
                {
                    Color = MakeSectionColor(Item.SectionIndex);
                }

                glUniformMatrix4fv(App.SolidUniforms.Model, 1, GL_FALSE, Item.WorldMatrix.M);
                glUniform4f(App.SolidUniforms.BaseColor, Color.X, Color.Y, Color.Z, Color.W);
                const FGeometryBuffers& Geo = It->second[static_cast<size_t>(Item.SectionIndex)];
                glBindVertexArray(Geo.Vao);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Geo.Ebo);
                glDrawElements(GL_TRIANGLES, Geo.IndexCount, GL_UNSIGNED_INT, nullptr);
            }
            glBindVertexArray(0);

            if (App.bShowBounds)
            {
                glUseProgram(App.BoundsProgram);
                glUniformMatrix4fv(App.BoundsUniforms.View, 1, GL_FALSE, View.M);
                glUniformMatrix4fv(App.BoundsUniforms.Proj, 1, GL_FALSE, Proj.M);
                glUniform4f(App.BoundsUniforms.LineColor, 0.04f, 0.08f, 0.95f, 1.0f);
                glBindVertexArray(App.BoundsVao);
                glDisable(GL_DEPTH_TEST);
                glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(App.Document.DrawItems.size() * 24));
                glEnable(GL_DEPTH_TEST);
                glBindVertexArray(0);
            }
        }

        void OnKey(GLFWwindow*, int Key, int, int Action, int)
        {
            if (!GApp || Action != GLFW_PRESS) return;
            switch (Key)
            {
            case GLFW_KEY_1: GApp->DisplayMode = EDisplayMode::Material; break;
            case GLFW_KEY_2: GApp->DisplayMode = EDisplayMode::ObjectId; break;
            case GLFW_KEY_3: GApp->DisplayMode = EDisplayMode::SectionTint; break;
            case GLFW_KEY_B: GApp->bShowBounds = !GApp->bShowBounds; break;
            default: break;
            }
        }

        void OnMouseButton(GLFWwindow*, int Button, int Action, int)
        {
            if (!GApp || Button != GLFW_MOUSE_BUTTON_LEFT) return;
            GApp->Camera.bDragging = (Action == GLFW_PRESS);
            if (GApp->Camera.bDragging)
            {
                glfwGetCursorPos(GApp->Window, &GApp->Camera.LastX, &GApp->Camera.LastY);
            }
        }

        void OnCursor(GLFWwindow*, double X, double Y)
        {
            if (!GApp || !GApp->Camera.bDragging) return;
            const double DX = X - GApp->Camera.LastX;
            const double DY = Y - GApp->Camera.LastY;
            GApp->Camera.LastX = X;
            GApp->Camera.LastY = Y;
            GApp->Camera.Yaw += static_cast<float>(DX * 0.004);
            GApp->Camera.Pitch -= static_cast<float>(DY * 0.004);
            GApp->Camera.Pitch = std::max(-1.4f, std::min(1.4f, GApp->Camera.Pitch));
        }

        void OnScroll(GLFWwindow*, double, double YOffset)
        {
            if (!GApp) return;
            const float Scale = (YOffset > 0.0) ? 0.90f : 1.10f;
            GApp->Camera.Distance = std::max(GApp->Document.SceneRadius * 0.08f, GApp->Camera.Distance * Scale);
        }
    }
}

int main(int argc, char** argv)
{
    using namespace Skylark::EngineAbilityLab;
    using FClock = std::chrono::steady_clock;

    FApp App{};
    FSKString Error;
    const std::filesystem::path ScenePath = ResolveScenePath(argc, argv);
    const auto LoadBegin = FClock::now();
    if (!LoadCadSceneDocument(ScenePath.string().c_str(), App.Document, Error))
    {
        std::fprintf(stderr, "LoadCadSceneDocument failed: %s\n", Error.c_str());
        return EXIT_FAILURE;
    }
    const double LoadMs = std::chrono::duration<double, std::milli>(FClock::now() - LoadBegin).count();
    App.Camera.Distance = App.Document.SceneRadius * 2.2f;

    if (!glfwInit())
    {
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    App.Window = glfwCreateWindow(1600, 900, "Skylark Engine Ability Lab", nullptr, nullptr);
    if (!App.Window)
    {
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(App.Window);
    glfwSwapInterval(0);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        glfwDestroyWindow(App.Window);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    App.SolidProgram = Link(GSolidVs, GSolidFs);
    App.BoundsProgram = Link(GBoundsVs, GBoundsFs);
    App.SolidUniforms.Model = glGetUniformLocation(App.SolidProgram, "uModel");
    App.SolidUniforms.View = glGetUniformLocation(App.SolidProgram, "uView");
    App.SolidUniforms.Proj = glGetUniformLocation(App.SolidProgram, "uProj");
    App.SolidUniforms.BaseColor = glGetUniformLocation(App.SolidProgram, "uBaseColor");
    App.SolidUniforms.CameraPos = glGetUniformLocation(App.SolidProgram, "uCameraPos");
    App.BoundsUniforms.View = glGetUniformLocation(App.BoundsProgram, "uView");
    App.BoundsUniforms.Proj = glGetUniformLocation(App.BoundsProgram, "uProj");
    App.BoundsUniforms.LineColor = glGetUniformLocation(App.BoundsProgram, "uLineColor");

    BuildMeshBuffers(App);
    BuildBoundsBuffers(App);

    GApp = &App;
    glfwSetKeyCallback(App.Window, OnKey);
    glfwSetMouseButtonCallback(App.Window, OnMouseButton);
    glfwSetCursorPosCallback(App.Window, OnCursor);
    glfwSetScrollCallback(App.Window, OnScroll);

    std::printf("Skylark Engine Ability Lab\n");
    std::printf("Scene    : %s\n", ScenePath.string().c_str());
    std::printf("Load ms  : %.2f\n", LoadMs);
    std::printf("DrawItems: %zu\n", App.Document.DrawItems.size());
    std::printf("Controls : 1 material / 2 object-id / 3 section tint / B bounds\n");

    while (!glfwWindowShouldClose(App.Window))
    {
        glfwPollEvents();
        Render(App);
        glfwSwapBuffers(App.Window);
    }

    glfwDestroyWindow(App.Window);
    glfwTerminate();
    return EXIT_SUCCESS;
}
