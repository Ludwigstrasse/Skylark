#include "ModelIO/SKStlFastReader.h"

#include <fstream>
#include <cstring>
#include <sstream>

namespace Skylark::VolumeStudio
{
    namespace
    {
        constexpr uint64 StlHeaderBytes = 80;
        constexpr uint64 StlTriangleCountBytes = 4;
        constexpr uint64 StlFacetBytes = 50;

        static uint32 ReadU32LittleEndian(const unsigned char* Bytes)
        {
            return static_cast<uint32>(Bytes[0]) |
                   (static_cast<uint32>(Bytes[1]) << 8u) |
                   (static_cast<uint32>(Bytes[2]) << 16u) |
                   (static_cast<uint32>(Bytes[3]) << 24u);
        }

        static float ReadF32LittleEndian(const unsigned char* Bytes)
        {
            const uint32 Raw = ReadU32LittleEndian(Bytes);
            float Value = 0.0f;
            static_assert(sizeof(Value) == sizeof(Raw));
            std::memcpy(&Value, &Raw, sizeof(float));
            return Value;
        }

        static FSKVector3f ReadVector3(const unsigned char* Bytes)
        {
            return FSKVector3f(ReadF32LittleEndian(Bytes + 0), ReadF32LittleEndian(Bytes + 4), ReadF32LittleEndian(Bytes + 8));
        }

        static uint64 QueryFileBytes(const std::filesystem::path& FilePath)
        {
            std::error_code ErrorCode;
            const auto Size = std::filesystem::file_size(FilePath, ErrorCode);
            if (ErrorCode)
            {
                return 0;
            }
            return static_cast<uint64>(Size);
        }
    }

    bool FSKStlFastReader::IsBinaryStlLayout(uint64 FileBytes, uint32 TriangleCountFromHeader)
    {
        const uint64 ExpectedBytes = StlHeaderBytes + StlTriangleCountBytes + static_cast<uint64>(TriangleCountFromHeader) * StlFacetBytes;
        return FileBytes == ExpectedBytes;
    }

    FSKStlFastReaderOutput FSKStlFastReader::ReadBinaryFile(const std::filesystem::path& FilePath, const FSKStlFastReaderOptions& Options)
    {
        FSKStlFastReaderOutput Output;

        if (FilePath.empty())
        {
            Output.Result = FSKVolumeStudioResult::Error(ESKVolumeStudioStatus::InvalidArgument, "STL path is empty.");
            return Output;
        }

        if (!std::filesystem::exists(FilePath))
        {
            Output.Result = FSKVolumeStudioResult::Error(ESKVolumeStudioStatus::FileNotFound, "STL file was not found: " + FilePath.string());
            return Output;
        }

        Output.FileBytes = QueryFileBytes(FilePath);
        if (Output.FileBytes < StlHeaderBytes + StlTriangleCountBytes)
        {
            Output.Result = FSKVolumeStudioResult::Error(ESKVolumeStudioStatus::CorruptFile, "STL file is smaller than the binary STL header.");
            return Output;
        }

        std::ifstream Input(FilePath, std::ios::binary);
        if (!Input)
        {
            Output.Result = FSKVolumeStudioResult::Error(ESKVolumeStudioStatus::IoError, "Failed to open STL file: " + FilePath.string());
            return Output;
        }

        unsigned char Header[StlHeaderBytes]{};
        unsigned char CountBytes[StlTriangleCountBytes]{};
        Input.read(reinterpret_cast<char*>(Header), sizeof(Header));
        Input.read(reinterpret_cast<char*>(CountBytes), sizeof(CountBytes));
        if (!Input)
        {
            Output.Result = FSKVolumeStudioResult::Error(ESKVolumeStudioStatus::CorruptFile, "Failed to read binary STL header.");
            return Output;
        }

        const uint32 HeaderTriangleCount = ReadU32LittleEndian(CountBytes);
        if (!IsBinaryStlLayout(Output.FileBytes, HeaderTriangleCount))
        {
            std::ostringstream Message;
            Message << "Unsupported STL layout. M1 reader accepts strict binary STL only. File bytes=" << Output.FileBytes
                    << ", header triangles=" << HeaderTriangleCount << ".";
            Output.Result = FSKVolumeStudioResult::Error(ESKVolumeStudioStatus::UnsupportedFormat, Message.str());
            return Output;
        }

        if (HeaderTriangleCount == 0)
        {
            Output.Result = FSKVolumeStudioResult::Error(ESKVolumeStudioStatus::EmptyInput, "Binary STL contains zero triangles.");
            return Output;
        }

        if (static_cast<uint64>(HeaderTriangleCount) > Options.MaxTriangleCount)
        {
            std::ostringstream Message;
            Message << "STL triangle count exceeds configured resource limit. triangles=" << HeaderTriangleCount
                    << ", max=" << Options.MaxTriangleCount << ".";
            Output.Result = FSKVolumeStudioResult::Error(ESKVolumeStudioStatus::ResourceLimitExceeded, Message.str());
            return Output;
        }

        const uint64 ReserveCount = Options.ReserveTriangleCountHint > 0 ? Options.ReserveTriangleCountHint : HeaderTriangleCount;
        Output.Mesh.ReserveTriangles(ReserveCount, Options.bStoreFacetNormals);

        unsigned char Facet[StlFacetBytes]{};
        uint64 AcceptedTriangles = 0;
        for (uint32 TriangleIndex = 0; TriangleIndex < HeaderTriangleCount; ++TriangleIndex)
        {
            Input.read(reinterpret_cast<char*>(Facet), sizeof(Facet));
            if (!Input)
            {
                Output.Result = FSKVolumeStudioResult::Error(ESKVolumeStudioStatus::CorruptFile, "Unexpected EOF while reading STL facets.");
                return Output;
            }

            const FSKVector3f Normal = ReadVector3(Facet + 0);
            const FSKVector3f P0 = ReadVector3(Facet + 12);
            const FSKVector3f P1 = ReadVector3(Facet + 24);
            const FSKVector3f P2 = ReadVector3(Facet + 36);

            if (Options.bRejectNonFiniteCoordinates && (!SKVSIsFinite(P0) || !SKVSIsFinite(P1) || !SKVSIsFinite(P2)))
            {
                ++Output.RejectedTriangleCount;
                continue;
            }

            const FSKVector3f* StoredNormal = Options.bStoreFacetNormals ? &Normal : nullptr;
            if (Output.Mesh.AppendTriangle(P0, P1, P2, StoredNormal))
            {
                ++AcceptedTriangles;
            }
            else
            {
                ++Output.RejectedTriangleCount;
            }
        }

        if (AcceptedTriangles == 0)
        {
            Output.Result = FSKVolumeStudioResult::Error(ESKVolumeStudioStatus::EmptyInput, "No valid STL triangles were accepted.");
            return Output;
        }

        std::ostringstream Message;
        Message << "Loaded binary STL. triangles=" << AcceptedTriangles
                << ", rejected=" << Output.RejectedTriangleCount
                << ", meshBytes=" << Output.Mesh.EstimateMemoryBytes() << ".";
        Output.Result = FSKVolumeStudioResult::Ok(Message.str());
        return Output;
    }
}
