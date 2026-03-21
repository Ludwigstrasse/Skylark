#include "SKBIM/SKBIMVisibility.h"

namespace Skylark
{
	bool FSKBimVisibilitySet::IsVisible(const FSKObjectId& OwnerId) const
	{
		if (!OwnerId.IsValid())
		{
			return true;
		}

		// Isolation mode: only show isolated ids (and not hidden).
		if (!Isolated.empty())
		{
			return (Isolated.find(OwnerId) != Isolated.end()) && (Hidden.find(OwnerId) == Hidden.end());
		}

		// Normal mode: show unless hidden.
		return (Hidden.find(OwnerId) == Hidden.end());
	}

	void FSKBimVisibilitySet::Hide(const FSKObjectId& OwnerId)
	{
		if (OwnerId.IsValid())
		{
			Hidden.insert(OwnerId);
		}
	}

	void FSKBimVisibilitySet::Show(const FSKObjectId& OwnerId)
	{
		Hidden.erase(OwnerId);
		Isolated.erase(OwnerId);
	}

	void FSKBimVisibilitySet::ClearHidden()
	{
		Hidden.clear();
	}

	void FSKBimVisibilitySet::Isolate(const FSKObjectId& OwnerId)
	{
		if (OwnerId.IsValid())
		{
			Isolated.insert(OwnerId);
		}
	}

	void FSKBimVisibilitySet::ClearIsolation()
	{
		Isolated.clear();
	}
}
