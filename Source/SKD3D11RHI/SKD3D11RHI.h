#pragma once
#include "SKRHI/SKRHI.h"

namespace Skylark
{
	/**
	 * D3D11 RHI backend (modern)
	 * - Implements SKRHI interfaces without leaking D3D headers to upper layers.
	 */
	class FSKD3D11RHIDevice final : public ISKRHIDevice
	{
	public:
		FSKD3D11RHIDevice();
		~FSKD3D11RHIDevice() override;

		bool Init(const FSKRHIDeviceDesc& Desc) override;
		void Shutdown() override;

		void BeginFrame() override;
		void EndFrame() override;
		ISKRHICommandList& GetImmediateCommandList() override;

		TUniquePtr<ISKRHISwapChain> CreateSwapChain(const FSKRHISwapChainDesc& Desc) override;

		TUniquePtr<ISKRHITexture2D> CreateTexture2D(const FSKRHITextureDesc& Desc) override;
		bool ReadbackTexturePixelRGBA8(ISKRHITexture2D& Texture, uint32 X, uint32 Y, uint8 OutRGBA[4]) override;

		ESKRHIApi GetApi() const override { return ESKRHIApi::D3D11; }

	private:
		struct FImpl;
		TUniquePtr<FImpl> Impl;
	};

	TUniquePtr<ISKRHIDevice> SKCreateD3D11RHIDevice();
}
