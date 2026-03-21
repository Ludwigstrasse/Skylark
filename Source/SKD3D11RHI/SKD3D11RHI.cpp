#include "SKD3D11RHI/SKD3D11RHI.h"
#include "SKCore/SKAssert.h"
#include "SKCore/SKCoreLog.h"

#include <algorithm>
#include <cstring>

#if defined(_WIN32)
	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif
	#include <windows.h>

	#include <d3d11.h>
	#include <dxgi1_6.h>
	#include <d3dcompiler.h>
	#include <wrl/client.h>
	#pragma comment(lib, "d3dcompiler.lib")
	#pragma comment(lib, "d3d11.lib")
	#pragma comment(lib, "dxgi.lib")

	using Microsoft::WRL::ComPtr;
#endif

namespace Skylark
{
#if defined(_WIN32)

	static DXGI_FORMAT SKToDXGIFormat(ESKRHIFormat Format)
	{
		switch (Format)
		{
		case ESKRHIFormat::R8G8B8A8_UNORM: return DXGI_FORMAT_R8G8B8A8_UNORM;
		case ESKRHIFormat::R32_UINT:       return DXGI_FORMAT_R32_UINT;
		case ESKRHIFormat::D24S8:          return DXGI_FORMAT_D24_UNORM_S8_UINT;
		default:                           return DXGI_FORMAT_UNKNOWN;
		}
	}

	namespace
	{
		class FSKD3D11RHITexture2D final : public ISKRHITexture2D
		{
		public:
			FSKD3D11RHITexture2D(const FSKRHITextureDesc& InDesc,
				ComPtr<ID3D11Texture2D> InTex,
				ComPtr<ID3D11RenderTargetView> InRTV)
				: Desc(InDesc)
				, Texture(std::move(InTex))
				, RTV(std::move(InRTV))
			{}

			const FSKRHITextureDesc& GetDesc() const override { return Desc; }
			ID3D11Texture2D* GetNative() const { return Texture.Get(); }
			ID3D11RenderTargetView* GetRTV() const { return RTV.Get(); }
			ID3D11DepthStencilView* GetDSV() const { return DSV.Get(); }

		private:
			FSKRHITextureDesc Desc{};
			ComPtr<ID3D11Texture2D> Texture;
			ComPtr<ID3D11RenderTargetView> RTV;
			ComPtr<ID3D11Texture2D> Depth;
			ComPtr<ID3D11DepthStencilView> DSV;
		};

		class FSKD3D11RHISwapChain final : public ISKRHISwapChain
		{
		public:
			FSKD3D11RHISwapChain(const FSKRHISwapChainDesc& InDesc, ComPtr<IDXGISwapChain1> InSwapChain, ComPtr<ID3D11Device> InDevice)
				: Desc(InDesc)
				, SwapChain(std::move(InSwapChain))
				, Device(std::move(InDevice))
			{
				RebuildBackBuffer();
			}

			const FSKRHISwapChainDesc& GetDesc() const override { return Desc; }

			void Resize(uint32 NewWidth, uint32 NewHeight) override
			{
				Desc.Width = (NewWidth == 0) ? 1u : NewWidth;
				Desc.Height = (NewHeight == 0) ? 1u : NewHeight;

				if (!SwapChain)
				{
					return;
				}

				// Release before ResizeBuffers
				RTV.Reset();

				HRESULT Hr = SwapChain->ResizeBuffers(
					Desc.BufferCount,
					Desc.Width,
					Desc.Height,
					DXGI_FORMAT_UNKNOWN,
					0);
				if (FAILED(Hr))
				{
					SK_LOG(GLogSkylark, ESKLogVerbosity::Error, "D3D11 SwapChain ResizeBuffers failed: 0x%08X", (unsigned)Hr);
					return;
				}

				RebuildBackBuffer();
			}

			void Present() override
			{
				if (!SwapChain)
				{
					return;
				}
				const UINT SyncInterval = Desc.bVSync ? 1u : 0u;
				SwapChain->Present(SyncInterval, 0);
			}

			ID3D11RenderTargetView* GetRTV() const { return RTV.Get(); }

		private:
			void RebuildBackBuffer()
			{
				if (!SwapChain || !Device)
				{
					return;
				}

				ComPtr<ID3D11Texture2D> BackBuffer;
				HRESULT Hr = SwapChain->GetBuffer(0, IID_PPV_ARGS(&BackBuffer));
				if (FAILED(Hr))
				{
					SK_LOG(GLogSkylark, ESKLogVerbosity::Error, "D3D11 SwapChain GetBuffer failed: 0x%08X", (unsigned)Hr);
					return;
				}

				Hr = Device->CreateRenderTargetView(BackBuffer.Get(), nullptr, &RTV);
				if (FAILED(Hr))
				{
					SK_LOG(GLogSkylark, ESKLogVerbosity::Error, "D3D11 CreateRenderTargetView failed: 0x%08X", (unsigned)Hr);
					return;
				}

				// Depth buffer (for CAD edge/HLR depth-tested overlays)
				D3D11_TEXTURE2D_DESC DepthDesc{};
				DepthDesc.Width = Desc.Width;
				DepthDesc.Height = Desc.Height;
				DepthDesc.MipLevels = 1;
				DepthDesc.ArraySize = 1;
				DepthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
				DepthDesc.SampleDesc.Count = 1;
				DepthDesc.Usage = D3D11_USAGE_DEFAULT;
				DepthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
				Hr = Device->CreateTexture2D(&DepthDesc, nullptr, &Depth);
				if (FAILED(Hr))
				{
					SK_LOG(GLogSkylark, ESKLogVerbosity::Error, "D3D11 CreateTexture2D(Depth) failed: 0x%08X", (unsigned)Hr);
					return;
				}
				Hr = Device->CreateDepthStencilView(Depth.Get(), nullptr, &DSV);
				if (FAILED(Hr))
				{
					SK_LOG(GLogSkylark, ESKLogVerbosity::Error, "D3D11 CreateDepthStencilView failed: 0x%08X", (unsigned)Hr);
					return;
				}
			}

		private:
			FSKRHISwapChainDesc Desc{};
			ComPtr<IDXGISwapChain1> SwapChain;
			ComPtr<ID3D11Device> Device;
			ComPtr<ID3D11RenderTargetView> RTV;
		};

		class FSKD3D11RHICommandList final : public ISKRHICommandList
		{
		public:
			FSKD3D11RHICommandList(ComPtr<ID3D11Device> InDevice, ComPtr<ID3D11DeviceContext> InContext)
				: Device(std::move(InDevice))
				, Context(std::move(InContext))
			{}

			void SetSwapChainRenderTarget(ISKRHISwapChain& InSwapChain) override
			{
				if (!Context)
				{
					return;
				}

				auto* D3D11SC = dynamic_cast<FSKD3D11RHISwapChain*>(&InSwapChain);
				if (!D3D11SC)
				{
					return;
				}

				ID3D11RenderTargetView* RTV = D3D11SC->GetRTV();
				ID3D11DepthStencilView* DSV = D3D11SC->GetDSV();
				Context->OMSetRenderTargets(1, &RTV, DSV);

				const auto& Desc = InSwapChain.GetDesc();
				D3D11_VIEWPORT VP{};
				VP.TopLeftX = 0.0f;
				VP.TopLeftY = 0.0f;
				VP.Width = static_cast<float>(Desc.Width);
				VP.Height = static_cast<float>(Desc.Height);
				VP.MinDepth = 0.0f;
				VP.MaxDepth = 1.0f;
				Context->RSSetViewports(1, &VP);

				BoundSwapChain = D3D11SC;
				BoundTexture = nullptr;
			}

			void SetRenderTargetTexture(ISKRHITexture2D& ColorTarget) override
			{
				if (!Context)
				{
					return;
				}

				auto* Tex = dynamic_cast<FSKD3D11RHITexture2D*>(&ColorTarget);
				if (!Tex)
				{
					return;
				}

				ID3D11RenderTargetView* RTV = Tex->GetRTV();
				if (!RTV)
				{
					return;
				}

				Context->OMSetRenderTargets(1, &RTV, nullptr);

				const auto& Desc = Tex->GetDesc();
				D3D11_VIEWPORT VP{};
				VP.TopLeftX = 0.0f;
				VP.TopLeftY = 0.0f;
				VP.Width = static_cast<float>(Desc.Width);
				VP.Height = static_cast<float>(Desc.Height);
				VP.MinDepth = 0.0f;
				VP.MaxDepth = 1.0f;
				Context->RSSetViewports(1, &VP);

				BoundTexture = Tex;
				BoundSwapChain = nullptr;
			}

			void ClearRenderTarget(const FSKRHIClearColor& Color) override
			{
				if (!Context)
				{
					return;
				}

				ID3D11RenderTargetView* RTV = nullptr;
				if (BoundSwapChain)
				{
					RTV = BoundSwapChain->GetRTV();
				}
				else if (BoundTexture)
				{
					RTV = BoundTexture->GetRTV();
				}

				if (!RTV)
				{
					return;
				}

				const float RGBA[4] = { Color.R, Color.G, Color.B, Color.A };
				Context->ClearRenderTargetView(RTV, RGBA);
				if (BoundSwapChain && BoundSwapChain->GetDSV())
				{
					Context->ClearDepthStencilView(BoundSwapChain->GetDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
				}
			}

			void DrawLineList(const FSKRHILineVertex* Vertices, uint32 VertexCount, const FSKRHILineDrawParams& Params) override
			{
				if (!Context || !Device || !Vertices || VertexCount == 0)
				{
					return;
				}

				EnsureLinePipeline();
				if (!LineVS || !LinePS || !LineLayout)
				{
					return;
				}

				const uint32 RequiredBytes = VertexCount * (uint32)sizeof(FSKRHILineVertex);
				EnsureDynamicVB(RequiredBytes);
				if (!LineVB)
				{
					return;
				}

				D3D11_MAPPED_SUBRESOURCE Mapped{};
				HRESULT Hr = Context->Map(LineVB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &Mapped);
				if (FAILED(Hr) || !Mapped.pData)
				{
					return;
				}
				std::memcpy(Mapped.pData, Vertices, RequiredBytes);
				Context->Unmap(LineVB.Get(), 0);

				UINT Stride = (UINT)sizeof(FSKRHILineVertex);
				UINT Offset = 0;
				ID3D11Buffer* VB = LineVB.Get();
				Context->IASetVertexBuffers(0, 1, &VB, &Stride, &Offset);
				Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
				Context->IASetInputLayout(LineLayout.Get());

				Context->VSSetShader(LineVS.Get(), nullptr, 0);
				Context->PSSetShader(LinePS.Get(), nullptr, 0);

				if (LineBlend)
				{
					const float BlendFactor[4] = {0,0,0,0};
					Context->OMSetBlendState(LineBlend.Get(), BlendFactor, 0xFFFFFFFFu);
				}
				if (LineRaster)
				{
					Context->RSSetState(LineRaster.Get());
				}
				if (Params.bDepthTest && LineDepthOn)
				{
					Context->OMSetDepthStencilState(LineDepthOn.Get(), 0);
				}
				else if (LineDepthOff)
				{
					Context->OMSetDepthStencilState(LineDepthOff.Get(), 0);
				}

				Context->Draw(VertexCount, 0);
			}

			void Flush() override
			{
				if (Context)
				{
					Context->Flush();
				}
			}

		private:
			void EnsureDynamicVB(uint32 RequiredBytes)
			{
				if (LineVB && LineVBCapacityBytes >= RequiredBytes)
				{
					return;
				}
				LineVB.Reset();
				LineVBCapacityBytes = 0;

				D3D11_BUFFER_DESC BD{};
				BD.ByteWidth = std::max(RequiredBytes, 4096u);
				BD.Usage = D3D11_USAGE_DYNAMIC;
				BD.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				BD.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
				HRESULT Hr = Device->CreateBuffer(&BD, nullptr, &LineVB);
				if (SUCCEEDED(Hr) && LineVB)
				{
					LineVBCapacityBytes = BD.ByteWidth;
				}
			}

			void EnsureLinePipeline()
			{
				if (LineVS && LinePS && LineLayout)
				{
					return;
				}

				static const char* ShaderSrc =
					"struct VSIn { float4 Pos : POSITION; float4 Color : COLOR0; };
"
					"struct VSOut { float4 Pos : SV_Position; float4 Color : COLOR0; };
"
					"VSOut VSMain(VSIn In) { VSOut O; O.Pos = In.Pos; O.Color = In.Color; return O; }
"
					"float4 PSMain(VSOut In) : SV_Target { return In.Color; }
";

				ComPtr<ID3DBlob> VSBlob;
				ComPtr<ID3DBlob> PSBlob;
				ComPtr<ID3DBlob> Err;
				UINT Flags = D3DCOMPILE_ENABLE_STRICTNESS;
				HRESULT HrVS = D3DCompile(ShaderSrc, std::strlen(ShaderSrc), "SKLine", nullptr, nullptr, "VSMain", "vs_5_0", Flags, 0, &VSBlob, &Err);
				if (FAILED(HrVS) || !VSBlob)
				{
					return;
				}
				HRESULT HrPS = D3DCompile(ShaderSrc, std::strlen(ShaderSrc), "SKLine", nullptr, nullptr, "PSMain", "ps_5_0", Flags, 0, &PSBlob, &Err);
				if (FAILED(HrPS) || !PSBlob)
				{
					return;
				}

				Device->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), nullptr, &LineVS);
				Device->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), nullptr, &LinePS);

				D3D11_INPUT_ELEMENT_DESC IED[] = {
					{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				};
				Device->CreateInputLayout(IED, 2, VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), &LineLayout);

				D3D11_BLEND_DESC BD{};
				BD.RenderTarget[0].BlendEnable = TRUE;
				BD.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
				BD.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
				BD.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
				BD.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
				BD.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
				BD.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
				BD.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
				Device->CreateBlendState(&BD, &LineBlend);

				D3D11_RASTERIZER_DESC RD{};
				RD.FillMode = D3D11_FILL_SOLID;
				RD.CullMode = D3D11_CULL_NONE;
				RD.DepthClipEnable = TRUE;
				Device->CreateRasterizerState(&RD, &LineRaster);

				D3D11_DEPTH_STENCIL_DESC DOn{};
				DOn.DepthEnable = TRUE;
				DOn.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
				DOn.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
				Device->CreateDepthStencilState(&DOn, &LineDepthOn);

				D3D11_DEPTH_STENCIL_DESC DOff{};
				DOff.DepthEnable = FALSE;
				DOff.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
				DOff.DepthFunc = D3D11_COMPARISON_ALWAYS;
				Device->CreateDepthStencilState(&DOff, &LineDepthOff);
			}

		private:
			ComPtr<ID3D11Device> Device;
			ComPtr<ID3D11DeviceContext> Context;
			FSKD3D11RHISwapChain* BoundSwapChain = nullptr;
			FSKD3D11RHITexture2D* BoundTexture = nullptr;

			ComPtr<ID3D11VertexShader> LineVS;
			ComPtr<ID3D11PixelShader> LinePS;
			ComPtr<ID3D11InputLayout> LineLayout;
			ComPtr<ID3D11BlendState> LineBlend;
			ComPtr<ID3D11RasterizerState> LineRaster;
			ComPtr<ID3D11DepthStencilState> LineDepthOn;
			ComPtr<ID3D11DepthStencilState> LineDepthOff;
			ComPtr<ID3D11Buffer> LineVB;
			uint32 LineVBCapacityBytes = 0;
		};
	}

	struct FSKD3D11RHIDevice::FImpl
	{
		ComPtr<ID3D11Device> Device;
		ComPtr<ID3D11DeviceContext> Context;
		ComPtr<IDXGIFactory2> Factory;
		TUniquePtr<FSKD3D11RHICommandList> Cmd;
		FSKRHIDeviceDesc Desc{};
	};

	static ComPtr<IDXGIFactory2> SKCreateDXGIFactory2(bool bDebug)
	{
		ComPtr<IDXGIFactory2> Factory;
		UINT Flags = 0;
		(void)bDebug;
		HRESULT Hr = CreateDXGIFactory2(Flags, IID_PPV_ARGS(&Factory));
		if (FAILED(Hr))
		{
			return nullptr;
		}
		return Factory;
	}

	FSKD3D11RHIDevice::FSKD3D11RHIDevice()
		: Impl(std::make_unique<FImpl>())
	{
	}

	FSKD3D11RHIDevice::~FSKD3D11RHIDevice()
	{
		Shutdown();
	}

	bool FSKD3D11RHIDevice::Init(const FSKRHIDeviceDesc& InDesc)
	{
		Impl->Desc = InDesc;

		Impl->Factory = SKCreateDXGIFactory2(InDesc.bEnableDebugLayer);
		if (!Impl->Factory)
		{
			SK_LOG(GLogSkylark, ESKLogVerbosity::Error, "D3D11RHI: CreateDXGIFactory2 failed.");
			return false;
		}

		UINT CreateFlags = 0;
		if (InDesc.bEnableDebugLayer)
		{
			CreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
		}

		D3D_FEATURE_LEVEL FeatureLevels[] = {
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};
		D3D_FEATURE_LEVEL ChosenLevel = D3D_FEATURE_LEVEL_11_0;

		HRESULT Hr = D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			CreateFlags,
			FeatureLevels,
			(UINT)(sizeof(FeatureLevels) / sizeof(FeatureLevels[0])),
			D3D11_SDK_VERSION,
			&Impl->Device,
			&ChosenLevel,
			&Impl->Context);

		if (FAILED(Hr) || !Impl->Device || !Impl->Context)
		{
			SK_LOG(GLogSkylark, ESKLogVerbosity::Error, "D3D11RHI: D3D11CreateDevice failed: 0x%08X", (unsigned)Hr);
			return false;
		}

		Impl->Cmd = std::make_unique<FSKD3D11RHICommandList>(Impl->Device, Impl->Context);
		SK_LOG(GLogSkylark, ESKLogVerbosity::Display, "D3D11RHI: device initialized.");
		return true;
	}

	void FSKD3D11RHIDevice::Shutdown()
	{
		if (!Impl)
		{
			return;
		}

		Impl->Cmd.reset();
		Impl->Context.Reset();
		Impl->Device.Reset();
		Impl->Factory.Reset();
	}

	void FSKD3D11RHIDevice::BeginFrame() {}
	void FSKD3D11RHIDevice::EndFrame() {}

	ISKRHICommandList& FSKD3D11RHIDevice::GetImmediateCommandList()
	{
		SK_CHECK(Impl && Impl->Cmd);
		return *Impl->Cmd;
	}

	TUniquePtr<ISKRHISwapChain> FSKD3D11RHIDevice::CreateSwapChain(const FSKRHISwapChainDesc& InDesc)
	{
		if (!Impl || !Impl->Factory || !Impl->Device)
		{
			return nullptr;
		}

		if (!InDesc.Window.IsValid())
		{
			SK_LOG(GLogSkylark, ESKLogVerbosity::Error, "D3D11RHI: SwapChain requires a valid native window.");
			return nullptr;
		}

		DXGI_SWAP_CHAIN_DESC1 Desc{};
		Desc.Width = InDesc.Width;
		Desc.Height = InDesc.Height;
		Desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		Desc.SampleDesc.Count = 1;
		Desc.SampleDesc.Quality = 0;
		Desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		Desc.BufferCount = InDesc.BufferCount;
		Desc.Scaling = DXGI_SCALING_STRETCH;
		Desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		Desc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

		ComPtr<IDXGISwapChain1> SwapChain;
		HWND Hwnd = reinterpret_cast<HWND>(InDesc.Window.HWnd);

		HRESULT Hr = Impl->Factory->CreateSwapChainForHwnd(
			Impl->Device.Get(),
			Hwnd,
			&Desc,
			nullptr,
			nullptr,
			&SwapChain);

		if (FAILED(Hr) || !SwapChain)
		{
			SK_LOG(GLogSkylark, ESKLogVerbosity::Error, "D3D11RHI: CreateSwapChainForHwnd failed: 0x%08X", (unsigned)Hr);
			return nullptr;
		}

		return std::make_unique<FSKD3D11RHISwapChain>(InDesc, SwapChain, Impl->Device);
	}

	TUniquePtr<ISKRHITexture2D> FSKD3D11RHIDevice::CreateTexture2D(const FSKRHITextureDesc& InDesc)
	{
		if (!Impl || !Impl->Device)
		{
			return nullptr;
		}

		const DXGI_FORMAT Format = SKToDXGIFormat(InDesc.Format);
		if (Format == DXGI_FORMAT_UNKNOWN)
		{
			return nullptr;
		}

		D3D11_TEXTURE2D_DESC Desc{};
		Desc.Width = std::max(1u, InDesc.Width);
		Desc.Height = std::max(1u, InDesc.Height);
		Desc.MipLevels = std::max(1u, InDesc.MipLevels);
		Desc.ArraySize = 1;
		Desc.Format = Format;
		Desc.SampleDesc.Count = 1;
		Desc.Usage = D3D11_USAGE_DEFAULT;
		Desc.BindFlags = 0;
		Desc.CPUAccessFlags = 0;
		Desc.MiscFlags = 0;

		if (InDesc.Flags & SK_Tex_RenderTarget)
		{
			Desc.BindFlags |= D3D11_BIND_RENDER_TARGET;
		}
		if (InDesc.Flags & SK_Tex_ShaderRes)
		{
			Desc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
		}

		ComPtr<ID3D11Texture2D> Tex;
		HRESULT Hr = Impl->Device->CreateTexture2D(&Desc, nullptr, &Tex);
		if (FAILED(Hr) || !Tex)
		{
			SK_LOG(GLogSkylark, ESKLogVerbosity::Error, "D3D11RHI: CreateTexture2D failed: 0x%08X", (unsigned)Hr);
			return nullptr;
		}

		ComPtr<ID3D11RenderTargetView> RTV;
		if (InDesc.Flags & SK_Tex_RenderTarget)
		{
			Hr = Impl->Device->CreateRenderTargetView(Tex.Get(), nullptr, &RTV);
			if (FAILED(Hr))
			{
				SK_LOG(GLogSkylark, ESKLogVerbosity::Error, "D3D11RHI: CreateRenderTargetView failed: 0x%08X", (unsigned)Hr);
				return nullptr;
			}
		}

		return std::make_unique<FSKD3D11RHITexture2D>(InDesc, Tex, RTV);
	}

	bool FSKD3D11RHIDevice::ReadbackTexturePixelRGBA8(ISKRHITexture2D& Texture, uint32 X, uint32 Y, uint8 OutRGBA[4])
	{
		if (!OutRGBA || !Impl || !Impl->Device || !Impl->Context)
		{
			return false;
		}

		auto* Tex = dynamic_cast<FSKD3D11RHITexture2D*>(&Texture);
		if (!Tex || Tex->GetDesc().Format != ESKRHIFormat::R8G8B8A8_UNORM)
		{
			return false;
		}

		const auto& Desc = Tex->GetDesc();
		const uint32 W = std::max(1u, Desc.Width);
		const uint32 H = std::max(1u, Desc.Height);
		X = std::min(X, W - 1u);
		Y = std::min(Y, H - 1u);

		D3D11_TEXTURE2D_DESC StagingDesc{};
		StagingDesc.Width = 1;
		StagingDesc.Height = 1;
		StagingDesc.MipLevels = 1;
		StagingDesc.ArraySize = 1;
		StagingDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		StagingDesc.SampleDesc.Count = 1;
		StagingDesc.Usage = D3D11_USAGE_STAGING;
		StagingDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

		ComPtr<ID3D11Texture2D> Staging;
		HRESULT Hr = Impl->Device->CreateTexture2D(&StagingDesc, nullptr, &Staging);
		if (FAILED(Hr) || !Staging)
		{
			return false;
		}

		D3D11_BOX SrcBox{};
		SrcBox.left = X;
		SrcBox.right = X + 1;
		SrcBox.top = Y;
		SrcBox.bottom = Y + 1;
		SrcBox.front = 0;
		SrcBox.back = 1;

		Impl->Context->CopySubresourceRegion(Staging.Get(), 0, 0, 0, 0, Tex->GetNative(), 0, &SrcBox);

		D3D11_MAPPED_SUBRESOURCE Mapped{};
		Hr = Impl->Context->Map(Staging.Get(), 0, D3D11_MAP_READ, 0, &Mapped);
		if (FAILED(Hr))
		{
			return false;
		}

		const uint8* P = reinterpret_cast<const uint8*>(Mapped.pData);
		OutRGBA[0] = P[0];
		OutRGBA[1] = P[1];
		OutRGBA[2] = P[2];
		OutRGBA[3] = P[3];

		Impl->Context->Unmap(Staging.Get(), 0);
		return true;
	}

#else

	struct FSKD3D11RHIDevice::FImpl {};

	FSKD3D11RHIDevice::FSKD3D11RHIDevice() : Impl(std::make_unique<FImpl>()) {}
	FSKD3D11RHIDevice::~FSKD3D11RHIDevice() = default;

	bool FSKD3D11RHIDevice::Init(const FSKRHIDeviceDesc& Desc)
	{
		(void)Desc;
		return false;
	}
	void FSKD3D11RHIDevice::Shutdown() {}
	void FSKD3D11RHIDevice::BeginFrame() {}
	void FSKD3D11RHIDevice::EndFrame() {}
	ISKRHICommandList& FSKD3D11RHIDevice::GetImmediateCommandList()
	{
		SK_CHECK(false);
		static struct Dummy : ISKRHICommandList
		{
			void SetSwapChainRenderTarget(ISKRHISwapChain&) override {}
			void SetRenderTargetTexture(ISKRHITexture2D&) override {}
			void ClearRenderTarget(const FSKRHIClearColor&) override {}
			void Flush() override {}
		} G;
		return G;
	}
	TUniquePtr<ISKRHISwapChain> FSKD3D11RHIDevice::CreateSwapChain(const FSKRHISwapChainDesc&) { return nullptr; }
	TUniquePtr<ISKRHITexture2D> FSKD3D11RHIDevice::CreateTexture2D(const FSKRHITextureDesc&) { return nullptr; }
	bool FSKD3D11RHIDevice::ReadbackTexturePixelRGBA8(ISKRHITexture2D&, uint32, uint32, uint8 OutRGBA[4]) { if(OutRGBA){OutRGBA[0]=OutRGBA[1]=OutRGBA[2]=OutRGBA[3]=0;} return false; }

#endif

	TUniquePtr<ISKRHIDevice> SKCreateD3D11RHIDevice()
	{
#if defined(_WIN32)
		return std::make_unique<FSKD3D11RHIDevice>();
#else
		return nullptr;
#endif
	}
}
