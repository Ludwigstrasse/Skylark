#pragma once
#include "SKRHI/SKRHI.h"

#include <vector>
#include <algorithm>

namespace Skylark
{
	class FSKNullRHITexture2D final : public ISKRHITexture2D
	{
	public:
		explicit FSKNullRHITexture2D(const FSKRHITextureDesc& InDesc)
			: Desc(InDesc)
		{
			const uint32 W = std::max(1u, Desc.Width);
			const uint32 H = std::max(1u, Desc.Height);
			Pixels.resize(static_cast<SIZE_T>(W) * static_cast<SIZE_T>(H) * 4u, 0u);
		}

		const FSKRHITextureDesc& GetDesc() const override { return Desc; }

		uint8* GetPixelPtr(uint32 X, uint32 Y)
		{
			const uint32 W = std::max(1u, Desc.Width);
			X = std::min(X, W - 1u);
			Y = std::min(Y, std::max(1u, Desc.Height) - 1u);
			const SIZE_T I = (static_cast<SIZE_T>(Y) * static_cast<SIZE_T>(W) + static_cast<SIZE_T>(X)) * 4u;
			return Pixels.data() + I;
		}

	private:
		FSKRHITextureDesc Desc{};
		std::vector<uint8> Pixels;
		friend class FSKNullRHICommandList;
		friend class FSKNullRHIDevice;
	};

	class FSKNullRHICommandList final : public ISKRHICommandList
	{
	public:
		void SetSwapChainRenderTarget(ISKRHISwapChain& SwapChain) override
		{
			(void)SwapChain;
			BoundTexture = nullptr;
		}

		void SetRenderTargetTexture(ISKRHITexture2D& ColorTarget) override
		{
			BoundTexture = dynamic_cast<FSKNullRHITexture2D*>(&ColorTarget);
		}

		void ClearRenderTarget(const FSKRHIClearColor& Color) override
		{
			if (!BoundTexture)
			{
				return;
			}

			const uint8 R = static_cast<uint8>(std::clamp(Color.R, 0.0f, 1.0f) * 255.0f);
			const uint8 G = static_cast<uint8>(std::clamp(Color.G, 0.0f, 1.0f) * 255.0f);
			const uint8 B = static_cast<uint8>(std::clamp(Color.B, 0.0f, 1.0f) * 255.0f);
			const uint8 A = static_cast<uint8>(std::clamp(Color.A, 0.0f, 1.0f) * 255.0f);

			const uint32 W = std::max(1u, BoundTexture->Desc.Width);
			const uint32 H = std::max(1u, BoundTexture->Desc.Height);

			for (uint32 y = 0; y < H; ++y)
			{
				for (uint32 x = 0; x < W; ++x)
				{
					uint8* P = BoundTexture->GetPixelPtr(x, y);
					P[0] = R; P[1] = G; P[2] = B; P[3] = A;
				}
			}
		}

		void DrawLineList(const FSKRHILineVertex* Vertices, uint32 VertexCount, const FSKRHILineDrawParams& Params) override
		{
			(void)Vertices; (void)VertexCount; (void)Params;
			// NullRHI: no-op
		}



		void DrawTriangleList(const FSKRHITriangleVertex* Vertices, uint32 VertexCount, const FSKRHITriangleDrawParams& Params) override
		{
			(void)Vertices; (void)VertexCount; (void)Params;
		}

		void DrawIndexedTriangleList(const FSKRHITriangleVertex* Vertices, uint32 VertexCount, const uint32* Indices, uint32 IndexCount, const FSKRHITriangleDrawParams& Params) override
		{
			(void)Vertices; (void)VertexCount; (void)Indices; (void)IndexCount; (void)Params;
		}

		void DrawIndexedInstancedTriangleList(const FSKRHITriangleVertex* Vertices, uint32 VertexCount, const uint32* Indices, uint32 IndexCount, const FSKRHITriangleInstance* Instances, uint32 InstanceCount, const FSKRHITriangleDrawParams& Params) override
		{
			(void)Vertices; (void)VertexCount; (void)Indices; (void)IndexCount; (void)Instances; (void)InstanceCount; (void)Params;
		}

		void Flush() override {}

	private:
		FSKNullRHITexture2D* BoundTexture = nullptr;
	};

	class FSKNullRHISwapChain final : public ISKRHISwapChain
	{
	public:
		explicit FSKNullRHISwapChain(const FSKRHISwapChainDesc& InDesc) : Desc(InDesc) {}

		const FSKRHISwapChainDesc& GetDesc() const override { return Desc; }

		void Resize(uint32 NewWidth, uint32 NewHeight) override
		{
			Desc.Width = (NewWidth == 0) ? 1u : NewWidth;
			Desc.Height = (NewHeight == 0) ? 1u : NewHeight;
		}

		void Present() override {}

	private:
		FSKRHISwapChainDesc Desc{};
	};

	class FSKNullRHIDevice final : public ISKRHIDevice
	{
	public:
		bool Init(const FSKRHIDeviceDesc& InDesc) override
		{
			Desc = InDesc;
			return true;
		}

		void Shutdown() override {}

		void BeginFrame() override {}
		void EndFrame() override {}

		ISKRHICommandList& GetImmediateCommandList() override { return Cmd; }

		TUniquePtr<ISKRHISwapChain> CreateSwapChain(const FSKRHISwapChainDesc& InDesc) override
		{
			return std::make_unique<FSKNullRHISwapChain>(InDesc);
		}

		TUniquePtr<ISKRHITexture2D> CreateTexture2D(const FSKRHITextureDesc& InDesc) override
		{
			return std::make_unique<FSKNullRHITexture2D>(InDesc);
		}

		bool ReadbackTexturePixelRGBA8(ISKRHITexture2D& Texture, uint32 X, uint32 Y, uint8 OutRGBA[4]) override
		{
			auto* Tex = dynamic_cast<FSKNullRHITexture2D*>(&Texture);
			if (!Tex || !OutRGBA)
			{
				return false;
			}

			uint8* P = Tex->GetPixelPtr(X, Y);
			OutRGBA[0] = P[0];
			OutRGBA[1] = P[1];
			OutRGBA[2] = P[2];
			OutRGBA[3] = P[3];
			return true;
		}

		ESKRHIApi GetApi() const override { return ESKRHIApi::Null; }

	private:
		FSKRHIDeviceDesc Desc{};
		FSKNullRHICommandList Cmd{};
	};
}
