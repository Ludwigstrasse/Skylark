#pragma once

#include "SKCore/SKCoreMinimal.h"
#include "SKPlatform/SKPlatform.h"
#include <memory>

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
		OpenGL,
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

	// ---- Shader / Pipeline (V17 minimal public API) ----
	enum class ESKRHIShaderStage : uint8
	{
		Unknown = 0,
		Vertex,
		Pixel,
		Compute,
	};

	struct FSKRHIShaderModuleDesc
	{
		ESKRHIShaderStage Stage = ESKRHIShaderStage::Unknown;
		const void* Bytecode = nullptr;
		SIZE_T BytecodeSize = 0;
		bool bBytecodeIsSpirv = false;
		const char* DebugName = nullptr;
	};

	class ISKRHIShaderModule
	{
	public:
		virtual ~ISKRHIShaderModule() = default;
		virtual const FSKRHIShaderModuleDesc& GetDesc() const = 0;
	};

	struct FSKRHIGraphicsPipelineDesc
	{
		const char* DebugName = nullptr;
		ISKRHIShaderModule* VertexShader = nullptr;
		ISKRHIShaderModule* PixelShader = nullptr;
		bool bDepthTest = true;
		bool bWireframe = false;
	};

	class ISKRHIGraphicsPipelineState
	{
	public:
		virtual ~ISKRHIGraphicsPipelineState() = default;
		virtual const FSKRHIGraphicsPipelineDesc& GetDesc() const = 0;
	};

	/**
	 * Minimal command list (UE: FRHICommandListImmediate)
	 */
	
	// ---- Line rendering (V7+) ----
	struct FSKRHILineVertex
	{
		// Clip-space position (x,y,z,w). Renderer is responsible for transformation.
		float X = 0.0f;
		float Y = 0.0f;
		float Z = 0.0f;
		float W = 1.0f;

		// Packed RGBA8 (R in lowest byte). Matches SKPackRGBA8.
		uint32 ColorRGBA8 = 0xFF000000u;
	};

	struct FSKRHILineDrawParams
	{
		bool bDepthTest = true;
		float Width = 1.0f;

		// Patterned lines (dashed). Backend may ignore.
		uint32 PatternMask = 0xFFFFFFFFu;
		float PatternScale = 1.0f;
	};


	// ---- Triangle rendering (P12 cross-backend parity) ----
	struct FSKRHITriangleVertex
	{
		// Object/local position by default. When bApplyTransform is false, this can also be clip-space.
		float X = 0.0f;
		float Y = 0.0f;
		float Z = 0.0f;
		float W = 1.0f;

		// Packed RGBA8 (R in lowest byte). Matches SKPackRGBA8.
		uint32 ColorRGBA8 = 0xFFFFFFFFu;
	};

	struct FSKRHITriangleInstance
	{
		FSKMatrix4f LocalToWorld = FSKMatrix4f::Identity();
		uint32 TintRGBA8 = 0xFFFFFFFFu;
	};

	struct FSKRHITriangleDrawParams
	{
		bool bDepthTest = true;
		bool bDepthWrite = true;
		bool bCullBackFace = false;
		bool bAlphaBlend = false;

		// When true, backend transforms the input vertices by Transform before rasterization.
		bool bApplyTransform = false;
		FSKMatrix4f Transform = FSKMatrix4f::Identity();
	};

struct FSKRHIResidentTriangleVertex
{
    float PX = 0.0f;
    float PY = 0.0f;
    float PZ = 0.0f;
    float NX = 0.0f;
    float NY = 0.0f;
    float NZ = 1.0f;
    uint32 ColorRGBA8 = 0xFFFFFFFFu;
};

struct FSKRHIResidentTriangleBufferDesc
{
    const FSKRHIResidentTriangleVertex* Vertices = nullptr;
    uint32 VertexCount = 0;
    const uint32* Indices = nullptr;
    uint32 IndexCount = 0;
    const char* DebugName = nullptr;
};

class ISKRHIResidentTriangleBuffer
{
public:
    virtual ~ISKRHIResidentTriangleBuffer() = default;
    virtual uint32 GetVertexCount() const = 0;
    virtual uint32 GetIndexCount() const = 0;
};

struct FSKRHIResidentTriangleDrawParams
{
    FSKMatrix4f LocalToWorld = FSKMatrix4f::Identity();
    FSKMatrix4f View = FSKMatrix4f::Identity();
    FSKMatrix4f Projection = FSKMatrix4f::Identity();
    bool bDepthTest = true;
    bool bDepthWrite = true;
    bool bCullBackFace = false;
    bool bAlphaBlend = false;
    bool bUseOverrideColor = false;
    uint32 OverrideColorRGBA8 = 0xFFFFFFFFu;
};

struct FSKRHIResidentLineVertex
{
    float PX = 0.0f;
    float PY = 0.0f;
    float PZ = 0.0f;
    uint32 ColorRGBA8 = 0xFFFFFFFFu;
};

struct FSKRHIResidentLineBufferDesc
{
    const FSKRHIResidentLineVertex* Vertices = nullptr;
    uint32 VertexCount = 0;
    const char* DebugName = nullptr;
};

class ISKRHIResidentLineBuffer
{
public:
    virtual ~ISKRHIResidentLineBuffer() = default;
    virtual uint32 GetVertexCount() const = 0;
};

struct FSKRHIResidentLineDrawParams
{
    FSKMatrix4f LocalToWorld = FSKMatrix4f::Identity();
    FSKMatrix4f View = FSKMatrix4f::Identity();
    FSKMatrix4f Projection = FSKMatrix4f::Identity();
    bool bDepthTest = true;
    float Width = 1.0f;
    bool bUseOverrideColor = false;
    uint32 OverrideColorRGBA8 = 0xFFFFFFFFu;
};

class ISKRHICommandList
	{
	public:
		virtual ~ISKRHICommandList() = default;

		// Bind render target
		virtual void SetSwapChainRenderTarget(ISKRHISwapChain& SwapChain) = 0;
		virtual void SetRenderTargetTexture(ISKRHITexture2D& ColorTarget) = 0;

		// Clear currently bound render target
		virtual void ClearRenderTarget(const FSKRHIClearColor& Color) = 0;

		// Optional public graphics pipeline path (V17). Legacy backends may treat this as a no-op.
		virtual void BeginRenderPass(ISKRHISwapChain& SwapChain, const FSKRHIClearColor* ClearColor = nullptr)
		{
			SetSwapChainRenderTarget(SwapChain);
			if (ClearColor)
			{
				ClearRenderTarget(*ClearColor);
			}
		}
		virtual void EndRenderPass() {}
		virtual void BindGraphicsPipeline(ISKRHIGraphicsPipelineState& Pipeline) { (void)Pipeline; }
		virtual void DrawPrimitive(uint32 VertexCount, uint32 FirstVertex = 0u) { (void)VertexCount; (void)FirstVertex; }

		// Draw line list (pairs of vertices). Used by CAD edges / HLR / overlays.
		virtual void DrawLineList(const FSKRHILineVertex* Vertices, uint32 VertexCount, const FSKRHILineDrawParams& Params) = 0;

		// Cross-backend triangle surface. D3D11 / OpenGL / Vulkan may provide backend-specific implementations.
		virtual void DrawTriangleList(const FSKRHITriangleVertex* Vertices, uint32 VertexCount, const FSKRHITriangleDrawParams& Params)
		{
			(void)Vertices; (void)VertexCount; (void)Params;
		}
		virtual void DrawIndexedTriangleList(const FSKRHITriangleVertex* Vertices, uint32 VertexCount, const uint32* Indices, uint32 IndexCount, const FSKRHITriangleDrawParams& Params)
		{
			(void)Vertices; (void)VertexCount; (void)Indices; (void)IndexCount; (void)Params;
		}
		virtual void DrawIndexedInstancedTriangleList(const FSKRHITriangleVertex* Vertices, uint32 VertexCount, const uint32* Indices, uint32 IndexCount, const FSKRHITriangleInstance* Instances, uint32 InstanceCount, const FSKRHITriangleDrawParams& Params)
		{
			(void)Vertices; (void)VertexCount; (void)Indices; (void)IndexCount; (void)Instances; (void)InstanceCount; (void)Params;
		}

		virtual std::unique_ptr<ISKRHIResidentTriangleBuffer> CreateResidentTriangleBuffer(const FSKRHIResidentTriangleBufferDesc& Desc)
		{
			(void)Desc;
			return {};
		}
		virtual std::unique_ptr<ISKRHIResidentLineBuffer> CreateResidentLineBuffer(const FSKRHIResidentLineBufferDesc& Desc)
		{
			(void)Desc;
			return {};
		}
		virtual void DrawResidentTriangleBuffer(const ISKRHIResidentTriangleBuffer& Buffer, const FSKRHIResidentTriangleDrawParams& Params)
		{
			(void)Buffer; (void)Params;
		}
		virtual void DrawResidentLineBuffer(const ISKRHIResidentLineBuffer& Buffer, const FSKRHIResidentLineDrawParams& Params)
		{
			(void)Buffer; (void)Params;
		}

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

		// Optional public shader / pipeline API (V17). Backends may return null when unsupported.
		virtual TUniquePtr<ISKRHIShaderModule> CreateShaderModule(const FSKRHIShaderModuleDesc& Desc) { (void)Desc; return {}; }
		virtual TUniquePtr<ISKRHIGraphicsPipelineState> CreateGraphicsPipeline(const FSKRHIGraphicsPipelineDesc& Desc) { (void)Desc; return {}; }

		virtual ESKRHIApi GetApi() const = 0;
	};

} // namespace Skylark
