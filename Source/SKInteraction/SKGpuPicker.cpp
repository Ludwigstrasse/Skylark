#include "SKInteraction/SKGpuPicker.h"

namespace Skylark
{
	FSKPickResult FSKGpuPicker::Pick(int32 ScreenX, int32 ScreenY)
	{
		FSKPickResult R{};
		if (!Device || !SelectionBuffer || !Table)
		{
			return R;
		}

		uint8 RGBA[4] = {};
		if (!Device->ReadbackTexturePixelRGBA8(*SelectionBuffer, (uint32)ScreenX, (uint32)ScreenY, RGBA))
		{
			return R;
		}

		const uint32 RawId = SKDecodeHitProxyIdRGBA8(RGBA);
		const FSKHitProxyId HitId{RawId};
		FSKTopoId Topo{};
		if (!Table->Resolve(HitId, Topo))
		{
			return R;
		}

		R.Type = ESKPickType::Object; // detailed Face/Edge etc resolved by Topo.Type later
		R.ObjectId = Topo.Hash64();   // bridge to legacy 64-bit until SolidDesigner ObjectId is wired
		R.SubId = Topo.SubId;
		return R;
	}
}
