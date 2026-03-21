#pragma once
#include "SKCore/SKCoreMinimal.h"
#include "SKRHI/SKRHI.h"

namespace Skylark
{
	/**
	 * HZB (Hierarchical Z-Buffer) (V11)
	 * - Provides an occlusion structure for GPU-driven culling.
	 * - D3D12 path will eventually build mipmapped depth pyramid via compute.
	 * - For now: interface + minimal scaffolding.
	 */

	struct FSKHZBDesc
	{
		uint32 Width = 1;
		uint32 Height = 1;
	};

	class ISKHZB
	{
	public:
		virtual ~ISKHZB() = default;
		virtual const FSKHZBDesc& GetDesc() const = 0;
		virtual ISKRHITexture2D* GetHZBTexture() const = 0;
	};

	class ISKHZBBuilder
	{
	public:
		virtual ~ISKHZBBuilder() = default;
		virtual TUniquePtr<ISKHZB> Build(ISKRHIDevice& Device, ISKRHICommandList& Cmd, ISKRHITexture2D& DepthTexture) = 0;
	};

	// Stub builder (CPU/no-op). Keeps pipeline stable on non-D3D12 backends.
	class FSKNullHZBBuilder final : public ISKHZBBuilder
	{
	public:
		TUniquePtr<ISKHZB> Build(ISKRHIDevice& Device, ISKRHICommandList& Cmd, ISKRHITexture2D& DepthTexture) override
		{
			(void)Device;
			(void)Cmd;
			(void)DepthTexture;
			return nullptr;
		}
	};
}
