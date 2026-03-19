#pragma once
#include "SKInteraction/SKInteraction.h"
#include "SKInteraction/SKHitProxyTable.h"
#include "SKRHI/SKRHI.h"

namespace Skylark
{
	/**
	 * GPU picker (id-buffer)
	 * - Reads a single pixel from the selection buffer (RGBA8) and resolves HitProxyId -> TopoId.
	 * - This is the industrial default for CAD hover/selection because it matches render state precisely.
	 */
	class FSKGpuPicker final : public ISKPicker
	{
	public:
		FSKGpuPicker(ISKRHIDevice* InDevice, ISKRHITexture2D* InSelectionBuffer, const FSKHitProxyTable* InTable)
			: Device(InDevice)
			, SelectionBuffer(InSelectionBuffer)
			, Table(InTable)
		{
		}

		FSKPickResult Pick(int32 ScreenX, int32 ScreenY) override;

	private:
		ISKRHIDevice* Device = nullptr;
		ISKRHITexture2D* SelectionBuffer = nullptr;
		const FSKHitProxyTable* Table = nullptr;
	};
}
