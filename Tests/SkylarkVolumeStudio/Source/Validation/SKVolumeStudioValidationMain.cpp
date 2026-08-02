#include "ModelIO/SKStlFastReader.h"
#include "SkylarkBridge/SKVolumeStudioSkylarkBridge.h"
#include "Voxel/SKHexahedralMesh.h"
#include "Voxel/SKVolumeVoxelGrid.h"

#include <array>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

namespace
{
    using Skylark::FSKVector3f;

    struct FTriangle
    {
        FSKVector3f P0;
        FSKVector3f P1;
        FSKVector3f P2;
    };

    static void WriteFloatLE(std::ofstream& Output, float Value)
    {
        static_assert(sizeof(float) == 4);
        std::uint32_t Raw = 0;
        std::memcpy(&Raw, &Value, sizeof(float));
        unsigned char Bytes[4] = {
            static_cast<unsigned char>((Raw >> 0u) & 0xFFu),
            static_cast<unsigned char>((Raw >> 8u) & 0xFFu),
            static_cast<unsigned char>((Raw >> 16u) & 0xFFu),
            static_cast<unsigned char>((Raw >> 24u) & 0xFFu)};
        Output.write(reinterpret_cast<const char*>(Bytes), sizeof(Bytes));
    }

    static void WriteUInt32LE(std::ofstream& Output, std::uint32_t Value)
    {
        unsigned char Bytes[4] = {
            static_cast<unsigned char>((Value >> 0u) & 0xFFu),
            static_cast<unsigned char>((Value >> 8u) & 0xFFu),
            static_cast<unsigned char>((Value >> 16u) & 0xFFu),
            static_cast<unsigned char>((Value >> 24u) & 0xFFu)};
        Output.write(reinterpret_cast<const char*>(Bytes), sizeof(Bytes));
    }

    static void WriteVector(std::ofstream& Output, const FSKVector3f& P)
    {
        WriteFloatLE(Output, P.X);
        WriteFloatLE(Output, P.Y);
        WriteFloatLE(Output, P.Z);
    }

    static bool WriteCubeBinaryStl(const std::filesystem::path& FilePath)
    {
        const FSKVector3f V000(0.0f, 0.0f, 0.0f);
        const FSKVector3f V100(1.0f, 0.0f, 0.0f);
        const FSKVector3f V110(1.0f, 1.0f, 0.0f);
        const FSKVector3f V010(0.0f, 1.0f, 0.0f);
        const FSKVector3f V001(0.0f, 0.0f, 1.0f);
        const FSKVector3f V101(1.0f, 0.0f, 1.0f);
        const FSKVector3f V111(1.0f, 1.0f, 1.0f);
        const FSKVector3f V011(0.0f, 1.0f, 1.0f);

        const std::vector<FTriangle> Triangles = {
            {V000, V100, V110}, {V000, V110, V010},
            {V001, V011, V111}, {V001, V111, V101},
            {V000, V001, V101}, {V000, V101, V100},
            {V100, V101, V111}, {V100, V111, V110},
            {V110, V111, V011}, {V110, V011, V010},
            {V010, V011, V001}, {V010, V001, V000}};

        std::ofstream Output(FilePath, std::ios::binary);
        if (!Output)
        {
            return false;
        }

        std::array<char, 80> Header{};
        const char* HeaderText = "SkylarkVolumeStudio validation cube";
        std::memcpy(Header.data(), HeaderText, std::strlen(HeaderText));
        Output.write(Header.data(), static_cast<std::streamsize>(Header.size()));
        WriteUInt32LE(Output, static_cast<std::uint32_t>(Triangles.size()));

        for (const FTriangle& Triangle : Triangles)
        {
            WriteVector(Output, FSKVector3f(0.0f, 0.0f, 1.0f));
            WriteVector(Output, Triangle.P0);
            WriteVector(Output, Triangle.P1);
            WriteVector(Output, Triangle.P2);
            const std::uint16_t Attribute = 0;
            Output.write(reinterpret_cast<const char*>(&Attribute), sizeof(Attribute));
        }

        return static_cast<bool>(Output);
    }

    static int Fail(const std::string& Message)
    {
        std::cerr << "[FAIL] " << Message << std::endl;
        return 1;
    }
}

int main()
{
    using namespace Skylark::VolumeStudio;

    const std::filesystem::path WorkDir = std::filesystem::temp_directory_path() / "SkylarkVolumeStudioValidation";
    std::error_code Ec;
    std::filesystem::create_directories(WorkDir, Ec);
    const std::filesystem::path CubeStlPath = WorkDir / "closed_cube_binary.stl";

    if (!WriteCubeBinaryStl(CubeStlPath))
    {
        return Fail("Failed to write validation STL file.");
    }

    FSKStlFastReaderOptions ReaderOptions;
    ReaderOptions.bStoreFacetNormals = false;
    const FSKStlFastReaderOutput ReadOutput = FSKStlFastReader::ReadBinaryFile(CubeStlPath, ReaderOptions);
    if (!ReadOutput.Result.IsOk())
    {
        return Fail(ReadOutput.Result.Message);
    }
    if (ReadOutput.Mesh.GetTriangleCount() != 12)
    {
        return Fail("Binary STL reader returned unexpected triangle count.");
    }

    FSKVoxelBuildSettings VoxelSettings;
    VoxelSettings.MaxResolutionPerAxis = 8;
    const FSKVoxelizationOutput VoxelOutput = FSKSurfaceVoxelizer::BuildConservativeVoxelGrid(ReadOutput.Mesh, VoxelSettings);
    if (!VoxelOutput.Result.IsOk())
    {
        return Fail(VoxelOutput.Result.Message);
    }
    if (VoxelOutput.Stats.BoundaryCellCount == 0 || VoxelOutput.Stats.OutputCellCount == 0)
    {
        return Fail("Voxelization produced no occupied cells.");
    }

    const FSKHexahedralMeshBuildOutput HexOutput = FSKHexahedralMeshBuilder::BuildFromOccupiedVoxels(VoxelOutput.Grid);
    if (!HexOutput.Result.IsOk())
    {
        return Fail(HexOutput.Result.Message);
    }
    if (HexOutput.Mesh.GetCellCount() != VoxelOutput.Stats.OutputCellCount || HexOutput.Mesh.GetNodeCount() == 0)
    {
        return Fail("Hexahedral mesh statistics do not match occupied voxel grid.");
    }

    Skylark::FSKMeshData PreviewMesh;
    FSKVolumeStudioPreviewMeshOptions PreviewOptions;
    PreviewOptions.MaxTriangles = 128;
    const FSKVolumeStudioResult PreviewResult = FSKVolumeStudioSkylarkBridge::BuildSurfacePreviewMesh(ReadOutput.Mesh, PreviewOptions, PreviewMesh);
    if (!PreviewResult.IsOk() || PreviewMesh.Sections.empty() || PreviewMesh.Sections[0].Vertices.size() != 36)
    {
        return Fail("Skylark preview mesh bridge validation failed.");
    }

    std::cout << "[OK] SkylarkVolumeStudio validation passed." << std::endl;
    std::cout << "Loaded triangles: " << ReadOutput.Mesh.GetTriangleCount() << std::endl;
    std::cout << "Surface mesh bytes: " << ReadOutput.Mesh.EstimateMemoryBytes() << std::endl;
    std::cout << "Voxel dims: " << VoxelOutput.Grid.DimX << " x " << VoxelOutput.Grid.DimY << " x " << VoxelOutput.Grid.DimZ << std::endl;
    std::cout << "Boundary cells: " << VoxelOutput.Stats.BoundaryCellCount << std::endl;
    std::cout << "Solid cells: " << VoxelOutput.Stats.SolidCellCount << std::endl;
    std::cout << "Hex nodes: " << HexOutput.Mesh.GetNodeCount() << std::endl;
    std::cout << "Hex cells: " << HexOutput.Mesh.GetCellCount() << std::endl;
    std::cout << "Preview vertices: " << PreviewMesh.Sections[0].Vertices.size() << std::endl;
    return 0;
}
