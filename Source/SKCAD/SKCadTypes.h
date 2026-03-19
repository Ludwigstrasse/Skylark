#pragma once
#include "SKCore/SKCoreMinimal.h"

namespace Skylark
{
	/**
	 * CAD 数据：为“显示”服务的轻量数据结构（非建模内核）
	 *
	 * OCCT 对齐点：
	 * - AIS_InteractiveObject / Prs3d_Presentation: 显示表达
	 * - SelectMgr_EntityOwner / Sensitive entities: 选择表达
	 * - TopoDS_Shape: 拓扑实体（需要稳定映射到引擎的 TopoId）
	 *
	 * SolidDesigner 对齐点：
	 * - ObjectId / GlobalId (参数化/文档级稳定)
	 * - Document GUID (跨文档命名空间)
	 * - SubId (面/边/点的局部标识)
	 */

	enum class ESKCadEntityType : uint8
	{
		Unknown = 0,
		Body,
		Face,
		Edge,
		Vertex,
	};

	struct FSKTopoId
	{
		FSKGuid DocumentGuid{}; // 文档命名空间
		FSKGuid ObjectGuid{};   // 组件/对象命名空间
		uint32 SubId = 0;       // Face/Edge/Vertex local id
		ESKCadEntityType Type = ESKCadEntityType::Unknown;

		// Legacy 兼容：旧代码可继续写入 Value（后续逐步废弃）
		uint64 LegacyValue = 0;

		bool IsValid() const
		{
			return ObjectGuid.IsValid() || LegacyValue != 0;
		}

		uint64 Hash64() const
		{
			uint64 H = SKHashGuid(DocumentGuid) ^ (SKHashGuid(ObjectGuid) + 0x9E3779B97F4A7C15ull);
			H ^= (uint64)SubId * 0xBF58476D1CE4E5B9ull;
			H ^= (uint64)Type * 0x94D049BB133111EBull;
			return H;
		}

		bool operator==(const FSKTopoId& Rhs) const
		{
			return DocumentGuid == Rhs.DocumentGuid
				&& ObjectGuid == Rhs.ObjectGuid
				&& SubId == Rhs.SubId
				&& Type == Rhs.Type
				&& LegacyValue == Rhs.LegacyValue;
		}
	};

	struct FSKCadEntityRef
	{
		FSKTopoId Id{};
	};
}
