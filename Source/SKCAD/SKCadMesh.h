#pragma once
#include "SKCore/SKCoreMinimal.h"
#include "SKCAD/SKCadTypes.h"

namespace Skylark
{
	struct FSKVertex
	{
		FSKVector3f Position{};
		FSKVector3f Normal{};
		float U = 0.0f;
		float V = 0.0f;
	};

	struct FSKMeshSection
	{
		FSKTopoId OwnerId{};             // 面/体的拓扑映射，用于拾取与高亮
		TArray<FSKVertex> Vertices;
		TArray<uint32> Indices;
	};

	struct FSKMeshData
	{
		TArray<FSKMeshSection> Sections;
	};
}
