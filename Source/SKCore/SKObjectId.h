#pragma once
#include "SKCore/SKCoreMinimal.h"
#include "SKCore/SKGuid.h"

namespace Skylark
{
	/**
	 * ObjectId / TopoId (UE-aligned id + OCCT selection-aligned sub-entity)
	 * - DocumentGuid: namespace across documents (file/session)
	 * - ObjectGuid  : stable id for part/component/object within the document
	 * - SubId       : stable local id for sub-entities (face/edge/vertex)
	 *
	 * NOTE:
	 * - In SolidDesigner, this aligns with GlobalID + Document GUID.
	 * - In IFC, this will align to persistent UID mapping on import/export.
	 */
	enum class ESKObjectEntityType : uint8
	{
		Unknown = 0,
		Body,
		Face,
		Edge,
		Vertex,
		Element, // BIM element (IFC/Revit)
		Cell,    // CAE cell/element
		Node,    // CAE node
	};

	struct FSKObjectId
	{
		FSKGuid DocumentGuid{};
		FSKGuid ObjectGuid{};
		uint32 SubId = 0;
		ESKObjectEntityType Type = ESKObjectEntityType::Unknown;

		// Legacy compatibility
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

		bool operator==(const FSKObjectId& Rhs) const
		{
			return DocumentGuid == Rhs.DocumentGuid
				&& ObjectGuid == Rhs.ObjectGuid
				&& SubId == Rhs.SubId
				&& Type == Rhs.Type
				&& LegacyValue == Rhs.LegacyValue;
		}

		bool operator!=(const FSKObjectId& Rhs) const { return !(*this == Rhs); }
	};

	struct FSKObjectIdHash
	{
		SIZE_T operator()(const FSKObjectId& Id) const
		{
			return (SIZE_T)Id.Hash64();
		}
	};
}
