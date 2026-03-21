#include "SkCadSceneImporter.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
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

        struct FSectionGpuBuffers
        {
            GLuint Vao = 0;
            GLuint Vbo = 0;
            GLuint EboSolid = 0;
            GLuint EboEdges = 0;
            GLuint EboBoundaryEdges = 0;
            GLuint EboSharpEdges = 0;
            GLsizei SolidIndexCount = 0;
            GLsizei EdgeIndexCount = 0;
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
            float PitchRadians = 0.5f;
            float Distance = 10.0f;
            bool bDragging = false;
            double LastMouseX = 0.0;
            double LastMouseY = 0.0;
        };

        struct FAppState
        {
            GLFWwindow* pWindow = nullptr;
            FSkCadSceneDocument Document{};
            std::map<uint64, FGeometryGpuBuffers> GeometryGpu;
            GLuint SolidProgram = 0;
            GLuint LineProgram = 0;
            EViewMode ViewMode = EViewMode::ShadedEdges;
            bool bExploded = false;
            bool bUseBoundaryEdges = false;
            bool bUseSharpEdgesOnly = false;
            FCameraState Camera{};
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

out vec4 FragColor;

void main()
{
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

    vec3 N = normalize(vWorldNormal);
    vec3 L = normalize(vec3(0.4, 0.8, 0.6));
    vec3 V = normalize(uCameraPos - vWorldPos);
    vec3 H = normalize(L + V);
    float diffuse = max(dot(N, L), 0.0);
    float specular = pow(max(dot(N, H), 0.0), 32.0);
    vec3 color = uBaseColor.rgb * (0.15 + diffuse * 0.85) + vec3(0.22) * specular;
    FragColor = vec4(color, uBaseColor.a);
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

        FAppVec3 GetCameraPosition(const FSkCadSceneDocument& Document, const FCameraState& Camera)
        {
            const float CP = std::cos(Camera.PitchRadians);
            const float SP = std::sin(Camera.PitchRadians);
            const float CY = std::cos(Camera.YawRadians);
            const float SY = std::sin(Camera.YawRadians);
            FAppVec3 Offset{};
            Offset.X = Camera.Distance * CP * CY;
            Offset.Y = Camera.Distance * CP * SY;
            Offset.Z = Camera.Distance * SP;
            return AppVec3Add(Document.SceneCenter, Offset);
        }

        FAppVec4 MakeColorFromId(uint32 StableId)
        {
            const uint8 R = static_cast<uint8>((StableId * 97u) & 0xFFu);
            const uint8 G = static_cast<uint8>((StableId * 57u) & 0xFFu);
            const uint8 B = static_cast<uint8>((StableId * 17u) & 0xFFu);
            return FAppVec4{ R / 255.0f, G / 255.0f, B / 255.0f, 1.0f };
        }

        FAppMat4 ComputeItemWorldMatrix(const FSkCadSceneDocument& Document, const FSkCadSceneDrawItem& Item, bool bExploded)
        {
            if (!bExploded)
            {
                return Item.WorldMatrix;
            }
            const FAppVec3 Delta = AppVec3Scale(Item.ExplosionDirection, Document.SceneRadius * 0.12f);
            return AppMat4Multiply(AppMat4Translation(Delta.X, Delta.Y, Delta.Z), Item.WorldMatrix);
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
                    const FSKWireframeData& Wireframe = pGeometry->Derived.SectionWireframes[SectionIndex];
                    const FSKEdgeIndexLists& EdgeLists = pGeometry->Derived.SectionEdges[SectionIndex];
                    FSectionGpuBuffers& GpuSection = GeometryGpu.Sections[SectionIndex];

                    glGenVertexArrays(1, &GpuSection.Vao);
                    glBindVertexArray(GpuSection.Vao);

                    glGenBuffers(1, &GpuSection.Vbo);
                    glBindBuffer(GL_ARRAY_BUFFER, GpuSection.Vbo);
                    glBufferData(GL_ARRAY_BUFFER,
                        static_cast<GLsizeiptr>(Section.Vertices.size() * sizeof(FSKVertex)),
                        Section.Vertices.data(),
                        GL_STATIC_DRAW);

                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(FSKVertex), reinterpret_cast<void*>(offsetof(FSKVertex, Position)));
                    glEnableVertexAttribArray(0);
                    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(FSKVertex), reinterpret_cast<void*>(offsetof(FSKVertex, Normal)));
                    glEnableVertexAttribArray(1);

                    glGenBuffers(1, &GpuSection.EboSolid);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GpuSection.EboSolid);
                    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                        static_cast<GLsizeiptr>(Section.Indices.size() * sizeof(uint32)),
                        Section.Indices.data(),
                        GL_STATIC_DRAW);
                    GpuSection.SolidIndexCount = static_cast<GLsizei>(Section.Indices.size());

                    glGenBuffers(1, &GpuSection.EboEdges);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GpuSection.EboEdges);
                    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                        static_cast<GLsizeiptr>(Wireframe.LineIndices.size() * sizeof(uint32)),
                        Wireframe.LineIndices.data(),
                        GL_STATIC_DRAW);
                    GpuSection.EdgeIndexCount = static_cast<GLsizei>(Wireframe.LineIndices.size());

                    glGenBuffers(1, &GpuSection.EboBoundaryEdges);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GpuSection.EboBoundaryEdges);
                    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                        static_cast<GLsizeiptr>(EdgeLists.Boundary.size() * sizeof(uint32)),
                        EdgeLists.Boundary.data(),
                        GL_STATIC_DRAW);
                    GpuSection.BoundaryEdgeIndexCount = static_cast<GLsizei>(EdgeLists.Boundary.size());

                    glGenBuffers(1, &GpuSection.EboSharpEdges);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GpuSection.EboSharpEdges);
                    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                        static_cast<GLsizeiptr>(EdgeLists.Sharp.size() * sizeof(uint32)),
                        EdgeLists.Sharp.data(),
                        GL_STATIC_DRAW);
                    GpuSection.SharpEdgeIndexCount = static_cast<GLsizei>(EdgeLists.Sharp.size());

                    glBindVertexArray(0);
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
                    if (Section.EboEdges) glDeleteBuffers(1, &Section.EboEdges);
                    if (Section.EboSolid) glDeleteBuffers(1, &Section.EboSolid);
                    if (Section.Vbo) glDeleteBuffers(1, &Section.Vbo);
                    if (Section.Vao) glDeleteVertexArrays(1, &Section.Vao);
                }
            }
            App.GeometryGpu.clear();
        }

        void DrawSolidSection(GLuint Program, const FSectionGpuBuffers& Section, const FAppMat4& Model, const FAppMat4& View, const FAppMat4& Proj, const FAppVec4& Color, const FAppVec3& CameraPos, int ShadingMode)
        {
            glUseProgram(Program);
            glUniformMatrix4fv(glGetUniformLocation(Program, "uModel"), 1, GL_FALSE, Model.M);
            glUniformMatrix4fv(glGetUniformLocation(Program, "uView"), 1, GL_FALSE, View.M);
            glUniformMatrix4fv(glGetUniformLocation(Program, "uProj"), 1, GL_FALSE, Proj.M);
            glUniform4f(glGetUniformLocation(Program, "uBaseColor"), Color.X, Color.Y, Color.Z, Color.W);
            glUniform3f(glGetUniformLocation(Program, "uCameraPos"), CameraPos.X, CameraPos.Y, CameraPos.Z);
            glUniform1i(glGetUniformLocation(Program, "uShadingMode"), ShadingMode);

            glBindVertexArray(Section.Vao);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Section.EboSolid);
            glDrawElements(GL_TRIANGLES, Section.SolidIndexCount, GL_UNSIGNED_INT, nullptr);
            glBindVertexArray(0);
        }

        void DrawEdgeSection(GLuint Program, const FSectionGpuBuffers& Section, const FAppMat4& Model, const FAppMat4& View, const FAppMat4& Proj, bool bBoundaryEdges, bool bSharpEdgesOnly)
        {
            glUseProgram(Program);
            glUniformMatrix4fv(glGetUniformLocation(Program, "uModel"), 1, GL_FALSE, Model.M);
            glUniformMatrix4fv(glGetUniformLocation(Program, "uView"), 1, GL_FALSE, View.M);
            glUniformMatrix4fv(glGetUniformLocation(Program, "uProj"), 1, GL_FALSE, Proj.M);
            glUniform4f(glGetUniformLocation(Program, "uLineColor"), 0.05f, 0.05f, 0.05f, 1.0f);

            GLuint Ebo = Section.EboEdges;
            GLsizei IndexCount = Section.EdgeIndexCount;
            if (bBoundaryEdges)
            {
                Ebo = Section.EboBoundaryEdges;
                IndexCount = Section.BoundaryEdgeIndexCount;
            }
            else if (bSharpEdgesOnly)
            {
                Ebo = Section.EboSharpEdges;
                IndexCount = Section.SharpEdgeIndexCount;
            }

            if (IndexCount <= 0)
            {
                return;
            }

            glBindVertexArray(Section.Vao);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Ebo);
            glDrawElements(GL_LINES, IndexCount, GL_UNSIGNED_INT, nullptr);
            glBindVertexArray(0);
        }

        void RenderScene(FAppState& App)
        {
            int Width = 1;
            int Height = 1;
            glfwGetFramebufferSize(App.pWindow, &Width, &Height);
            glViewport(0, 0, Width, Height);
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            glClearColor(0.94f, 0.95f, 0.97f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            const FAppVec3 CameraPos = GetCameraPosition(App.Document, App.Camera);
            const FAppMat4 View = AppMat4LookAt(CameraPos, App.Document.SceneCenter, FAppVec3{ 0.0f, 0.0f, 1.0f });
            const float Aspect = static_cast<float>(Width) / static_cast<float>(std::max(1, Height));
            const FAppMat4 Proj = AppMat4Perspective(45.0f * 3.1415926f / 180.0f, Aspect, std::max(0.01f, App.Document.SceneRadius * 0.001f), App.Document.SceneRadius * 20.0f);

            const bool bDrawEdges = (App.ViewMode == EViewMode::ShadedEdges) || (App.ViewMode == EViewMode::Wireframe) || (App.ViewMode == EViewMode::XRay);
            const bool bSolidPass = App.ViewMode != EViewMode::Wireframe;
            const bool bObjectId = App.ViewMode == EViewMode::ObjectId;
            const bool bNormals = App.ViewMode == EViewMode::Normals;
            const bool bXRay = App.ViewMode == EViewMode::XRay;

            if (bXRay)
            {
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            }
            else
            {
                glDisable(GL_BLEND);
            }

            for (const FSkCadSceneDrawItem& Item : App.Document.DrawItems)
            {
                auto GeometryIt = App.GeometryGpu.find(Item.GeometryKey);
                if (GeometryIt == App.GeometryGpu.end())
                {
                    continue;
                }
                if (Item.SectionIndex < 0 || static_cast<SIZE_T>(Item.SectionIndex) >= GeometryIt->second.Sections.size())
                {
                    continue;
                }

                const FSectionGpuBuffers& Section = GeometryIt->second.Sections[static_cast<SIZE_T>(Item.SectionIndex)];
                const FAppMat4 Model = ComputeItemWorldMatrix(App.Document, Item, App.bExploded);

                FAppVec4 BaseColor = MakeColorFromId(Item.StableId);
                if (Item.MaterialIndex >= 0 && static_cast<SIZE_T>(Item.MaterialIndex) < App.Document.Materials.size())
                {
                    BaseColor = App.Document.Materials[static_cast<SIZE_T>(Item.MaterialIndex)].BaseColor;
                    BaseColor.W = 1.0f;
                }
                if (bXRay)
                {
                    BaseColor.W = 0.32f;
                }

                if (bSolidPass)
                {
                    const int ShadingMode = bObjectId ? 1 : (bNormals ? 2 : 0);
                    DrawSolidSection(App.SolidProgram, Section, Model, View, Proj, BaseColor, CameraPos, ShadingMode);
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
                    glLineWidth(1.0f);
                    DrawEdgeSection(App.LineProgram, Section, Model, View, Proj, App.bUseBoundaryEdges, App.bUseSharpEdgesOnly);
                    glEnable(GL_DEPTH_TEST);
                }
            }
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
            case GLFW_KEY_E: GApp->bExploded = !GApp->bExploded; break;
            case GLFW_KEY_B:
                GApp->bUseBoundaryEdges = !GApp->bUseBoundaryEdges;
                if (GApp->bUseBoundaryEdges)
                {
                    GApp->bUseSharpEdgesOnly = false;
                }
                break;
            case GLFW_KEY_C:
                GApp->bUseSharpEdgesOnly = !GApp->bUseSharpEdgesOnly;
                if (GApp->bUseSharpEdgesOnly)
                {
                    GApp->bUseBoundaryEdges = false;
                }
                break;
            default:
                break;
            }
        }

        void MouseButtonCallback(GLFWwindow* pWindow, int Button, int Action, int Mods)
        {
            (void)pWindow; (void)Mods;
            if (!GApp || Button != GLFW_MOUSE_BUTTON_LEFT)
            {
                return;
            }
            GApp->Camera.bDragging = (Action == GLFW_PRESS);
            if (GApp->Camera.bDragging)
            {
                glfwGetCursorPos(GApp->pWindow, &GApp->Camera.LastMouseX, &GApp->Camera.LastMouseY);
            }
        }

        void CursorPosCallback(GLFWwindow* pWindow, double X, double Y)
        {
            (void)pWindow;
            if (!GApp || !GApp->Camera.bDragging)
            {
                return;
            }

            const double DeltaX = X - GApp->Camera.LastMouseX;
            const double DeltaY = Y - GApp->Camera.LastMouseY;
            GApp->Camera.LastMouseX = X;
            GApp->Camera.LastMouseY = Y;

            GApp->Camera.YawRadians += static_cast<float>(DeltaX * 0.005);
            GApp->Camera.PitchRadians -= static_cast<float>(DeltaY * 0.005);
            GApp->Camera.PitchRadians = std::max(-1.4f, std::min(1.4f, GApp->Camera.PitchRadians));
        }

        void ScrollCallback(GLFWwindow* pWindow, double XOffset, double YOffset)
        {
            (void)pWindow; (void)XOffset;
            if (!GApp)
            {
                return;
            }
            const float Scale = (YOffset > 0.0) ? 0.9f : 1.1f;
            GApp->Camera.Distance = std::max(GApp->Document.SceneRadius * 0.1f, GApp->Camera.Distance * Scale);
        }
    }
}

int main(int argc, char** argv)
{
    using namespace Skylark::CadSceneTest;

    const char* pSceneFile = (argc > 1) ? argv[1] : "Tests/SkylarkCadSceneTechniques/Assets/geforce.csf.gz";

    FAppState App{};
    FSKString Error;
    if (!LoadCadSceneDocument(pSceneFile, App.Document, Error))
    {
        std::fprintf(stderr, "LoadCadSceneDocument failed: %s\n", Error.c_str());
        return EXIT_FAILURE;
    }
    App.Camera.Distance = App.Document.SceneRadius * 3.0f;

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

    App.pWindow = glfwCreateWindow(1600, 900, "Skylark Cad Scene Techniques", nullptr, nullptr);
    if (!App.pWindow)
    {
        std::fprintf(stderr, "glfwCreateWindow failed.\n");
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(App.pWindow);
    glfwSwapInterval(1);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::fprintf(stderr, "glewInit failed.\n");
        glfwDestroyWindow(App.pWindow);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    App.SolidProgram = CreateProgram(GSolidVs, GSolidFs);
    App.LineProgram = CreateProgram(GLineVs, GLineFs);
    BuildGpuBuffers(App);

    GApp = &App;
    glfwSetKeyCallback(App.pWindow, KeyCallback);
    glfwSetMouseButtonCallback(App.pWindow, MouseButtonCallback);
    glfwSetCursorPosCallback(App.pWindow, CursorPosCallback);
    glfwSetScrollCallback(App.pWindow, ScrollCallback);

    std::printf("Controls:\n");
    std::printf("  1 shaded\n");
    std::printf("  2 shaded + edges\n");
    std::printf("  3 wireframe\n");
    std::printf("  4 xray\n");
    std::printf("  5 object-id color\n");
    std::printf("  6 normal visualization\n");
    std::printf("  E exploded view\n");
    std::printf("  B boundary edges\n");
    std::printf("  C sharp edges\n");
    std::printf("  Mouse drag orbit, wheel zoom\n");

    while (!glfwWindowShouldClose(App.pWindow))
    {
        glfwPollEvents();
        RenderScene(App);
        glfwSwapBuffers(App.pWindow);
    }

    DestroyGpuBuffers(App);
    if (App.SolidProgram) glDeleteProgram(App.SolidProgram);
    if (App.LineProgram) glDeleteProgram(App.LineProgram);

    glfwDestroyWindow(App.pWindow);
    glfwTerminate();
    return EXIT_SUCCESS;
}
