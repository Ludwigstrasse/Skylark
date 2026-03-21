#pragma once
#include "SKCore/SKCoreMinimal.h"
#include "SKCAD/SKCadTypes.h"
#include "SKLineRender/SKLineTypes.h"
#include "SKGeometry/SKGeometryTypes.h"

namespace Skylark
{
	struct FSKHLRRequest
	{
		FSKMatrix4f ViewProj = FSKMatrix4f::Identity();
		uint64 GeometryKey = 0;
		FSKTopoId OwnerId{};
	};

	struct FSKHLRResult
	{
		TArray<FSKLineSegment> Visible;
		TArray<FSKLineSegment> Hidden;
	};

	class ISKHLRProvider
	{
	public:
		virtual ~ISKHLRProvider() = default;
		virtual bool ComputeHLR(const FSKHLRRequest& Req, FSKHLRResult& Out) = 0;
	};

	struct FSKSectionCapRequest
	{
		uint64 GeometryKey = 0;
		// TODO(V11): plane equation, multiple planes, cap style
		FSKMatrix4f Plane = FSKMatrix4f::Identity();
	};

	class ISKSectionCapProvider
	{
	public:
		virtual ~ISKSectionCapProvider() = default;
		virtual bool BuildCapMesh(const FSKSectionCapRequest& Req, FSKMeshData& OutCap) = 0;
	};
}
