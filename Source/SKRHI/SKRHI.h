#pragma once

#include "SKCore/SKCoreMinimal.h"
#include "SKPlatform/SKPlatform.h"

namespace Skylark
{
	/**
	 * SKRHI (UE5-aligned)
	 * - Purpose: Decouple renderer logic from graphics backend.
	 * - This is NOT the legacy SKEngine2::SKDX11Renderer path; that lives in SKLegacy* modules.
	 *
	 * V5: introduce a minimal resource model (Texture2D) + CPU readback for selection buffer.
	 *     This is intentionally small (RDG-Min needs it), but the boundary matches UE's RHI shape.
	 */

	enum class ESKRHIApi : uint8
	{
		Null = 0,
		D3D11,
		D3D12,
		Vulkan,
		Metal,
	};

	// ---- Formats ----
	enum class ESKRHIFormat : uint8
	{
		Unknown = 0,
		R8G8B8A8_UNORM,
		R32_UINT,
		D24S8,
	};

	enum ESKRHITextureFlags : uint32
	{
		SK_Tex_None         = 0u,
		SK_Tex_RenderTarget = 1u << 0u,
		SK_Tex_ShaderRes    = 1u << 1u,
		SK_Tex_CPUReadback  = 1u << 2u,
	};

	inline ESKRHITextureFlags operator|(ESKRHITextureFlags A, ESKRHITextureFlags B)
	{
		return static_cast<ESKRHITextureFlags>(static_cast<uint32>(A) | static_cast<uint32>(B));
	}

	struct FSKRHITextureDesc
	{
		uint32 Width = 1;
		uint32 Height = 1;
		ESKRHIFormat Format = ESKRHIFormat::R8G8B8A8_UNORM;
		uint32 Flags = SK_Tex_None;
		uint32 MipLevels = 1;
	};

	// ---- SwapChain ----
	struct FSKRHISwapChainDesc
	{
		FSKNativeWindow Window{};
		uint32 Width = 1280;
		uint32 Height = 720;
		bool bVSync = true;
		uint32 BufferCount = 2;
	};

	struct FSKRHIDeviceDesc
	{
		ESKRHIApi Api = ESKRHIApi::Null;
		// Optional: adapter LUID / PCI id / index
		int32 AdapterIndex = -1;
		bool bEnableDebugLayer = false;
	};

	class ISKRHISwapChain
	{
	public:
		virtual ~ISKRHISwapChain() = default;

		virtual const FSKRHISwapChainDesc& GetDesc() const = 0;
		virtual void Resize(uint32 NewWidth, uint32 NewHeight) = 0;
		virtual void Present() = 0;
	};

	// ---- Resources (V5 minimal) ----
	class ISKRHITexture2D
	{
	public:
		virtual ~ISKRHITexture2D() = default;
		virtual const FSKRHITextureDesc& GetDesc() const = 0;
	};

	struct FSKRHIClearColor
	{
		float R = 0.0f;
		float G = 0.0f;
		float B = 0.0f;
		float A = 1.0f;
	};

	/**
	 * Minimal command list (UE: FRHICommandListImmediate)
	 */
	class ISKRHICommandList
	{
	public:
		virtual ~ISKRHICommandList() = default;

		// Bind render target
		virtual void SetSwapChainRenderTarget(ISKRHISwapChain& SwapChain) = 0;
		virtual void SetRenderTargetTexture(ISKRHITexture2D& ColorTarget) = 0;

		// Clear currently bound render target
		virtual void ClearRenderTarget(const FSKRHIClearColor& Color) = 0;

		virtual void Flush() = 0;
	};

	class ISKRHIDevice
	{
	public:
		virtual ~ISKRHIDevice() = default;

		virtual bool Init(const FSKRHIDeviceDesc& Desc) = 0;
		virtual void Shutdown() = 0;

		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;

		// Immediate command list (backend-specific).
		virtual ISKRHICommandList& GetImmediateCommandList() = 0;

		virtual TUniquePtr<ISKRHISwapChain> CreateSwapChain(const FSKRHISwapChainDesc& Desc) = 0;

		// Resources
		virtual TUniquePtr<ISKRHITexture2D> CreateTexture2D(const FSKRHITextureDesc& Desc) = 0;

		// CPU readback (blocking; used for selection buffer in V5)
		virtual bool ReadbackTexturePixelRGBA8(ISKRHITexture2D& Texture, uint32 X, uint32 Y, uint8 OutRGBA[4]) = 0;

		virtual ESKRHIApi GetApi() const = 0;
	};

} // namespace Skylark
