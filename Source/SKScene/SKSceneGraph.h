#pragma once
#include "SKCore/SKCoreMinimal.h"
#include "SKCore/SKObjectId.h"
#include "SKScene/SKSceneView.h"
#include "SKLWC/SKLWC.h"

namespace Skylark
{
	/**
	 * SceneGraph (V8)
	 * - OSG-inspired Node/StateSet/Visitor with Update/Cull traversal.
	 * - Outputs RenderProxies and InstanceBatches (renderer-agnostic).
	 *
	 * Design alignment:
	 * - UE5: Scene traversal builds visible primitives / draw lists.
	 * - OSG: Visitor traversal + StateSet inheritance.
	 * - OCCT: TopoId/OwnerId are first-class for selection & highlight.
	 */

	// ---------------------------------------------
	// Visibility provider (optional)
	// ---------------------------------------------
	class ISKVisibilityProvider
	{
	public:
		virtual ~ISKVisibilityProvider() = default;
		virtual bool IsVisible(const FSKObjectId& OwnerId) const = 0;
		virtual bool HasIsolation() const = 0;
	};

	// ---------------------------------------------
	// StateSet (minimal)
	// ---------------------------------------------
	class FSKStateSet
	{
	public:
		uint64 MaterialKey = 0;
		uint64 RenderStateKey = 0;
	};

	// ---------------------------------------------
	// Node base
	// ---------------------------------------------
	class FSKNode : public std::enable_shared_from_this<FSKNode>
	{
	public:
		explicit FSKNode(FSKString InName) : Name(std::move(InName)) {}
		virtual ~FSKNode() = default;

		const FSKString& GetName() const { return Name; }

		void AddChild(TSharedPtr<FSKNode> Child)
		{
			if (Child)
			{
				Children.push_back(std::move(Child));
			}
		}

		const TArray<TSharedPtr<FSKNode>>& GetChildren() const { return Children; }

		void SetLocalTransform(const FSKMatrix4f& InLocal)
		{
			LocalTransformF = InLocal;
			LocalTransformD = SKCastToDouble(InLocal);
		}

		void SetLocalTransformD(const FSKMatrix4d& InLocal)
		{
			LocalTransformD = InLocal;
			// cache float copy (rotation/scale) + raw translation
			for (int r = 0; r < 4; ++r)
			{
				for (int c = 0; c < 4; ++c)
				{
					LocalTransformF.M[r][c] = (float)InLocal.M[r][c];
				}
			}
		}

		const FSKMatrix4f& GetLocalTransform() const { return LocalTransformF; }
		const FSKMatrix4d& GetLocalTransformD() const { return LocalTransformD; }

		void SetStateSet(TSharedPtr<FSKStateSet> InState) { State = std::move(InState); }
		const TSharedPtr<FSKStateSet>& GetStateSet() const { return State; }

		// BIM semantic filters (bitmasks). Default = visible in all categories/levels/systems.
		void SetCategoryMask(uint64 InMask) { CategoryMask = InMask; }
		uint64 GetCategoryMask() const { return CategoryMask; }

		void SetLevelMask(uint64 InMask) { LevelMask = InMask; }
		uint64 GetLevelMask() const { return LevelMask; }

		void SetSystemMask(uint64 InMask) { SystemMask = InMask; }
		uint64 GetSystemMask() const { return SystemMask; }

		void SetVisibilityMask(uint64 InMask) { VisibilityMask = InMask; }
		uint64 GetVisibilityMask() const { return VisibilityMask; }

		void SetVisible(bool bInVisible) { bVisible = bInVisible; }
		bool IsVisibleFlag() const { return bVisible; }

		void SetSelectable(bool bInSelectable) { bSelectable = bInSelectable; }
		bool IsSelectable() const { return bSelectable; }

		void SetOwnerId(const FSKObjectId& InOwner) { OwnerId = InOwner; }
		const FSKObjectId& GetOwnerId() const { return OwnerId; }

		void SetGlobalId(FSKString InGlobalId) { GlobalId = std::move(InGlobalId); }
		const FSKString& GetGlobalId() const { return GlobalId; }

		// Geometry payload (optional)
		virtual bool IsGeometry() const { return false; }
		virtual bool IsInstanced() const { return false; }

		virtual uint64 GetGeometryKey() const { return 0; }
		virtual int32 GetSectionIndex() const { return -1; }
		virtual uint32 GetHitProxyId() const { return 0; }

	private:
		FSKString Name;
		FSKMatrix4d LocalTransformD = FSKMatrix4d::Identity();
		FSKMatrix4f LocalTransformF = FSKMatrix4f::Identity();
		TSharedPtr<FSKStateSet> State;
		TArray<TSharedPtr<FSKNode>> Children;

		// semantic and visibility
		uint64 CategoryMask = ~0ull;
		uint64 LevelMask = ~0ull;
		uint64 SystemMask = ~0ull;
		uint64 VisibilityMask = ~0ull;
		bool bVisible = true;
		bool bSelectable = true;

		FSKObjectId OwnerId{};
		FSKString GlobalId;
	};

	class FSKGeometryNode final : public FSKNode
	{
	public:
		explicit FSKGeometryNode(FSKString InName) : FSKNode(std::move(InName)) {}

		bool IsGeometry() const override { return true; }

		void SetGeometryKey(uint64 InKey) { GeometryKey = InKey; }
		uint64 GetGeometryKey() const override { return GeometryKey; }

		void SetSectionIndex(int32 InSectionIndex) { SectionIndex = InSectionIndex; }
		int32 GetSectionIndex() const override { return SectionIndex; }

		void SetHitProxyId(uint32 InId) { HitProxyId = InId; }
		uint32 GetHitProxyId() const override { return HitProxyId; }

	private:
		uint64 GeometryKey = 0;
		int32 SectionIndex = -1;
		uint32 HitProxyId = 0;
	};

	class FSKInstanceNode final : public FSKNode
	{
	public:
		explicit FSKInstanceNode(FSKString InName) : FSKNode(std::move(InName)) {}

		bool IsGeometry() const override { return true; }
		bool IsInstanced() const override { return true; }

		void SetGeometryKey(uint64 InKey) { GeometryKey = InKey; }
		uint64 GetGeometryKey() const override { return GeometryKey; }

		void SetSectionIndex(int32 InSectionIndex) { SectionIndex = InSectionIndex; }
		int32 GetSectionIndex() const override { return SectionIndex; }

		// Add one instance
		void AddInstance(const FSKMatrix4f& InLocal, uint32 InHitProxyId, const FSKObjectId& InOwnerId)
		{
			InstanceLocal.push_back(InLocal);
			InstanceHitProxyId.push_back(InHitProxyId);
			InstanceOwnerId.push_back(InOwnerId);
		}

		SIZE_T GetInstanceCount() const { return InstanceLocal.size(); }
		const TArray<FSKMatrix4f>& GetInstanceLocalTransforms() const { return InstanceLocal; }
		const TArray<uint32>& GetInstanceHitProxyIds() const { return InstanceHitProxyId; }
		const TArray<FSKObjectId>& GetInstanceOwnerIds() const { return InstanceOwnerId; }

	private:
		uint64 GeometryKey = 0;
		int32 SectionIndex = -1;
		TArray<FSKMatrix4f> InstanceLocal;
		TArray<uint32> InstanceHitProxyId;
		TArray<FSKObjectId> InstanceOwnerId;
	};

	// ---------------------------------------------
	// Visitor
	// ---------------------------------------------
	class ISKSceneVisitor
	{
	public:
		virtual ~ISKSceneVisitor() = default;
		virtual void Begin() {}
		virtual void End() {}

		virtual void VisitNode(const TSharedPtr<FSKNode>& Node, const FSKMatrix4d& LocalToWorld, uint64 EffectiveMaterialKey) = 0;
	};

	struct FSKSceneCullFilter
	{
		uint64 CategoryMask = ~0ull;
		uint64 LevelMask = ~0ull;
		uint64 SystemMask = ~0ull;
		uint64 ViewVisibilityMask = ~0ull;
	};

	struct FSKSceneCullParams
	{
		FSKSceneCullFilter Filter{};
		const ISKVisibilityProvider* Visibility = nullptr;
		bool bBuildInstanceBatches = true;
	};

	class FSKUpdateVisitor final : public ISKSceneVisitor
	{
	public:
		void VisitNode(const TSharedPtr<FSKNode>& Node, const FSKMatrix4d& LocalToWorld, uint64 EffectiveMaterialKey) override
		{
			(void)Node;
			(void)LocalToWorld;
			(void)EffectiveMaterialKey;
			// V8 placeholder: animation / dirty-prop / bounds update
		}
	};

	class FSKCullVisitor final : public ISKSceneVisitor
	{
	public:
		FSKCullVisitor(FSKSceneView& OutView, FSKSceneCullParams InParams)
			: View(OutView)
			, Params(InParams)
		{
		}

		void Begin() override
		{
			View.VisibleProxies.clear();
			View.VisibleInstances.clear();
			InstanceBatchMap.clear();
		}

		void VisitNode(const TSharedPtr<FSKNode>& Node, const FSKMatrix4d& LocalToWorld, uint64 EffectiveMaterialKey) override
		{
			if (!Node)
			{
				return;
			}
			if (!Node->IsVisibleFlag())
			{
				return;
			}

			// Per-view visibility
			if ((Node->GetVisibilityMask() & Params.Filter.ViewVisibilityMask) == 0)
			{
				return;
			}

			// BIM semantic filters
			if ((Node->GetCategoryMask() & Params.Filter.CategoryMask) == 0) return;
			if ((Node->GetLevelMask() & Params.Filter.LevelMask) == 0) return;
			if ((Node->GetSystemMask() & Params.Filter.SystemMask) == 0) return;

			// Optional visibility provider (Hide/Isolate)
			if (Params.Visibility && Node->GetOwnerId().IsValid())
			{
				if (!Params.Visibility->IsVisible(Node->GetOwnerId()))
				{
					return;
				}
			}

			if (!Node->IsGeometry())
			{
				return;
			}

			const uint64 GeometryKey = Node->GetGeometryKey();
			if (GeometryKey == 0)
			{
				return;
			}

			const uint64 MaterialKey = EffectiveMaterialKey;
			const int32 SectionIndex = Node->GetSectionIndex();

			if (Params.bBuildInstanceBatches && Node->IsInstanced())
			{
				const auto* IN = dynamic_cast<const FSKInstanceNode*>(Node.get());
				if (!IN)
				{
					return;
				}

				// Group by (GeometryKey, MaterialKey, SectionIndex).
				// SectionIndex must be part of the batch key; otherwise different CAD sections
				// sharing the same source geometry/material would be merged into one draw batch.
				const uint64 H = HashBatchKey(GeometryKey, MaterialKey, SectionIndex);
				auto It = InstanceBatchMap.find(H);
				SIZE_T BatchIndex = 0;
				if (It == InstanceBatchMap.end())
				{
					FSKInstanceBatch B;
					B.GeometryKey = GeometryKey;
					B.MaterialKey = MaterialKey;
					B.SectionIndex = SectionIndex;
					View.VisibleInstances.push_back(std::move(B));
					BatchIndex = View.VisibleInstances.size() - 1;
					InstanceBatchMap.emplace(H, BatchIndex);
				}
				else
				{
					BatchIndex = It->second;
				}

				auto& Batch = View.VisibleInstances[BatchIndex];

				const auto& LocalList = IN->GetInstanceLocalTransforms();
				const auto& HitList = IN->GetInstanceHitProxyIds();
				for (SIZE_T i = 0; i < LocalList.size(); ++i)
				{
					const FSKMatrix4d InstanceLTW = SKMatrixMultiplyD(LocalToWorld, SKCastToDouble(LocalList[i]));
					Batch.InstanceLocalToWorld.push_back(SKMakeCameraRelativeMatrix(InstanceLTW, View.ViewOrigin));
					Batch.InstanceHitProxyId.push_back((i < HitList.size()) ? HitList[i] : 0);
				}
				return;
			}

			// Non-instanced proxy
			FSKRenderProxy Proxy{};
			Proxy.LocalToWorld = SKMakeCameraRelativeMatrix(LocalToWorld, View.ViewOrigin);
			Proxy.HitProxyId = Node->GetHitProxyId();
			Proxy.GeometryKey = GeometryKey;
			Proxy.MaterialKey = MaterialKey;
			Proxy.SectionIndex = SectionIndex;
			View.VisibleProxies.push_back(Proxy);
		}

	private:
		static uint64 HashBatchKey(uint64 GeometryKey, uint64 MaterialKey, int32 SectionIndex)
		{
			// 64-bit mix. SectionIndex is part of the drawable identity; otherwise
			// different CAD sections would be batched together and drawn as full meshes.
			uint64 H = GeometryKey * 0x9E3779B97F4A7C15ull;
			H ^= (MaterialKey + 0xBF58476D1CE4E5B9ull) + (H << 6) + (H >> 2);
			const uint64 SectionKey = static_cast<uint64>(static_cast<int64>(SectionIndex) + 1ll);
			H ^= (SectionKey + 0x94D049BB133111EBull) + (H << 6) + (H >> 2);
			return H;
		}

	private:
		FSKSceneView& View;
		FSKSceneCullParams Params{};
		std::unordered_map<uint64, SIZE_T> InstanceBatchMap;
	};

	// ---------------------------------------------
	// Scene index (metadata)
	// ---------------------------------------------
	class FSKSceneIndex
	{
	public:
		void Clear() { ByOwnerHash.clear(); }

		void Add(const TSharedPtr<FSKNode>& Node)
		{
			if (!Node)
			{
				return;
			}
			const auto& Id = Node->GetOwnerId();
			if (!Id.IsValid())
			{
				return;
			}
			ByOwnerHash[(uint64)Id.Hash64()] = Node;
		}

		TSharedPtr<FSKNode> Find(const FSKObjectId& OwnerId) const
		{
			auto It = ByOwnerHash.find((uint64)OwnerId.Hash64());
			if (It == ByOwnerHash.end())
			{
				return nullptr;
			}
			return It->second.lock();
		}

	private:
		std::unordered_map<uint64, TWeakPtr<FSKNode>> ByOwnerHash;
	};

	// ---------------------------------------------
	// SceneGraph root
	// ---------------------------------------------
	class FSKSceneGraph
	{
	public:
		FSKSceneGraph()
		{
			Root = std::make_shared<FSKNode>("Root");
		}

		TSharedPtr<FSKNode> GetRoot() const { return Root; }

		void Traverse(ISKSceneVisitor& Visitor)
		{
			Visitor.Begin();
			TraverseRecursive(Visitor, Root, FSKMatrix4d::Identity(), 0);
			Visitor.End();
		}

		void RebuildIndex()
		{
			Index.Clear();
			RebuildIndexRecursive(Root);
		}

		TSharedPtr<FSKNode> FindNodeByOwnerId(const FSKObjectId& OwnerId) const
		{
			return Index.Find(OwnerId);
		}

	private:
		void TraverseRecursive(ISKSceneVisitor& Visitor, const TSharedPtr<FSKNode>& Node, const FSKMatrix4d& ParentToWorld, uint64 ParentMaterialKey)
		{
			if (!Node)
			{
				return;
			}

			uint64 EffectiveMaterialKey = ParentMaterialKey;
			if (const auto& State = Node->GetStateSet())
			{
				if (State->MaterialKey != 0)
				{
					EffectiveMaterialKey = State->MaterialKey;
				}
			}

			const FSKMatrix4d LocalToWorld = SKMatrixMultiplyD(ParentToWorld, Node->GetLocalTransformD());
			Visitor.VisitNode(Node, LocalToWorld, EffectiveMaterialKey);

			for (const auto& Child : Node->GetChildren())
			{
				TraverseRecursive(Visitor, Child, LocalToWorld, EffectiveMaterialKey);
			}
		}

		void RebuildIndexRecursive(const TSharedPtr<FSKNode>& Node)
		{
			if (!Node)
			{
				return;
			}
			Index.Add(Node);
			for (const auto& Child : Node->GetChildren())
			{
				RebuildIndexRecursive(Child);
			}
		}

	private:
		TSharedPtr<FSKNode> Root;
		FSKSceneIndex Index;
	};
}
