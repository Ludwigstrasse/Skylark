#pragma once
#include "SKCore/SKCoreMinimal.h"
#include "SKCore/SKObjectId.h"

namespace Skylark
{
	/**
	 * CAD types (aligned with engine-wide ObjectId)
	 *
	 * Compatibility:
	 * - Historical V5 files used FSKTopoId + ESKCadEntityType.
	 * - V6 promotes them into SKCore as FSKObjectId + ESKObjectEntityType to avoid module cycles
	 *   (SKGeometry/SKInteraction need ids without depending on SKCAD).
	 */
	using ESKCadEntityType = ESKObjectEntityType;
	using FSKTopoId = FSKObjectId;
	using FSKTopoIdHash = FSKObjectIdHash;
}
