#pragma once
#include "SKCore/SKCoreMinimal.h"
#include "SKCAD/SKCadTypes.h"
#include "SKInteraction/SKHitProxyTable.h"

namespace Skylark
{
	enum class ESKPickType : uint8
	{
		None = 0,
		Vertex,
		Edge,
		Face,
		Object,
		BimElement,
	};

	struct FSKPickResult
	{
		ESKPickType Type = ESKPickType::None;

		// Bridge to SolidDesigner:
		// - ObjectId: stable id in Document/Model layer (for now: hashed TopoId or legacy)
		// - SubId: topo (V/E/F) local id for fine-grained selection
		uint64 ObjectId = 0;
		uint32 SubId = 0;

		FSKVector3f WorldPos{};
		FSKVector3f WorldNormal{};
		float Depth = 0.0f;
	};

	class ISKPicker
	{
	public:
		virtual ~ISKPicker() = default;
		virtual FSKPickResult Pick(int32 ScreenX, int32 ScreenY) = 0;
	};

	class ISKSelectionSet
	{
	public:
		virtual ~ISKSelectionSet() = default;
		virtual void Clear() = 0;
		virtual void Add(uint64 ObjectId, uint32 SubId = 0) = 0;
		virtual bool Contains(uint64 ObjectId, uint32 SubId = 0) const = 0;
	};

	// Default selection set implementation
	class FSKSelectionSet final : public ISKSelectionSet
	{
	public:
		void Clear() override { Selected.clear(); }
		void Add(uint64 ObjectId, uint32 SubId = 0) override
		{
			Selected.insert((static_cast<uint64>(SubId) << 32) | (ObjectId & 0xFFFFFFFFULL));
		}
		bool Contains(uint64 ObjectId, uint32 SubId = 0) const override
		{
			return Selected.find((static_cast<uint64>(SubId) << 32) | (ObjectId & 0xFFFFFFFFULL)) != Selected.end();
		}

	private:
		TSet<uint64> Selected;
	};

	// Selection manager: owns primary selection + hover selection.
	class FSKSelectionManager
	{
	public:
		ISKSelectionSet& GetSelection() { return Selection; }
		ISKSelectionSet& GetHoverSelection() { return HoverSelection; }

		void SetHovered(const FSKPickResult& Hit)
		{
			HoverSelection.Clear();
			if (Hit.ObjectId != 0)
			{
				HoverSelection.Add(Hit.ObjectId, Hit.SubId);
			}
		}

		void ToggleSelect(const FSKPickResult& Hit)
		{
			if (Hit.ObjectId == 0) return;

			if (Selection.Contains(Hit.ObjectId, Hit.SubId))
			{
				Selection.Clear();
			}
			else
			{
				Selection.Add(Hit.ObjectId, Hit.SubId);
			}
		}

	private:
		FSKSelectionSet Selection;
		FSKSelectionSet HoverSelection;
	};
}
