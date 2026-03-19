#pragma once
#include "SKCore/SKCoreMinimal.h"
#include "SKScene/SKSceneView.h"

namespace Skylark
{
	/**
	 * SceneGraph (OSG-inspired, minimal)
	 *
	 * - Node: hierarchical structure with optional StateSet.
	 * - Visitor: Update/Cull traversal.
	 *
	 * This module is intentionally renderer-agnostic: it outputs RenderProxies, not draw calls.
	 */

	class FSKStateSet
	{
	public:
		uint64 MaterialKey = 0;
	};

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

		void SetLocalTransform(const FSKMatrix4f& InLocal) { LocalTransform = InLocal; }
		const FSKMatrix4f& GetLocalTransform() const { return LocalTransform; }

		void SetStateSet(TSharedPtr<FSKStateSet> InState) { State = std::move(InState); }
		const TSharedPtr<FSKStateSet>& GetStateSet() const { return State; }

		virtual bool IsGeometry() const { return false; }

		// Geometry payload (optional)
		virtual uint64 GetGeometryKey() const { return 0; }
		virtual uint32 GetHitProxyId() const { return 0; }

	private:
		FSKString Name;
		FSKMatrix4f LocalTransform = FSKMatrix4f::Identity();
		TSharedPtr<FSKStateSet> State;
		TArray<TSharedPtr<FSKNode>> Children;
	};

	class FSKGeometryNode final : public FSKNode
	{
	public:
		explicit FSKGeometryNode(FSKString InName) : FSKNode(std::move(InName)) {}

		bool IsGeometry() const override { return true; }

		void SetGeometryKey(uint64 InKey) { GeometryKey = InKey; }
		uint64 GetGeometryKey() const override { return GeometryKey; }

		void SetHitProxyId(uint32 InId) { HitProxyId = InId; }
		uint32 GetHitProxyId() const override { return HitProxyId; }

	private:
		uint64 GeometryKey = 0;
		uint32 HitProxyId = 0;
	};

	class ISKSceneVisitor
	{
	public:
		virtual ~ISKSceneVisitor() = default;
		virtual void Begin() {}
		virtual void End() {}

		virtual void PushTransform(const FSKMatrix4f& LocalToWorld) { (void)LocalToWorld; }
		virtual void PopTransform() {}

		virtual void VisitNode(const TSharedPtr<FSKNode>& Node, const FSKMatrix4f& LocalToWorld) = 0;
	};

	class FSKUpdateVisitor final : public ISKSceneVisitor
	{
	public:
		void VisitNode(const TSharedPtr<FSKNode>& Node, const FSKMatrix4f& LocalToWorld) override
		{
			(void)Node;
			(void)LocalToWorld;
			// V5: placeholder (animation/transform propagation can be added here)
		}
	};

	class FSKCullVisitor final : public ISKSceneVisitor
	{
	public:
		explicit FSKCullVisitor(FSKSceneView& OutView) : View(OutView) {}

		void Begin() override { View.VisibleProxies.clear(); }

		void VisitNode(const TSharedPtr<FSKNode>& Node, const FSKMatrix4f& LocalToWorld) override
		{
			if (!Node)
			{
				return;
			}
			if (Node->IsGeometry())
			{
				FSKRenderProxy Proxy{};
				Proxy.LocalToWorld = LocalToWorld;
				Proxy.HitProxyId = Node->GetHitProxyId();
				Proxy.GeometryKey = Node->GetGeometryKey();
				if (const auto& State = Node->GetStateSet())
				{
					Proxy.MaterialKey = State->MaterialKey;
				}
				View.VisibleProxies.push_back(Proxy);
			}
		}

	private:
		FSKSceneView& View;
	};

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
			TraverseRecursive(Visitor, Root, FSKMatrix4f::Identity());
			Visitor.End();
		}

	private:
		void TraverseRecursive(ISKSceneVisitor& Visitor, const TSharedPtr<FSKNode>& Node, const FSKMatrix4f& ParentToWorld)
		{
			if (!Node)
			{
				return;
			}

			// LocalToWorld = ParentToWorld * Local (row/col-major not enforced here; treat as placeholder)
			FSKMatrix4f LocalToWorld = ParentToWorld; // TODO: proper matrix multiply
			(void)Node->GetLocalTransform();

			Visitor.VisitNode(Node, LocalToWorld);

			for (const auto& Child : Node->GetChildren())
			{
				TraverseRecursive(Visitor, Child, LocalToWorld);
			}
		}

	private:
		TSharedPtr<FSKNode> Root;
	};
}
