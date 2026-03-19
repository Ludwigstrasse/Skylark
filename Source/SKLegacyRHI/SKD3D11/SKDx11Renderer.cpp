#include "SKDx11Renderer.h"
#include "SKDx11Resource.h"
#include "SKTexture.h"
#include "SKCubeTexture.h"
#include "SKVertexBuffer.h"
#include "SKVShader.h"
#include "SKPShader.h"
#include "SKGShader.h"
#include "SKDShader.h"
#include "SKHShader.h"
#include "SKGeometry.h"
#include "SKSegment3.h"
#include "SK2DTexture.h"
#include "SK3DTexture.h"
#include "SKCubeTexture.h"
#include "SKDepthStencil.h"
#include "SKUserConstant.h"
#include "SKDx11ShaderInclude.h"
#include "SKShaderKey.h"
#include "SKRenderTarget.h"
#include "SKGraphicInclude.h"
#include "SKProfiler.h"
using namespace SKEngine2;
DECLEAR_TIME_PROFILENODE(Dx11DrawMesh, )
SKDx11ShaderInclude * SKDX11Renderer::ms_pDx11IncludeShader = NULL;
TCHAR SKDX11Renderer::ms_cVertexShaderProgramVersion[] = _T("vs_5_0");
TCHAR SKDX11Renderer::ms_cPixelShaderProgramVersion[] = _T("ps_5_0");
TCHAR SKDX11Renderer::ms_cGeometryShaderProgramVersion[] = _T("gs_5_0");
TCHAR SKDX11Renderer::ms_cGDomainShaderProgramVersion[] = _T("ds_5_0");
TCHAR SKDX11Renderer::ms_cHullShaderProgramVersion[] = _T("hs_5_0");
SKString SKDX11Renderer::ms_IncludeShader;
SKString SKDX11Renderer::ms_DynamicShader;
SKAdapterInfo	SKDX11Renderer::ms_AdapterInfo[5];
UINT SKDX11Renderer::ms_uiAdapterNum = 0;
IDXGIFactory* SKDX11Renderer::ms_pDXGI = NULL;
DWORD SKDX11Renderer::ms_dwAlphaBlend[] =
{
	D3D11_BLEND_ZERO,
	D3D11_BLEND_ONE,
	D3D11_BLEND_SRC_COLOR,
	D3D11_BLEND_INV_SRC_COLOR,
	D3D11_BLEND_SRC_ALPHA,
	D3D11_BLEND_INV_SRC_ALPHA,
	D3D11_BLEND_DEST_ALPHA,
	D3D11_BLEND_INV_DEST_ALPHA,
	D3D11_BLEND_DEST_COLOR,
	D3D11_BLEND_INV_DEST_COLOR

};
DWORD SKDX11Renderer::ms_dwAlphaBlendOP[] =
{
	D3D11_BLEND_OP_ADD,
	D3D11_BLEND_OP_SUBTRACT,
	D3D11_BLEND_OP_REV_SUBTRACT,
	D3D11_BLEND_OP_MIN,
	D3D11_BLEND_OP_MAX
};
DWORD SKDX11Renderer::ms_dwCullType[] =
{
	D3D11_CULL_NONE,
	D3D11_CULL_FRONT,
	D3D11_CULL_BACK
};
DWORD SKDX11Renderer::ms_dwStencilOperation[] =
{
	D3D11_STENCIL_OP_KEEP,
	D3D11_STENCIL_OP_ZERO,
	D3D11_STENCIL_OP_REPLACE,
	D3D11_STENCIL_OP_INCR,
	D3D11_STENCIL_OP_DECR,
	D3D11_STENCIL_OP_INVERT,
	D3D11_STENCIL_OP_INCR_SAT,
	D3D11_STENCIL_OP_DECR_SAT
};
DWORD SKDX11Renderer::ms_dwCompare[] =
{
	D3D11_COMPARISON_NEVER,
	D3D11_COMPARISON_LESS,
	D3D11_COMPARISON_EQUAL,
	D3D11_COMPARISON_LESS_EQUAL,
	D3D11_COMPARISON_GREATER,
	D3D11_COMPARISON_NOT_EQUAL,
	D3D11_COMPARISON_GREATER_EQUAL,
	D3D11_COMPARISON_ALWAYS
};
DWORD SKDX11Renderer::ms_dwTextureFormatType[] =
{
	DXGI_FORMAT_B8G8R8A8_UNORM,
	DXGI_FORMAT_D16_UNORM,
	DXGI_FORMAT_R24_UNORM_X8_TYPELESS,
	DXGI_FORMAT_D24_UNORM_S8_UINT,
	DXGI_FORMAT_R32G32B32A32_FLOAT,
	DXGI_FORMAT_R16G16B16A16_FLOAT,
	DXGI_FORMAT_R16G16_FLOAT,
	DXGI_FORMAT_R16_FLOAT,
	DXGI_FORMAT_R32_FLOAT,
	DXGI_FORMAT_UNKNOWN,
	DXGI_FORMAT_D24_UNORM_S8_UINT,
	DXGI_FORMAT_R32G32_FLOAT,
	DXGI_FORMAT_B5G6R5_UNORM,
	DXGI_FORMAT_A8_UNORM,
	DXGI_FORMAT_BC2_UNORM,
	DXGI_FORMAT_BC3_UNORM

};
DWORD SKDX11Renderer::ms_dwSRGBTextureFormatType[] =
{
	DXGI_FORMAT_B8G8R8A8_UNORM_SRGB,
	DXGI_FORMAT_UNKNOWN,
	DXGI_FORMAT_UNKNOWN,
	DXGI_FORMAT_UNKNOWN,
	DXGI_FORMAT_UNKNOWN,
	DXGI_FORMAT_UNKNOWN,
	DXGI_FORMAT_UNKNOWN,
	DXGI_FORMAT_UNKNOWN,
	DXGI_FORMAT_UNKNOWN,
	DXGI_FORMAT_UNKNOWN,
	DXGI_FORMAT_UNKNOWN,
	DXGI_FORMAT_UNKNOWN,
	DXGI_FORMAT_UNKNOWN,
	DXGI_FORMAT_UNKNOWN,
	DXGI_FORMAT_BC2_UNORM_SRGB,
	DXGI_FORMAT_BC3_UNORM_SRGB
};
DWORD SKDX11Renderer::ms_dwTextureFilterType[SKSamplerDesc::FM_MAX][SKSamplerDesc::FM_MAX][SKSamplerDesc::FM_MAX] =
{
	D3D11_FILTER_MIN_MAG_MIP_POINT,	// none none none
	D3D11_FILTER_MIN_MAG_MIP_POINT,	// none none point
	D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR, // none none line
	D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR, // none none anisotropic

	D3D11_FILTER_MIN_MAG_MIP_POINT,// none point none
	D3D11_FILTER_MIN_MAG_MIP_POINT,// none point point
	D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR, // none point line
	D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR, // none point anisotropic

	D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT,// none line none
	D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT,// none line point
	D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR, // none line line
	D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR, // none line anisotropic

	D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT, //none, anisotropic, none
	D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT,	//none, anisotropic, point
	D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR, // none anisotropic line
	D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR, // none anisotropic anisotropic



	D3D11_FILTER_MIN_MAG_MIP_POINT, //point, none, none
	D3D11_FILTER_MIN_MAG_MIP_POINT,	//point, none, point
	D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR, // point none line
	D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR, // point none anisotropic

	D3D11_FILTER_MIN_MAG_MIP_POINT, //point, point, none
	D3D11_FILTER_MIN_MAG_MIP_POINT,	//point, point, point
	D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR, // point point line
	D3D11_FILTER_MIN_MAG_MIP_POINT, // point point anisotropic

	D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT, //point, line, none
	D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT,	//point, line, point
	D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR, // point line line
	D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR, // point line anisotropic

	D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT, //point, anisotropic, none
	D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT,	//point, anisotropic, point
	D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR, // point anisotropic line
	D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR, // point anisotropic anisotropic



	D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT, //line, none, none
	D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT,	//line, none, point
	D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR, // line none line
	D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR, // line none anisotropic

	D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT, //line, point, none
	D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT,	//line, point, point
	D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR, // line point line
	D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR, // line point anisotropic

	D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT, //line, line, none
	D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT,	//line, line, point
	D3D11_FILTER_MIN_MAG_MIP_LINEAR, // line line line
	D3D11_FILTER_MIN_MAG_MIP_LINEAR, // line line anisotropic

	D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT, //line, anisotropic, none
	D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT,	//line, anisotropic, point
	D3D11_FILTER_MIN_MAG_MIP_LINEAR, // line anisotropic line
	D3D11_FILTER_MIN_MAG_MIP_LINEAR, // line anisotropic anisotropic



	D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT, //anisotropic, none, none
	D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT,	//anisotropic, none, point
	D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR, // anisotropic none line
	D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR, // anisotropic none anisotropic

	D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT, //anisotropic, point, none
	D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT,	//anisotropic, point, point
	D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR, // anisotropic point line
	D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR, // anisotropic point anisotropic

	D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT, //anisotropic, line, none
	D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT,	//anisotropic, line, point
	D3D11_FILTER_MIN_MAG_MIP_LINEAR, // anisotropic line line
	D3D11_FILTER_MIN_MAG_MIP_LINEAR, // anisotropic line anisotropic

	D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT, //anisotropic, anisotropic, none
	D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT,	//anisotropic, anisotropic, point
	D3D11_FILTER_MIN_MAG_MIP_LINEAR, // anisotropic anisotropic line
	D3D11_FILTER_ANISOTROPIC, // anisotropic anisotropic anisotropic
	 
};
DWORD SKDX11Renderer::ms_dwTextureAddrType[] =
{
	D3D11_TEXTURE_ADDRESS_CLAMP,
	D3D11_TEXTURE_ADDRESS_WRAP,
	D3D11_TEXTURE_ADDRESS_MIRROR,
	D3D11_TEXTURE_ADDRESS_BORDER,
};

DWORD SKDX11Renderer::ms_dwPrimitiveType[] =
{
	D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
	D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
};
DWORD SKDX11Renderer::ms_dwDevTypes[] = 
{ 
	D3D_DRIVER_TYPE_HARDWARE,
	D3D_DRIVER_TYPE_REFERENCE
};
DWORD SKDX11Renderer::ms_dwMultiSampleTypes[] =
{
	1 ,
	2 ,
	4 ,
	8 ,
	16 
};
DWORD SKDX11Renderer::ms_dwLockFlag[] = 
{
	D3D11_MAP_WRITE_NO_OVERWRITE,
	D3D11_MAP_WRITE_DISCARD,
	D3D11_MAP_READ
};
DWORD SKDX11Renderer::ms_dwVertexFormatDateType[] = 
{
	DXGI_FORMAT_R32_FLOAT,
	DXGI_FORMAT_R32G32_FLOAT,
	DXGI_FORMAT_R32G32B32_FLOAT,
	DXGI_FORMAT_R32G32B32A32_FLOAT,

	DXGI_FORMAT_R16_UINT,
	DXGI_FORMAT_R32_UINT,

	DXGI_FORMAT_R8G8B8A8_UINT,
	DXGI_FORMAT_R8G8B8A8_UNORM,

	DXGI_FORMAT_R16G16_SINT,
	DXGI_FORMAT_R16G16_SNORM,
	DXGI_FORMAT_R16G16_UNORM,

	DXGI_FORMAT_R16G16B16A16_SINT,
	DXGI_FORMAT_R16G16B16A16_SNORM,
	DXGI_FORMAT_R16G16B16A16_UNORM,

	DXGI_FORMAT_R16G16_FLOAT,
	DXGI_FORMAT_R16G16B16A16_FLOAT,

	DXGI_FORMAT_R8G8B8A8_UNORM
};
TCHAR* SKDX11Renderer::ms_dwVertexFormatClareType[] =
{
	"float ",
	"float2 ",
	"float3 ",
	"float4 ",

	"uint ",
	"uint ",

	"uint4 ",
	"float4 ",

	"int2 ",
	"float2 ",
	"float2 ",

	"int4 ",
	"float4 ",
	"float4 ",

	"float2 ",
	"float4 ",

	"float4 "
};
TCHAR* SKDX11Renderer::ms_dwVertexFormatSemantics[] =
{
	"POSITION",
	"TEXCOORD",
	"NORMAL",
	"TANGENT",
	"BINORMAL",
	"PSIZE",
	"COLOR",
	"FOG",
	"SV_Depth",
	"BLENDWEIGHT",
	"BLENDINDICES"
};
int SKDX11Renderer::GetRendererType() const
{

	return SKRenderer::RAT_DIRECTX11;
}
const TCHAR * SKDX11Renderer::GetRendererStringType() const
{
	return _T("DX11Renderer");
}
SKDX11Renderer::SKDX11Renderer(HWND hMainWindow, unsigned int uiScreenWidth, unsigned int uiScreenHeight, bool bIsWindowed,
							  unsigned int uiAnisotropy,unsigned int uiMultisample,
							  ChildWindowInfo * pChildWindow,int uiNumChildWindow)
{
	
	if (uiNumChildWindow > 0 && pChildWindow && bIsWindowed) 
	{
		m_pChildWindowInfo = SK_NEW ChildWindowInfo[uiNumChildWindow];
		SKMemcpy(m_pChildWindowInfo, pChildWindow, sizeof(ChildWindowInfo) * uiNumChildWindow);
		m_iNumChildWindow = uiNumChildWindow;
	}
	else 
	{
		uiNumChildWindow = 0;

	}

	m_hMainWindow  = hMainWindow;
	m_bWindowed = bIsWindowed;
	m_uiScreenHeight = uiScreenHeight;
	m_uiScreenWidth = uiScreenWidth;
	m_uiCurRTWidth = uiScreenWidth;
	m_uiCurRTHeight = uiScreenHeight;

	HRESULT hResult = NULL;
	m_uinAdapter = 0;
	hResult = ms_pDXGI->EnumAdapters(m_uinAdapter, &m_pAdapter);
	SKMAC_ASSERT(!FAILED(hResult));

	DXGI_ADAPTER_DESC AdapterDesc;
	hResult = m_pAdapter->GetDesc(&AdapterDesc);
	SKMAC_ASSERT(!FAILED(hResult));
	hResult = m_pAdapter->EnumOutputs(0, &m_pAdapterOutput);
	SKMAC_ASSERT(!FAILED(hResult));

	TCHAR TempName[128];
	SKWcsToMbs(TempName, 128, AdapterDesc.Description, 128);
	m_AdapterName = TempName;


	unsigned int DeviceFlags = 0; ;

	DeviceFlags |= D3D11_CREATE_DEVICE_SINGLETHREADED;
#ifdef _DEBUG
	DeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	UINT numDriverTypes = ARRAYSIZE(ms_dwDevTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);
	D3D_FEATURE_LEVEL OutFeatureLevel;

	
	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		hResult = D3D11CreateDevice(
			NULL,
			(D3D_DRIVER_TYPE)ms_dwDevTypes[driverTypeIndex],
			NULL,
			DeviceFlags,
			featureLevels,
			numFeatureLevels,
			D3D11_SDK_VERSION,
			&m_pDevice,
			&OutFeatureLevel,
			&m_pDeviceContext
			);
		if (SUCCEEDED(hResult))
		{
			m_uiDevType = driverTypeIndex;
			break;
		}
	}
	m_uiMaxMultisample = 0;
	unsigned int NumMultiSampleQualities = 0;
	for (int uiMultiSampleTypes = MS_MAX - 1; uiMultiSampleTypes >= MS_NONE; uiMultiSampleTypes--)
	{
		hResult = m_pDevice->CheckMultisampleQualityLevels((DXGI_FORMAT)ms_dwTextureFormatType[SFT_A8R8G8B8], ms_dwMultiSampleTypes[uiMultiSampleTypes], &NumMultiSampleQualities);
		if (SUCCEEDED(hResult) && NumMultiSampleQualities > 0)
		{
			m_uiMaxMultisample = uiMultiSampleTypes;
			break;
		}
	}

	

	
	

	m_pMainChain = NULL;
	m_uiDisplayFormat = SFT_A8R8G8B8;
	m_uiBufferFormat = SFT_A8R8G8B8;
	m_uiDepthStencilFormat = SFT_D24S8;

	SKMemset(&m_SwapChainDesc, 0, sizeof(m_SwapChainDesc));
	m_SwapChainDesc.BufferCount = 1;
	m_SwapChainDesc.BufferDesc.Width = uiScreenWidth;
	m_SwapChainDesc.BufferDesc.Height = uiScreenHeight;
	m_SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	m_SwapChainDesc.OutputWindow = m_hMainWindow;
	m_SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	m_SwapChainDesc.Windowed = m_bWindowed;
	m_SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	m_SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	
	m_SwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	m_SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;


	if (uiMultisample > m_uiMaxMultisample)
	{
		m_uiCurMultisample = m_uiMaxMultisample;
	}
	else
	{
		m_uiCurMultisample = uiMultisample;
	}

	m_uiCurRTMultisampler = m_uiCurMultisample;
	hResult = m_pDevice->CheckMultisampleQualityLevels((DXGI_FORMAT)ms_dwTextureFormatType[SFT_A8R8G8B8], ms_dwMultiSampleTypes[m_uiCurMultisample], &NumMultiSampleQualities);
	SKMAC_ASSERT(!FAILED(hResult) && NumMultiSampleQualities > 0);
	m_dwMultisampleQuality = NumMultiSampleQualities;
		

	m_SwapChainDesc.SampleDesc.Count = ms_dwMultiSampleTypes[m_uiCurMultisample];
	m_SwapChainDesc.SampleDesc.Quality = m_dwMultisampleQuality - 1;
	IDXGIDevice * pDXGIDevice;
	hResult = m_pDevice->QueryInterface(IID_IDXGIDevice,(void**)&pDXGIDevice);
	SKMAC_ASSERT(!FAILED(hResult));
	hResult = ms_pDXGI->CreateSwapChain(pDXGIDevice, &m_SwapChainDesc, &m_pMainChain);
	SKMAC_ASSERT(!FAILED(hResult));
		

	ID3D11Texture2D* pBackBuffer = NULL;
	hResult = m_pMainChain->GetBuffer(0, IID_ID3D11Texture2D, (LPVOID*)&pBackBuffer);
	SKMAC_ASSERT(!FAILED(hResult));

	hResult = m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pMainRenderTargetView);
	SKMAC_ASSERT(!FAILED(hResult));
	SKMAC_RELEASE(pBackBuffer);


	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = uiScreenWidth;
	descDepth.Height = uiScreenHeight;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = ms_dwMultiSampleTypes[m_uiCurMultisample];
	descDepth.SampleDesc.Quality = m_dwMultisampleQuality - 1;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hResult = m_pDevice->CreateTexture2D(&descDepth, NULL, &m_pMainDepthStencil);
	SKMAC_ASSERT(!FAILED(hResult));
		

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hResult = m_pDevice->CreateDepthStencilView(m_pMainDepthStencil, &descDSV, &m_pMainDepthStencilView);
	SKMAC_ASSERT(!FAILED(hResult));

	m_uiCurAnisotropy = D3D11_MAX_MAXANISOTROPY;
	

	//m_uiPixelShaderVersion = d3dCaps.PixelShaderVersion;

	m_uiMaxTexture = (TEXLEVEL < D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT) ? TEXLEVEL : D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT;
	m_uiMaxRTNum = D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT;
	m_uiMaxVTexture = (TEXLEVEL < D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT) ? TEXLEVEL : D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT;
	m_uiMaxGTexture = (TEXLEVEL < D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT) ? TEXLEVEL : D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT;
	m_uiMaxDTexture = (TEXLEVEL < D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT) ? TEXLEVEL : D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT;
	m_uiMaxHTexture = (TEXLEVEL < D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT) ? TEXLEVEL : D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT;

	m_uiMaxTextureHeight = 8192;
	m_uiMaxTextureWidth = 8192;
	m_uiMaxUseClipPlane = 8;
	

	
	m_pChain = NULL;
	m_pChainRenderTargetView = NULL;
	m_pChainDepthStencil = NULL;
	m_pChainDepthStencilView = NULL;
	if ( (m_iNumChildWindow > 0) && m_bWindowed) 
	{
		m_pChain = SK_NEW IDXGISwapChain*[m_iNumChildWindow];
		m_pChainRenderTargetView = SK_NEW ID3D11RenderTargetView*[m_iNumChildWindow];
		m_pChainDepthStencil = SK_NEW ID3D11Texture2D*[m_iNumChildWindow];
		m_pChainDepthStencilView = SK_NEW ID3D11DepthStencilView*[m_iNumChildWindow];
		for (int i = 0 ; i < m_iNumChildWindow ;i++)
		{
			m_pChain[i] = NULL;
			m_pChainRenderTargetView[i] = NULL;
			m_pChainDepthStencil[i] = NULL;
			m_pChainDepthStencilView[i] = NULL;
		}
		for (int i = 0; i < m_iNumChildWindow; i++) 
		{
			m_SwapChainDesc.OutputWindow = m_pChildWindowInfo[i].m_hHwnd;
			m_SwapChainDesc.BufferDesc.Width = m_pChildWindowInfo[i].m_uiWidth;
			m_SwapChainDesc.BufferDesc.Height = m_pChildWindowInfo[i].m_uiHeight;
			hResult = ms_pDXGI->CreateSwapChain(pDXGIDevice, &m_SwapChainDesc, &m_pChain[i]);
			SKMAC_ASSERT(!FAILED(hResult));
			ID3D11Texture2D* pBackBuffer = NULL;
			hResult = m_pChain[i]->GetBuffer(0, IID_ID3D11Texture2D, (LPVOID*)&pBackBuffer);
			SKMAC_ASSERT(!FAILED(hResult));
			hResult = m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pChainRenderTargetView[i]);
			SKMAC_ASSERT(!FAILED(hResult));
			SKMAC_RELEASE(pBackBuffer);
			if (m_pChildWindowInfo[i].m_bDepth)
			{
				descDepth.Width = m_pChildWindowInfo[i].m_uiWidth;
				descDepth.Height = m_pChildWindowInfo[i].m_uiHeight;
				hResult = m_pDevice->CreateTexture2D(&descDepth, NULL, &m_pChainDepthStencil[i]);
				SKMAC_ASSERT(!FAILED(hResult));
				hResult = m_pDevice->CreateDepthStencilView(m_pChainDepthStencil[i], &descDSV, &m_pChainDepthStencilView[i]);
				SKMAC_ASSERT(!FAILED(hResult));
			}
		}
	}
	SKMAC_RELEASE(pDXGIDevice);
	SetDefaultValue();
	SKMemset(m_uiShaderBuffer, 0, MAX_SHADER_BUFFER);
	SKMemset(m_pTexAllStateBuffer,0,sizeof(SKTexAllState *)* MAX_TEXTURE_BUFFER);

	m_pDeviceContext->OMSetRenderTargets(1, &m_pMainRenderTargetView, m_pMainDepthStencilView);
	SetViewPort();


	unsigned int uiFontNum = 200;
	D3D11_BUFFER_DESC VertexBufferDesc;
	VertexBufferDesc.ByteWidth = sizeof(SCREEN_QUAD_TYPE)* 4 * uiFontNum;
	VertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; 
	VertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	VertexBufferDesc.MiscFlags = 0;
	hResult = m_pDevice->CreateBuffer(&VertexBufferDesc, NULL, &m_pFontVertexBuffer);
	SKMAC_ASSERT(!FAILED(hResult));
	D3D11_BUFFER_DESC IndexBufferDesc;
	IndexBufferDesc.ByteWidth = sizeof(SKUSHORT_INDEX) * 6 * uiFontNum;
	IndexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	IndexBufferDesc.MiscFlags = 0;
	hResult = m_pDevice->CreateBuffer(&IndexBufferDesc, NULL, &m_pFontIndexBuffer);
	SKMAC_ASSERT(!FAILED(hResult));
}
SKDX11Renderer::~SKDX11Renderer()
{
	
 	ReleaseDefaultValue();
	SKMAC_RELEASE(m_pFontVertexBuffer);
	SKMAC_RELEASE(m_pFontIndexBuffer);
	SKMAC_RELEASE(m_pMainDepthStencilView);
	SKMAC_RELEASE(m_pMainRenderTargetView);
 	for (int i = 0 ; i < m_iNumChildWindow ;i++)
 	{
		SKMAC_RELEASE(m_pChainDepthStencilView[i]);
		SKMAC_RELEASE(m_pChainDepthStencil[i]);	
 		SKMAC_RELEASE(m_pChainRenderTargetView[i]);
		SKMAC_RELEASE(m_pChain[i]);
 	}
	SKMAC_RELEASE(m_pMainDepthStencil);
	SKMAC_RELEASE(m_pMainChain);

	SKMAC_DELETEA(m_pChain);
	SKMAC_DELETEA(m_pChainRenderTargetView);
	SKMAC_DELETEA(m_pChainDepthStencil);
	SKMAC_DELETEA(m_pChainDepthStencilView);

	SKMAC_RELEASE(m_pAdapter);
	SKMAC_RELEASE(m_pAdapterOutput);
	SKMAC_RELEASE(m_pDeviceContext);
 	SKMAC_RELEASE(m_pDevice);
}
IMPLEMENT_PRIORITY(SKDX11Renderer)
IMPLEMENT_INITIAL_ONLY_BEGIN(SKDX11Renderer)
ADD_PRIORITY(SKResourceManager);
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState);
IMPLEMENT_INITIAL_ONLY_END
bool SKDX11Renderer::InitialDefaultState()
{
	IDXGIAdapter* pAdapter;
	IDXGIOutput* pAdapterOutput;
	SKMAC_RELEASE(ms_pDXGI);
	HRESULT hResult = NULL;
	hResult = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&ms_pDXGI);
	SKMAC_ASSERT(!FAILED(hResult));
	SKMAC_ASSERT(ms_pDXGI);
	ms_uiAdapterNum = 0;
	if (ms_pDXGI->EnumAdapters(ms_uiAdapterNum, &pAdapter) == DXGI_ERROR_NOT_FOUND)
	{
		return false;
	}
	if (pAdapter->EnumOutputs(0, &pAdapterOutput) == DXGI_ERROR_NOT_FOUND)
	{
		return false;
	}
	SKMAC_RELEASE(pAdapterOutput);
	SKMAC_RELEASE(pAdapter);
	ms_uiAdapterNum++;
	while (true)
	{
		if (ms_pDXGI->EnumAdapters(ms_uiAdapterNum, &pAdapter) != DXGI_ERROR_NOT_FOUND)
		{
			if (pAdapter->EnumOutputs(0, &pAdapterOutput) != DXGI_ERROR_NOT_FOUND)
			{
				SKMAC_RELEASE(pAdapterOutput);
				SKMAC_RELEASE(pAdapter);
				ms_uiAdapterNum++;
			}
			else
			{
				SKMAC_RELEASE(pAdapter);
				break;
			}
			
		}
		else
		{
			break;
		}
	}

	DXGI_ADAPTER_DESC AdapterDesc;
	for (unsigned int nAdapter = 0; nAdapter < ms_uiAdapterNum; nAdapter++)
	{
		hResult = ms_pDXGI->EnumAdapters(nAdapter, &pAdapter);
		SKMAC_ASSERT(!FAILED(hResult));
		hResult = pAdapter->GetDesc(&AdapterDesc);
		SKMAC_ASSERT(!FAILED(hResult));
		hResult = pAdapter->EnumOutputs(0, &pAdapterOutput);
		SKMAC_ASSERT(!FAILED(hResult));
		TCHAR TempName[128];
		SKWcsToMbs(TempName, 128, AdapterDesc.Description, 128);
		ms_AdapterInfo[nAdapter].AdapterName = TempName;

		unsigned int DisplayModeNumA8R8G8B8 = 0;
		hResult = pAdapterOutput->GetDisplayModeList((DXGI_FORMAT)ms_dwTextureFormatType[SKRenderer::SFT_A8R8G8B8], DXGI_ENUM_MODES_INTERLACED, &DisplayModeNumA8R8G8B8, NULL);
		SKMAC_ASSERT(!FAILED(hResult));
		ms_AdapterInfo[nAdapter].uiDisplayModeNum = DisplayModeNumA8R8G8B8;
		ms_AdapterInfo[nAdapter].DisplayMode.Clear();
		ms_AdapterInfo[nAdapter].DisplayMode.SetBufferNum(DisplayModeNumA8R8G8B8);
		DXGI_MODE_DESC* displayModes = SK_NEW DXGI_MODE_DESC[DisplayModeNumA8R8G8B8];
		hResult = pAdapterOutput->GetDisplayModeList((DXGI_FORMAT)ms_dwTextureFormatType[SKRenderer::SFT_A8R8G8B8], DXGI_ENUM_MODES_INTERLACED, &DisplayModeNumA8R8G8B8, displayModes);
		SKMAC_ASSERT(!FAILED(hResult));
		SKMAC_RELEASE(pAdapterOutput);
		SKMAC_RELEASE(pAdapter);
		for (UINT nMode = 0; nMode < DisplayModeNumA8R8G8B8; nMode++)
		{
			TCHAR temp[20];
			
			ms_AdapterInfo[nAdapter].DisplayMode[nMode].uiBitFormat = SKRenderer::SFT_A8R8G8B8;
			ms_AdapterInfo[nAdapter].DisplayMode[nMode].uiHeight = displayModes[nMode].Height;
			ms_AdapterInfo[nAdapter].DisplayMode[nMode].uiWidth = displayModes[nMode].Width;
			ms_AdapterInfo[nAdapter].DisplayMode[nMode].uiRefreshRate = displayModes[nMode].RefreshRate.Numerator / displayModes[nMode].RefreshRate.Denominator;
			SKSprintf(temp,20, "%d * %d * 32", displayModes[nMode].Width, displayModes[nMode].Height);
			ms_AdapterInfo[nAdapter].DisplayMode[nMode].StringExpress = temp;
		}
		SKMAC_DELETEA(displayModes);
	}

	SKString RenderAPIPre = SKResourceManager::GetRenderTypeShaderPath(RAT_DIRECTX11);
	SKFile IncludeShaderFile;
	SKString IncludeShaderPath = SKResourceManager::ms_ShaderPath + RenderAPIPre + _T("SKShader.txt");
	if (!IncludeShaderFile.Open(IncludeShaderPath.GetBuffer(),SKFile::OM_RB))
	{
		return false;
	}
	unsigned int uiIncludeSize = IncludeShaderFile.GetFileSize();
	ms_IncludeShader.SetTCHARBufferNum(uiIncludeSize);
	if (!IncludeShaderFile.Read(ms_IncludeShader.GetBuffer(),uiIncludeSize,1))
	{
		return false;
	}


	SKFile DynamicShaderFile;
	SKString DynamicShaderPath = SKResourceManager::ms_ShaderPath + RenderAPIPre + _T("SKDymaticSKShader.txt");
	if(!DynamicShaderFile.Open(DynamicShaderPath.GetBuffer(),SKFile::OM_RB))
	{
		return false;
	}
	unsigned int uiSize = DynamicShaderFile.GetFileSize();
	SKString VDynamicShaderString;
	VDynamicShaderString.SetTCHARBufferNum(uiSize);
	if(!DynamicShaderFile.Read(VDynamicShaderString.GetBuffer(),uiSize,1))
	{
		return false;
	}
	ms_DynamicShader.Format(VDynamicShaderString.GetBuffer(),SKResourceManager::GetGpuSkinBoneNum() * 3);

	ms_pDx11IncludeShader = SK_NEW SKDx11ShaderInclude();
	SKMAC_ASSERT(ms_pDx11IncludeShader)
	return 1;
}
bool SKDX11Renderer::TerminalDefaultState()
{
	SKMAC_DELETE(ms_pDx11IncludeShader);
	SKMAC_RELEASE(ms_pDXGI);
	return 1;
}
void SKDX11Renderer::ClearBuffers(unsigned int uiClearFlag)
{ 
	if (!uiClearFlag)
	{
		return;
	}
	DWORD Flag = 0;
	if (uiClearFlag & CF_COLOR)
	{
		ClearBackBuffer();
	}
	
	if (uiClearFlag & CF_DEPTH)
	{
		Flag |= D3D11_CLEAR_DEPTH;
	}
	if (uiClearFlag & CF_STENCIL)
	{
		Flag |= D3D11_CLEAR_STENCIL;
	}
	if (Flag)
	{
		ClearDepthStencilView(Flag, m_fClearDepth, m_uiClearStencil);
	}
}


void SKDX11Renderer::ClearZBuffer()
{
	ClearDepthStencilView(D3D11_CLEAR_DEPTH, m_fClearDepth, m_uiClearStencil);
	
}
void SKDX11Renderer::ClearStencilBuffer()
{ 
	ClearDepthStencilView(D3D11_CLEAR_STENCIL, m_fClearDepth, m_uiClearStencil);
	
}
void SKDX11Renderer::ClearBuffers()
{
	ClearBuffers(CF_COLOR | CF_DEPTH |CF_STENCIL);
}
unsigned int SKDX11Renderer::SetDepthStencilState(SKDepthStencilState * pDepthStencilState, bool bForceSet)
{
	unsigned int FRI = SKRenderer::SetDepthStencilState(pDepthStencilState, bForceSet);
	if (FRI == FRI_SAMERESOURCE)
		return 1;
	else if (FRI == FRI_FAIL)
	{
		OMSetDepthStencilState(NULL, 0);
		return 0;
	}
	if (!pDepthStencilState)
	{
		pDepthStencilState = (SKDepthStencilState *)SKDepthStencilState::GetDefault();
	}
	SKDepthStencilStateID *pDepthStencilStateID = (SKDepthStencilStateID *)pDepthStencilState->GetIdentifier();
	if (!pDepthStencilStateID)
		return 0;

	m_pDepthStencilState = pDepthStencilState;
	OMSetDepthStencilState(pDepthStencilStateID, pDepthStencilState->GetDepthStencilDesc().m_uiReference);
	return true;
}
unsigned int SKDX11Renderer::SetRasterizerState(SKRasterizerState * pRasterizerState, bool bForceSet)
{
	unsigned int FRI = SKRenderer::SetRasterizerState(pRasterizerState, bForceSet);
	if (FRI == FRI_SAMERESOURCE)
		return 1;
	else if (FRI == FRI_FAIL)
	{
		RSSetState(NULL);
		return 0;
	}
	if (!pRasterizerState)
	{
		pRasterizerState = (SKRasterizerState *)SKRasterizerState::GetDefault();
	}
	SKRasterStateID *pRasterStateID = (SKRasterStateID *)pRasterizerState->GetIdentifier();
	if (!pRasterStateID)
		return 0;

	m_pRasterizerState = pRasterizerState;
	RSSetState(pRasterStateID);
	return true;
}
void SKDX11Renderer::ClearBuffers(unsigned int uiClearFlag, int iXPos, int iYPos, int iWidth,
	int iHeight)
{
	
// 	SKRasterizerState * pEnableScissor = ((SKRasterizerState *)SKRasterizerState::GetScissorEnable());
// 	if (!LoadRasterizerState(pEnableScissor))
// 	{
// 		ClearBuffers(uiClearFlag);
// 		return;
// 	}
// 
// 	UINT SRNum = 0;
// 	D3D11_RECT Temp[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
// 	m_pDeviceContext->RSGetScissorRects(&SRNum, Temp);
// 	ID3D11RasterizerState * pTempState = NULL;
// 	m_pDeviceContext->RSGetState(&pTempState);
// 
// 	SKRasterStateID *pRasterStateID = (SKRasterStateID *)pEnableScissor->GetIdentifier();
// 	if (!pRasterStateID)
// 		return ;
// 	m_pDeviceContext->RSSetState(pRasterStateID->m_pRasterState);
// 	m_pDeviceContext->RSSetScissorRects(0,NULL);
// 	ClearBuffers(uiClearFlag);
// 
// 	m_pDeviceContext->RSSetState(pTempState);
// 	m_pDeviceContext->RSSetScissorRects(SRNum, Temp);

	ClearBuffers(uiClearFlag);
}
void SKDX11Renderer::ClearBackBuffer(int iXPos, int iYPos, int iWidth,
	int iHeight)
{
// 	SKRasterizerState * pEnableScissor = ((SKRasterizerState *)SKRasterizerState::GetScissorEnable());
// 	if (!LoadRasterizerState(pEnableScissor))
// 	{
// 		ClearBackBuffer();
// 		return;
// 	}
// 
// 	UINT SRNum = 0;
// 	D3D11_RECT Temp[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
// 	m_pDeviceContext->RSGetScissorRects(&SRNum, Temp);
// 	ID3D11RasterizerState * pTempState = NULL;
// 	m_pDeviceContext->RSGetState(&pTempState);
// 
// 	SKRasterStateID *pRasterStateID = (SKRasterStateID *)pEnableScissor->GetIdentifier();
// 	if (!pRasterStateID)
// 		return;
// 	D3D11_RECT Rect = { iXPos, iYPos, iXPos + iWidth - 1, iYPos + iHeight - 1 };
// 	m_pDeviceContext->RSSetState(pRasterStateID->m_pRasterState);
// 	m_pDeviceContext->RSSetScissorRects(1, &Rect);
// 	ClearBackBuffer();
// 
// 	m_pDeviceContext->RSSetState(pTempState);
// 	m_pDeviceContext->RSSetScissorRects(SRNum, Temp);
	ClearBackBuffer();
}
void SKDX11Renderer::ClearZBuffer(int iXPos, int iYPos, int iWidth,
	int iHeight)
{
// 	SKRasterizerState * pEnableScissor = ((SKRasterizerState *)SKRasterizerState::GetScissorEnable());
// 	if (!LoadRasterizerState(pEnableScissor))
// 	{
// 		ClearZBuffer();
// 		return;
// 	}
// 
// 	UINT SRNum = 0;
// 	D3D11_RECT Temp[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
// 	m_pDeviceContext->RSGetScissorRects(&SRNum, Temp);
// 	ID3D11RasterizerState * pTempState = NULL;
// 	m_pDeviceContext->RSGetState(&pTempState);
// 
// 	SKRasterStateID *pRasterStateID = (SKRasterStateID *)pEnableScissor->GetIdentifier();
// 	if (!pRasterStateID)
// 		return;
//	D3D11_RECT Rect = { iXPos, iYPos, iXPos + iWidth - 1, iYPos + iHeight - 1 };
//	m_pDeviceContext->RSSetState(pRasterStateID->m_pRasterState);
//	m_pDeviceContext->RSSetScissorRects(1, &Rect);
// 	ClearZBuffer();
// 
// 	m_pDeviceContext->RSSetState(pTempState);
// 	m_pDeviceContext->RSSetScissorRects(SRNum, Temp);
	ClearZBuffer();
}
void SKDX11Renderer::ClearStencilBuffer(int iXPos, int iYPos, int iWidth,
	int iHeight)
{
// 	SKRasterizerState * pEnableScissor = ((SKRasterizerState *)SKRasterizerState::GetScissorEnable());
// 	if (!LoadRasterizerState(pEnableScissor))
// 	{
// 		ClearStencilBuffer();
// 		return;
// 	}
// 
// 	UINT SRNum = 0;
// 	D3D11_RECT Temp[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
// 	m_pDeviceContext->RSGetScissorRects(&SRNum, Temp);
// 	ID3D11RasterizerState * pTempState = NULL;
// 	m_pDeviceContext->RSGetState(&pTempState);
// 
// 	SKRasterStateID *pRasterStateID = (SKRasterStateID *)pEnableScissor->GetIdentifier();
// 	if (!pRasterStateID)
// 		return;
//	D3D11_RECT Rect = { iXPos, iYPos, iXPos + iWidth - 1, iYPos + iHeight - 1 };
//	m_pDeviceContext->RSSetState(pRasterStateID->m_pRasterState);
//	m_pDeviceContext->RSSetScissorRects(1, &Rect);
// 	ClearStencilBuffer();
// 
// 	m_pDeviceContext->RSSetState(pTempState);
// 	m_pDeviceContext->RSSetScissorRects(SRNum, Temp);
	ClearStencilBuffer();
}
void SKDX11Renderer::ClearBuffers(int iXPos, int iYPos, int iWidth,
	int iHeight)
{
// 	SKRasterizerState * pEnableScissor = ((SKRasterizerState *)SKRasterizerState::GetScissorEnable());
// 	if (!LoadRasterizerState(pEnableScissor))
// 	{
// 		ClearBuffers(CF_COLOR | CF_DEPTH |CF_STENCIL);
// 		return;
// 	}
// 
// 	UINT SRNum = 0;
// 	D3D11_RECT Temp[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
// 	m_pDeviceContext->RSGetScissorRects(&SRNum, Temp);
// 	ID3D11RasterizerState * pTempState = NULL;
// 	m_pDeviceContext->RSGetState(&pTempState);
// 
// 	SKRasterStateID *pRasterStateID = (SKRasterStateID *)pEnableScissor->GetIdentifier();
// 	if (!pRasterStateID)
// 		return;
//	D3D11_RECT Rect = { iXPos, iYPos, iXPos + iWidth - 1, iYPos + iHeight - 1 };
//	m_pDeviceContext->RSSetState(pRasterStateID->m_pRasterState);
//	m_pDeviceContext->RSSetScissorRects(1, &Rect);
// 	ClearBuffers(CF_COLOR | CF_DEPTH |CF_STENCIL);
// 
// 	m_pDeviceContext->RSSetState(pTempState);
// 	m_pDeviceContext->RSSetScissorRects(SRNum, Temp);
	ClearBuffers(CF_COLOR | CF_DEPTH | CF_STENCIL);
}
bool SKDX11Renderer::SetViewPort(SKViewPort *pViewPort)
{
	SKRenderer::SetViewPort(pViewPort);
	D3D11_VIEWPORT View;
	
	if (pViewPort)
	{
		SKViewPort ViewPort;
		ViewPort = *pViewPort;
		View.TopLeftX = ViewPort.XMin * m_uiCurRTWidth;
		View.TopLeftY = ViewPort.YMin * m_uiCurRTHeight;
		View.Width = ViewPort.XMax * m_uiCurRTWidth - View.TopLeftX;
		View.Height = ViewPort.YMax * m_uiCurRTHeight - View.TopLeftY;
		View.MinDepth = ViewPort.ZMin;
		View.MaxDepth = ViewPort.ZMax;
	}
	else
	{
		View.TopLeftX = 0;
		View.TopLeftY = 0;
		View.Width = (SKREAL)m_uiCurRTWidth;
		View.Height = (SKREAL)m_uiCurRTHeight;
		View.MinDepth = 0.0f;
		View.MaxDepth = 1.0f;

	}

	RSSetViewports(View);
	return true;
}
bool SKDX11Renderer::UseWindow(int uiWindowID)
{ 
	if (!SKRenderer::UseWindow(uiWindowID))
		return 0;
	UseWindowEx(uiWindowID);
	return true;
}

bool SKDX11Renderer::CooperativeLevel()
{ 
	return true; 
}
bool SKDX11Renderer::BeginRendering()
{ 
	SKRenderer::BeginRendering();
	return true;
}
bool SKDX11Renderer::EndRendering()
{ 
	SKRenderer::EndRendering(); 
	EndRender();
	
	return true;
}
unsigned int SKDX11Renderer::SetTexture(SKTexture* pTexture, unsigned int i)
{ 
	unsigned int FRI = SKRenderer::SetTexture(pTexture, i);
	if (FRI == FRI_SAMERESOURCE)
		return 1;
	else if (FRI == FRI_FAIL)
	{
		PSSetShaderResources(i, NULL);
		return 0;
	}
	SKTextureID *pTextureID = (SKTextureID *)pTexture->GetIdentifier();
	if (!pTextureID)
		return 0;

	PSSetShaderResources(i, pTextureID);

	return 1;
}
ID3DBlob* SKDX11Renderer::CreateVertexFormatShader(const SKArray<SKVertexFormat::VERTEXFORMAT_TYPE>& Layouts)
{
	unsigned int SemanticNum = sizeof(ms_dwVertexFormatSemantics) / sizeof(TCHAR *);
	SKString ShaderString = _T("struct SK_INPUT{");
	for (unsigned int i = 0; i < Layouts.GetNum(); i++)
	{
		SKString DeclareString = ms_dwVertexFormatClareType[Layouts[i].DateType];
		SKString NameString = ms_dwVertexFormatSemantics[Layouts[i].Semantics];
		SKString IndexString = IntToString(Layouts[i].SemanticsIndex);
		ShaderString += DeclareString + NameString + IndexString + _T(" : ") + NameString + IndexString + _T(";");
	}
	ShaderString += _T("};float4 VS(SK_INPUT input) : SV_POSITION{return float4(0.0f,0.0f,0.0f,0.0f);}");

	HRESULT hResult = NULL;
	ID3DBlob* pCode = NULL;
	ID3DBlob* pErrors = NULL;
	DWORD Flags = D3DCOMPILE_WARNINGS_ARE_ERRORS | D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	Flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	Flags = D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

	hResult = D3DCompile((LPCSTR)ShaderString.GetBuffer(), ShaderString.GetLength(),
		NULL, NULL, ms_pDx11IncludeShader, _T("VS"),
		ms_cVertexShaderProgramVersion, Flags, 0, &pCode, &pErrors);
#ifdef _DEBUG
	if (pErrors && pErrors->GetBufferPointer())
	{
		SKLog Error;
		SKString SKErrorString = SKResourceManager::ms_OutPutLogPath + _T("SKError.txt");
		Error.Open(SKErrorString.GetBuffer());
		Error.WriteInfo("\n");
		Error.WriteInfo((const TCHAR *)pErrors->GetBufferPointer());
		Error.WriteInfo("\n");
		SKOutPutDebugString((const TCHAR *)pErrors->GetBufferPointer());
	}
#endif
	SKMAC_ASSERT(!FAILED(hResult));
	SKMAC_ASSERT(pCode);
	SKMAC_RELEASE(pErrors);
	return pCode;

}
bool SKDX11Renderer::OnLoadVBufferFormat(SKVertexFormat * pVertexFormat, SKResourceIdentifier *&pID)
{ 
	if (!pVertexFormat->m_FormatArray.GetNum())
		return 0;
	SKVBufferFormatID * pVBufferFormatID = NULL;
	pVBufferFormatID = SK_NEW SKVBufferFormatID;
	if (!pVBufferFormatID)
		return 0;
	pID = pVBufferFormatID;

	
	CreateInputLayout(pVertexFormat->m_FormatArray, pVBufferFormatID);
	return true; 
}
bool SKDX11Renderer::OnReleaseVBufferFormat(SKResourceIdentifier* pVBufferID)
{ 
	SKMAC_DELETE(pVBufferID);
	return true;
}
void SKDX11Renderer::DeviceLost()
{ 
	m_pDeviceContext->OMSetRenderTargets(0, NULL, NULL);
	SKMAC_RELEASE(m_pMainRenderTargetView);
	SKMAC_RELEASE(m_pMainDepthStencilView);
	SKMAC_RELEASE(m_pMainDepthStencil);
	m_pDeviceContext->ClearState();
	m_pDeviceContext->Flush();
}
void SKDX11Renderer::ResetDevice()
{ 
	HRESULT hResult = NULL;
	hResult = m_pMainChain->ResizeBuffers(1, m_uiScreenWidth, m_uiScreenHeight, m_SwapChainDesc.BufferDesc.Format, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
	SKMAC_ASSERT(!FAILED(hResult));
	DXGI_MODE_DESC BufferDesc = m_SwapChainDesc.BufferDesc;
	hResult = m_pMainChain->ResizeTarget(&BufferDesc);
	SKMAC_ASSERT(!FAILED(hResult));
	hResult = m_pMainChain->SetFullscreenState(!m_bWindowed, NULL);
	SKMAC_ASSERT(!FAILED(hResult));
	ID3D11Texture2D* pBackBuffer = NULL;
	hResult = m_pMainChain->GetBuffer(0, IID_ID3D11Texture2D, (LPVOID*)&pBackBuffer);
	SKMAC_ASSERT(!FAILED(hResult));
	hResult = m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pMainRenderTargetView);
	SKMAC_ASSERT(!FAILED(hResult));
	SKMAC_RELEASE(pBackBuffer);

	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = m_uiScreenWidth;
	descDepth.Height = m_uiScreenHeight;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = ms_dwMultiSampleTypes[m_uiCurMultisample];
	descDepth.SampleDesc.Quality = m_dwMultisampleQuality - 1;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hResult = m_pDevice->CreateTexture2D(&descDepth, NULL, &m_pMainDepthStencil);
	SKMAC_ASSERT(!FAILED(hResult));


	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hResult = m_pDevice->CreateDepthStencilView(m_pMainDepthStencil, &descDSV, &m_pMainDepthStencilView);
	SKMAC_ASSERT(!FAILED(hResult));
	m_pDeviceContext->OMSetRenderTargets(1, &m_pMainRenderTargetView, m_pMainDepthStencilView);
	SetViewPort();

	RenderStateToDefault();
}
bool SKDX11Renderer::ChangeScreenSize(unsigned int uiWidth, unsigned int uiHeight, bool bWindow)
{
	if (!SKRenderer::ChangeScreenSize(uiWidth, uiHeight, bWindow))
	{
		return false;
	}
	m_SwapChainDesc.Windowed = bWindow;
	m_SwapChainDesc.BufferDesc.Height = m_uiScreenWidth;
	m_SwapChainDesc.BufferDesc.Width = m_uiScreenWidth;
	DeviceLost();
	ResetDevice();
	return true;
}
bool SKDX11Renderer::DrawMesh(SKGeometry * pGeometry, SKRenderState * pRenderState, SKVShader * pVShader, SKPShader * pPShader, SKGShader * pGShader)
{
	ADD_TIME_PROFILE(Dx11DrawMesh);
	if (!SKRenderer::DrawMesh(pGeometry, pRenderState, pVShader, pPShader, pGShader))
		return 0;
	SKDynamicBufferGeometry * pDBGeometry = DynamicCast<SKDynamicBufferGeometry>(pGeometry);
	if (pDBGeometry)
	{
		DrawDynamicBufferMesh(pDBGeometry);
	}
	else
	{
		if (pGeometry->GetMeshDate()->GetVertexBuffer()->GetSwapChainNum() == 1 &&
			pGeometry->GetMeshDate()->GetIndexBuffer()->GetSwapChainNum() == 1)
		{
			DrawMesh(pGeometry);
		}
		else
		{
			DrawMesh1(pGeometry);
		}
	}
	return true;
}
bool SKDX11Renderer::DrawScreen(SCREEN_QUAD_TYPE ScreenQuad[4])
{
	SKRenderer::DrawScreen(ScreenQuad);
	if (!ScreenQuad)
	{
		ScreenQuad = ms_FullScreen;
	}
	return DrawScreen(ScreenQuad, 4, ms_FullScreenI, 6);
}
bool SKDX11Renderer::DrawScreen(SCREEN_QUAD_TYPE * pScreenBuffer, unsigned int uiVertexNum,SKUSHORT_INDEX * pIndexBuffer, unsigned int uiIndexNum)
{
	if (!SKRenderer::DrawScreen(pScreenBuffer, uiVertexNum, pIndexBuffer, uiIndexNum))
	{
		return false;
	}
	DrawDynamicScreenData(pScreenBuffer, uiVertexNum, sizeof(SCREEN_QUAD_TYPE), pIndexBuffer, uiIndexNum);
	return true;
}
bool SKDX11Renderer::DrawScreenFont(SCREEN_FONT_TYPE * pScreenFontBuffer, unsigned int uiVertexNum,
	SKUSHORT_INDEX * pIndexBuffer, unsigned int uiIndexNum)
{
	if (!SKRenderer::DrawScreenFont(pScreenFontBuffer, uiVertexNum, pIndexBuffer, uiIndexNum))
	{
		return false;
	}
	DrawDynamicScreenData(pScreenFontBuffer, uiVertexNum, sizeof(SCREEN_FONT_TYPE), pIndexBuffer, uiIndexNum);
	return true;
}
unsigned int SKDX11Renderer::SetBlendState(SKBlendState * pBlendState, bool bForceSet)
{ 
	unsigned int FRI = SKRenderer::SetBlendState(pBlendState, bForceSet);
	if (FRI == FRI_SAMERESOURCE)
		return FRI_SUCCESS;
	SKMAC_ASSERT(FRI == FRI_SUCCESS);
	if (FRI == FRI_FAIL)
	{
		OMSetBlendState(NULL);
		return FRI_FAIL;
	}
	if (!pBlendState)
	{
		pBlendState = (SKBlendState *)SKBlendState::GetDefault();
	}
	

	m_pBlendState = pBlendState;

	SKBlendStateID* pBlendStateID = (SKBlendStateID *)m_pBlendState->GetIdentifier();
	if (!pBlendStateID)
		return 0;
	OMSetBlendState(pBlendStateID);
	
	return 1;
}
void SKDX11Renderer::SetClipPlane(SKArray<SKPlane3> &Plane, bool bEnable){ SKMAC_ASSERT(0); return; }
void SKDX11Renderer::SetScissorRect(SKArray<SKRect2> &Rect, bool bEnable)
{ 
	
	if (bEnable && Rect.GetNum())
	{
		D3D11_RECT Temp[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
		unsigned int i = 0;
		for (; i < Rect.GetNum(); i++)
		{
			if (i >= D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)
			{
				break;
			}

			Temp[i].left = (LONG)Rect[i].m_Min.x;
			Temp[i].top = (LONG)Rect[i].m_Min.y;
			Temp[i].right = (LONG)Rect[i].m_Max.x;
			Temp[i].bottom = (LONG)Rect[i].m_Max.y;			
		}
		RSSetScissorRects(i, Temp);
	}
	else
	{
		RSSetScissorRects(0,NULL);
	}
}
void SKDX11Renderer::SetVShaderConstant(SKVShader * pShader)
{
	
	if (!pShader->m_bCreatePara)
	{
		return;
	}
	if (!pShader->m_uiConstBufferSize)
	{
		return;
	}
	for (unsigned int i = 0; i < pShader->m_pUserConstant.GetNum(); i++)
	{
		SKUserConstant * pUserConstant = pShader->m_pUserConstant[i];
		unsigned int uiSize = pUserConstant->GetSize();
		
		unsigned int uiOffSet = pUserConstant->GetRegisterIndex();
		SKMemcpy(&m_uiShaderBuffer[uiOffSet], pUserConstant->GetDate(), uiSize);
	}
	SKVProgramID * pVShaderID = (SKVProgramID *)pShader->GetIdentifier();
	if (!pVShaderID || !pVShaderID->m_pShaderBuffer)
	{
		return;
	}
	UpdateSubresource((ID3D11Resource **)&pVShaderID->m_pShaderBuffer, m_uiShaderBuffer, pShader->m_uiConstBufferSize);
}
void SKDX11Renderer::SetVShaderSampler(SKVShader * pShader)
{
	SKMemset(m_pTexAllStateBuffer, 0, sizeof(SKTexAllState *)* MAX_TEXTURE_BUFFER);
	if (pShader->m_bCreatePara)
	{
		for (unsigned int uiTexid = 0; uiTexid < pShader->m_pUserSampler.GetNum(); uiTexid++)
		{
			if (pShader->m_pUserSampler[uiTexid]->GetRegisterIndex() + pShader->m_pUserSampler[uiTexid]->GetRegisterNum() <= m_uiMaxTexture)
			{
				for (unsigned int i = 0; i < pShader->m_pUserSampler[uiTexid]->GetRegisterNum(); i++)
				{
					m_pTexAllStateBuffer[pShader->m_pUserSampler[uiTexid]->GetRegisterIndex() + i] = pShader->m_pUserSampler[uiTexid]->GetTex(i);
				}
			}

		}
	}
	for (unsigned int k = 0; k < m_uiMaxTexture; k++)
	{
		SetVTexAllState(m_pTexAllStateBuffer[k], k);
	}
}

void SKDX11Renderer::SetPShaderSampler(SKPShader * pShader)
{
	SKMemset(m_pTexAllStateBuffer, 0, sizeof(SKTexAllState *)* MAX_TEXTURE_BUFFER);
	if (pShader->m_bCreatePara)
	{
		for (unsigned int uiTexid = 0; uiTexid < pShader->m_pUserSampler.GetNum(); uiTexid++)
		{
			if (pShader->m_pUserSampler[uiTexid]->GetRegisterIndex() + pShader->m_pUserSampler[uiTexid]->GetRegisterNum() <= m_uiMaxTexture)
			{
				for (unsigned int i = 0; i < pShader->m_pUserSampler[uiTexid]->GetRegisterNum(); i++)
				{
					m_pTexAllStateBuffer[pShader->m_pUserSampler[uiTexid]->GetRegisterIndex() + i] = pShader->m_pUserSampler[uiTexid]->GetTex(i);
				}
			}

		}
	}
	for (unsigned int k = 0; k < m_uiMaxTexture; k++)
	{
		SetTexAllState(m_pTexAllStateBuffer[k], k);
	}
}
void SKDX11Renderer::SetDShaderSampler(SKDShader * pShader)
{
	SKMemset(m_pTexAllStateBuffer, 0, sizeof(SKTexAllState *)* MAX_TEXTURE_BUFFER);
	if (pShader->m_bCreatePara)
	{
		for (unsigned int uiTexid = 0; uiTexid < pShader->m_pUserSampler.GetNum(); uiTexid++)
		{
			if (pShader->m_pUserSampler[uiTexid]->GetRegisterIndex() + pShader->m_pUserSampler[uiTexid]->GetRegisterNum() <= m_uiMaxGTexture)
			{
				for (unsigned int i = 0; i < pShader->m_pUserSampler[uiTexid]->GetRegisterNum(); i++)
				{
					m_pTexAllStateBuffer[pShader->m_pUserSampler[uiTexid]->GetRegisterIndex() + i] = pShader->m_pUserSampler[uiTexid]->GetTex(i);
				}
			}

		}
	}
	for (unsigned int k = 0; k < m_uiMaxDTexture; k++)
	{
		SetDTexAllState(m_pTexAllStateBuffer[k], k);
	}
}
void SKDX11Renderer::SetHShaderSampler(SKHShader * pShader)
{
	SKMemset(m_pTexAllStateBuffer, 0, sizeof(SKTexAllState *)* MAX_TEXTURE_BUFFER);
	if (pShader->m_bCreatePara)
	{
		for (unsigned int uiTexid = 0; uiTexid < pShader->m_pUserSampler.GetNum(); uiTexid++)
		{
			if (pShader->m_pUserSampler[uiTexid]->GetRegisterIndex() + pShader->m_pUserSampler[uiTexid]->GetRegisterNum() <= m_uiMaxGTexture)
			{
				for (unsigned int i = 0; i < pShader->m_pUserSampler[uiTexid]->GetRegisterNum(); i++)
				{
					m_pTexAllStateBuffer[pShader->m_pUserSampler[uiTexid]->GetRegisterIndex() + i] = pShader->m_pUserSampler[uiTexid]->GetTex(i);
				}
			}

		}
	}
	for (unsigned int k = 0; k < m_uiMaxHTexture; k++)
	{
		SetHTexAllState(m_pTexAllStateBuffer[k], k);
	}
}
void SKDX11Renderer::SetGShaderSampler(SKGShader * pShader)
{
	SKMemset(m_pTexAllStateBuffer, 0, sizeof(SKTexAllState *)* MAX_TEXTURE_BUFFER);
	if (pShader->m_bCreatePara)
	{
		for (unsigned int uiTexid = 0; uiTexid < pShader->m_pUserSampler.GetNum(); uiTexid++)
		{
			if (pShader->m_pUserSampler[uiTexid]->GetRegisterIndex() + pShader->m_pUserSampler[uiTexid]->GetRegisterNum() <= m_uiMaxGTexture)
			{
				for (unsigned int i = 0; i < pShader->m_pUserSampler[uiTexid]->GetRegisterNum(); i++)
				{
					m_pTexAllStateBuffer[pShader->m_pUserSampler[uiTexid]->GetRegisterIndex() + i] = pShader->m_pUserSampler[uiTexid]->GetTex(i);
				}
			}

		}
	}
	for (unsigned int k = 0; k < m_uiMaxGTexture; k++)
	{
		SetGTexAllState(m_pTexAllStateBuffer[k], k);
	}
}
unsigned int SKDX11Renderer::SetVShader(SKVShader * pVShader)
{
	if (!pVShader)
		return FRI_FAIL;

	if (pVShader == m_pVShader)
	{

		SetVShaderConstant(pVShader);
		SetVShaderSampler(pVShader);

		return FRI_SUCCESS;
	}
	if (!SetVShaderProgram(pVShader))
		return FRI_FAIL;

	m_pVShader = pVShader;
	SetVShaderConstant(pVShader);
	SetVShaderSampler(pVShader);
	return FRI_SUCCESS;
}
void SKDX11Renderer::SetPShaderConstant(SKPShader * pShader)
{

	if (!pShader->m_bCreatePara)
	{
		return;
	}
	if (!pShader->m_uiConstBufferSize)
	{
		return;
	}
	for (unsigned int i = 0; i < pShader->m_pUserConstant.GetNum(); i++)
	{
		SKUserConstant * pUserConstant = pShader->m_pUserConstant[i];
		unsigned int uiSize = pUserConstant->GetSize();	
		unsigned int uiOffSet = pUserConstant->GetRegisterIndex();
		SKMemcpy(&m_uiShaderBuffer[uiOffSet], pUserConstant->GetDate(), uiSize);
	}
	SKPProgramID * pPShaderID = (SKPProgramID *)pShader->GetIdentifier();
	if (!pPShaderID || !pPShaderID->m_pShaderBuffer)
	{
		return;
	}
	UpdateSubresource((ID3D11Resource **)&pPShaderID->m_pShaderBuffer, m_uiShaderBuffer, pShader->m_uiConstBufferSize);
}
void SKDX11Renderer::SetGShaderConstant(SKGShader * pShader)
{
	if (!pShader->m_bCreatePara)
	{
		return;
	}
	if (!pShader->m_uiConstBufferSize)
	{
		return;
	}
	for (unsigned int i = 0; i < pShader->m_pUserConstant.GetNum(); i++)
	{
		SKUserConstant * pUserConstant = pShader->m_pUserConstant[i];
		unsigned int uiSize = pUserConstant->GetSize();
		unsigned int uiOffSet = pUserConstant->GetRegisterIndex();
		SKMemcpy(&m_uiShaderBuffer[uiOffSet], pUserConstant->GetDate(), uiSize);
	}
	SKGProgramID * pGShaderID = (SKGProgramID *)pShader->GetIdentifier();
	if (!pGShaderID || !pGShaderID->m_pShaderBuffer)
	{
		return;
	}
	UpdateSubresource((ID3D11Resource **)&pGShaderID->m_pShaderBuffer, m_uiShaderBuffer, pShader->m_uiConstBufferSize);
}
void SKDX11Renderer::SetHShaderConstant(SKHShader * pShader)
{
	if (!pShader->m_bCreatePara)
	{
		return;
	}
	if (!pShader->m_uiConstBufferSize)
	{
		return;
	}
	for (unsigned int i = 0; i < pShader->m_pUserConstant.GetNum(); i++)
	{
		SKUserConstant * pUserConstant = pShader->m_pUserConstant[i];
		unsigned int uiSize = pUserConstant->GetSize();
		unsigned int uiOffSet = pUserConstant->GetRegisterIndex();
		SKMemcpy(&m_uiShaderBuffer[uiOffSet], pUserConstant->GetDate(), uiSize);
	}
	SKHProgramID * pHShaderID = (SKHProgramID *)pShader->GetIdentifier();
	if (!pHShaderID || !pHShaderID->m_pShaderBuffer)
	{
		return;
	}
	UpdateSubresource((ID3D11Resource **)&pHShaderID->m_pShaderBuffer, m_uiShaderBuffer, pShader->m_uiConstBufferSize);
}
void SKDX11Renderer::SetDShaderConstant(SKDShader * pShader)
{
	if (!pShader->m_bCreatePara)
	{
		return;
	}
	if (!pShader->m_uiConstBufferSize)
	{
		return;
	}
	for (unsigned int i = 0; i < pShader->m_pUserConstant.GetNum(); i++)
	{
		SKUserConstant * pUserConstant = pShader->m_pUserConstant[i];
		unsigned int uiSize = pUserConstant->GetSize();
		unsigned int uiOffSet = pUserConstant->GetRegisterIndex();
		SKMemcpy(&m_uiShaderBuffer[uiOffSet], pUserConstant->GetDate(), uiSize);
	}
	SKDProgramID * pDShaderID = (SKDProgramID *)pShader->GetIdentifier();
	if (!pDShaderID || !pDShaderID->m_pShaderBuffer)
	{
		return;
	}
	UpdateSubresource((ID3D11Resource **)&pDShaderID->m_pShaderBuffer, m_uiShaderBuffer, pShader->m_uiConstBufferSize);
}
unsigned int SKDX11Renderer::SetDShader(SKDShader * pDShader)
{
	if (m_pDShader == pDShader)
	{
		if (pDShader == NULL)
		{
			return FRI_SUCCESS;
		}
		SetDShaderConstant(pDShader);
		SetDShaderSampler(pDShader);
		return FRI_SUCCESS;
	}
	SetDShaderProgram(pDShader);

	m_pDShader = pDShader;
	if (pDShader == NULL)
	{
		return FRI_SUCCESS;
	}
	SetDShaderConstant(pDShader);
	SetDShaderSampler(pDShader);
	return FRI_SUCCESS;
}
unsigned int SKDX11Renderer::SetHShader(SKHShader * pHShader)
{
	if (m_pHShader == pHShader)
	{
		if (pHShader == NULL)
		{
			return FRI_SUCCESS;
		}
		SetHShaderConstant(pHShader);
		SetHShaderSampler(pHShader);
		return FRI_SUCCESS;
	}
	SetHShaderProgram(pHShader);

	m_pHShader = pHShader;
	if (pHShader == NULL)
	{
		return FRI_SUCCESS;
	}
	SetHShaderConstant(pHShader);
	SetHShaderSampler(pHShader);
	return FRI_SUCCESS;
}
unsigned int SKDX11Renderer::SetGShader(SKGShader * pGShader)
{
	if (m_pGShader == pGShader)
	{
		if (pGShader == NULL)
		{
			return FRI_SUCCESS;
		}
		SetGShaderConstant(pGShader);
		SetGShaderSampler(pGShader);
		return FRI_SUCCESS;
	}
	SetGShaderProgram(pGShader);

	m_pGShader = pGShader;
	if (pGShader == NULL)
	{
		return FRI_SUCCESS;
	}
	SetGShaderConstant(pGShader);
	SetGShaderSampler(pGShader);
	return FRI_SUCCESS;
}
unsigned int SKDX11Renderer::SetPShader(SKPShader * pPShader)
{  
	if (!pPShader)
		return FRI_FAIL;

	if (m_pPshader == pPShader)
	{
		SetPShaderConstant(pPShader);
		SetPShaderSampler(pPShader);
		return FRI_SUCCESS;
	}
	if (!SetPShaderProgram(pPShader))
		return FRI_FAIL;

	m_pPshader = pPShader;
	SetPShaderConstant(pPShader);
	SetPShaderSampler(pPShader);
	return FRI_SUCCESS;
}
unsigned int SKDX11Renderer::SetVBuffer(SKVertexBuffer * pVBuffer)
{
	unsigned int FRI = SKRenderer::SetVBuffer(pVBuffer);
	if (FRI == FRI_SAMERESOURCE)
		return 1;
	else if (FRI == FRI_FAIL)
	{
		IASetVertexBuffers(NULL, 0);
		return 0;
	}
	SKVBufferID* pVBufferID = (SKVBufferID *)pVBuffer->GetIdentifier();
	if (!pVBufferID)
		return 0;
	IASetVertexBuffers(pVBufferID, pVBuffer->GetOneVertexSize());

	return 1;
}

unsigned int SKDX11Renderer::SetIBuffer(SKIndexBuffer* pIBuffer)
{
	unsigned int FRI = SKRenderer::SetIBuffer(pIBuffer);
	if (FRI == FRI_SAMERESOURCE)
		return 1;
	else if (FRI == FRI_FAIL)
	{
		IASetIndexBuffer(NULL, DXGI_FORMAT_UNKNOWN);	
		return 1;
	}
	SKIBufferID* pIBufferID = (SKIBufferID *)pIBuffer->GetIdentifier();
	if (!pIBufferID)
		return 0;

	IASetIndexBuffer(pIBufferID, (DXGI_FORMAT)ms_dwVertexFormatDateType[pIBuffer->GetDateType()]);
	return 1;
}

unsigned int SKDX11Renderer::SetVTexture(SKTexture* pTexture, unsigned int i)
{ 
	unsigned int FRI = SKRenderer::SetVTexture(pTexture, i);
	if (FRI == FRI_SAMERESOURCE)
		return 1;
	else if (FRI == FRI_FAIL)
	{
		SKSetShaderResources(i,NULL);
		return 0;
	}
	SKTextureID *pTextureID = (SKTextureID *)pTexture->GetIdentifier();
	if (!pTextureID)
		return 0;
	SKSetShaderResources(i,pTextureID);
	return true;
}
unsigned int SKDX11Renderer::SetGTexture(SKTexture* pTexture, unsigned int i)
{
	unsigned int FRI = SKRenderer::SetGTexture(pTexture, i);
	if (FRI == FRI_SAMERESOURCE)
		return 1;
	else if (FRI == FRI_FAIL)
	{
		GSSetShaderResources(i, NULL);
		return 0;
	}
	SKTextureID *pTextureID = (SKTextureID *)pTexture->GetIdentifier();
	if (!pTextureID)
		return 0;
	GSSetShaderResources(i, pTextureID);
	return true;
}
unsigned int SKDX11Renderer::SetDTexture(SKTexture* pTexture, unsigned int i)
{
	unsigned int FRI = SKRenderer::SetDTexture(pTexture, i);
	if (FRI == FRI_SAMERESOURCE)
		return 1;
	else if (FRI == FRI_FAIL)
	{
		DSSetShaderResources(i, NULL);
		return 0;
	}
	SKTextureID *pTextureID = (SKTextureID *)pTexture->GetIdentifier();
	if (!pTextureID)
		return 0;
	DSSetShaderResources(i, pTextureID);
	return true;
}
unsigned int SKDX11Renderer::SetHTexture(SKTexture* pTexture, unsigned int i)
{
	unsigned int FRI = SKRenderer::SetHTexture(pTexture, i);
	if (FRI == FRI_SAMERESOURCE)
		return 1;
	else if (FRI == FRI_FAIL)
	{
		HSSetShaderResources(i, NULL);
		return 0;
	}
	SKTextureID *pTextureID = (SKTextureID *)pTexture->GetIdentifier();
	if (!pTextureID)
		return 0;
	HSSetShaderResources(i, pTextureID);
	return true;
}
unsigned int SKDX11Renderer::SetHSamplerState(SKSamplerState * pSamplerState, unsigned int i, bool bForceSet)
{
	unsigned int FRI = SKRenderer::SetHSamplerState(pSamplerState, i, bForceSet);
	if (FRI == FRI_SAMERESOURCE)
		return FRI_SUCCESS;
	SKMAC_ASSERT(FRI == FRI_SUCCESS);
	if (FRI == FRI_FAIL)
	{
		HSSetSamplers(i, NULL);
		return FRI_FAIL;
	}
	if (!pSamplerState)
	{
		pSamplerState = (SKSamplerState *)SKSamplerState::GetDefault();
	}


	m_pHSamplerState[i] = pSamplerState;

	SKSamplerStateID* pSamplerStateID = (SKSamplerStateID *)m_pHSamplerState[i]->GetIdentifier();
	if (!pSamplerStateID)
		return 0;
	HSSetSamplers(i, pSamplerStateID);
	return 1;
}
unsigned int SKDX11Renderer::SetDSamplerState(SKSamplerState * pSamplerState, unsigned int i, bool bForceSet)
{
	unsigned int FRI = SKRenderer::SetDSamplerState(pSamplerState, i, bForceSet);
	if (FRI == FRI_SAMERESOURCE)
		return FRI_SUCCESS;
	SKMAC_ASSERT(FRI == FRI_SUCCESS);
	if (FRI == FRI_FAIL)
	{
		DSSetSamplers(i, NULL);
		return FRI_FAIL;
	}
	if (!pSamplerState)
	{
		pSamplerState = (SKSamplerState *)SKSamplerState::GetDefault();
	}


	m_pDSamplerState[i] = pSamplerState;

	SKSamplerStateID* pSamplerStateID = (SKSamplerStateID *)m_pDSamplerState[i]->GetIdentifier();
	if (!pSamplerStateID)
		return 0;
	DSSetSamplers(i, pSamplerStateID);
	return 1;
}
unsigned int SKDX11Renderer::SetGSamplerState(SKSamplerState * pSamplerState, unsigned int i, bool bForceSet)
{
	unsigned int FRI = SKRenderer::SetGSamplerState(pSamplerState, i, bForceSet);
	if (FRI == FRI_SAMERESOURCE)
		return FRI_SUCCESS;
	SKMAC_ASSERT(FRI == FRI_SUCCESS);
	if (FRI == FRI_FAIL)
	{
		GSSetSamplers(i, NULL);
		return FRI_FAIL;
	}
	if (!pSamplerState)
	{
		pSamplerState = (SKSamplerState *)SKSamplerState::GetDefault();
	}


	m_pGSamplerState[i] = pSamplerState;

	SKSamplerStateID* pSamplerStateID = (SKSamplerStateID *)m_pGSamplerState[i]->GetIdentifier();
	if (!pSamplerStateID)
		return 0;
	GSSetSamplers(i, pSamplerStateID);
	return 1;
}
unsigned int SKDX11Renderer::SetVSamplerState(SKSamplerState * pSamplerState, unsigned int i, bool bForceSet)
{
	unsigned int FRI = SKRenderer::SetVSamplerState(pSamplerState, i, bForceSet);
	if (FRI == FRI_SAMERESOURCE)
		return FRI_SUCCESS;
	SKMAC_ASSERT(FRI == FRI_SUCCESS);
	if (FRI == FRI_FAIL)
	{
		SKSetSamplers(i, NULL);
		return FRI_FAIL;
	}
	if (!pSamplerState)
	{
		pSamplerState = (SKSamplerState *)SKSamplerState::GetDefault();
	}


	m_pVSamplerState[i] = pSamplerState;

	SKSamplerStateID* pSamplerStateID = (SKSamplerStateID *)m_pVSamplerState[i]->GetIdentifier();
	if (!pSamplerStateID)
		return 0;
	SKSetSamplers(i, pSamplerStateID);
	return 1;
}
unsigned int SKDX11Renderer::SetSamplerState(SKSamplerState * pSamplerState, unsigned int i, bool bForceSet)
{
	unsigned int FRI = SKRenderer::SetSamplerState(pSamplerState, i, bForceSet);
	if (FRI == FRI_SAMERESOURCE)
		return FRI_SUCCESS;
	SKMAC_ASSERT(FRI == FRI_SUCCESS);
	if (FRI == FRI_FAIL)
	{
		PSSetSamplers(i, NULL);
		return FRI_FAIL;
	}
	if (!pSamplerState)
	{
		pSamplerState = (SKSamplerState *)SKSamplerState::GetDefault();
	}


	m_pPSamplerState[i] = pSamplerState;

	SKSamplerStateID* pSamplerStateID = (SKSamplerStateID *)m_pPSamplerState[i]->GetIdentifier();
	if (!pSamplerStateID)
		return 0;
	PSSetSamplers(i, pSamplerStateID);
	return 1;
}

bool SKDX11Renderer::SetRenderTargets(SKRenderTarget *pRenderTarget[], unsigned int uiNum)
{ 
	
	

	if (!SKRenderer::SetRenderTargets(pRenderTarget,uiNum))
	{
		SKMAC_ASSERT(0);
		return false;
	}	
	SKRenderTargetID * RTVTemp[MAX_RENDER_TARGET] = { NULL };
	unsigned int uiTargetNum = 0;
	for (unsigned int i = 0; i < MAX_RENDER_TARGET; i++)
	{
		if (pRenderTarget[i])
		{
			RTVTemp[uiTargetNum] = (SKRenderTargetID *)pRenderTarget[i]->GetIdentifier();
			uiTargetNum++;
			
		}
		else
		{
			break;
		}
	}
	SetRenderTargetsEx(RTVTemp, uiTargetNum, m_uiBackMacthType);

	return true;
}
bool SKDX11Renderer::EndRenderTargets(SKRenderTarget *pRenderTarget[], unsigned int uiNum)
{
	
	if (!SKRenderer::EndRenderTargets(pRenderTarget, uiNum))
	{
		SKMAC_ASSERT(0);
		return false;
	}
	SKRenderTargetID * RTVTemp[MAX_RENDER_TARGET] = { NULL };
	unsigned int uiTargetNum = 0;
	for (unsigned int i = 0; i < MAX_RENDER_TARGET; i++)
	{
		if (pRenderTarget[i])
		{
			RTVTemp[uiTargetNum] = (SKRenderTargetID *)pRenderTarget[i]->GetIdentifier();
			uiTargetNum++;

		}
		else
		{
			break;
		}
	}
	EndRenderTargetsEx(RTVTemp, uiTargetNum);


	return true;
}

bool SKDX11Renderer::SetDepthStencilBuffer(SKDepthStencil * pDepthStencilBuffer)
{ 
	

	if (!SKRenderer::SetDepthStencilBuffer(pDepthStencilBuffer))
	{
		SKMAC_ASSERT(0);
		return 0;
	}	
	SKDepthStencilID *pDepthStencilID = (SKDepthStencilID *)pDepthStencilBuffer->GetIdentifier();
	if (!pDepthStencilID)
	{
		return 0;
	}
	SetDepthStencilBufferEx(pDepthStencilID);
	return true;
}
bool SKDX11Renderer::EndDepthStencilBuffer(SKDepthStencil * pDepthStencilBuffer)
{ 
	
	if (!SKRenderer::EndDepthStencilBuffer(pDepthStencilBuffer))
	{	
		SKMAC_ASSERT(0);
		return 0;
	}
	SKDepthStencilID *pDepthStencilID = (SKDepthStencilID *)pDepthStencilBuffer->GetIdentifier();
	if (!pDepthStencilID)
		return 0;
	EndDepthStencilBufferEx(pDepthStencilID);
	return 1;
}


bool SKDX11Renderer::SetVShaderProgram(SKVShader*	pVShaderProgram)
{ 
	if (!SKRenderer::SetVShaderProgram(pVShaderProgram))
	{
		SKSetShader(NULL);
		return 0;
	}

	SKVProgramID *pVProgramID = (SKVProgramID *)pVShaderProgram->GetIdentifier();
	if (!pVProgramID)
		return FRI_FAIL;
	SKSetShader(pVProgramID);
	return 1;
}
bool SKDX11Renderer::SetGShaderProgram(SKGShader*pGShaderProgram)
{
	if (!SKRenderer::SetGShaderProgram(pGShaderProgram))
	{
		GSSetShader(NULL);
		return 0;
	}
	SKGProgramID *pGProgramID = (SKGProgramID *)pGShaderProgram->GetIdentifier();
	if (!pGProgramID)
		return FRI_FAIL;
	GSSetShader(pGProgramID);
	return 1;
}
bool SKDX11Renderer::SetDShaderProgram(SKDShader*pDShaderProgram)
{
	if (!SKRenderer::SetDShaderProgram(pDShaderProgram))
	{
		DSSetShader(NULL);
		return 0;
	}
	SKDProgramID *pDProgramID = (SKDProgramID *)pDShaderProgram->GetIdentifier();
	if (!pDProgramID)
		return FRI_FAIL;
	DSSetShader(pDProgramID);
	return 1;
}
bool SKDX11Renderer::SetHShaderProgram(SKHShader*pHShaderProgram)
{
	if (!SKRenderer::SetHShaderProgram(pHShaderProgram))
	{
		HSSetShader(NULL);
		return 0;
	}
	SKHProgramID *pHProgramID = (SKHProgramID *)pHShaderProgram->GetIdentifier();
	if (!pHProgramID)
		return FRI_FAIL;
	HSSetShader(pHProgramID);
	return 1;
}
bool SKDX11Renderer::SetPShaderProgram(SKPShader*pPShaderProgram)
{
	if (!SKRenderer::SetPShaderProgram(pPShaderProgram))
	{
		PSSetShader(NULL);
		return 0;
	}
	SKPProgramID *pPProgramID = (SKPProgramID *)pPShaderProgram->GetIdentifier();
	if (!pPProgramID)
		return FRI_FAIL;
	PSSetShader(pPProgramID);
	return 1;
}
D3D_SHADER_MACRO * SKDX11Renderer::GetDefine(SKMap<SKString, SKString> & Define)
{
	if (Define.GetNum())
	{
		D3D_SHADER_MACRO * pMacro = SK_NEW D3D_SHADER_MACRO[Define.GetNum() + 1];
		for (unsigned int i = 0; i < Define.GetNum(); i++)
		{
			pMacro[i].Name = Define[i].Key.GetBuffer();
			pMacro[i].Definition = Define[i].Value.GetBuffer();
		}
		pMacro[Define.GetNum()].Name = NULL;
		pMacro[Define.GetNum()].Definition = NULL;
		return pMacro;
	}
	return NULL;
}
//Load Release
bool SKDX11Renderer::OnLoadVShaderProgram(SKVShader * pVShaderProgram, SKResourceIdentifier *&pID)
{ 
	SKVProgramID *pVProgramID = NULL;
	pVProgramID = SK_NEW SKVProgramID;
	if (!pVProgramID)
		return 0;
	pID = pVProgramID;

	if (!pVShaderProgram->GetCacheBuffer())
	{
		CreateVShaderFromString(pVShaderProgram, pVProgramID);
	}
	else
	{
		CreateVShaderFromCache(pVShaderProgram,pVProgramID);
	}

	
	return true;
}
bool SKDX11Renderer::OnReleaseVShaderProgram(SKResourceIdentifier * pVShaderProgramID)
{
	SKVProgramID *pVProgramID = (SKVProgramID *)pVShaderProgramID;

	SKMAC_DELETE(pVProgramID)
	return 1;
}
bool SKDX11Renderer::OnLoadGShaderProgram(SKGShader * pGShaderProgram, SKResourceIdentifier *&pID)
{
	SKGProgramID *pGProgramID = NULL;
	pGProgramID = SK_NEW SKGProgramID;
	if (!pGProgramID)
		return 0;
	pID = pGProgramID;
	if (!pGShaderProgram->GetCacheBuffer())
	{
		CreateGShaderFromString(pGShaderProgram, pGProgramID);
	}
	else
	{
		CreateGShaderFromCache(pGShaderProgram, pGProgramID);
	}


	return true;
}
bool SKDX11Renderer::OnLoadDShaderProgram(SKDShader * pDShaderProgram, SKResourceIdentifier *&pID)
{
	SKDProgramID *pDProgramID = NULL;
	pDProgramID = SK_NEW SKDProgramID;
	if (!pDProgramID)
		return 0;
	pID = pDProgramID;
	if (!pDShaderProgram->GetCacheBuffer())
	{
		CreateDShaderFromString(pDShaderProgram, pDProgramID);
	}
	else
	{
		CreateDShaderFromCache(pDShaderProgram, pDProgramID);
	}


	return true;
}
bool SKDX11Renderer::OnLoadHShaderProgram(SKHShader * pHShaderProgram, SKResourceIdentifier *&pID)
{
	SKHProgramID *pHProgramID = NULL;
	pHProgramID = SK_NEW SKHProgramID;
	if (!pHProgramID)
		return 0;
	pID = pHProgramID;
	if (!pHShaderProgram->GetCacheBuffer())
	{
		CreateHShaderFromString(pHShaderProgram, pHProgramID);
	}
	else
	{
		CreateHShaderFromCache(pHShaderProgram, pHProgramID);
	}


	return true;
}
bool SKDX11Renderer::OnLoadPShaderProgram(SKPShader* pPShaderProgram, SKResourceIdentifier *&pID)
{ 
	SKPProgramID *pPProgramID = NULL;
	pPProgramID = SK_NEW SKPProgramID;
	if (!pPProgramID)
		return 0;
	pID = pPProgramID;
	if (!pPShaderProgram->GetCacheBuffer())
	{
		CreatePShaderFromString(pPShaderProgram, pPProgramID);
	}
	else
	{
		CreatePShaderFromCache(pPShaderProgram, pPProgramID);
	}

	
	return true;
}
bool SKDX11Renderer::OnReleasePShaderProgram(SKResourceIdentifier* pPShaderProgramID)
{
	SKPProgramID *pPProgramID = (SKPProgramID *)pPShaderProgramID;

	SKMAC_DELETE(pPProgramID);
	return 1;
}
bool SKDX11Renderer::OnReleaseGShaderProgram(SKResourceIdentifier * pGShaderProgramID)
{
	SKGProgramID *pGProgramID = (SKGProgramID *)pGShaderProgramID;

	SKMAC_DELETE(pGProgramID);
	return 1;
}
bool SKDX11Renderer::OnReleaseDShaderProgram(SKResourceIdentifier * pDShaderProgramID)
{
	SKDProgramID *pDProgramID = (SKDProgramID *)pDShaderProgramID;

	SKMAC_DELETE(pDProgramID);
	return 1;
}
bool SKDX11Renderer::OnReleaseHShaderProgram(SKResourceIdentifier * pHShaderProgramID)
{
	SKHProgramID *pHProgramID = (SKHProgramID *)pHShaderProgramID;

	SKMAC_DELETE(pHProgramID);
	return 1;
}
bool SKDX11Renderer::OnLoadTexture(SKTexture * pTexture, SKResourceIdentifier *&pID)
{ 
	SKTextureID * pTextureID = NULL;
	pTextureID = SK_NEW SKTextureID;
	if (!pTextureID)
		return 0;

	
	pID = pTextureID;
	
	unsigned int Width = pTexture->GetWidth(0);
	unsigned int Height = pTexture->GetHeight(0);
	unsigned int Depth = pTexture->GetLength(0);
	unsigned int MipLevels = pTexture->GetMipLevel();
	unsigned int ArraySize = 1;
	DXGI_FORMAT Format = pTexture->m_bSRGB ? (DXGI_FORMAT)ms_dwSRGBTextureFormatType[pTexture->GetFormatType()] : (DXGI_FORMAT)ms_dwTextureFormatType[pTexture->GetFormatType()];
	unsigned int Count = 0;
	unsigned int Quality = 0;
	unsigned int BindFlags = D3D11_BIND_SHADER_RESOURCE;
	unsigned int MiscFlags = 0;// D3D11_RESOURCE_MISC_GENERATE_MIPS;
	D3D11_USAGE Usage = D3D11_USAGE_DEFAULT;
	unsigned int CPUAccessFlags = 0;
	bool bRT = false;
	if (pTexture->GetTexType() == SKTexture::TT_2D && ((VS2DTexture *)pTexture)->IsRenderTarget())
	{

		Usage = D3D11_USAGE_DEFAULT;
		BindFlags |= D3D11_BIND_RENDER_TARGET;
		CPUAccessFlags = 0;
		SKRenderTarget * pRenderTarget = ((VS2DTexture *)pTexture)->GetCurRenderTarget();
		if (pRenderTarget)
		{
			Count = GetSuitableMultisample(pRenderTarget->GetMulSample());		
			bRT = true;
		}
	}
	else if (pTexture->GetTexType() == SKTexture::TT_CUBE && ((SKCubeTexture *)pTexture)->IsRenderTarget())
	{
		Usage = D3D11_USAGE_DEFAULT;
		BindFlags |= D3D11_BIND_RENDER_TARGET;
		CPUAccessFlags = 0;
		SKRenderTarget * pRenderTarget = ((SKCubeTexture *)pTexture)->GetCurRenderTarget(0);
		if (pRenderTarget)
		{
			Count = GetSuitableMultisample(pRenderTarget->GetMulSample());
			bRT = true;

		}
	}
	else
	{
		if (!pTexture->IsStatic())
		{
			Usage = D3D11_USAGE_DYNAMIC;
			CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			
		}
		else
		{
			Usage = D3D11_USAGE_IMMUTABLE;
			CPUAccessFlags = 0;
			
		}
	}
	




	if (pTexture->GetTexType() == SKTexture::TT_2D)
	{

		D3D11_TEXTURE2D_DESC desc;
		desc.Width = Width;
		desc.Height = Height;
		desc.MipLevels = MipLevels;
		desc.ArraySize = ArraySize;
		desc.Format = Format;
		desc.SampleDesc.Count = ms_dwMultiSampleTypes[Count];
		desc.SampleDesc.Quality = Quality;
		desc.BindFlags = BindFlags;
		desc.MiscFlags = MiscFlags;
		desc.CPUAccessFlags = CPUAccessFlags;
		desc.Usage = Usage;

		CreateTexture2D((VS2DTexture*)pTexture, desc, bRT, pTextureID);


		
		
	}
	else if (pTexture->GetTexType() == SKTexture::TT_3D)
	{
		D3D11_TEXTURE3D_DESC desc;
		desc.Width = Width;
		desc.Height = Height;
		desc.Depth = Depth;
		desc.MipLevels = MipLevels;
		desc.Format = Format;
		desc.BindFlags = BindFlags;
		desc.MiscFlags = MiscFlags;
		desc.Usage = Usage;
		desc.CPUAccessFlags = CPUAccessFlags;

		CreateTexture3D((VS3DTexture *)pTexture, desc, pTextureID);


	}
	else if (pTexture->GetTexType() == SKTexture::TT_CUBE)
	{
		D3D11_TEXTURE2D_DESC desc;
		desc.Width = Width;
		desc.Height = Height;
		desc.MipLevels = MipLevels;
		desc.ArraySize = ArraySize * SKCubeTexture::F_MAX;
		desc.Format = Format;
		desc.SampleDesc.Count = ms_dwMultiSampleTypes[Count];
		desc.SampleDesc.Quality = Quality;
		desc.BindFlags = BindFlags;
		desc.MiscFlags = MiscFlags | D3D11_RESOURCE_MISC_TEXTURECUBE;
		desc.CPUAccessFlags = CPUAccessFlags;
		desc.Usage = Usage;

		CreateTextureCube((SKCubeTexture *)pTexture, desc, bRT, pTextureID);



	}
	else if (pTexture->GetTexType() == SKTexture::TT_1D)
	{
		D3D11_TEXTURE1D_DESC desc;
		desc.Width = Width;
		desc.ArraySize = ArraySize;
		desc.MipLevels = MipLevels;
		desc.Format = Format;
		desc.BindFlags = BindFlags;
		desc.MiscFlags = MiscFlags;
		desc.Usage = Usage;
		desc.CPUAccessFlags = CPUAccessFlags;

		CreateTexture1D((VS1DTexture *)pTexture, desc, pTextureID);

	}
	else
	{
		SKMAC_ASSERT(0);
	}

	return 1;
}
bool SKDX11Renderer::OnReleaseTexture(SKResourceIdentifier * pTextureID)
{
	SKMAC_DELETE(pTextureID);
	return 1;
}

bool SKDX11Renderer::OnLoadDepthStencil(SKDepthStencil * pDepthStencil, SKResourceIdentifier *&pID)
{ 
	SKDepthStencilID * pDepthStencilID = NULL;
	pDepthStencilID = SK_NEW SKDepthStencilID;
	if (!pDepthStencilID)
		return 0;

	
	pID = pDepthStencilID;

	unsigned int Width = pDepthStencil->GetWidth();
	unsigned int Height = pDepthStencil->GetHeight();
	unsigned int MipLevels = 1;
	unsigned int ArraySize = 1;
	DXGI_FORMAT Format = (DXGI_FORMAT)ms_dwTextureFormatType[pDepthStencil->GetFormatType()];
	unsigned int BindFlags = D3D11_BIND_DEPTH_STENCIL;
	unsigned int MiscFlags = 0;
	D3D11_USAGE Usage = D3D11_USAGE_DEFAULT;
	unsigned int CPUAccessFlags = 0;
	unsigned int Count = GetSuitableMultisample(pDepthStencil->GetMulSample());
	

	D3D11_TEXTURE2D_DESC desc;
	desc.Width = Width;
	desc.Height = Height;
	desc.MipLevels = MipLevels;
	desc.ArraySize = ArraySize;
	desc.Format = Format;
	desc.SampleDesc.Count = ms_dwMultiSampleTypes[Count];
	desc.SampleDesc.Quality = 0;
	desc.BindFlags = BindFlags;
	desc.MiscFlags = MiscFlags;
	desc.CPUAccessFlags = CPUAccessFlags;
	desc.Usage = Usage;

	CreateDepth(desc, pDepthStencilID);
	
	return true;
}
bool SKDX11Renderer::OnReleaseDepthStencil(SKResourceIdentifier * pDepthStencilID)
{ 
	SKMAC_DELETE(pDepthStencilID);
	return 1;
}

bool SKDX11Renderer::OnLoadRenderTarget(SKRenderTarget * pRenderTarget, SKResourceIdentifier *&pID)
{ 
	if (!pRenderTarget)
	{
		return false;
	}
	
	SKRenderTargetID *pRenderTargetID = SK_NEW SKRenderTargetID;
	pID = pRenderTargetID;
	
	DXGI_FORMAT Format = (DXGI_FORMAT)ms_dwTextureFormatType[pRenderTarget->GetFormatType()];
	unsigned int Count = GetSuitableMultisample(pRenderTarget->GetMulSample());
	SKTexture * pTexture = pRenderTarget->GetCreateBy();
	SKTextureID * pTextureID = NULL;
	if (pTexture)
	{
		pTexture->LoadResource(this);
		pTextureID = (SKTextureID *)pTexture->GetIdentifier();
	}
	OnLoadRenderTargetEx(pRenderTarget->GetWidth(), pRenderTarget->GetHeight(), Format, ms_dwMultiSampleTypes[Count],
		pRenderTarget->GetLockFlag(), pTexture->GetTexType(), pRenderTarget->GetLevel(), pRenderTarget->GetParam(),
		pTextureID, pRenderTargetID);

	return true;
}
bool SKDX11Renderer::OnReleaseRenderTarget(SKResourceIdentifier * pRenderTarget)
{ 
	SKMAC_DELETE(pRenderTarget);
	return true;
}

bool SKDX11Renderer::OnLoadVBufferDate(SKVertexBuffer * pVBuffer, SKResourceIdentifier *&pID)
{
	SKVBufferID * pVBufferID = NULL;
	pVBufferID = SK_NEW SKVBufferID;
	if (!pVBufferID)
		return 0;
	pID = pVBufferID;


	unsigned int uiOneVextexSize = pVBuffer->GetOneVertexSize();

	unsigned int uiTotalSize = uiOneVextexSize * pVBuffer->GetVertexNum();

	

	D3D11_USAGE Usage = D3D11_USAGE_DEFAULT;
	unsigned int CPUAccessFlags = 0;
	if (!pVBuffer->IsStatic())
	{
		Usage = D3D11_USAGE_DYNAMIC;
		CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		Usage = D3D11_USAGE_IMMUTABLE;
		CPUAccessFlags = 0;
	}
	OnLoadVertexBufferEx(uiOneVextexSize, uiTotalSize, CPUAccessFlags, Usage, pVBuffer, pVBufferID);
	return true;
}
bool SKDX11Renderer::OnReleaseVBufferDate(SKResourceIdentifier* pVBufferID)
{
	SKMAC_DELETE(pVBufferID);
	return 1;
}



bool SKDX11Renderer::OnLoadIBuffer(SKIndexBuffer* pIBuffer, SKResourceIdentifier *&pID)
{ 
	SKIBufferID * pIBufferID = NULL;
	pIBufferID = SK_NEW SKIBufferID;
	if (!pIBufferID)
		return 0;
	pID = pIBufferID;

	void* pBuffer = NULL;
	if (pIBuffer->GetIndexDate())
	{
		pBuffer = pIBuffer->GetIndexDate()->GetDate();
	}
	unsigned int ByteWidth = pIBuffer->GetByteSize();
	D3D11_USAGE Usage = D3D11_USAGE_DEFAULT;
	unsigned int CPUAccessFlags = 0;

	if (!pIBuffer->IsStatic())
	{
		Usage = D3D11_USAGE_DYNAMIC;
		CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		Usage = D3D11_USAGE_IMMUTABLE;
		CPUAccessFlags = 0;
	}
	OnLoadIndexBufferEx(ByteWidth, Usage, CPUAccessFlags, pBuffer, pIBufferID);
	return true;
	
}
bool SKDX11Renderer::OnReleaseIBuffer(SKResourceIdentifier* pIBufferID)
{ 
	SKMAC_DELETE(pIBufferID);
	return 1;
}



bool SKDX11Renderer::OnLoadBlendState(SKBlendState * pBlendState, SKResourceIdentifier *&pID)
{ 
	SKBlendStateID * pBlendStateID = NULL;
	pBlendStateID = SK_NEW SKBlendStateID;
	if (!pBlendStateID)
		return 0;
	pID = pBlendStateID;
	const SKBlendDesc& BD = pBlendState->GetBlendDesc();
	D3D11_BLEND_DESC desc;
	
	desc.AlphaToCoverageEnable = BD.bAlphaToCoverageEnable;
	desc.IndependentBlendEnable = BD.bIndependentBlendEnable;
	for (unsigned int i = 0; i < 8; ++i)
	{
		D3D11_RENDER_TARGET_BLEND_DESC& out = desc.RenderTarget[i];
		out.BlendEnable = BD.bBlendEnable[i];
		out.SrcBlend = (D3D11_BLEND)ms_dwAlphaBlend[BD.ucSrcBlend[i]];
		out.DestBlend = (D3D11_BLEND)ms_dwAlphaBlend[BD.ucDestBlend[i]];
		out.BlendOp = (D3D11_BLEND_OP)ms_dwAlphaBlendOP[BD.ucBlendOp[i]];
		out.SrcBlendAlpha = (D3D11_BLEND)ms_dwAlphaBlend[BD.ucSrcBlendAlpha[i]];
		out.DestBlendAlpha = (D3D11_BLEND)ms_dwAlphaBlend[BD.ucDestBlendAlpha[i]];
		out.BlendOpAlpha = (D3D11_BLEND_OP)ms_dwAlphaBlendOP[BD.ucBlendOpAlpha[i]];
		out.RenderTargetWriteMask = BD.ucWriteMask[i];
	}
	CreateBlendState(desc, pBlendStateID);
	return true;
}
bool SKDX11Renderer::OnReleaseBlendState(SKResourceIdentifier * pBlendStateID)
{ 
	SKMAC_DELETE(pBlendStateID);
	return 1;
}

bool SKDX11Renderer::OnLoadDepthStencilState(SKDepthStencilState * pDepthStencilState, SKResourceIdentifier *&pID)
{ 
	SKDepthStencilStateID * pDepthStenciStateID = NULL;
	pDepthStenciStateID = SK_NEW SKDepthStencilStateID;
	if (!pDepthStenciStateID)
		return 0;
	pID = pDepthStenciStateID;
	const SKDepthStencilDesc& DSD = pDepthStencilState->GetDepthStencilDesc();
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	
	depthStencilDesc.DepthEnable = DSD.m_bDepthEnable;
	depthStencilDesc.DepthWriteMask = DSD.m_bDepthWritable ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = (D3D11_COMPARISON_FUNC)ms_dwCompare[DSD.m_uiDepthCompareMethod];

	depthStencilDesc.StencilEnable = DSD.m_bStencilEnable;
	depthStencilDesc.StencilReadMask = DSD.m_uiMask;
	depthStencilDesc.StencilWriteMask = DSD.m_uiWriteMask;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = (D3D11_STENCIL_OP)ms_dwStencilOperation[DSD.m_uiSFailZPassOP];
	depthStencilDesc.FrontFace.StencilDepthFailOp = (D3D11_STENCIL_OP)ms_dwStencilOperation[DSD.m_uiSPassZFailOP];
	depthStencilDesc.FrontFace.StencilPassOp = (D3D11_STENCIL_OP)ms_dwStencilOperation[DSD.m_uiSPassZPassOP];
	depthStencilDesc.FrontFace.StencilFunc = (D3D11_COMPARISON_FUNC)ms_dwCompare[DSD.m_uiStencilCompareMethod];

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = (D3D11_STENCIL_OP)ms_dwStencilOperation[DSD.m_uiCCW_SFailZPassOP];
	depthStencilDesc.BackFace.StencilDepthFailOp = (D3D11_STENCIL_OP)ms_dwStencilOperation[DSD.m_uiCCW_SPassZFailOP];
	depthStencilDesc.BackFace.StencilPassOp = (D3D11_STENCIL_OP)ms_dwStencilOperation[DSD.m_uiCCW_SPassZPassOP];
	depthStencilDesc.BackFace.StencilFunc = (D3D11_COMPARISON_FUNC)ms_dwCompare[DSD.m_uiCCW_StencilCompareMethod];

	CreateDepthStencilState(depthStencilDesc, pDepthStenciStateID);

	return true;
}
bool SKDX11Renderer::OnReleaseDepthStencilState(SKResourceIdentifier * pDepthStencilStateID)
{ 
	SKMAC_DELETE(pDepthStencilStateID);
	return 1;
}

bool SKDX11Renderer::OnLoadRasterizerState(SKRasterizerState * pRasterizerState, SKResourceIdentifier *&pID)
{ 
	SKRasterStateID * pRasterStateID = NULL;
	pRasterStateID = SK_NEW SKRasterStateID;
	if (!pRasterStateID)
		return 0;
	pID = pRasterStateID;
	const SKRasterizerDesc& RD = pRasterizerState->GetRasterizerDesc();
	D3D11_RASTERIZER_DESC rasterDesc;
	
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = (D3D11_CULL_MODE)ms_dwCullType[RD.m_uiCullType];
	rasterDesc.DepthBias = (int)RD.m_fDepthBias;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = (RD.m_bWireEnable) ? D3D11_FILL_WIREFRAME :D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = RD.m_bScissorTestEnable;
	rasterDesc.SlopeScaledDepthBias = 0.0f;
	CreateRasterizerState(rasterDesc, pRasterStateID);
	return true; 
}
bool SKDX11Renderer::OnReleaseRasterizerState(SKResourceIdentifier * pRasterizerStateID)
{ 
	SKMAC_DELETE(pRasterizerStateID);
	return 1;
}

bool SKDX11Renderer::OnLoadSamplerState(SKSamplerState * pSamplerState, SKResourceIdentifier *&pID)
{ 
	SKSamplerStateID * pSamplerStateID = NULL;
	pSamplerStateID = SK_NEW SKSamplerStateID;
	if (!pSamplerStateID)
		return 0;
	pID = pSamplerStateID;
	const SKSamplerDesc& SD = pSamplerState->GetSamplerDesc();
	D3D11_SAMPLER_DESC sampDesc;
	
	sampDesc.Filter = (D3D11_FILTER)ms_dwTextureFilterType[SD.m_uiMin][SD.m_uiMag][SD.m_uiMip];
	sampDesc.AddressU = (D3D11_TEXTURE_ADDRESS_MODE)ms_dwTextureAddrType[SD.m_uiCoordU];
	sampDesc.AddressV = (D3D11_TEXTURE_ADDRESS_MODE)ms_dwTextureAddrType[SD.m_uiCoordV];
	sampDesc.AddressW = (D3D11_TEXTURE_ADDRESS_MODE)ms_dwTextureAddrType[SD.m_uiCoordW];
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = SD.m_uiMipLevel * 1.0f;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	sampDesc.BorderColor[0] = SD.m_BorderColor.r;
	sampDesc.BorderColor[1] = SD.m_BorderColor.g;
	sampDesc.BorderColor[2] = SD.m_BorderColor.b;
	sampDesc.BorderColor[3] = SD.m_BorderColor.a;
	if (SD.m_uiAniLevel <= m_uiCurAnisotropy)
	{
		sampDesc.MaxAnisotropy = SD.m_uiAniLevel;
	}
	else
	{
		sampDesc.MaxAnisotropy = m_uiCurAnisotropy;

	}
	sampDesc.MipLODBias = SD.m_MipLODBias;
	CreateSamplerState(sampDesc, pSamplerStateID);
	return true;
}
bool SKDX11Renderer::OnReleaseSamplerState(SKResourceIdentifier * pSamplerStateID)
{ 
	SKMAC_DELETE(pSamplerStateID);
	return 1;
}

unsigned int SKDX11Renderer::SetVertexFormat(SKVertexFormat * pVertexFormat)
{ 
	unsigned int FRI = SKRenderer::SetVertexFormat(pVertexFormat);
	if (FRI == FRI_SAMERESOURCE)
		return 1;
	else if (FRI == FRI_FAIL)
	{
		IASetInputLayout(NULL);
		return 0;
	}

	SKVBufferFormatID * pVBufferFormatID = (SKVBufferFormatID *)pVertexFormat->GetIdentifier();
	if (!pVBufferFormatID)
		return 0;
	IASetInputLayout(pVBufferFormatID);
	return 1;
}

void * SKDX11Renderer::Lock(SKVertexBuffer * pVertexBuffer)
{ 
	if (!pVertexBuffer)
		return NULL;

	if (pVertexBuffer->IsStatic())
		return NULL;

	if (!LoadVBuffer(pVertexBuffer))
		return NULL;

	SKVBufferID* pVBufferID = (SKVBufferID *)pVertexBuffer->GetIdentifier();

	if (!pVBufferID->m_pVertexBuffer)
	{
		return NULL;
	}

	D3D11_MAPPED_SUBRESOURCE MappedResource;
	
	HRESULT hResult = NULL;
	hResult = m_pDeviceContext->Map(pVBufferID->m_pVertexBuffer, 0, (D3D11_MAP)ms_dwLockFlag[pVertexBuffer->GetLockFlag()], 0, &MappedResource);
	SKMAC_ASSERT(!FAILED(hResult));
	return MappedResource.pData;
}
void SKDX11Renderer::UnLock(SKIndexBuffer * pIndexBuffer)
{
	if (!pIndexBuffer)
		return;

	if (pIndexBuffer->IsStatic())
		return;
	SKIBufferID* pIBufferID = (SKIBufferID *)pIndexBuffer->GetIdentifier();
	if (!pIBufferID)
	{
		return;
	}
	if (!pIBufferID->m_IndexBuffer)
		return;
	m_pDeviceContext->Unmap(pIBufferID->m_IndexBuffer, 0);
	return;
}
void SKDX11Renderer::UnLock(SKVertexBuffer * pVertexBuffer)
{ 
	if (!pVertexBuffer)
		return;


	if (pVertexBuffer->IsStatic())
		return;

	SKVBufferID* pVBufferID = (SKVBufferID *)pVertexBuffer->GetIdentifier();
	if (!pVBufferID)
	{
		return;
	}
	if (!pVBufferID->m_pVertexBuffer)
	{
		return;
	}

	m_pDeviceContext->Unmap(pVBufferID->m_pVertexBuffer, 0);
	return;
}

void * SKDX11Renderer::Lock(SKIndexBuffer * pIndexBuffer)
{ 
	if (!pIndexBuffer)
		return NULL;


	if (pIndexBuffer->IsStatic())
		return NULL;

	if (!LoadIBuffer(pIndexBuffer))
		return NULL;
	SKIBufferID* pIBufferID = (SKIBufferID *)pIndexBuffer->GetIdentifier();

	if (!pIBufferID->m_IndexBuffer)
	{
		return NULL;
	}
	D3D11_MAPPED_SUBRESOURCE MappedResource;

	HRESULT hResult = NULL;
	hResult = m_pDeviceContext->Map(pIBufferID->m_IndexBuffer, 0, (D3D11_MAP)ms_dwLockFlag[pIndexBuffer->GetLockFlag()], 0, &MappedResource);
	SKMAC_ASSERT(!FAILED(hResult));
	return MappedResource.pData;
}

void * SKDX11Renderer::Lock(SKTexture * pTexture, unsigned int uiLevel, unsigned int uiFace)
{ 
	if (!pTexture)
		return NULL;

	if (pTexture->IsStatic() || uiLevel >= pTexture->GetMipLevel())
		return NULL;
	if (!LoadTexture(pTexture))
		return NULL;
	SKTextureID * pTextureID = (SKTextureID *)pTexture->GetIdentifier();
	if (!pTextureID)
		return NULL;
	if (!pTextureID->m_pTexture)
		return NULL;
	HRESULT hResult = NULL;
	if (pTexture->GetTexType() == SKTexture::TT_2D)
	{
		if (((VS2DTexture*)pTexture)->IsRenderTarget())
		{
			return NULL;
		}
		D3D11_MAPPED_SUBRESOURCE MappedResource;
		const UINT Subresource = D3D11CalcSubresource(uiLevel, 0, pTexture->GetMipLevel());
		HRESULT hResult = NULL;
		hResult = m_pDeviceContext->Map(pTextureID->m_pTexture, Subresource, (D3D11_MAP)ms_dwLockFlag[pTexture->GetLockFlag()], 0, &MappedResource);
		SKMAC_ASSERT(!FAILED(hResult));
		return MappedResource.pData;
	}
	else if (pTexture->GetTexType() == SKTexture::TT_3D)
	{
		D3D11_MAPPED_SUBRESOURCE MappedResource;
		const UINT Subresource = D3D11CalcSubresource(uiLevel, 0, pTexture->GetMipLevel());
		HRESULT hResult = NULL;
		hResult = m_pDeviceContext->Map(pTextureID->m_pTexture, Subresource, (D3D11_MAP)ms_dwLockFlag[pTexture->GetLockFlag()], 0, &MappedResource);
		SKMAC_ASSERT(!FAILED(hResult));
		return MappedResource.pData;
	}
	else if (pTexture->GetTexType() == SKTexture::TT_CUBE)
	{
		if (uiFace > SKCubeTexture::F_MAX)
			return NULL;
		if (((SKCubeTexture*)pTexture)->IsRenderTarget())
		{
			return NULL;
		}
		D3D11_MAPPED_SUBRESOURCE MappedResource;
		const UINT Subresource = D3D11CalcSubresource(uiLevel, uiFace, pTexture->GetMipLevel());
		HRESULT hResult = NULL;
		hResult = m_pDeviceContext->Map(pTextureID->m_pTexture, Subresource, (D3D11_MAP)ms_dwLockFlag[pTexture->GetLockFlag()], 0, &MappedResource);
		SKMAC_ASSERT(!FAILED(hResult));
		return MappedResource.pData;

	}
	else
	{
		SKMAC_ASSERT(0);
		return NULL;
	}
}
void SKDX11Renderer::UnLock(SKTexture * pTexture, unsigned int uiLevel, unsigned int uiFace)
{ 
	if (!pTexture)
		return;

	if (pTexture->IsStatic() || uiLevel >= pTexture->GetMipLevel())
		return;
	SKTextureID * pTextureID = (SKTextureID *)pTexture->GetIdentifier();
	if (!pTextureID)
		return;
	if (!pTextureID->m_pTexture)
		return;
	HRESULT hResult = NULL;
	if (pTexture->GetTexType() == SKTexture::TT_2D)
	{
		if (((VS2DTexture*)pTexture)->IsRenderTarget())
		{
			return;
		}
		const UINT Subresource = D3D11CalcSubresource(uiLevel, 0, pTexture->GetMipLevel());
		m_pDeviceContext->Unmap(pTextureID->m_pTexture, Subresource);
		return;
	}
	else if (pTexture->GetTexType() == SKTexture::TT_3D)
	{
		const UINT Subresource = D3D11CalcSubresource(uiLevel, 0, pTexture->GetMipLevel());
		m_pDeviceContext->Unmap(pTextureID->m_pTexture, Subresource);
		return;
	}
	else if (pTexture->GetTexType() == SKTexture::TT_CUBE)
	{
		if (uiFace > SKCubeTexture::F_MAX)
			return;
		if (((SKCubeTexture*)pTexture)->IsRenderTarget())
		{
			return ;
		}
		const UINT Subresource = D3D11CalcSubresource(uiLevel, uiFace, pTexture->GetMipLevel());
		m_pDeviceContext->Unmap(pTextureID->m_pTexture, Subresource);
		return;

	}
	else
	{
		SKMAC_ASSERT(0);
		return;
	}
	return;
}
bool SKDX11Renderer::CopyResourceBuffer(VS2DTexture * pSource, SKCubeTexture * pDest, unsigned int uiFace)
{ 
	if (!SKRenderer::CopyResourceBuffer(pSource, pDest, uiFace))
	{
		return false;
	}
	SKTextureID *pTextureID = (SKTextureID *)pSource->GetIdentifier();
	if (!pTextureID)
	{
		return false;
	}

	SKTextureID * pCubTextureID = (SKTextureID *)pDest->GetIdentifier();
	if (!pCubTextureID)
	{
		return false;
	}
	
	for (unsigned int i = 0; i < pDest->GetMipLevel(); i++)
	{
		const UINT DestSubresource = D3D11CalcSubresource(i, uiFace, pSource->GetMipLevel());
		const UINT SourceSubresource = D3D11CalcSubresource(i, 0, pSource->GetMipLevel());
		m_pDeviceContext->CopySubresourceRegion(pCubTextureID->m_pTexture, DestSubresource, 0, 0, 0, pTextureID->m_pTexture, SourceSubresource, NULL);
	}
	return true;
}
void * SKDX11Renderer::Lock(SKRenderTarget * pRenderTarget)
{
	return NULL;
}
void SKDX11Renderer::UnLock(SKRenderTarget * pRenderTarget)
{

}

void * SKDX11Renderer::Lock(SKDepthStencil * pDepthStencil)
{
	return NULL;
}
void SKDX11Renderer::UnLock(SKDepthStencil * pDepthStencil)
{

}
unsigned int SKDX11Renderer::GetTextureCompressLen(unsigned int uiLen)
{
	return Max((unsigned int)1, (unsigned int)(uiLen / 4));
}
void SKDX11Renderer::GetIncludeShader(SKString &OutString)
{
	OutString = ms_IncludeShader;
}

void SKDX11Renderer::GetDynamicShader(SKString &OutString)
{
	OutString = ms_DynamicShader;
}
const SKAdapterInfo* SKDX11Renderer::GetAdapterInfo(unsigned int & uiAdapterNum)
{
	uiAdapterNum = ms_uiAdapterNum;
	return ms_AdapterInfo;
}
bool SKDX11Renderer::IsSupportMulBufferSwtich()const
{
	return false;
}
bool SKDX11Renderer::IsSupportGS()const
{
	return true;
}