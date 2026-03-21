#pragma once
#include "SKCore/SKCoreMinimal.h"
#include "SKCore/SKObjectId.h"
#include "SKScene/SKSceneGraph.h"

namespace Skylark
{
	/**
	 * BIM Visibility Set (V8)
	 * - Provides Hide/Show/Isolate controls.
	 * - Implements ISKVisibilityProvider so SceneGraph culling can honor visibility without depending on UI.
	 */
	class FSKBimVisibilitySet final : public ISKVisibilityProvider
	{
	public:
		// ISKVisibilityProvider
		bool IsVisible(const FSKObjectId& OwnerId) const override;
		bool HasIsolation() const override { return !Isolated.empty(); }

		// API
		void Hide(const FSKObjectId& OwnerId);
		void Show(const FSKObjectId& OwnerId);
		void ClearHidden();

		void Isolate(const FSKObjectId& OwnerId);
		void ClearIsolation();

	private:
		std::unordered_set<FSKObjectId, FSKObjectIdHash> Hidden;
		std::unordered_set<FSKObjectId, FSKObjectIdHash> Isolated;
	};
}
