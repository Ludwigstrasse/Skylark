#pragma once
#include "SKCore/SKCoreMinimal.h"
#include "SKCore/SKObjectId.h"
#include "SKScene/SKSceneGraph.h"

namespace Skylark
{
	/**
	 * SKAnnotation (V9)
	 * - Lightweight annotation/markup nodes:
	 *   - leaders/arrows/text placeholders
	 * - Rendering currently uses line list overlay (text is a future module).
	 */

	struct FSKAnnotationStyle
	{
		uint32 ColorRGBA8 = 0xFF202020u;
		float LineWidth = 1.0f;
	};

	struct FSKAnnotation
	{
		FSKObjectId OwnerId{};
		FSKString Text; // placeholder (requires text renderer)
		FSKVector3f WorldA{};
		FSKVector3f WorldB{};
		FSKAnnotationStyle Style{};
	};

	class FSKAnnotationManager final
	{
	public:
		void Clear() { Items.clear(); }
		void Add(const FSKAnnotation& A) { Items.push_back(A); }
		const TArray<FSKAnnotation>& GetAll() const { return Items; }

	private:
		TArray<FSKAnnotation> Items;
	};

	// Optional SceneGraph node for annotation (future: billboards/text).
	class FSKAnnotationNode final : public FSKNode
	{
	public:
		explicit FSKAnnotationNode(FSKString InName) : FSKNode(std::move(InName)) {}

		void SetAnnotation(const FSKAnnotation& In) { Ann = In; }
		const FSKAnnotation& GetAnnotation() const { return Ann; }

	private:
		FSKAnnotation Ann{};
	};
}
