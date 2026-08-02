#include "Import/Stl/StlImporter.h"

#include "Core/MappedFile.h"
#include "Core/Stopwatch.h"
#include "Import/FormatDetector.h"

#include <algorithm>
#include <array>
#include <charconv>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <limits>
#include <sstream>

namespace Skylark::FormatWorkbench
{
    namespace
    {
        constexpr std::uint64_t BinaryStlHeaderBytes = 80;
        constexpr std::uint64_t BinaryStlCountBytes = 4;
        constexpr std::uint64_t BinaryStlTriangleBytes = 50;

        static float ReadFloat32LE(const std::uint8_t* Ptr)
        {
            float Value = 0.0f;
            std::memcpy(&Value, Ptr, sizeof(float));
            return Value;
        }

        static std::uint32_t ReadUInt32LE(const std::uint8_t* Ptr)
        {
            std::uint32_t Value = 0;
            std::memcpy(&Value, Ptr, sizeof(std::uint32_t));
            return Value;
        }

        static std::uint16_t ReadUInt16LE(const std::uint8_t* Ptr)
        {
            std::uint16_t Value = 0;
            std::memcpy(&Value, Ptr, sizeof(std::uint16_t));
            return Value;
        }

        static FwVec3f Sub(const FwVec3f& A, const FwVec3f& B)
        {
            return { A.X - B.X, A.Y - B.Y, A.Z - B.Z };
        }

        static FwVec3f Cross(const FwVec3f& A, const FwVec3f& B)
        {
            return { A.Y * B.Z - A.Z * B.Y, A.Z * B.X - A.X * B.Z, A.X * B.Y - A.Y * B.X };
        }

        static FwVec3f NormalizeOrDefault(FwVec3f N)
        {
            const float Length = std::sqrt(N.X * N.X + N.Y * N.Y + N.Z * N.Z);
            if (Length <= 1.0e-20f || !std::isfinite(Length))
                return { 0.0f, 0.0f, 1.0f };
            return { N.X / Length, N.Y / Length, N.Z / Length };
        }

        static bool IsInvalidNormal(const FwVec3f& N)
        {
            const float Length2 = N.X * N.X + N.Y * N.Y + N.Z * N.Z;
            return Length2 <= 1.0e-20f || !std::isfinite(Length2);
        }

        static std::uint32_t PackSnorm10(float Value)
        {
            const float Clamped = std::max(-1.0f, std::min(1.0f, Value));
            const int Packed = static_cast<int>(std::lrint(Clamped * 511.0f));
            return static_cast<std::uint32_t>(Packed) & 0x3FFu;
        }

        static std::uint32_t PackNormal1010102(const FwVec3f& Normal)
        {
            const FwVec3f N = NormalizeOrDefault(Normal);
            return PackSnorm10(N.X) | (PackSnorm10(N.Y) << 10u) | (PackSnorm10(N.Z) << 20u);
        }

        static FwVec3f ComputeFaceNormal(const FwVec3f& P0, const FwVec3f& P1, const FwVec3f& P2)
        {
            return NormalizeOrDefault(Cross(Sub(P1, P0), Sub(P2, P0)));
        }

        static bool IsBinaryStlBySize(const FwMappedFile& File, std::uint32_t& OutTriangles)
        {
            if (File.Size() < BinaryStlHeaderBytes + BinaryStlCountBytes)
                return false;
            OutTriangles = ReadUInt32LE(File.Data() + BinaryStlHeaderBytes);
            const std::uint64_t Expected = BinaryStlHeaderBytes + BinaryStlCountBytes + static_cast<std::uint64_t>(OutTriangles) * BinaryStlTriangleBytes;
            return Expected == File.Size();
        }

        static void ReserveBlock(FwTriangleMeshBlock& Block, std::uint32_t TriangleCount, bool StoreAttributes)
        {
            Block.TriangleCount = TriangleCount;
            Block.Positions.reserve(static_cast<std::size_t>(TriangleCount) * 9u);
            Block.PackedFaceNormals.reserve(TriangleCount);
            if (StoreAttributes)
                Block.AttributeBytes.reserve(TriangleCount);
        }

        static void AppendTriangle(FwTriangleMeshBlock& Block, const FwVec3f& Normal, const FwVec3f& P0, const FwVec3f& P1, const FwVec3f& P2, std::uint16_t Attribute, bool StoreAttributes)
        {
            const FwVec3f FinalNormal = IsInvalidNormal(Normal) ? ComputeFaceNormal(P0, P1, P2) : NormalizeOrDefault(Normal);
            Block.Positions.push_back(P0.X);
            Block.Positions.push_back(P0.Y);
            Block.Positions.push_back(P0.Z);
            Block.Positions.push_back(P1.X);
            Block.Positions.push_back(P1.Y);
            Block.Positions.push_back(P1.Z);
            Block.Positions.push_back(P2.X);
            Block.Positions.push_back(P2.Y);
            Block.Positions.push_back(P2.Z);
            Block.PackedFaceNormals.push_back(PackNormal1010102(FinalNormal));
            if (StoreAttributes)
                Block.AttributeBytes.push_back(Attribute);
            Block.Bounds.Encapsulate(P0);
            Block.Bounds.Encapsulate(P1);
            Block.Bounds.Encapsulate(P2);
        }

        static FwImportResult ImportBinaryStl(const std::filesystem::path& Path, FwMappedFile& File, std::uint32_t TriangleCount, const FwImportOptions& Options, const FwProgressCallback& Progress)
        {
            FwImportResult Result;
            Result.Document = std::make_unique<FwModelDocument>();
            Result.Document->SourcePath = Path;
            Result.Document->Format = EModelFileFormat::BinaryStl;
            Result.Document->SourceEncoding = "binary-little-endian";
            Result.Document->FileBytes = File.Size();
            Result.Report.Format = EModelFileFormat::BinaryStl;
            Result.Report.FileBytes = File.Size();

            const std::uint64_t TrianglesToRead = Options.MaxTriangles > 0 ? std::min<std::uint64_t>(TriangleCount, Options.MaxTriangles) : TriangleCount;
            if (TrianglesToRead < TriangleCount)
                Result.Report.AddWarning("Triangle count was capped by MaxTriangles option.");

            const std::uint32_t BlockLimit = std::max<std::uint32_t>(1u, Options.TrianglesPerBlock);
            const std::uint8_t* Cursor = File.Data() + BinaryStlHeaderBytes + BinaryStlCountBytes;

            std::uint64_t TrianglesRead = 0;
            while (TrianglesRead < TrianglesToRead)
            {
                const std::uint32_t CurrentBlockTriangles = static_cast<std::uint32_t>(std::min<std::uint64_t>(BlockLimit, TrianglesToRead - TrianglesRead));
                FwTriangleMeshBlock Block;
                Block.Name = "stl_block_" + std::to_string(Result.Document->MeshBlocks.size());
                Block.FirstTriangle = TrianglesRead;
                ReserveBlock(Block, CurrentBlockTriangles, Options.StoreAttributeBytes);

                for (std::uint32_t LocalTriangle = 0; LocalTriangle < CurrentBlockTriangles; ++LocalTriangle)
                {
                    const std::uint8_t* T = Cursor + (TrianglesRead + LocalTriangle) * BinaryStlTriangleBytes;
                    FwVec3f Normal{ ReadFloat32LE(T + 0), ReadFloat32LE(T + 4), ReadFloat32LE(T + 8) };
                    FwVec3f P0{ ReadFloat32LE(T + 12), ReadFloat32LE(T + 16), ReadFloat32LE(T + 20) };
                    FwVec3f P1{ ReadFloat32LE(T + 24), ReadFloat32LE(T + 28), ReadFloat32LE(T + 32) };
                    FwVec3f P2{ ReadFloat32LE(T + 36), ReadFloat32LE(T + 40), ReadFloat32LE(T + 44) };
                    const std::uint16_t Attribute = ReadUInt16LE(T + 48);
                    AppendTriangle(Block, Normal, P0, P1, P2, Attribute, Options.StoreAttributeBytes);
                }

                Result.Document->Bounds.Encapsulate(Block.Bounds);
                Result.Document->TotalTriangles += CurrentBlockTriangles;
                Result.Document->TotalVertices += static_cast<std::uint64_t>(CurrentBlockTriangles) * 3ull;
                Result.Document->MeshBlocks.push_back(std::move(Block));
                TrianglesRead += CurrentBlockTriangles;

                if (Progress)
                {
                    const double Percent = TrianglesToRead == 0 ? 100.0 : (static_cast<double>(TrianglesRead) * 100.0 / static_cast<double>(TrianglesToRead));
                    if (!Progress(Percent, "binary-stl-import"))
                    {
                        Result.Report.AddError("Import was cancelled by progress callback.");
                        return Result;
                    }
                }
            }

            Result.Report.Succeeded = true;
            Result.Report.Triangles = Result.Document->TotalTriangles;
            Result.Report.Vertices = Result.Document->TotalVertices;
            Result.Report.MeshBlocks = static_cast<std::uint64_t>(Result.Document->MeshBlocks.size());
            Result.Report.CompactCpuBytes = Result.Document->CpuBytes();
            Result.Report.AddInfo("Imported binary STL through compact triangle-stream path; no OCCT TopoDS_Face per facet was created.");
            return Result;
        }

        static bool ParseFacetNormal(const std::string& Line, FwVec3f& Normal)
        {
            std::istringstream Stream(Line);
            std::string Facet;
            std::string NormalWord;
            return static_cast<bool>(Stream >> Facet >> NormalWord >> Normal.X >> Normal.Y >> Normal.Z) && Facet == "facet" && NormalWord == "normal";
        }

        static bool ParseVertex(const std::string& Line, FwVec3f& Vertex)
        {
            std::istringstream Stream(Line);
            std::string Word;
            return static_cast<bool>(Stream >> Word >> Vertex.X >> Vertex.Y >> Vertex.Z) && Word == "vertex";
        }

        static FwImportResult ImportAsciiStl(const std::filesystem::path& Path, const FwImportOptions& Options, const FwProgressCallback& Progress)
        {
            FwImportResult Result;
            Result.Document = std::make_unique<FwModelDocument>();
            Result.Document->SourcePath = Path;
            Result.Document->Format = EModelFileFormat::AsciiStl;
            Result.Document->SourceEncoding = "ascii";
            Result.Report.Format = EModelFileFormat::AsciiStl;

            std::ifstream Input(Path);
            if (!Input)
            {
                Result.Report.AddError("Cannot open ASCII STL file.");
                return Result;
            }

            FwTriangleMeshBlock Block;
            Block.Name = "ascii_stl_block_0";
            ReserveBlock(Block, std::min<std::uint32_t>(Options.TrianglesPerBlock, 100000), Options.StoreAttributeBytes);

            std::string Line;
            FwVec3f Normal{};
            std::array<FwVec3f, 3> Vertices{};
            std::uint32_t VertexInFacet = 0;
            std::uint64_t ImportedTriangles = 0;
            while (std::getline(Input, Line))
            {
                const auto First = Line.find_first_not_of(" \t\r\n");
                if (First != std::string::npos)
                    Line.erase(0, First);

                FwVec3f Parsed{};
                if (ParseFacetNormal(Line, Parsed))
                {
                    Normal = Parsed;
                    VertexInFacet = 0;
                    continue;
                }

                if (ParseVertex(Line, Parsed))
                {
                    if (VertexInFacet < 3)
                        Vertices[VertexInFacet++] = Parsed;
                    if (VertexInFacet == 3)
                    {
                        if (Options.MaxTriangles > 0 && ImportedTriangles >= Options.MaxTriangles)
                            break;

                        if (Block.Positions.size() / 9u >= Options.TrianglesPerBlock)
                        {
                            Result.Document->Bounds.Encapsulate(Block.Bounds);
                            Result.Document->MeshBlocks.push_back(std::move(Block));
                            Block = FwTriangleMeshBlock{};
                            Block.Name = "ascii_stl_block_" + std::to_string(Result.Document->MeshBlocks.size());
                            Block.FirstTriangle = ImportedTriangles;
                            ReserveBlock(Block, Options.TrianglesPerBlock, Options.StoreAttributeBytes);
                        }
                        AppendTriangle(Block, Normal, Vertices[0], Vertices[1], Vertices[2], 0, Options.StoreAttributeBytes);
                        ++ImportedTriangles;
                        VertexInFacet = 0;
                        if (Progress && ImportedTriangles % 100000 == 0)
                            Progress(0.0, "ascii-stl-import");
                    }
                }
            }

            if (!Block.Positions.empty())
            {
                Block.TriangleCount = static_cast<std::uint32_t>(Block.Positions.size() / 9u);
                Result.Document->Bounds.Encapsulate(Block.Bounds);
                Result.Document->MeshBlocks.push_back(std::move(Block));
            }

            Result.Document->TotalTriangles = ImportedTriangles;
            Result.Document->TotalVertices = ImportedTriangles * 3ull;
            Result.Report.Succeeded = true;
            Result.Report.Triangles = Result.Document->TotalTriangles;
            Result.Report.Vertices = Result.Document->TotalVertices;
            Result.Report.MeshBlocks = static_cast<std::uint64_t>(Result.Document->MeshBlocks.size());
            Result.Report.CompactCpuBytes = Result.Document->CpuBytes();
            Result.Report.AddWarning("ASCII STL is supported for correctness. Commercial high-throughput benchmark must use binary STL.");
            return Result;
        }
    }

    bool FwStlImporter::CanImport(const std::filesystem::path& Path) const
    {
        const EModelFileFormat Format = DetectFormatByExtension(Path);
        return Format == EModelFileFormat::BinaryStl;
    }

    FwImportResult FwStlImporter::Import(const std::filesystem::path& Path, const FwImportOptions& Options, const FwProgressCallback& Progress) const
    {
        FwStopwatch Stopwatch;
        const FwProcessMemorySnapshot MemoryBefore = QueryProcessMemorySnapshot();
        FwImportResult Result;

        FwMappedFile File;
        std::string Error;
        if (!File.OpenReadOnly(Path, Error))
        {
            Result.Report.AddError(Error);
            return Result;
        }

        std::uint32_t BinaryTriangleCount = 0;
        if (IsBinaryStlBySize(File, BinaryTriangleCount))
        {
            Result = ImportBinaryStl(Path, File, BinaryTriangleCount, Options, Progress);
        }
        else
        {
            File.Close();
            Result = ImportAsciiStl(Path, Options, Progress);
            Result.Report.FileBytes = std::filesystem::file_size(Path);
        }

        Result.Report.MemoryBefore = MemoryBefore;
        Result.Report.MemoryAfter = QueryProcessMemorySnapshot();
        Result.Report.ImportMilliseconds = Stopwatch.ElapsedMilliseconds();
        return Result;
    }

    bool GenerateBinaryStlFile(const std::filesystem::path& Path, std::uint64_t TriangleCount, std::string& ErrorMessage)
    {
        std::ofstream Output(Path, std::ios::binary);
        if (!Output)
        {
            ErrorMessage = "Cannot create binary STL file.";
            return false;
        }

        char Header[80]{};
        const char* Name = "SkylarkFormatWorkbench generated binary STL";
        std::memcpy(Header, Name, std::min<std::size_t>(std::strlen(Name), sizeof(Header)));
        Output.write(Header, sizeof(Header));

        if (TriangleCount > static_cast<std::uint64_t>(std::numeric_limits<std::uint32_t>::max()))
        {
            ErrorMessage = "Binary STL triangle count exceeds uint32 limit.";
            return false;
        }
        const std::uint32_t Count32 = static_cast<std::uint32_t>(TriangleCount);
        Output.write(reinterpret_cast<const char*>(&Count32), sizeof(Count32));

        for (std::uint64_t I = 0; I < TriangleCount; ++I)
        {
            const float CellX = static_cast<float>(I % 10000u);
            const float CellY = static_cast<float>((I / 10000u) % 10000u);
            const float Normal[3] = { 0.0f, 0.0f, 1.0f };
            const float P0[3] = { CellX, CellY, 0.0f };
            const float P1[3] = { CellX + 1.0f, CellY, 0.0f };
            const float P2[3] = { CellX, CellY + 1.0f, 0.0f };
            const std::uint16_t Attribute = 0;
            Output.write(reinterpret_cast<const char*>(Normal), sizeof(Normal));
            Output.write(reinterpret_cast<const char*>(P0), sizeof(P0));
            Output.write(reinterpret_cast<const char*>(P1), sizeof(P1));
            Output.write(reinterpret_cast<const char*>(P2), sizeof(P2));
            Output.write(reinterpret_cast<const char*>(&Attribute), sizeof(Attribute));
        }
        return true;
    }
}
