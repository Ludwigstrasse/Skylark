#pragma once

#include "SKCore/SKCoreMinimal.h"

#include <array>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <limits>

namespace Skylark::VolumeStudio
{
    enum class ESKVolumeStudioStatus : uint8
    {
        Ok = 0,
        InvalidArgument,
        FileNotFound,
        UnsupportedFormat,
        CorruptFile,
        IoError,
        ResourceLimitExceeded,
        EmptyInput,
        InternalError
    };

    struct FSKVolumeStudioResult
    {
        ESKVolumeStudioStatus Status = ESKVolumeStudioStatus::Ok;
        FSKString Message;

        bool IsOk() const { return Status == ESKVolumeStudioStatus::Ok; }

        static FSKVolumeStudioResult Ok(FSKString InMessage = {})
        {
            FSKVolumeStudioResult Result;
            Result.Status = ESKVolumeStudioStatus::Ok;
            Result.Message = std::move(InMessage);
            return Result;
        }

        static FSKVolumeStudioResult Error(ESKVolumeStudioStatus InStatus, FSKString InMessage)
        {
            FSKVolumeStudioResult Result;
            Result.Status = InStatus;
            Result.Message = std::move(InMessage);
            return Result;
        }
    };

    struct FSKVolumeStudioAabb
    {
        FSKVector3f Min{
            +std::numeric_limits<float>::infinity(),
            +std::numeric_limits<float>::infinity(),
            +std::numeric_limits<float>::infinity()};

        FSKVector3f Max{
            -std::numeric_limits<float>::infinity(),
            -std::numeric_limits<float>::infinity(),
            -std::numeric_limits<float>::infinity()};

        bool IsValid() const
        {
            return Min.X <= Max.X && Min.Y <= Max.Y && Min.Z <= Max.Z;
        }

        void Encapsulate(const FSKVector3f& Point)
        {
            Min.X = std::min(Min.X, Point.X);
            Min.Y = std::min(Min.Y, Point.Y);
            Min.Z = std::min(Min.Z, Point.Z);
            Max.X = std::max(Max.X, Point.X);
            Max.Y = std::max(Max.Y, Point.Y);
            Max.Z = std::max(Max.Z, Point.Z);
        }

        FSKVector3f Extent() const
        {
            if (!IsValid())
            {
                return FSKVector3f(0.0f, 0.0f, 0.0f);
            }
            return FSKVector3f(Max.X - Min.X, Max.Y - Min.Y, Max.Z - Min.Z);
        }
    };

    struct FSKVolumeStudioTriangleRange
    {
        uint64 FirstTriangle = 0;
        uint64 TriangleCount = 0;
    };

    struct FSKVolumeStudioBuildStats
    {
        uint64 InputTriangleCount = 0;
        uint64 OutputVertexCount = 0;
        uint64 OutputCellCount = 0;
        uint64 BoundaryCellCount = 0;
        uint64 SolidCellCount = 0;
        uint64 MemoryBytes = 0;
        double ElapsedSeconds = 0.0;
    };

    inline FSKVector3f SKVSMin(const FSKVector3f& A, const FSKVector3f& B)
    {
        return FSKVector3f(std::min(A.X, B.X), std::min(A.Y, B.Y), std::min(A.Z, B.Z));
    }

    inline FSKVector3f SKVSMax(const FSKVector3f& A, const FSKVector3f& B)
    {
        return FSKVector3f(std::max(A.X, B.X), std::max(A.Y, B.Y), std::max(A.Z, B.Z));
    }

    inline bool SKVSIsFinite(const FSKVector3f& P)
    {
        return std::isfinite(P.X) && std::isfinite(P.Y) && std::isfinite(P.Z);
    }
}
