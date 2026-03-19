#include "SKDx9Renderer.h"
#include "SKDx9Resource.h"
#include "SKTexture.h"
#include "SKCubeTexture.h"
#include "SKVertexBuffer.h"
#include "SKVShader.h"
#include "SKPShader.h"
#include "SKGShader.h"
#include "SKGeometry.h"
#include "SKSegment3.h"
#include "SK2DTexture.h"
#include "SK3DTexture.h"
#include "SKCubeTexture.h"
#include "SKDepthStencil.h"
#include "SKUserConstant.h"
#include "SKDx9ShaderInclude.h"
#include "SKShaderKey.h"
#include "SKRenderTarget.h"
#include "SKGraphicInclude.h"
#include "SKProfiler.h"
using namespace SKEngine2;
DECLEAR_TIME_PROFILENODE(Dx9DrawMesh, )
LPDIRECT3D9 SKDX9Renderer::ms_pMain = NULL;
SKDx9ShaderInclude * SKDX9Renderer::ms_pDx9IncludeShader = NULL;
TCHAR SKDX9Renderer::ms_cVertexShaderProgramVersion[] = _T("vs_3_0");
TCHAR SKDX9Renderer::ms_cPixelShaderProgramVersion[] = _T("ps_3_0");
SKString SKDX9Renderer::ms_IncludeShader;
SKString SKDX9Renderer::ms_DynamicShader;
SKAdapterInfo	SKDX9Renderer::ms_AdapterInfo[5];
UINT SKDX9Renderer::ms_uiAdapterNum = 0;
DWORD SKDX9Renderer::ms_dwAlphaBlend[] = 
{
	D3DBLEND_ZERO,
	D3DBLEND_ONE,
	D3DBLEND_SRCCOLOR,
	D3DBLEND_INVSRCCOLOR,
	D3DBLEND_SRCALPHA,
	D3DBLEND_INVSRCALPHA,
	D3DBLEND_DESTALPHA,
	D3DBLEND_INVDESTALPHA,
	D3DBLEND_DESTCOLOR,
	D3DBLEND_INVDESTCOLOR

};
DWORD SKDX9Renderer::ms_dwAlphaBlendOP[] = 
{
	D3DBLENDOP_ADD,
	D3DBLENDOP_SUBTRACT,
	D3DBLENDOP_REVSUBTRACT,
	D3DBLENDOP_MIN,
	D3DBLENDOP_MAX 
};
DWORD SKDX9Renderer::ms_dwCullType[] =
{
	D3DCULL_NONE,
	D3DCULL_CW,
	D3DCULL_CCW
};
DWORD SKDX9Renderer::ms_dwStencilOperation[] = 
{
	D3DSTENCILOP_KEEP,
	D3DSTENCILOP_ZERO,
	D3DSTENCILOP_REPLACE,
	D3DSTENCILOP_INCR,
	D3DSTENCILOP_DECR,
	D3DSTENCILOP_INVERT,
	D3DSTENCILOP_INCRSAT,
	D3DSTENCILOP_DECRSAT
};
DWORD SKDX9Renderer::ms_dwCompare[] =
{
	D3DCMP_NEVER,
	D3DCMP_LESS,
	D3DCMP_EQUAL,
	D3DCMP_LESSEQUAL,
	D3DCMP_GREATER,
	D3DCMP_NOTEQUAL,
	D3DCMP_GREATEREQUAL,
	D3DCMP_ALWAYS
};
DWORD SKDX9Renderer::ms_dwTextureFormatType[] = 
{
	D3DFMT_A8R8G8B8,      
	D3DFMT_D16,          
	D3DFMT_D24X8,         
	D3DFMT_D32,           
	D3DFMT_A32B32G32R32F, 
	D3DFMT_A16B16G16R16F, 
	D3DFMT_G16R16F, 
	D3DFMT_R16F, 
	D3DFMT_R32F,
	D3DFMT_UNKNOWN,
	D3DFMT_D24S8,
	D3DFMT_G32R32F,
	D3DFMT_R5G6B5,
	D3DFMT_A8,
	D3DFMT_DXT3,
	D3DFMT_DXT5

};
DWORD SKDX9Renderer::ms_dwTextureFilterType[] =
{
	D3DTEXF_NONE,      
	D3DTEXF_POINT,       
	D3DTEXF_LINEAR,        
	D3DTEXF_ANISOTROPIC  
};
DWORD SKDX9Renderer::ms_dwTextureAddrType[] =
{
	D3DTADDRESS_CLAMP,      
	D3DTADDRESS_WRAP,       
	D3DTADDRESS_MIRROR,     
	D3DTADDRESS_BORDER,    
};
DWORD SKDX9Renderer::ms_dwPrimitiveType[] =
{
	D3DPT_POINTLIST,
	D3DPT_LINELIST,
	D3DPT_TRIANGLELIST
};
DWORD SKDX9Renderer::ms_dwDevTypes[] = 
{ 
	D3DDEVTYPE_HAL, 
	D3DDEVTYPE_REF
};
DWORD SKDX9Renderer::ms_dwMultiSampleTypes[] =
{
	D3DMULTISAMPLE_NONE      ,
	D3DMULTISAMPLE_2_SAMPLES ,
	D3DMULTISAMPLE_4_SAMPLES ,
	D3DMULTISAMPLE_8_SAMPLES ,
	D3DMULTISAMPLE_16_SAMPLES 
};
DWORD SKDX9Renderer::ms_dwCubeMapFace[] =
{
	D3DCUBEMAP_FACE_POSITIVE_X,
	D3DCUBEMAP_FACE_NEGATIVE_X,
	D3DCUBEMAP_FACE_POSITIVE_Y,
	D3DCUBEMAP_FACE_NEGATIVE_Y,
	D3DCUBEMAP_FACE_POSITIVE_Z,
	D3DCUBEMAP_FACE_NEGATIVE_Z
};
DWORD SKDX9Renderer::ms_dwLockFlag[] = 
{
	D3DLOCK_NOOVERWRITE,
	D3DLOCK_DISCARD,
	D3DLOCK_READONLY
};
DWORD SKDX9Renderer::ms_dwVertexFormatDateType[] = 
{
	D3DDECLTYPE_FLOAT1, 
	D3DDECLTYPE_FLOAT2,
	D3DDECLTYPE_FLOAT3,
	D3DDECLTYPE_FLOAT4,

	0,
	D3DDECLTYPE_D3DCOLOR,

	D3DDECLTYPE_UBYTE4,
	D3DDECLTYPE_UBYTE4N,

	D3DDECLTYPE_SHORT2,
	D3DDECLTYPE_SHORT2N,
	D3DDECLTYPE_USHORT2N,

	D3DDECLTYPE_SHORT4,
	D3DDECLTYPE_SHORT4N,
	D3DDECLTYPE_USHORT4N,

	D3DDECLTYPE_FLOAT16_2,
	D3DDECLTYPE_FLOAT16_4,

	D3DDECLTYPE_UBYTE4N
};
DWORD SKDX9Renderer::ms_dwVertexFormatSemantics[] = 
{
	D3DDECLUSAGE_POSITION,
	D3DDECLUSAGE_TEXCOORD,
	D3DDECLUSAGE_NORMAL,
	D3DDECLUSAGE_TANGENT,
	D3DDECLUSAGE_BINORMAL,
	D3DDECLUSAGE_PSIZE,
	D3DDECLUSAGE_COLOR,        
	D3DDECLUSAGE_FOG,           
	D3DDECLUSAGE_DEPTH,
	D3DDECLUSAGE_BLENDWEIGHT,
	D3DDECLUSAGE_BLENDINDICES
};
int SKDX9Renderer::GetRendererType() const
{

	return SKRenderer::RAT_DIRECTX9;
}
const TCHAR * SKDX9Renderer::GetRendererStringType() const
{ return _T("DX9Renderer"); }
unsigned int SKDX9Renderer::SetGSamplerState(SKSamplerState * pSamplerState, unsigned int i, bool bForceSet)
{
	SKRenderer::SetGSamplerState(pSamplerState, i, bForceSet);

	return 1;
}
unsigned int SKDX9Renderer::SetDSamplerState(SKSamplerState * pSamplerState, unsigned int i, bool bForceSet)
{
	SKRenderer::SetDSamplerState(pSamplerState, i, bForceSet);

	return 1;
}
unsigned int SKDX9Renderer::SetHSamplerState(SKSamplerState * pSamplerState, unsigned int i, bool bForceSet)
{
	SKRenderer::SetHSamplerState(pSamplerState, i, bForceSet);

	return 1;
}
unsigned int SKDX9Renderer::SetVSamplerState(SKSamplerState * pSamplerState, unsigned int i, bool bForceSet)
{
	unsigned int FRI = SKRenderer::SetSamplerState(pSamplerState, i, bForceSet);
	if (FRI == FRI_SAMERESOURCE)
		return FRI_SUCCESS;
	SKMAC_ASSERT(FRI == FRI_SUCCESS);
	if (FRI == FRI_FAIL)
	{
		return FRI_FAIL;
	}
	if (!pSamplerState)
	{
		pSamplerState = (SKSamplerState *)SKSamplerState::GetDefault();
	}

	m_pVSamplerState[i] = pSamplerState;
	return 1;
}
unsigned int SKDX9Renderer::SetSamplerState(SKSamplerState * pSamplerState, unsigned int i, bool bForceSet)
{
	unsigned int FRI = SKRenderer::SetSamplerState(pSamplerState, i, bForceSet);
	if (FRI == FRI_SAMERESOURCE)
		return FRI_SUCCESS;
	SKMAC_ASSERT(FRI == FRI_SUCCESS);
	if (FRI == FRI_FAIL)
	{
		return FRI_FAIL;
	}
	if (!pSamplerState)
	{
		pSamplerState = (SKSamplerState *)SKSamplerState::GetDefault();
	}
	const SKSamplerDesc & BlendStateSrc = m_pPSamplerState[i]->GetSamplerDesc();
	const SKSamplerDesc & BlendStateDest = pSamplerState->GetSamplerDesc();

	if (BlendStateSrc.m_uiCoordU != BlendStateDest.m_uiCoordU)
	{
		SetSamplerState(i, D3DSAMP_ADDRESSU, ms_dwTextureAddrType[BlendStateDest.m_uiCoordU]);
	}

	if (BlendStateSrc.m_uiCoordV != BlendStateDest.m_uiCoordV)
	{

		SetSamplerState(i, D3DSAMP_ADDRESSV, ms_dwTextureAddrType[BlendStateDest.m_uiCoordV]);
	}

	if (BlendStateSrc.m_uiCoordW != BlendStateDest.m_uiCoordW)
	{
		SetSamplerState(i, D3DSAMP_ADDRESSW, ms_dwTextureAddrType[BlendStateDest.m_uiCoordW]);
	}

	if (BlendStateSrc.m_BorderColor.GetDWARGB() != BlendStateDest.m_BorderColor.GetDWARGB())
	{

		SetSamplerState(i, D3DSAMP_BORDERCOLOR, BlendStateDest.m_BorderColor.GetDWARGB());
	}


	if (BlendStateSrc.m_uiMag != BlendStateDest.m_uiMag)
	{

		SetSamplerState(i, D3DSAMP_MAGFILTER, ms_dwTextureFilterType[BlendStateDest.m_uiMag]);
	}

	if (BlendStateSrc.m_uiMin != BlendStateDest.m_uiMin)
	{
		SetSamplerState(i, D3DSAMP_MINFILTER, ms_dwTextureFilterType[BlendStateDest.m_uiMin]);
	}

	if (BlendStateSrc.m_uiMip != BlendStateDest.m_uiMip)
	{
		SetSamplerState(i, D3DSAMP_MIPFILTER, ms_dwTextureFilterType[BlendStateDest.m_uiMip]);
	}

	if (BlendStateSrc.m_uiMipLevel != BlendStateDest.m_uiMipLevel)
	{
		SetSamplerState(i, D3DSAMP_MAXMIPLEVEL, BlendStateDest.m_uiMipLevel);
	}


	if (BlendStateDest.m_uiAniLevel <= m_uiCurAnisotropy)
	{
		if (BlendStateSrc.m_uiAniLevel != BlendStateDest.m_uiAniLevel)
		{
			SetSamplerState(i, D3DSAMP_MAXANISOTROPY, BlendStateDest.m_uiAniLevel);
		}
	}
	else
	{
		if (BlendStateSrc.m_uiAniLevel > m_uiCurAnisotropy)
		{
			SetSamplerState(i, D3DSAMP_MAXANISOTROPY, m_uiCurAnisotropy);
		}

	}
	m_pPSamplerState[i] = pSamplerState;
	return 1;
}
unsigned int SKDX9Renderer::SetBlendState(SKBlendState * pBlendState,bool bForceSet)
{
	unsigned int FRI = SKRenderer::SetBlendState(pBlendState, bForceSet);
	if (FRI == FRI_SAMERESOURCE)
		return FRI_SUCCESS;
	SKMAC_ASSERT(FRI == FRI_SUCCESS);
	if (FRI == FRI_FAIL)
	{
		return FRI_FAIL;
	}
	if (!pBlendState)
	{
		pBlendState = (SKBlendState *)SKBlendState::GetDefault();
	}
	const SKBlendDesc & BlendStateSrc = m_pBlendState->GetBlendDesc();
	const SKBlendDesc & BlendStateDest = pBlendState->GetBlendDesc();
	if (BlendStateSrc.bBlendEnable[0] != BlendStateDest.bBlendEnable[0])
	{
		SetRenderState(D3DRS_ALPHABLENDENABLE, BlendStateDest.bBlendEnable[0]);
	}


	if (BlendStateSrc.ucSrcBlend[0] != BlendStateDest.ucSrcBlend[0])
	{
		SetRenderState(D3DRS_SRCBLEND,
			ms_dwAlphaBlend[BlendStateDest.ucSrcBlend[0]]);
	}

	if (BlendStateSrc.ucDestBlend[0] != BlendStateDest.ucDestBlend[0])
	{
		SetRenderState(D3DRS_DESTBLEND,
			ms_dwAlphaBlend[BlendStateDest.ucDestBlend[0]]);
	}

	if (BlendStateSrc.ucBlendOp[0] != BlendStateDest.ucBlendOp[0])
	{
		SetRenderState(D3DRS_BLENDOP,
			ms_dwAlphaBlendOP[BlendStateDest.ucBlendOp[0]]);
	}



	if (BlendStateSrc.bAlphaBlendEnable[0] != BlendStateDest.bAlphaBlendEnable[0])
	{
		SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, BlendStateDest.bAlphaBlendEnable[0]);
	}


	if (BlendStateSrc.ucSrcBlendAlpha[0] != BlendStateDest.ucSrcBlendAlpha[0])
	{
		SetRenderState(D3DRS_SRCBLENDALPHA,
			ms_dwAlphaBlend[BlendStateDest.ucSrcBlendAlpha[0]]);
	}


	if (BlendStateSrc.ucDestBlendAlpha[0] != BlendStateDest.ucDestBlendAlpha[0])
	{
		SetRenderState(D3DRS_DESTBLENDALPHA,
			ms_dwAlphaBlend[BlendStateDest.ucDestBlendAlpha[0]]);
	}


	if (BlendStateSrc.ucBlendOpAlpha[0] != BlendStateDest.ucBlendOpAlpha[0])
	{
		SetRenderState(D3DRS_BLENDOPALPHA,
			ms_dwAlphaBlendOP[BlendStateDest.ucBlendOpAlpha[0]]);
	}


	for (unsigned int i = 0; i < 4; i++)
	{

		if (BlendStateSrc.ucWriteMask[i] != BlendStateDest.ucWriteMask[i])
		{
			DWORD dwMask = 0;

			if (BlendStateDest.ucWriteMask[i] & SKBlendDesc::WM_RED)
			{
				dwMask |= D3DCOLORWRITEENABLE_RED;
			}

			if (BlendStateDest.ucWriteMask[i] & SKBlendDesc::WM_Green)
			{
				dwMask |= D3DCOLORWRITEENABLE_GREEN;
			}

			if (BlendStateDest.ucWriteMask[i] & SKBlendDesc::WM_BLUE)
			{
				dwMask |= D3DCOLORWRITEENABLE_BLUE;
			}

			if (BlendStateDest.ucWriteMask[i] & SKBlendDesc::WM_ALPHA)
			{
				dwMask |= D3DCOLORWRITEENABLE_ALPHA;
			}

			if (i == 0)
			{
				SetRenderState(D3DRS_COLORWRITEENABLE, dwMask);
			}
			else
			{
				SetRenderState(D3DRENDERSTATETYPE(D3DRS_COLORWRITEENABLE1 + i - 1), dwMask);
			}
		}

	}

	m_pBlendState = pBlendState;
	return 1;
}
unsigned int SKDX9Renderer::SetDepthStencilState(SKDepthStencilState * pDepthStencilState, bool bForceSet)
{
	unsigned int FRI = SKRenderer::SetDepthStencilState(pDepthStencilState, bForceSet);
	if (FRI == FRI_SAMERESOURCE)
		return FRI_SUCCESS;
	SKMAC_ASSERT(FRI == FRI_SUCCESS);
	if (FRI == FRI_FAIL)
	{
		return FRI_FAIL;
	}
	if (!pDepthStencilState)
	{
		pDepthStencilState = (SKDepthStencilState *)SKDepthStencilState::GetDefault();
	}
	const SKDepthStencilDesc & DepthStencilDescDest = pDepthStencilState->GetDepthStencilDesc();
	const SKDepthStencilDesc & DepthStencilDescSrc = m_pDepthStencilState->GetDepthStencilDesc();

	if (DepthStencilDescSrc.m_bDepthEnable != DepthStencilDescDest.m_bDepthEnable)
	{
		if (DepthStencilDescDest.m_bDepthEnable)
		{
			SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		}
		else
		{
			SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		}
	}



	if (DepthStencilDescSrc.m_uiDepthCompareMethod != DepthStencilDescDest.m_uiDepthCompareMethod)
	{
		SetRenderState(D3DRS_ZFUNC,
			ms_dwCompare[DepthStencilDescDest.m_uiDepthCompareMethod]);
	}

	if (DepthStencilDescSrc.m_bDepthWritable != DepthStencilDescDest.m_bDepthWritable)
	{

		SetRenderState(D3DRS_ZWRITEENABLE, DepthStencilDescDest.m_bDepthWritable);
	}


	if (DepthStencilDescSrc.m_bStencilEnable != DepthStencilDescDest.m_bStencilEnable)
	{

		SetRenderState(D3DRS_STENCILENABLE, DepthStencilDescDest.m_bStencilEnable);
	}


	if (DepthStencilDescSrc.m_uiStencilCompareMethod != DepthStencilDescDest.m_uiStencilCompareMethod)
	{

		SetRenderState(D3DRS_STENCILFUNC,
			ms_dwCompare[DepthStencilDescDest.m_uiStencilCompareMethod]);
	}


	if (DepthStencilDescSrc.m_uiCCW_StencilCompareMethod != DepthStencilDescDest.m_uiCCW_StencilCompareMethod)
	{

		SetRenderState(D3DRS_CCW_STENCILFUNC,
			ms_dwCompare[DepthStencilDescDest.m_uiCCW_StencilCompareMethod]);
	}

	if (DepthStencilDescSrc.m_uiReference != DepthStencilDescDest.m_uiReference)
	{

		SetRenderState(D3DRS_STENCILREF,
			DepthStencilDescDest.m_uiReference);
	}

	if (DepthStencilDescSrc.m_uiMask != DepthStencilDescDest.m_uiMask)
	{
		SetRenderState(D3DRS_STENCILMASK,
			DepthStencilDescDest.m_uiMask);
	}

	if (DepthStencilDescSrc.m_uiWriteMask != DepthStencilDescDest.m_uiWriteMask)
	{
		SetRenderState(D3DRS_STENCILWRITEMASK,
			DepthStencilDescDest.m_uiWriteMask);
	}

	if (DepthStencilDescSrc.m_uiSFailZPassOP != DepthStencilDescDest.m_uiSFailZPassOP)
	{
		SetRenderState(D3DRS_STENCILFAIL,
			ms_dwStencilOperation[DepthStencilDescDest.m_uiSFailZPassOP]);
	}

	if (DepthStencilDescSrc.m_uiSPassZFailOP != DepthStencilDescDest.m_uiSPassZFailOP)
	{
		SetRenderState(D3DRS_STENCILZFAIL,
			ms_dwStencilOperation[DepthStencilDescDest.m_uiSPassZFailOP]);
	}

	if (DepthStencilDescSrc.m_uiSPassZPassOP != DepthStencilDescDest.m_uiSPassZPassOP)
	{
		SetRenderState(D3DRS_STENCILPASS,
			ms_dwStencilOperation[DepthStencilDescDest.m_uiSPassZPassOP]);
	}

	if (DepthStencilDescSrc.m_uiCCW_SFailZPassOP != DepthStencilDescDest.m_uiCCW_SFailZPassOP)
	{
		SetRenderState(D3DRS_CCW_STENCILFAIL,
			ms_dwStencilOperation[DepthStencilDescDest.m_uiCCW_SFailZPassOP]);
	}

	if (DepthStencilDescSrc.m_uiCCW_SPassZFailOP != DepthStencilDescDest.m_uiCCW_SPassZFailOP)
	{
		SetRenderState(D3DRS_CCW_STENCILZFAIL,
			ms_dwStencilOperation[DepthStencilDescDest.m_uiCCW_SPassZFailOP]);
	}

	if (DepthStencilDescSrc.m_uiCCW_SPassZPassOP != DepthStencilDescDest.m_uiCCW_SPassZPassOP)
	{
		SetRenderState(D3DRS_CCW_STENCILPASS,
			ms_dwStencilOperation[DepthStencilDescDest.m_uiCCW_SPassZPassOP]);
	}
	if (DepthStencilDescSrc.m_bTwoSideStencilMode != DepthStencilDescDest.m_bTwoSideStencilMode)
	{
		SetRenderState(D3DRS_TWOSIDEDSTENCILMODE,
			DepthStencilDescDest.m_bTwoSideStencilMode);
	}

	m_pDepthStencilState = pDepthStencilState;
	return 1;
}
unsigned int SKDX9Renderer::SetRasterizerState(SKRasterizerState * pRasterizerState, bool bForceSet)
{
	unsigned int FRI = SKRenderer::SetRasterizerState(pRasterizerState, bForceSet);
	if (FRI == FRI_SAMERESOURCE)
		return FRI_SUCCESS;
	SKMAC_ASSERT(FRI == FRI_SUCCESS);
	if (FRI == FRI_FAIL)
	{
		return FRI_FAIL;
	}
	if (!pRasterizerState)
	{
		pRasterizerState = (SKRasterizerState *)SKRasterizerState::GetDefault();
	}
	const SKRasterizerDesc & RasterizerStateDest = pRasterizerState->GetRasterizerDesc();
	const SKRasterizerDesc & RasterizerStateScr = m_pRasterizerState->GetRasterizerDesc();
	if (RasterizerStateScr.m_fDepthBias != RasterizerStateDest.m_fDepthBias)
	{
		SetRenderState(D3DRS_DEPTHBIAS, *(DWORD *)&RasterizerStateDest.m_fDepthBias);
	}
	if (RasterizerStateScr.m_bClipPlaneEnable != RasterizerStateDest.m_bClipPlaneEnable)
	{
		if (!RasterizerStateDest.m_bClipPlaneEnable)
		{
			SetRenderState(D3DRS_CLIPPLANEENABLE, 0);
		}
	}
	if (RasterizerStateScr.m_bScissorTestEnable != RasterizerStateDest.m_bScissorTestEnable)
	{
		SetRenderState(D3DRS_SCISSORTESTENABLE, RasterizerStateDest.m_bScissorTestEnable);
	}
	if (RasterizerStateScr.m_bWireEnable != RasterizerStateDest.m_bWireEnable)
	{
		if (RasterizerStateDest.m_bWireEnable)
		{
			SetRenderState(D3DRS_FILLMODE,
				D3DFILL_WIREFRAME);
		}
		else
		{
			SetRenderState(D3DRS_FILLMODE,
				D3DFILL_SOLID);
		}
	}
	if (RasterizerStateScr.m_uiCullType != RasterizerStateDest.m_uiCullType)
	{
		SetRenderState(D3DRS_CULLMODE,
			ms_dwCullType[RasterizerStateDest.m_uiCullType]);
	}
	m_pRasterizerState = pRasterizerState;
	return 1;
}
void SKDX9Renderer::SetClipPlane(SKArray<SKPlane3> &Plane, bool bEnable)
{
	if (bEnable)
	{
		for (unsigned int i = 0; i < Plane.GetNum(); i++)
		{
			SetRenderState(D3DRS_CLIPPLANEENABLE, 1 << i);
			SetClipPlane(i, (SKREAL *)&Plane[i]);
		}
	}
	else
	{
		SetRenderState(D3DRS_CLIPPLANEENABLE, 0);
	}

}
void SKDX9Renderer::SetScissorRect(SKArray<SKRect2> &Rect, bool bEnable)
{
	if (bEnable)
	{
		RECT Temp;
		Temp.left = (LONG)Rect[0].m_Min.x;
		Temp.top = (LONG)Rect[0].m_Min.y;
		Temp.right = (LONG)Rect[0].m_Max.x;
		Temp.bottom = (LONG)Rect[0].m_Max.y;
		SetScissorRect(&Temp);
	}
	else
	{
		SetScissorRect(NULL);
	}
}
void SKDX9Renderer::ClearBackBuffer()
{

	Clear(0, 0, D3DCLEAR_TARGET, m_ClearColor.GetDWARGB(), 1.0f, 0);
}
void SKDX9Renderer::ClearZBuffer()
{
	Clear(0, 0, D3DCLEAR_ZBUFFER, 0, m_fClearDepth, 0);
}
void SKDX9Renderer::ClearStencilBuffer()
{
	Clear(0, 0, D3DCLEAR_STENCIL, 0, 1.0f, (DWORD)m_uiClearStencil);
}
void SKDX9Renderer::ClearBuffers(unsigned int uiClearFlag)
{
	if (!uiClearFlag)
	{
		return;
	}
	DWORD Flag = 0;
	if (uiClearFlag & CF_COLOR)
	{
		Flag |= D3DCLEAR_TARGET;
	}
	if (uiClearFlag & CF_DEPTH)
	{
		Flag |= D3DCLEAR_ZBUFFER;
	}
	if (uiClearFlag & CF_STENCIL)
	{
		Flag |= D3DCLEAR_STENCIL;
	}
	Clear(0, 0,
		Flag,
		m_ClearColor.GetDWARGB(), m_fClearDepth, (DWORD)m_uiClearStencil);

}
void SKDX9Renderer::ClearBuffers()
{
	Clear(0, 0,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		m_ClearColor.GetDWARGB(), m_fClearDepth, (DWORD)m_uiClearStencil);
}
void SKDX9Renderer::ClearBackBuffer(int iXPos, int iYPos, int iWidth, int iHeight)
{
	D3DRECT kRect;
	kRect.x1 = (long)iXPos;
	kRect.y1 = (long)iYPos;
	kRect.x2 = (long)(iXPos + iWidth - 1);
	kRect.y2 = (long)(iYPos + iHeight - 1);


	Clear(1, &kRect, D3DCLEAR_TARGET, m_ClearColor.GetDWARGB(),
		1.0f, 0);

}
void SKDX9Renderer::ClearZBuffer(int iXPos, int iYPos, int iWidth, int iHeight)
{
	D3DRECT kRect;
	kRect.x1 = (long)iXPos;
	kRect.y1 = (long)iYPos;
	kRect.x2 = (long)(iXPos + iWidth - 1);
	kRect.y2 = (long)(iYPos + iHeight - 1);

	Clear(1, &kRect, D3DCLEAR_ZBUFFER, 0,
		m_fClearDepth, 0);

}
void SKDX9Renderer::ClearStencilBuffer(int iXPos, int iYPos, int iWidth, int iHeight)
{
	D3DRECT kRect;
	kRect.x1 = (long)iXPos;
	kRect.y1 = (long)iYPos;
	kRect.x2 = (long)(iXPos + iWidth - 1);
	kRect.y2 = (long)(iYPos + iHeight - 1);

	Clear(1, &kRect, D3DCLEAR_STENCIL, 0, 1.0f,
		(DWORD)m_uiClearStencil);
}
void SKDX9Renderer::ClearBuffers(unsigned int uiClearFlag, int iXPos, int iYPos, int iWidth,
	int iHeight)
{
	if (!uiClearFlag)
	{
		return;
	}
	DWORD Flag = 0;
	if (uiClearFlag & CF_COLOR)
	{
		Flag |= D3DCLEAR_TARGET;
	}
	if (uiClearFlag & CF_DEPTH)
	{
		Flag |= D3DCLEAR_ZBUFFER;
	}
	if (uiClearFlag & CF_STENCIL)
	{
		Flag |= D3DCLEAR_STENCIL;
	}
	D3DRECT kRect;
	kRect.x1 = (long)iXPos;
	kRect.y1 = (long)iYPos;
	kRect.x2 = (long)(iXPos + iWidth - 1);
	kRect.y2 = (long)(iYPos + iHeight - 1);


	Clear(1, &kRect,
		Flag,
		m_ClearColor.GetDWARGB(), m_fClearDepth, (DWORD)m_uiClearStencil);
}
void SKDX9Renderer::ClearBuffers(int iXPos, int iYPos, int iWidth, int iHeight)
{
	D3DRECT kRect;
	kRect.x1 = (long)iXPos;
	kRect.y1 = (long)iYPos;
	kRect.x2 = (long)(iXPos + iWidth - 1);
	kRect.y2 = (long)(iYPos + iHeight - 1);


	Clear(1, &kRect,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		m_ClearColor.GetDWARGB(), m_fClearDepth, (DWORD)m_uiClearStencil);

}
void SKDX9Renderer::GetShaderInstruction(ID3DXBuffer * pShader, unsigned int &uiArithmeticInstructionSlots, unsigned int &uiTextureInstructionSlots)
{
	HRESULT hResult = NULL;
	ID3DXBuffer* DisassemblyBuffer;
	hResult = D3DXDisassembleShader((const DWORD*)pShader->GetBufferPointer(), FALSE, NULL, &DisassemblyBuffer);
	SKMAC_ASSERT(!FAILED(hResult));

	SKString TempCache = (TCHAR*)DisassemblyBuffer->GetBufferPointer();

	SKMAC_RELEASE(DisassemblyBuffer);

	TCHAR TempBuffer[SKString::BUFFER_SIZE];

	int iIndex = TempCache.GetSubStringIndex(_T("approximately"), -1);

	SKString Useful = &TempCache[iIndex];

	iIndex = Useful.GetSubStringIndex(_T("instruction"), 1);

	int iInstructionStringNum = iIndex - SKStrLen(_T("approximately"));

	SKMemcpy(TempBuffer, Useful.GetBuffer() + SKStrLen(_T("approximately")), iInstructionStringNum);

	TempBuffer[iInstructionStringNum] = _T('\0');

	SKString InstructionString = TempBuffer;

	uiArithmeticInstructionSlots = StringToInt(InstructionString);
	unsigned int TextureIndex = Useful.GetSubStringIndex(_T("texture"), 1);

	if (TextureIndex != -1)
	{
		iIndex = Useful.GetSubStringIndex(_T("("), 1);

		SKMemcpy(TempBuffer, Useful.GetBuffer() + iIndex + 1, TextureIndex - iIndex - 1);
		TempBuffer[TextureIndex - iIndex - 1] = _T('\0');
		SKString TextureString = TempBuffer;
		uiTextureInstructionSlots = StringToInt(TextureString);
		uiArithmeticInstructionSlots -= uiTextureInstructionSlots;

	}

}
void SKDX9Renderer::OnLoadVShaderFromString(SKVShader * pVShaderProgram, SKVProgramID *pID)
{
	HRESULT hResult = NULL;
	LPD3DXBUFFER pCode = NULL;
	LPD3DXBUFFER pErrors = NULL;
	DWORD Flags = NULL;
	// #ifdef _DEBUG
	// 	Flags = D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION;
	// #endif
	SKMap<SKString, SKString> Define;
	pVShaderProgram->m_ShaderKey.GetDefine(Define);
	D3DXMACRO * pMacro = GetDefine(Define);
	LPD3DXCONSTANTTABLE          pConstantTable = NULL; //
	if (pVShaderProgram->GetBuffer().GetLength())
	{
		hResult = D3DXCompileShader((LPCSTR)pVShaderProgram->GetBuffer().GetBuffer(), pVShaderProgram->GetBuffer().GetLength(), pMacro, ms_pDx9IncludeShader, pVShaderProgram->GetMainFunName().GetBuffer(),
			ms_cVertexShaderProgramVersion, Flags, &pCode, &pErrors, &pConstantTable);


	}
	else if (pVShaderProgram->GetResourceName().GetLength())
	{
		SKString RenderAPIPre = SKResourceManager::GetRenderTypeShaderPath(RAT_DIRECTX9);
		SKString Path = SKResourceManager::ms_ShaderPath + RenderAPIPre + pVShaderProgram->GetResourceName().GetString();
		hResult = D3DXCompileShaderFromFile((LPCSTR)Path.GetBuffer(), pMacro, ms_pDx9IncludeShader, pVShaderProgram->GetMainFunName().GetBuffer(),
			ms_cVertexShaderProgramVersion, Flags, &pCode, &pErrors, &pConstantTable);
	}
	else
		SKMAC_ASSERT(0);

	SKMAC_DELETEA(pMacro);
#ifdef _DEBUG
	if (pErrors && pErrors->GetBufferPointer())
	{
		SKLog Error;
		SKString SKErrorString = SKResourceManager::ms_OutPutLogPath + _T("SKError.txt");
		Error.Open(SKErrorString.GetBuffer());
		Error.WriteInfo(pVShaderProgram->GetResourceName().GetBuffer());
		Error.WriteInfo("\n");
		Error.WriteInfo((const TCHAR *)pErrors->GetBufferPointer());
		SKOutPutDebugString(pVShaderProgram->GetResourceName().GetBuffer());
		Error.WriteInfo("\n");
		SKOutPutDebugString((const TCHAR *)pErrors->GetBufferPointer());
	}
#endif
	SKMAC_ASSERT(!FAILED(hResult));
	SKMAC_ASSERT(pCode);
	SKMAC_ASSERT(pConstantTable);

	if (!pVShaderProgram->m_pUserConstant.GetNum() && !pVShaderProgram->m_pUserSampler.GetNum())
	{
		D3DXCONSTANTTABLE_DESC ConstantTableDesc;
		hResult = pConstantTable->GetDesc(&ConstantTableDesc);
		SKMAC_ASSERT(!FAILED(hResult));

		for (unsigned int uiConstantIndex = 0; uiConstantIndex < ConstantTableDesc.Constants; uiConstantIndex++)
		{
			// Read the constant description.
			D3DXHANDLE ConstantHandle = pConstantTable->GetConstant(NULL, uiConstantIndex);
			D3DXCONSTANT_DESC ConstantDesc;
			unsigned int NumConstants = 1;
			pConstantTable->GetConstantDesc(ConstantHandle, &ConstantDesc, &NumConstants);
			if (ConstantDesc.RegisterSet != D3DXRS_SAMPLER)
			{

				unsigned int uiValueType = 0;
				if (ConstantDesc.RegisterSet == D3DXRS_BOOL)
				{
					uiValueType = SKUserConstant::VT_BOOL;
				}
				else if (ConstantDesc.RegisterSet == D3DXRS_INT4)
				{
					uiValueType = SKUserConstant::VT_INT;
				}
				else if (ConstantDesc.RegisterSet == D3DXRS_FLOAT4)
				{
					uiValueType = SKUserConstant::VT_FLOAT;
				}
				else
				{
					SKMAC_ASSERT(0);
				}
				SKUserConstant * pUserConstant =
					SK_NEW SKUserConstant(ConstantDesc.Name, NULL, ConstantDesc.Bytes, ConstantDesc.RegisterIndex, ConstantDesc.RegisterCount, uiValueType);
				pVShaderProgram->m_pUserConstant.AddElement(pUserConstant);

			}
			else
			{
				unsigned int uiType = 0;
				if (ConstantDesc.Type == D3DXPT_SAMPLER1D)
				{
					uiType = SKTexture::TT_1D;
				}
				else if (ConstantDesc.Type == D3DXPT_SAMPLER2D)
				{
					uiType = SKTexture::TT_2D;
				}
				else if (ConstantDesc.Type == D3DXPT_SAMPLER3D)
				{
					uiType = SKTexture::TT_3D;
				}
				else if (ConstantDesc.Type == D3DXPT_SAMPLERCUBE)
				{
					uiType = SKTexture::TT_CUBE;
				}
				else
				{
					SKMAC_ASSERT(0);
				}
				SKUserSampler * pUerSampler = SK_NEW SKUserSampler(ConstantDesc.Name, uiType, ConstantDesc.RegisterIndex, ConstantDesc.RegisterCount);
				pVShaderProgram->m_pUserSampler.AddElement(pUerSampler);
			}
		}
		SKMAC_RELEASE(pConstantTable);
		pVShaderProgram->m_bCreatePara = true;
	}
	hResult = m_pDevice->CreateVertexShader((DWORD*)pCode->GetBufferPointer(), &pID->m_pVertexShader);
	SKMAC_ASSERT(!FAILED(hResult));
	pVShaderProgram->SetCacheBuffer(pCode->GetBufferPointer(), pCode->GetBufferSize());
	GetShaderInstruction(pCode, pVShaderProgram->m_uiArithmeticInstructionSlots, pVShaderProgram->m_uiTextureInstructionSlots);
	SKMAC_RELEASE(pCode);

	SKMAC_RELEASE(pErrors);
}
bool SKDX9Renderer::OnLoadVShaderProgram(SKVShader * pVShaderProgram, SKResourceIdentifier *&pID)
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
		// 		HRESULT hResult = NULL;
		// 		hResult = m_pDevice->CreateVertexShader((DWORD*)pVShaderProgram->GetCacheBuffer(),&pVProgramID->m_pVertexShader );
		// 		SKMAC_ASSERT(!FAILED(hResult));
		CreateVShaderFromCache(pVShaderProgram, pVProgramID);
	}

	return 1;
}
bool SKDX9Renderer::OnReleaseVShaderProgram(SKResourceIdentifier * pVShaderProgramID)
{
	SKVProgramID *pVProgramID = (SKVProgramID *)pVShaderProgramID;

	//SKMAC_RELEASE(pVProgramID->m_pConstantTable);

	//SKMAC_RELEASE(pVProgramID->m_pVertexShader);
	SKMAC_DELETE(pVProgramID)
		return 1;
}
void SKDX9Renderer::OnLoadPShaderFromString(SKPShader* pPShaderProgram, SKPProgramID *pID)
{
	HRESULT hResult = NULL;
	LPD3DXBUFFER pCode = NULL;
	LPD3DXBUFFER pErrors = NULL;
	DWORD Flags = NULL;
	// #ifdef _DEBUG
	// 	Flags = D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION;
	// #endif
	LPD3DXCONSTANTTABLE      pConstantTable; //
	SKMap<SKString, SKString> Define;
	pPShaderProgram->m_ShaderKey.GetDefine(Define);
	D3DXMACRO * pMacro = GetDefine(Define);
	if (pPShaderProgram->GetBuffer().GetLength())
	{
		hResult = D3DXCompileShader((LPCSTR)pPShaderProgram->GetBuffer().GetBuffer(), pPShaderProgram->GetBuffer().GetLength(), pMacro, ms_pDx9IncludeShader, pPShaderProgram->GetMainFunName().GetBuffer(),
			ms_cPixelShaderProgramVersion, Flags, &pCode, &pErrors, &pConstantTable);


	}
	else if (pPShaderProgram->GetResourceName().GetLength())
	{
		SKString RenderAPIPre = SKResourceManager::GetRenderTypeShaderPath(RAT_DIRECTX9);
		SKString Path = SKResourceManager::ms_ShaderPath + RenderAPIPre + pPShaderProgram->GetResourceName().GetString();
		hResult = D3DXCompileShaderFromFile((LPCSTR)Path.GetBuffer(), pMacro, ms_pDx9IncludeShader, pPShaderProgram->GetMainFunName().GetBuffer(),
			ms_cPixelShaderProgramVersion, Flags, &pCode, &pErrors, &pConstantTable);
	}
	else
	{
		SKMAC_ASSERT(0);
	}
	SKMAC_DELETEA(pMacro);
#ifdef _DEBUG
	if (pErrors && pErrors->GetBufferPointer())
	{
		SKLog Error;
		SKString PSErrorString = SKResourceManager::ms_OutPutLogPath + _T("PSError.txt");
		Error.Open(PSErrorString.GetBuffer());
		Error.WriteInfo(pPShaderProgram->GetResourceName().GetBuffer());
		Error.WriteInfo("\n");
		Error.WriteInfo((const TCHAR *)pErrors->GetBufferPointer());
		SKOutPutDebugString(pPShaderProgram->GetResourceName().GetBuffer());
		Error.WriteInfo("\n");
		SKOutPutDebugString((const TCHAR *)pErrors->GetBufferPointer());
	}
#endif
	SKMAC_ASSERT(!FAILED(hResult));
	SKMAC_ASSERT(pCode);
	SKMAC_ASSERT(pConstantTable);
	if (!pPShaderProgram->m_pUserConstant.GetNum() && !pPShaderProgram->m_pUserSampler.GetNum())
	{
		D3DXCONSTANTTABLE_DESC ConstantTableDesc;
		hResult = pConstantTable->GetDesc(&ConstantTableDesc);
		SKMAC_ASSERT(!FAILED(hResult));
		for (unsigned int uiConstantIndex = 0; uiConstantIndex < ConstantTableDesc.Constants; uiConstantIndex++)
		{
			// Read the constant description.
			D3DXHANDLE ConstantHandle = pConstantTable->GetConstant(NULL, uiConstantIndex);
			D3DXCONSTANT_DESC ConstantDesc;
			unsigned int NumConstants = 1;
			pConstantTable->GetConstantDesc(ConstantHandle, &ConstantDesc, &NumConstants);


			if (ConstantDesc.RegisterSet != D3DXRS_SAMPLER)
			{

				unsigned int uiValueType = 0;
				if (ConstantDesc.RegisterSet == D3DXRS_BOOL)
				{
					uiValueType = SKUserConstant::VT_BOOL;
				}
				else if (ConstantDesc.RegisterSet == D3DXRS_INT4)
				{
					uiValueType = SKUserConstant::VT_INT;
				}
				else if (ConstantDesc.RegisterSet == D3DXRS_FLOAT4)
				{
					uiValueType = SKUserConstant::VT_FLOAT;
				}
				else
				{
					SKMAC_ASSERT(0);
				}
				SKUserConstant * pUserConstant =
					SK_NEW SKUserConstant(ConstantDesc.Name, NULL, ConstantDesc.Bytes, ConstantDesc.RegisterIndex, ConstantDesc.RegisterCount, uiValueType);
				pPShaderProgram->m_pUserConstant.AddElement(pUserConstant);

			}
			else
			{
				unsigned int uiType = 0;
				if (ConstantDesc.Type == D3DXPT_SAMPLER1D)
				{
					uiType = SKTexture::TT_1D;
				}
				else if (ConstantDesc.Type == D3DXPT_SAMPLER2D)
				{
					uiType = SKTexture::TT_2D;
				}
				else if (ConstantDesc.Type == D3DXPT_SAMPLER3D)
				{
					uiType = SKTexture::TT_3D;
				}
				else if (ConstantDesc.Type == D3DXPT_SAMPLERCUBE)
				{
					uiType = SKTexture::TT_CUBE;
				}
				else
				{
					SKMAC_ASSERT(0);
				}
				SKUserSampler * pUerSampler = SK_NEW SKUserSampler(ConstantDesc.Name, uiType, ConstantDesc.RegisterIndex, ConstantDesc.RegisterCount);
				pPShaderProgram->m_pUserSampler.AddElement(pUerSampler);
			}
		}
		SKMAC_RELEASE(pConstantTable)
			pPShaderProgram->m_bCreatePara = true;
	}

	hResult = m_pDevice->CreatePixelShader((DWORD*)pCode->GetBufferPointer(), &pID->m_pPixelShader);
	pPShaderProgram->SetCacheBuffer(pCode->GetBufferPointer(), pCode->GetBufferSize());
	SKMAC_ASSERT(!FAILED(hResult));
	GetShaderInstruction(pCode, pPShaderProgram->m_uiArithmeticInstructionSlots, pPShaderProgram->m_uiTextureInstructionSlots);
	SKMAC_RELEASE(pCode);
	SKMAC_RELEASE(pErrors);
}
bool SKDX9Renderer::OnLoadPShaderProgram(SKPShader* pPShaderProgram, SKResourceIdentifier *&pID)
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
		// 		HRESULT hResult = NULL;
		// 		hResult = m_pDevice->CreatePixelShader((DWORD*)pPShaderProgram->GetCacheBuffer(),&pPProgramID->m_pPixelShader);
		// 		SKMAC_ASSERT(!FAILED(hResult));
		CreatePShaderFromCache(pPShaderProgram, pPProgramID);
	}

	return 1;

}
bool SKDX9Renderer::OnLoadGShaderProgram(SKGShader * pGShaderProgram, SKResourceIdentifier *&pID)
{
	return 1;
}
bool SKDX9Renderer::OnReleaseGShaderProgram(SKResourceIdentifier * pGShaderProgramID)
{
	return 1;
}
bool SKDX9Renderer::OnLoadDShaderProgram(SKDShader * pDShaderProgram, SKResourceIdentifier *&pID)
{
	return 1;
}
bool SKDX9Renderer::OnReleaseDShaderProgram(SKResourceIdentifier * pDShaderProgramID)
{
	return 1;
}
bool SKDX9Renderer::OnLoadHShaderProgram(SKHShader * pHShaderProgram, SKResourceIdentifier *&pID)
{
	return 1;
}
bool SKDX9Renderer::OnReleaseHShaderProgram(SKResourceIdentifier * pHShaderProgramID)
{
	return 1;
}
bool SKDX9Renderer::OnReleasePShaderProgram(SKResourceIdentifier* pPShaderProgramID)
{
	SKPProgramID *pPProgramID = (SKPProgramID *)pPShaderProgramID;


	//SKMAC_RELEASE(pPProgramID->m_pConstantTable);


	//SKMAC_RELEASE(pPProgramID->m_pPixelShader);

	SKMAC_DELETE(pPProgramID);
	return 1;

}
bool SKDX9Renderer::OnLoadTexture(SKTexture * pTexture, SKResourceIdentifier *&pID)
{

	SKTextureID * pTextureID = NULL;
	pTextureID = SK_NEW SKTextureID;
	if (!pTextureID)
		return 0;
	pID = pTextureID;
	DWORD dwUsage = 0;
	D3DPOOL Pool;
	DWORD LockFlag;
	if (pTexture->GetTexType() == SKTexture::TT_2D && ((VS2DTexture *)pTexture)->IsRenderTarget())
	{

		Pool = D3DPOOL_DEFAULT;
		dwUsage |= D3DUSAGE_RENDERTARGET;
		LockFlag = D3DLOCK_DISCARD;
	}
	else if (pTexture->GetTexType() == SKTexture::TT_CUBE && ((SKCubeTexture *)pTexture)->IsRenderTarget())
	{
		Pool = D3DPOOL_DEFAULT;
		dwUsage |= D3DUSAGE_RENDERTARGET;
		LockFlag = D3DLOCK_DISCARD;
	}
	else
	{
		if (!pTexture->IsStatic())
		{
			dwUsage |= D3DUSAGE_DYNAMIC;
			Pool = D3DPOOL_DEFAULT;
			LockFlag = D3DLOCK_DISCARD;
			//dwUsage |= D3DUSAGE_WRITEONLY;
		}
		else
		{
			Pool = D3DPOOL_MANAGED;
			LockFlag = 0;

		}
	}



	if (pTexture->GetTexType() == SKTexture::TT_2D)
	{
		//		D3DLOCKED_RECT LockRect;
		// 		LPDIRECT3DTEXTURE9 pDXTexture;
		// 
		// 		
		// 		hResult = D3DXCreateTexture(m_pDevice,pTexture->GetWidth(0),
		// 			pTexture->GetHeight(0),pTexture->GetMipLevel(),dwUsage,(D3DFORMAT)ms_dwTextureFormatType[pTexture->GetFormatType()],Pool,&pDXTexture);
		// 		SKMAC_ASSERT(!FAILED(hResult));
		//		pTextureID->m_pTexture =  pDXTexture;


		// 		if(!((VS2DTexture *)pTexture)->IsRenderTarget())
		// 		{
		// 			for (unsigned int i = 0 ; i < pTexture->GetMipLevel() ; i++)
		// 			{
		// 				if (!pTexture->GetBuffer(i))
		// 				{
		// 					continue;
		// 				}
		// 				hResult = pDXTexture->LockRect(i,&LockRect,0,LockFlag);
		// 				SKMAC_ASSERT(!FAILED(hResult));
		// 				SKMemcpy(LockRect.pBits,pTexture->GetBuffer(i),pTexture->GetByteSize(i));
		// 				hResult = pDXTexture->UnlockRect(i);
		// 				SKMAC_ASSERT(!FAILED(hResult));
		// 			}		
		// 			
		// 
		// 		}
		Create2DTexture(pTexture, dwUsage, (D3DFORMAT)ms_dwTextureFormatType[pTexture->GetFormatType()], Pool, LockFlag, &pTextureID->m_pTexture);

	}
	else if (pTexture->GetTexType() == SKTexture::TT_3D)
	{
		// 		LPDIRECT3DVOLUMETEXTURE9 pDXTexture;
		// 		
		// 			
		// 		hResult = D3DXCreateVolumeTexture(m_pDevice,pTexture->GetWidth(0),
		// 						pTexture->GetHeight(0),pTexture->GetLength(0),pTexture->GetMipLevel(),dwUsage,
		// 						(D3DFORMAT)ms_dwTextureFormatType[pTexture->GetFormatType()],
		// 						Pool,&pDXTexture);
		// 		SKMAC_ASSERT(!FAILED(hResult));
		// 		pTextureID->m_pTexture =  pDXTexture;	


		// 		for (unsigned int i = 0 ; i < pTexture->GetMipLevel() ; i++)
		// 		{
		// 			if (!pTexture->GetBuffer(i))
		// 			{
		// 				continue;
		// 			}
		// 			D3DLOCKED_BOX kLockBox;
		// 			hResult = pDXTexture->LockBox(i,&kLockBox,0,LockFlag);
		// 			SKMAC_ASSERT(!FAILED(hResult));
		// 			SKMemcpy(kLockBox.pBits,pTexture->GetBuffer(i),pTexture->GetByteSize(i));
		// 			hResult = pDXTexture->UnlockBox(i);
		// 			SKMAC_ASSERT(!FAILED(hResult));
		// 		}



		CreateVolumeTexture(pTexture, dwUsage, (D3DFORMAT)ms_dwTextureFormatType[pTexture->GetFormatType()], Pool, LockFlag, &pTextureID->m_pTexture);

	}
	else if (pTexture->GetTexType() == SKTexture::TT_CUBE)
	{
		//		D3DLOCKED_RECT LockRect;
		// 		LPDIRECT3DCUBETEXTURE9 pDXTexture;
		// 
		// 			
		// 		hResult = D3DXCreateCubeTexture(m_pDevice,pTexture->GetWidth(0),
		// 			pTexture->GetMipLevel(),dwUsage,(D3DFORMAT)ms_dwTextureFormatType[pTexture->GetFormatType()],Pool,&pDXTexture);
		// 		SKMAC_ASSERT(!FAILED(hResult));
		// 		pTextureID->m_pTexture =  pDXTexture;


		// 		SKCubeTexture * pCubeTexture =DynamicCast<SKCubeTexture>(pTexture);
		// 		if(!pCubeTexture->IsRenderTarget())
		// 		{
		// 			for (unsigned int j = 0 ; j < pTexture->GetMipLevel() ; j++)
		// 			{
		// 				for(unsigned int i = 0 ; i < 6 ; i++)
		// 				{
		// 					if(!pCubeTexture->GetFaceBuffer(j,i))
		// 						continue;
		// 					hResult = pDXTexture->LockRect((D3DCUBEMAP_FACES)ms_dwCubeMapFace[i],j,&LockRect,NULL,LockFlag);
		// 					SKMAC_ASSERT(!FAILED(hResult));
		// 					SKMemcpy(LockRect.pBits,pCubeTexture->GetFaceBuffer(j,i),pCubeTexture->GetFaceByteSize(j));
		// 					hResult = pDXTexture->UnlockRect((D3DCUBEMAP_FACES)ms_dwCubeMapFace[i],j);
		// 					SKMAC_ASSERT(!FAILED(hResult));
		// 				}
		// 			}
		// 		}
		CreateCubeTexture(pTexture, dwUsage, (D3DFORMAT)ms_dwTextureFormatType[pTexture->GetFormatType()], Pool, LockFlag, &pTextureID->m_pTexture);


	}
	else if (pTexture->GetTexType() == SKTexture::TT_1D)
	{
		Create1DTexture(pTexture, dwUsage, (D3DFORMAT)ms_dwTextureFormatType[pTexture->GetFormatType()], Pool, LockFlag, &pTextureID->m_pTexture);
	}
	else
	{
		SKMAC_ASSERT(0);
	}

	return 1;
}
bool SKDX9Renderer::OnReleaseTexture(SKResourceIdentifier * pTextureID)
{
	//	SKTextureID *pTextureID = (SKTextureID *)pTextureID;


	SKMAC_DELETE(pTextureID);
	return 1;
}
bool SKDX9Renderer::OnLoadRenderTarget(SKRenderTarget * pRenderTarget, SKResourceIdentifier *&pID)
{
	if (!pRenderTarget)
	{
		return false;
	}

	SKRenderTargetID *pRenderTargetID = SK_NEW SKRenderTargetID;
	pID = pRenderTargetID;
	SKTexture * pTexture = pRenderTarget->GetCreateBy();
	bool b1 = false;
	bool b2 = false;
	if (pTexture && pRenderTarget->GetMulSample() == SKRenderer::MS_NONE)
	{
		pTexture->LoadResource(this);
		b1 = true;

	}
	else if (pTexture)
	{
		pTexture->LoadResource(this);
		b1 = true;
		b2 = true;

	}
	else
	{
		b2 = true;
	}
	if (b1)
	{
		SKTextureID *pTextureID = (SKTextureID *)pTexture->GetIdentifier();
		if (!pTextureID)
		{
			return 0;
		}
		if (pTexture->GetTexType() == SKTexture::TT_2D)
		{
			GetSurfaceLevel((LPDIRECT3DTEXTURE9 *)&pTextureID->m_pTexture, pRenderTarget->GetLevel(), &pRenderTargetID->m_pTextureSurface);
			// 			HRESULT hResult = NULL;
			// 			hResult = ((LPDIRECT3DTEXTURE9)pTextureID->m_pTexture)->GetSurfaceLevel(pRenderTarget->GetLevel(),&pRenderTargetID->m_pTextureSurface);
			// 			SKMAC_ASSERT(!FAILED(hResult));
		}
		else if (pTexture->GetTexType() == SKTexture::TT_CUBE)
		{
			GetCubeMapSurface((LPDIRECT3DCUBETEXTURE9*)&pTextureID->m_pTexture, (D3DCUBEMAP_FACES)ms_dwCubeMapFace[pRenderTarget->GetParam()], pRenderTarget->GetLevel(), &pRenderTargetID->m_pTextureSurface);
			// 			HRESULT hResult = NULL;
			// 			hResult = ((LPDIRECT3DCUBETEXTURE9)pTextureID->m_pTexture)->GetCubeMapSurface((D3DCUBEMAP_FACES)ms_dwCubeMapFace[pRenderTarget->GetParam()],pRenderTarget->GetLevel(),&pRenderTargetID->m_pTextureSurface);
			// 			SKMAC_ASSERT(!FAILED(hResult));
		}
		else
		{
			return false;
		}
	}
	if (b2)
	{
		CreateRenderTarget((D3DFORMAT)ms_dwTextureFormatType[pRenderTarget->GetFormatType()], (D3DMULTISAMPLE_TYPE)ms_dwMultiSampleTypes[GetSuitableMultisample(pRenderTarget->GetMulSample())],
			pRenderTarget->GetWidth(), pRenderTarget->GetHeight(), &pRenderTargetID->m_pRenderTarget);
		// 		HRESULT hResult = NULL;
		// 		DWORD uiMulSample;
		// 		hResult = ms_pMain->CheckDeviceMultiSampleType(m_uinAdapter,(D3DDEVTYPE)ms_dwDevTypes[m_uiDevType], 
		// 			(D3DFORMAT)ms_dwTextureFormatType[pRenderTarget->GetFormatType()],m_bWindowed,(D3DMULTISAMPLE_TYPE)ms_dwMultiSampleTypes[pRenderTarget->GetMulSample()], &uiMulSample);
		// 		if(FAILED(hResult))			
		// 		{
		// 			SKMAC_ASSERT(0);
		// 		}
		// 		hResult = m_pDevice->CreateRenderTarget(pRenderTarget->GetWidth(),pRenderTarget->GetHeight(),
		// 			(D3DFORMAT)ms_dwTextureFormatType[pRenderTarget->GetFormatType()],(D3DMULTISAMPLE_TYPE)ms_dwMultiSampleTypes[pRenderTarget->GetMulSample()],
		// 			uiMulSample - 1,false,&pRenderTargetID->m_pRenderTarget,NULL);
		// 		SKMAC_ASSERT(!FAILED(hResult));

	}


	return true;
}
bool SKDX9Renderer::OnReleaseRenderTarget(SKResourceIdentifier * pRenderTarget)
{
	SKMAC_DELETE(pRenderTarget);
	return true;
}
bool SKDX9Renderer::OnLoadDepthStencil(SKDepthStencil * pDepthStencil, SKResourceIdentifier *&pID)
{
	SKDepthStencilID  * pDepthStencilID = NULL;
	pDepthStencilID = SK_NEW SKDepthStencilID;
	if (!pDepthStencilID)
		return 0;
	pID = pDepthStencilID;
	CreateDepthStencilSurface((D3DFORMAT)ms_dwTextureFormatType[pDepthStencil->GetFormatType()], (D3DMULTISAMPLE_TYPE)ms_dwMultiSampleTypes[GetSuitableMultisample(pDepthStencil->GetMulSample())],
		pDepthStencil->GetWidth(), pDepthStencil->GetHeight(), &pDepthStencilID->m_pDepthStencilBuffer);

	// 	HRESULT hResult = NULL;
	// 	DWORD uiMulSample;
	// 	hResult = ms_pMain->CheckDeviceMultiSampleType(m_uinAdapter,(D3DDEVTYPE)ms_dwDevTypes[m_uiDevType], 
	// 		(D3DFORMAT)ms_dwTextureFormatType[pDepthStencil->GetFormatType()],m_bWindowed,(D3DMULTISAMPLE_TYPE)ms_dwMultiSampleTypes[pDepthStencil->GetMulSample()], &uiMulSample);
	// 	if(FAILED(hResult))			
	// 	{
	// 		SKMAC_ASSERT(0);
	// 	}
	// 
	// 	hResult =m_pDevice->CreateDepthStencilSurface(pDepthStencil->GetWidth(),pDepthStencil->GetHeight(),
	// 		(D3DFORMAT)ms_dwTextureFormatType[pDepthStencil->GetFormatType()],(D3DMULTISAMPLE_TYPE)ms_dwMultiSampleTypes[pDepthStencil->GetMulSample()],uiMulSample - 1,FALSE,
	// 		&pDepthStencilID->m_pDepthStencilBuffer,NULL);
	// 	SKMAC_ASSERT(!FAILED(hResult));
	return 1;

}
bool SKDX9Renderer::OnReleaseDepthStencil(SKResourceIdentifier * pDepthStencilID)
{
	SKMAC_DELETE(pDepthStencilID);
	return 1;
}
bool SKDX9Renderer::OnLoadVBufferDate(SKVertexBuffer * pVBuffer, SKResourceIdentifier *&pID)
{

	SKVBufferID * pVBufferID = NULL;
	pVBufferID = SK_NEW SKVBufferID;
	if (!pVBufferID)
		return 0;
	pID = pVBufferID;


	unsigned int uiOneVextexSize = pVBuffer->GetOneVertexSize();

	unsigned int uiTotalSize = uiOneVextexSize * pVBuffer->GetVertexNum();

	DWORD dwUsage = 0;
	D3DPOOL Pool = D3DPOOL_DEFAULT;
	DWORD LockFlag = 0;
	if (!pVBuffer->IsStatic())
	{
		dwUsage |= D3DUSAGE_DYNAMIC;
		dwUsage |= D3DUSAGE_WRITEONLY;
		LockFlag = D3DLOCK_DISCARD;
	}
	else
	{
		dwUsage |= D3DUSAGE_WRITEONLY;
	}

	CreateVertexBuffer(uiTotalSize, dwUsage, Pool, &pVBufferID->m_pVertexBuffer, LockFlag, pVBuffer, uiOneVextexSize);

	// 	HRESULT hResult = NULL;
	// 	unsigned char *pResource;
	// 	hResult = m_pDevice->CreateVertexBuffer(uiTotalSize,dwUsage,0,Pool,
	// 		&pVBufferID->m_pVertexBuffer,0);
	// 	SKMAC_ASSERT(!FAILED(hResult));
	// 
	// 	hResult = pVBufferID->m_pVertexBuffer->Lock(0,uiTotalSize,(void**)(&pResource),LockFlag);
	// 	SKMAC_ASSERT(!FAILED(hResult));
	// 
	// 	SKDataBuffer * pDate;
	// 
	// 
	// 	unsigned int iVertexSize = 0;
	// 	for(unsigned int i = 0 ; i < pVBuffer->GetPositionLevel(); i++)
	// 	{
	// 		pDate = pVBuffer->GetPositionDate(i);
	// 		if(pDate)
	// 		{
	// 			for(unsigned int j = 0 ; j < pDate->GetNum() ; j++)
	// 			{
	// 				SKMemcpy(pResource + uiOneVextexSize * j + iVertexSize,
	// 					(unsigned char *)pDate->GetDate() + j * pDate->GetStride(),pDate->GetStride());
	// 
	// 			}
	// 			iVertexSize += pDate->GetStride();
	// 		}
	// 
	// 	}
	// 
	// 	for(unsigned int i = 0 ; i < pVBuffer->GetTexCoordLevel(); i++)
	// 	{
	// 		pDate = pVBuffer->GetTexCoordDate(i);
	// 		if(pDate)
	// 		{
	// 			for(unsigned int j = 0 ; j < pDate->GetNum() ; j++)
	// 			{
	// 				SKMemcpy(pResource + uiOneVextexSize * j + iVertexSize,
	// 					(unsigned char *)pDate->GetDate() + j * pDate->GetStride(),pDate->GetStride());
	// 
	// 			}
	// 			iVertexSize += pDate->GetStride();
	// 		}
	// 	}
	// 
	// 	for(unsigned int i = 0 ; i < pVBuffer->GetNormalLevel(); i++)
	// 	{
	// 		pDate = pVBuffer->GetNormalDate(i);
	// 		if(pDate)
	// 		{
	// 			for(unsigned int j = 0 ; j < pDate->GetNum() ; j++)
	// 			{
	// 				SKMemcpy(pResource + uiOneVextexSize * j + iVertexSize,
	// 					(unsigned char *)pDate->GetDate() + j * pDate->GetStride(),pDate->GetStride());
	// 
	// 			}
	// 			iVertexSize += pDate->GetStride();
	// 		}
	// 	}
	// 
	// 	pDate = pVBuffer->GetTangentDate();
	// 	if(pDate)
	// 	{
	// 		for(unsigned int j = 0 ; j < pDate->GetNum() ; j++)
	// 		{
	// 			SKMemcpy(pResource + uiOneVextexSize * j + iVertexSize,
	// 				(unsigned char *)pDate->GetDate() + j * pDate->GetStride(),pDate->GetStride());
	// 
	// 		}
	// 		iVertexSize += pDate->GetStride();
	// 	}
	// 
	// 
	// 	pDate = pVBuffer->GetBinormalDate();
	// 	if(pDate)
	// 	{
	// 		for(unsigned int j = 0 ; j < pDate->GetNum() ; j++)
	// 		{
	// 			SKMemcpy(pResource + uiOneVextexSize * j + iVertexSize,
	// 				(unsigned char *)pDate->GetDate() + j * pDate->GetStride(),pDate->GetStride());
	// 
	// 		}
	// 		iVertexSize += pDate->GetStride();
	// 	}
	// 
	// 	pDate = pVBuffer->GetPSizeDate();
	// 	if(pDate)
	// 	{
	// 		for(unsigned int j = 0 ; j < pDate->GetNum() ; j++)
	// 		{
	// 			SKMemcpy(pResource + uiOneVextexSize * j + iVertexSize,
	// 				(unsigned char *)pDate->GetDate() + j * pDate->GetStride(),pDate->GetStride());
	// 
	// 		}
	// 		iVertexSize += pDate->GetStride();
	// 	}
	// 
	// 	for(unsigned int i = 0 ; i < pVBuffer->GetColorLevel(); i++)
	// 	{
	// 
	// 		pDate = pVBuffer->GetColorDate(i);
	// 		if(pDate)
	// 		{
	// 			for(unsigned int j = 0 ; j < pDate->GetNum() ; j++)
	// 			{
	// 				SKMemcpy(pResource + uiOneVextexSize * j + iVertexSize,
	// 					(unsigned char *)pDate->GetDate() + j * pDate->GetStride(),pDate->GetStride());
	// 
	// 			}
	// 			iVertexSize += pDate->GetStride();
	// 		}
	// 	}
	// 
	// 	pDate = pVBuffer->GetFogDate();
	// 	if(pDate)
	// 	{
	// 		for(unsigned int j = 0 ; j < pDate->GetNum() ; j++)
	// 		{
	// 			SKMemcpy(pResource + uiOneVextexSize * j + iVertexSize,
	// 				(unsigned char *)pDate->GetDate() + j * pDate->GetStride(),pDate->GetStride());
	// 
	// 		}
	// 		iVertexSize += pDate->GetStride();
	// 	}
	// 
	// 	pDate = pVBuffer->GetDepthDate();
	// 	if(pDate)
	// 	{
	// 		for(unsigned int j = 0 ; j < pDate->GetNum() ; j++)
	// 		{
	// 			SKMemcpy(pResource + uiOneVextexSize * j + iVertexSize,
	// 				(unsigned char *)pDate->GetDate() + j * pDate->GetStride(),pDate->GetStride());
	// 
	// 		}
	// 		iVertexSize += pDate->GetStride();
	// 	}
	// 
	// 	pDate = pVBuffer->GetBlendWeightDate();
	// 	if(pDate)
	// 	{
	// 		for(unsigned int j = 0 ; j < pDate->GetNum() ; j++)
	// 		{
	// 			SKMemcpy(pResource + uiOneVextexSize * j + iVertexSize,
	// 				(unsigned char *)pDate->GetDate() + j * pDate->GetStride(),pDate->GetStride());
	// 
	// 		}
	// 		iVertexSize += pDate->GetStride();
	// 	}
	// 
	// 	pDate = pVBuffer->GetBlendIndicesDate();
	// 	if(pDate)
	// 	{
	// 		for(unsigned int j = 0 ; j < pDate->GetNum() ; j++)
	// 		{
	// 			SKMemcpy(pResource + uiOneVextexSize * j + iVertexSize,
	// 				(unsigned char *)pDate->GetDate() + j * pDate->GetStride(),pDate->GetStride());
	// 
	// 		}
	// 		iVertexSize += pDate->GetStride();
	// 	}
	// 
	// 
	// 	hResult = pVBufferID->m_pVertexBuffer->Unlock();
	// 	SKMAC_ASSERT(!FAILED(hResult));

	return 1;

}
bool SKDX9Renderer::OnReleaseVBufferDate(SKResourceIdentifier* pVBufferID)
{
	//	SKVBufferID * pVBufferID = (SKVBufferID *)pVBufferID;

	SKMAC_DELETE(pVBufferID);
	return 1;
}
bool SKDX9Renderer::OnLoadVBufferFormat(SKVertexFormat * pVertexFormat, SKResourceIdentifier *&pID)
{
	if (!pVertexFormat->m_FormatArray.GetNum())
		return 0;
	SKVBufferFormatID * pVBufferFormatID = NULL;
	pVBufferFormatID = SK_NEW SKVBufferFormatID;
	if (!pVBufferFormatID)
		return 0;
	pID = pVBufferFormatID;

	SKArray<D3DVERTEXELEMENT9> Elements;
	D3DVERTEXELEMENT9 Element;
	Element.Stream = 0;
	Element.Method = D3DDECLMETHOD_DEFAULT;

	for (unsigned int i = 0; i < pVertexFormat->m_FormatArray.GetNum(); i++)
	{
		SKVertexFormat::VERTEXFORMAT_TYPE &Format = pVertexFormat->m_FormatArray[i];
		Element.Offset = Format.OffSet;
		Element.Type = (BYTE)ms_dwVertexFormatDateType[Format.DateType];
		Element.Usage = (BYTE)ms_dwVertexFormatSemantics[Format.Semantics];
		Element.UsageIndex = Format.SemanticsIndex;
		Elements.AddElement(Element);
	}

	D3DVERTEXELEMENT9 kSentinel = D3DDECL_END();
	Elements.AddElement(kSentinel);
	CreateVertexDeclaration(Elements, &pVBufferFormatID->m_pDeclaration);
	// 	HRESULT hResult = NULL;
	// 	hResult = m_pDevice->CreateVertexDeclaration(&Elements[0],
	// 		&pVBufferFormatID->m_pDeclaration);
	// 	SKMAC_ASSERT(!FAILED(hResult));
	return 1;
}
bool SKDX9Renderer::OnReleaseVBufferFormat(SKResourceIdentifier* pVBufferID)
{
	SKMAC_DELETE(pVBufferID);
	return 1;
}
bool SKDX9Renderer::OnLoadIBuffer(SKIndexBuffer* pIBuffer, SKResourceIdentifier *&pID)
{
	SKIBufferID * pIBufferID = NULL;
	pIBufferID = SK_NEW SKIBufferID;
	if (!pIBufferID)
		return 0;
	pID = pIBufferID;
	HRESULT hResult = NULL;
	D3DFORMAT DateFormat;
	if (pIBuffer->GetDateType() == SKDataBuffer::DT_USHORT)
		DateFormat = D3DFMT_INDEX16;
	else if (pIBuffer->GetDateType() == SKDataBuffer::DT_UINT)
		DateFormat = D3DFMT_INDEX32;
	else
		return 0;
	DWORD dwUsage = 0;
	D3DPOOL Pool = D3DPOOL_DEFAULT;
	DWORD LockFlag = 0;

	if (!pIBuffer->IsStatic())
	{
		dwUsage |= D3DUSAGE_DYNAMIC;
		dwUsage |= D3DUSAGE_WRITEONLY;
		LockFlag = D3DLOCK_DISCARD;
	}
	else
	{
		dwUsage |= D3DUSAGE_WRITEONLY;
	}
	void* pBuffer = NULL;
	if (pIBuffer->GetIndexDate())
	{
		pBuffer = pIBuffer->GetIndexDate()->GetDate();
	}
	CreateIndexBuffer(pIBuffer->GetByteSize(), dwUsage, DateFormat, Pool, &pIBufferID->m_IndexBuffer, pBuffer, LockFlag);
	// 	hResult = m_pDevice->CreateIndexBuffer(pIBuffer->GetByteSize(),dwUsage,
	// 		DateFormat,Pool,&pIBufferID->m_IndexBuffer,0);
	// 	SKMAC_ASSERT(!FAILED(hResult));
	// 
	// 	unsigned char* pcIndices;
	// 	hResult = pIBufferID->m_IndexBuffer->Lock(0,pIBuffer->GetByteSize(),(void**)(&pcIndices),LockFlag);
	// 	SKMAC_ASSERT(!FAILED(hResult));
	// 
	// 	if(pIBuffer->GetIndexDate())
	// 	{
	// 		void* pBuffer = pIBuffer->GetIndexDate()->GetDate();
	// 		if (pBuffer)
	// 		{
	// 			SKMemcpy(pcIndices,pBuffer,pIBuffer->GetByteSize());
	// 		}
	// 		
	// 	}
	// 	hResult = pIBufferID->m_IndexBuffer->Unlock();
	// 	SKMAC_ASSERT(!FAILED(hResult));
	return 1;
}
bool SKDX9Renderer::OnReleaseIBuffer(SKResourceIdentifier* pIBufferID)
{
	//	SKIBufferID * pIBufferID = (SKIBufferID *) pIBufferID;

	SKMAC_DELETE(pIBufferID);
	return 1;
}
bool SKDX9Renderer::OnLoadBlendState(SKBlendState * pBlendState, SKResourceIdentifier *&pID)
{
	return 1;
}
bool SKDX9Renderer::OnReleaseBlendState(SKResourceIdentifier * pBlendStateID)
{
	SKMAC_DELETE(pBlendStateID);
	return 1;
}

bool SKDX9Renderer::OnLoadDepthStencilState(SKDepthStencilState * pDepthStencilState, SKResourceIdentifier *&pID)
{
	return 1;
}
bool SKDX9Renderer::OnReleaseDepthStencilState(SKResourceIdentifier * pDepthStencilStateID)
{
	SKMAC_DELETE(pDepthStencilStateID);
	return 1;
}

bool SKDX9Renderer::OnLoadRasterizerState(SKRasterizerState * pRasterizerState, SKResourceIdentifier *&pID)
{

	return 1;
}
bool SKDX9Renderer::OnReleaseRasterizerState(SKResourceIdentifier * pRasterizerStateID)
{
	SKMAC_DELETE(pRasterizerStateID);
	return 1;
}
bool SKDX9Renderer::OnLoadSamplerState(SKSamplerState * pSamplerState, SKResourceIdentifier *&pID)
{
	return 1;
}
bool SKDX9Renderer::OnReleaseSamplerState(SKResourceIdentifier * pSamplerStateID)
{
	SKMAC_DELETE(pSamplerStateID);
	return 1;
}
bool SKDX9Renderer::SetVShaderProgram(SKVShader*	pVShaderProgram)
{
	if (!SKRenderer::SetVShaderProgram(pVShaderProgram))
	{
		SetVertexShader(NULL);
		return 0;
	}

	SKVProgramID *pVProgramID = (SKVProgramID *)pVShaderProgram->GetIdentifier();
	if (!pVProgramID)
		return FRI_FAIL;
	SetVertexShader(&pVProgramID->m_pVertexShader);
	return 1;
}
bool SKDX9Renderer::SetGShaderProgram(SKGShader*pGShaderProgram)
{
	return 1;
}
bool SKDX9Renderer::SetDShaderProgram(SKDShader*	pDShaderProgram)
{
	return 1;
}
bool SKDX9Renderer::SetHShaderProgram(SKHShader*	pHShaderProgram)
{
	return 1;
}
bool SKDX9Renderer::SetPShaderProgram(SKPShader*pPShaderProgram)
{
	if (!SKRenderer::SetPShaderProgram(pPShaderProgram))
	{
		SetPixelShader(NULL);
		return 0;
	}
	SKPProgramID *pPProgramID = (SKPProgramID *)pPShaderProgram->GetIdentifier();
	if (!pPProgramID)
		return FRI_FAIL;
	SetPixelShader(&pPProgramID->m_pPixelShader);
	return 1;
}
unsigned int SKDX9Renderer::SetGTexture(SKTexture* pTexture, unsigned int i)
{
	SKRenderer::SetGTexture(pTexture, i);
	return 1;
}
unsigned int SKDX9Renderer::SetDTexture(SKTexture* pTexture, unsigned int i)
{
	SKRenderer::SetDTexture(pTexture, i);
	return 1;
}
unsigned int SKDX9Renderer::SetHTexture(SKTexture* pTexture, unsigned int i)
{
	SKRenderer::SetHTexture(pTexture, i);
	return 1;
}
unsigned int SKDX9Renderer::SetVTexture(SKTexture* pTexture, unsigned int i)
{
	unsigned int FRI = SKRenderer::SetVTexture(pTexture, i);
	if (FRI == FRI_SAMERESOURCE)
		return 1;
	else if (FRI == FRI_FAIL)
	{
		SetTexture(D3DDMAPSAMPLER + 1 + i, NULL);
		return 0;
	}
	SKTextureID *pTextureID = (SKTextureID *)pTexture->GetIdentifier();
	if (!pTextureID)
		return 0;


	SetTexture(D3DDMAPSAMPLER + 1 + i, &pTextureID->m_pTexture);
	return 1;

}
unsigned int SKDX9Renderer::SetTexture(SKTexture* pTexture, unsigned int i)
{
	unsigned int FRI = SKRenderer::SetTexture(pTexture, i);
	if (FRI == FRI_SAMERESOURCE)
		return 1;
	else if (FRI == FRI_FAIL)
	{
		SetTexture(i, NULL);
		return 0;
	}
	SKTextureID *pTextureID = (SKTextureID *)pTexture->GetIdentifier();
	if (!pTextureID)
		return 0;


	SetTexture(i, &pTextureID->m_pTexture);

	return 1;

}
void SKDX9Renderer::SetGShaderConstant(SKGShader * pShader)
{

}
void SKDX9Renderer::SetGShaderSampler(SKGShader * pShader)
{

}
void SKDX9Renderer::SetDShaderConstant(SKDShader * pShader)
{

}
void SKDX9Renderer::SetDShaderSampler(SKDShader * pShader)
{

}
void SKDX9Renderer::SetHShaderConstant(SKHShader * pShader)
{

}
void SKDX9Renderer::SetHShaderSampler(SKHShader * pShader)
{

}
void SKDX9Renderer::SetVShaderConstant(SKVShader * pShader)
{
	unsigned int uiFloatRegisterID = 0;
	unsigned int uiBoolRegisterID = 0;
	unsigned int uiIntRegisterID = 0;
	// 	SKMemset(m_fFloatShaderBuffer,0,sizeof(SKREAL) * MAX_SHADER_BUFFER);
	// 	SKMemset(m_iIntShaderBuffer,0,sizeof(int) * MAX_SHADER_BUFFER);
	// 	SKMemset(m_bBoolShaderBuffer,0,sizeof(bool)* MAX_SHADER_BUFFER);
	if (!pShader->m_bCreatePara)
	{
		return;
	}
	for (unsigned int i = 0; i < pShader->m_pUserConstant.GetNum(); i++)
	{
		SKUserConstant * pUserConstant = pShader->m_pUserConstant[i];

		if (pUserConstant->GetValueType() == SKUserConstant::VT_FLOAT)
		{
			SKMemcpy(&m_fFloatShaderBuffer[pUserConstant->GetRegisterIndex() << 2],
				pUserConstant->GetDate(), sizeof(SKREAL)* (pUserConstant->GetRegisterNum() << 2));
			uiFloatRegisterID = uiFloatRegisterID + pUserConstant->GetRegisterNum();
		}
		if (pUserConstant->GetValueType() == SKUserConstant::VT_INT)
		{
			SKMemcpy(&m_iIntShaderBuffer[pUserConstant->GetRegisterIndex() << 2],
				pUserConstant->GetDate(), sizeof(int)* (pUserConstant->GetRegisterNum() << 2));
			uiIntRegisterID = uiIntRegisterID + pUserConstant->GetRegisterNum();
		}
		if (pUserConstant->GetValueType() == SKUserConstant::VT_BOOL)
		{
			SKMemcpy(&m_bBoolShaderBuffer[pUserConstant->GetRegisterIndex() << 2],
				pUserConstant->GetDate(), sizeof(bool)* (pUserConstant->GetRegisterNum() << 2));
			uiBoolRegisterID = uiBoolRegisterID + pUserConstant->GetRegisterNum();
		}

	}
	if (uiFloatRegisterID)
	{
		SetVProgramConstant(0, m_fFloatShaderBuffer,
			uiFloatRegisterID, SKUserConstant::VT_FLOAT);
	}
	if (uiIntRegisterID)
	{
		SetVProgramConstant(0, m_iIntShaderBuffer,
			uiIntRegisterID, SKUserConstant::VT_INT);
	}
	if (uiBoolRegisterID)
	{
		SetVProgramConstant(0, m_bBoolShaderBuffer,
			uiBoolRegisterID, SKUserConstant::VT_BOOL);
	}
}
void SKDX9Renderer::SetVShaderSampler(SKVShader * pShader)
{

	SKMemset(m_pTexAllStateBuffer, 0, sizeof(SKTexAllState *)* MAX_TEXTURE_BUFFER);
	if (pShader->m_bCreatePara)
	{
		for (unsigned int uiTexid = 0; uiTexid < pShader->m_pUserSampler.GetNum(); uiTexid++)
		{
			if (pShader->m_pUserSampler[uiTexid]->GetRegisterIndex() + pShader->m_pUserSampler[uiTexid]->GetRegisterNum() <= m_uiMaxVTexture)
			{
				for (unsigned int i = 0; i < pShader->m_pUserSampler[uiTexid]->GetRegisterNum(); i++)
				{
					m_pTexAllStateBuffer[pShader->m_pUserSampler[uiTexid]->GetRegisterIndex() + i] = pShader->m_pUserSampler[uiTexid]->GetTex(i);
				}

			}

		}
	}

	for (unsigned int k = 0; k < m_uiMaxVTexture; k++)
	{
		SetVTexAllState(m_pTexAllStateBuffer[k], k);
	}
}
void SKDX9Renderer::SetPShaderConstant(SKPShader * pShader)
{
	unsigned int uiFloatRegisterID = 0;
	unsigned int uiBoolRegisterID = 0;
	unsigned int uiIntRegisterID = 0;
	// 	SKMemset(m_fFloatShaderBuffer,0,sizeof(SKREAL) * MAX_SHADER_BUFFER);
	// 	SKMemset(m_iIntShaderBuffer,0,sizeof(int) * MAX_SHADER_BUFFER);
	// 	SKMemset(m_bBoolShaderBuffer,0,sizeof(bool)* MAX_SHADER_BUFFER);
	if (!pShader->m_bCreatePara)
	{
		return;
	}
	for (unsigned int i = 0; i < pShader->m_pUserConstant.GetNum(); i++)
	{
		SKUserConstant * pUserConstant = pShader->m_pUserConstant[i];

		if (pUserConstant->GetValueType() == SKUserConstant::VT_FLOAT)
		{
			SKMemcpy(&m_fFloatShaderBuffer[pUserConstant->GetRegisterIndex() << 2],
				pUserConstant->GetDate(), sizeof(SKREAL)* (pUserConstant->GetRegisterNum() << 2));
			uiFloatRegisterID = uiFloatRegisterID + pUserConstant->GetRegisterNum();
		}
		if (pUserConstant->GetValueType() == SKUserConstant::VT_INT)
		{
			SKMemcpy(&m_iIntShaderBuffer[pUserConstant->GetRegisterIndex() << 2],
				pUserConstant->GetDate(), sizeof(int)* (pUserConstant->GetRegisterNum() << 2));
			uiIntRegisterID = uiIntRegisterID + pUserConstant->GetRegisterNum();
		}
		if (pUserConstant->GetValueType() == SKUserConstant::VT_BOOL)
		{
			SKMemcpy(&m_bBoolShaderBuffer[pUserConstant->GetRegisterIndex() << 2],
				pUserConstant->GetDate(), sizeof(bool)* (pUserConstant->GetRegisterNum() << 2));
			uiBoolRegisterID = uiBoolRegisterID + pUserConstant->GetRegisterNum();
		}

	}
	if (uiFloatRegisterID)
	{
		SetPProgramConstant(0, m_fFloatShaderBuffer,
			uiFloatRegisterID, SKUserConstant::VT_FLOAT);
	}
	if (uiIntRegisterID)
	{
		SetPProgramConstant(0, m_iIntShaderBuffer,
			uiIntRegisterID, SKUserConstant::VT_INT);
	}
	if (uiBoolRegisterID)
	{
		SetPProgramConstant(0, m_bBoolShaderBuffer,
			uiBoolRegisterID, SKUserConstant::VT_BOOL);
	}
}
void SKDX9Renderer::SetPShaderSampler(SKPShader * pShader)
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
void SKDX9Renderer::SetTexAllState(SKTexAllState * pTexAllState, unsigned int i)
{
	SKRenderer::SetTexAllState(pTexAllState, i);
	SKMAC_ASSERT(i < TEXLEVEL);
	if (i >= m_uiMaxTexture)
		return;
	if (pTexAllState)
	{
		if (m_bSRGB[i] != pTexAllState->GetSRGB())
		{
			SetSamplerState(i, D3DSAMP_SRGBTEXTURE, pTexAllState->GetSRGB());
			m_bSRGB[i] = pTexAllState->GetSRGB();
		}

	}
	else
	{
		if (m_bSRGB[i])
		{
			m_bSRGB[i] = false;
			SetSamplerState(i, D3DSAMP_SRGBTEXTURE, 0);
		}

	}
}
D3DXMACRO * SKDX9Renderer::GetDefine(SKMap<SKString, SKString> & Define)
{
	if (Define.GetNum())
	{
		D3DXMACRO * pMacro = SK_NEW D3DXMACRO[Define.GetNum() + 1];
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
unsigned int SKDX9Renderer::SetVShader(SKVShader * pVShader)
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
unsigned int SKDX9Renderer::SetGShader(SKGShader * pGShader)
{
	return FRI_SUCCESS;
}
unsigned int SKDX9Renderer::SetDShader(SKDShader * pDShader)
{
	return FRI_SUCCESS;
}
unsigned int SKDX9Renderer::SetHShader(SKHShader * pHShader)
{
	return FRI_SUCCESS;
}
unsigned int SKDX9Renderer::SetPShader(SKPShader * pPShader)
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
unsigned int SKDX9Renderer::SetVBuffer(SKVertexBuffer * pVBuffer)
{
	unsigned int FRI = SKRenderer::SetVBuffer(pVBuffer);
	if (FRI == FRI_SAMERESOURCE)
		return 1;
	else if (FRI == FRI_FAIL)
	{
		SetStreamSource(0, NULL, 0, 0);
		return 0;
	}
	SKVBufferID* pVBufferID = (SKVBufferID *)pVBuffer->GetIdentifier();
	if (!pVBufferID)
		return 0;

	SetStreamSource(0, &pVBufferID->m_pVertexBuffer, 0,
		pVBuffer->GetOneVertexSize());
	return 1;
}
unsigned int SKDX9Renderer::SetVertexFormat(SKVertexFormat * pVertexFormat)
{
	unsigned int FRI = SKRenderer::SetVertexFormat(pVertexFormat);
	if (FRI == FRI_SAMERESOURCE)
		return 1;
	else if (FRI == FRI_FAIL)
	{
		SetVertexDeclaration(NULL);
		return 0;
	}

	SKVBufferFormatID * pVBufferFormatID = (SKVBufferFormatID *)pVertexFormat->GetIdentifier();
	if (!pVBufferFormatID)
		return 0;

	SetVertexDeclaration(&pVBufferFormatID->m_pDeclaration);

	return 1;
}
unsigned int SKDX9Renderer::SetIBuffer(SKIndexBuffer* pIBuffer)
{
	unsigned int FRI = SKRenderer::SetIBuffer(pIBuffer);
	if (FRI == FRI_SAMERESOURCE)
		return 1;
	else if (FRI == FRI_FAIL)
	{
		SetIndices(NULL);	//Ϊ modify 2010.10.16
		return 1;
	}
	SKIBufferID* pIBufferID = (SKIBufferID *)pIBuffer->GetIdentifier();
	if (!pIBufferID)
		return 0;
	SetIndices(&pIBufferID->m_IndexBuffer);
	return 1;
}
bool SKDX9Renderer::CopyResourceBuffer(VS2DTexture * pSource, SKCubeTexture * pDest, unsigned int uiFace)
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
		RECT Rect = { 0, 0, pSource->GetWidth(i) - 1, pSource->GetHeight(i) - 1 };

		CopyResourceBuffer(i, ((LPDIRECT3DTEXTURE9 *)&pTextureID->m_pTexture), ((LPDIRECT3DCUBETEXTURE9 *)&pCubTextureID->m_pTexture), &Rect, (D3DTEXTUREFILTERTYPE)ms_dwTextureFilterType[SKSamplerDesc::FM_LINE], (D3DCUBEMAP_FACES)ms_dwCubeMapFace[uiFace]);

	}
	return true;
}
bool SKDX9Renderer::SetRenderTarget(SKRenderTarget *pRenderTarget, unsigned int i)
{

	if (!SKRenderer::SetRenderTarget(pRenderTarget, i))
	{
		if (!pRenderTarget && i > 0 && i < m_uiMaxRTNum)
		{
			SetRenderTarget(i, NULL);
		}
		return 0;
	}
	SKRenderTargetID *pRenderTargetID = (SKRenderTargetID *)pRenderTarget->GetIdentifier();
	if (!pRenderTargetID)
	{
		return 0;
	}

	if (pRenderTarget->GetMulSample() == MS_NONE && pRenderTarget->GetCreateBy())
	{
		SetRenderTarget(i, &pRenderTargetID->m_pTextureSurface, &pRenderTargetID->m_pSaveRenderTarget);
	}
	else
	{
		SetRenderTarget(i, &pRenderTargetID->m_pRenderTarget, &pRenderTargetID->m_pSaveRenderTarget);
	}

	// 	SKMAC_RELEASE(pRenderTargetID->m_pSaveRenderTarget);
	// 	hResult = m_pDevice->GetRenderTarget(i,&pRenderTargetID->m_pSaveRenderTarget);
	// 	//i != 0  buffer⣬ûм
	// 	SKMAC_ASSERT(!FAILED(hResult));
	// 	hResult = m_pDevice->SetRenderTarget(i,pSurface);
	// 	SKMAC_ASSERT(!FAILED(hResult));
	return 1;
}
bool SKDX9Renderer::EndRenderTarget(SKRenderTarget *pRenderTarget, unsigned int i)
{
	if (!SKRenderer::EndRenderTarget(pRenderTarget, i))
		return 0;
	SKRenderTargetID *pRenderTargetID = (SKRenderTargetID *)pRenderTarget->GetIdentifier();
	if (!pRenderTargetID)
		return 0;

	SKTexture * pTexture = pRenderTarget->GetCreateBy();
	if (pRenderTarget->GetMulSample() != MS_NONE && pTexture)
	{

		RECT SourceRect = { 0, 0, pRenderTarget->GetWidth() - 1, pRenderTarget->GetHeight() - 1 };
		RECT DestRect = { 0, 0, pTexture->GetWidth(pRenderTarget->GetLevel()) - 1, pTexture->GetHeight(pRenderTarget->GetLevel()) - 1 };
		StretchRect(&pRenderTargetID->m_pRenderTarget, &SourceRect,
			&pRenderTargetID->m_pTextureSurface, &DestRect, (D3DTEXTUREFILTERTYPE)ms_dwTextureFilterType[SKSamplerDesc::FM_LINE]);

	}
	EndRenderTarget(i, &pRenderTargetID->m_pSaveRenderTarget);
	// 	hResult = m_pDevice->SetRenderTarget(i,pRenderTargetID->m_pSaveRenderTarget);
	// 	SKMAC_ASSERT(!FAILED(hResult));
	// 	SKMAC_RELEASE(pRenderTargetID->m_pSaveRenderTarget);

	return 1;
}
bool SKDX9Renderer::SetDepthStencilBuffer(SKDepthStencil * pDepthStencilBuffer)
{
	if (!SKRenderer::SetDepthStencilBuffer(pDepthStencilBuffer))
	{
		SetDepthStencilSurface(NULL);
		return 0;
	}
	SKDepthStencilID *pDepthStencilID = (SKDepthStencilID *)pDepthStencilBuffer->GetIdentifier();
	if (!pDepthStencilID)
	{
		return 0;
	}
	SetDepthStencilSurface(&pDepthStencilID->m_pDepthStencilBuffer, &pDepthStencilID->m_pSaveDepthStencilBuffer);
	// 	SKMAC_RELEASE(pDepthStencilID->m_pSaveDepthStencilBuffer)
	// 	hResult = m_pDevice->GetDepthStencilSurface(&pDepthStencilID->m_pSaveDepthStencilBuffer);
	// 	SKMAC_ASSERT(!FAILED(hResult));
	// 	hResult = m_pDevice->SetDepthStencilSurface(pDepthStencilID->m_pDepthStencilBuffer);
	// 	SKMAC_ASSERT(!FAILED(hResult));
	return 1;

}
bool SKDX9Renderer::EndDepthStencilBuffer(SKDepthStencil * pDepthStencilBuffer)
{
	if (!SKRenderer::EndDepthStencilBuffer(pDepthStencilBuffer))
		return 0;
	SKDepthStencilID *pDepthStencilID = (SKDepthStencilID *)pDepthStencilBuffer->GetIdentifier();
	if (!pDepthStencilID)
		return 0;
	// 	if(!pDepthStencilID->m_pSaveDepthStencilBuffer)
	// 		return 0;
	EndDepthStencilSurface(&pDepthStencilID->m_pSaveDepthStencilBuffer);
	// 	hResult = m_pDevice->SetDepthStencilSurface(pDepthStencilID->m_pSaveDepthStencilBuffer);
	// 	SKMAC_ASSERT(!FAILED(hResult));
	// 	SKMAC_RELEASE(pDepthStencilID->m_pSaveDepthStencilBuffer)
	return 1;
}
unsigned int SKDX9Renderer::GetTextureCompressLen(unsigned int uiLen)
{
	return Max((unsigned int)1, (unsigned int)(uiLen / 4));
}
bool SKDX9Renderer::SetVProgramConstant(unsigned int uiStartRegister, void * pDate,
	unsigned int RegisterNum, unsigned int uiType)
{
	SetVertexShaderConstant(uiStartRegister, pDate, RegisterNum, uiType);
	// 	if(uiType == SKUserConstant::VT_BOOL)
	// 	{
	// 		hResult = m_pDevice->SetVertexShaderConstantB(uiStartRegister,(const BOOL *)pDate,RegisterNum);
	// 		SKMAC_ASSERT(!FAILED(hResult));
	// 	}
	// 	else if(uiType == SKUserConstant::VT_FLOAT)
	// 	{
	// 		hResult = m_pDevice->SetVertexShaderConstantF(uiStartRegister,(const SKREAL *)pDate,RegisterNum);
	// 		SKMAC_ASSERT(!FAILED(hResult));
	// 	}
	// 	else if(uiType == SKUserConstant::VT_INT)
	// 	{
	// 		hResult = m_pDevice->SetVertexShaderConstantI(uiStartRegister,(const int *)pDate,RegisterNum);
	// 		SKMAC_ASSERT(!FAILED(hResult));
	// 	}
	// 	else 
	// 	{
	// 		SKMAC_ASSERT(0);
	// 	}
	return 1;
}
bool SKDX9Renderer::SetPProgramConstant(unsigned int uiStartRegister, void * pDate,
	unsigned int RegisterNum, unsigned int uiType)
{
	SetPixelShaderConstant(uiStartRegister, pDate, RegisterNum, uiType);
	// 	if(uiType == SKUserConstant::VT_BOOL)
	// 	{
	// 		hResult = m_pDevice->SetPixelShaderConstantB(uiStartRegister,(const BOOL *)pDate,RegisterNum);
	// 		SKMAC_ASSERT(!FAILED(hResult));
	// 	}
	// 	else if(uiType == SKUserConstant::VT_FLOAT)
	// 	{
	// 		hResult = m_pDevice->SetPixelShaderConstantF(uiStartRegister,(const SKREAL *)pDate,RegisterNum);
	// 		SKMAC_ASSERT(!FAILED(hResult));
	// 	}
	// 	else if(uiType == SKUserConstant::VT_INT)
	// 	{
	// 		hResult = m_pDevice->SetPixelShaderConstantI(uiStartRegister,(const int *)pDate,RegisterNum);
	// 		SKMAC_ASSERT(!FAILED(hResult));
	// 	}
	// 	else 
	// 	{
	// 		SKMAC_ASSERT(0);
	// 	}
	return 1;
}
bool SKDX9Renderer::DrawMesh(SKGeometry * pGeometry, SKRenderState * pRenderState, SKVShader * pVShader, SKPShader * pPShader, SKGShader * pGShader)
{

	ADD_TIME_PROFILE(Dx9DrawMesh);
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

	return 1;
}
bool SKDX9Renderer::DrawScreen(SCREEN_QUAD_TYPE ScreenQuad[4])
{
	if (!SKRenderer::DrawScreen(ScreenQuad))
		return 0;
	if (!ScreenQuad)
	{
		ScreenQuad = ms_FullScreen;
	}
	DrawScreenEX1(ScreenQuad);
	return 1;
}
bool SKDX9Renderer::DrawScreenFont(SCREEN_FONT_TYPE * pScreenFontBuffer, unsigned int uiVertexNum,
	SKUSHORT_INDEX * pIndexBuffer, unsigned int uiIndexNum)
{
	if (!SKRenderer::DrawScreenFont(pScreenFontBuffer, uiVertexNum, pIndexBuffer, uiIndexNum))
	{
		return false;
	}
	unsigned int uiNumTri = uiIndexNum / 3;
	SKMAC_ASSERT(uiNumTri);
	if (!uiNumTri)
	{
		return false;
	}
	DrawScreenFontEX(pScreenFontBuffer, uiVertexNum, pIndexBuffer, uiNumTri);
	return true;
}
bool SKDX9Renderer::DrawScreen(SCREEN_QUAD_TYPE * pScreenBuffer, unsigned int uiVertexNum,
	SKUSHORT_INDEX * pIndexBuffer, unsigned int uiIndexNum)
{
	if (!SKRenderer::DrawScreen(pScreenBuffer, uiVertexNum, pIndexBuffer, uiIndexNum))
	{
		return false;
	}
	unsigned int uiNumTri = uiIndexNum / 3;
	SKMAC_ASSERT(uiNumTri);
	if (!uiNumTri)
	{
		return false;
	}
	DrawScreenEX2(pScreenBuffer, uiVertexNum, pIndexBuffer, uiNumTri);
	// 	D3DFORMAT DateFormat;
	// 
	// 	DateFormat =  D3DFMT_INDEX16;
	// 
	// 	hResult = 
	// 		m_pDevice->DrawIndexedPrimitiveUP((D3DPRIMITIVETYPE)ms_dwPrimitiveType[SKMeshDate::MDT_TRIANGLE],
	// 		0,uiVertexNum,uiNumTri,pIndexBuffer,DateFormat,pScreenBuffer,sizeof(SCREEN_QUAD_TYPE));
	// 	SKMAC_ASSERT(!FAILED(hResult));
	return true;
}
bool SKDX9Renderer::SetViewPort(SKViewPort *pViewPort)
{
	SKRenderer::SetViewPort(pViewPort);
	D3DVIEWPORT9 View;
	if (pViewPort)
	{
		SKViewPort ViewPort;
		ViewPort = *pViewPort;
		View.X = Rounding(ViewPort.XMin * m_uiCurRTWidth);
		View.Y = Rounding(ViewPort.YMin * m_uiCurRTHeight);
		View.Width = Rounding(ViewPort.XMax * m_uiCurRTWidth) - View.X;
		View.Height = Rounding(ViewPort.YMax * m_uiCurRTHeight) - View.Y;
		View.MinZ = ViewPort.ZMin;
		View.MaxZ = ViewPort.ZMax;
	}
	else
	{
		View.X = 0;
		View.Y = 0;
		View.Width = m_uiCurRTWidth;
		View.Height = m_uiCurRTHeight;
		View.MinZ = 0.0f;
		View.MaxZ = 1.0f;

	}



	SetViewport(&View);
	return true;
}
SKDX9Renderer::SKDX9Renderer(HWND hMainWindow, unsigned int uiScreenWidth, unsigned int uiScreenHeight, bool bIsWindowed,
	unsigned int uiAnisotropy, unsigned int uiMultisample,
	ChildWindowInfo * pChildWindow, int uiNumChildWindow)
{
	//ǷӴ
	if (uiNumChildWindow > 0 && pChildWindow && bIsWindowed)
	{
		m_pChildWindowInfo = SK_NEW ChildWindowInfo[uiNumChildWindow];
		SKMemcpy(m_pChildWindowInfo, pChildWindow, sizeof(ChildWindowInfo)* uiNumChildWindow);
		m_iNumChildWindow = uiNumChildWindow;
	}
	else
	{
		uiNumChildWindow = 0;

	}

	m_hMainWindow = hMainWindow;
	m_bWindowed = bIsWindowed;
	m_uiScreenHeight = uiScreenHeight;
	m_uiScreenWidth = uiScreenWidth;
	m_uiCurRTWidth = uiScreenWidth;
	m_uiCurRTHeight = uiScreenHeight;
	SKMemset(&m_Present, 0, sizeof(m_Present));
	if (bIsWindowed)
	{
		m_Present.FullScreen_RefreshRateInHz = 0;
	}
	else
	{
		m_Present.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	}
	m_uinAdapter = 0;
	m_uiDevType = 0;
	//ȡʾģʽ
	D3DDISPLAYMODE d3dDisplayMode;
	HRESULT hResult = NULL;
	hResult = ms_pMain->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3dDisplayMode);
	SKMAC_ASSERT(!FAILED(hResult));

	D3DADAPTER_IDENTIFIER9 d3dAdapterIdentifier;
	hResult = ms_pMain->GetAdapterIdentifier(D3DADAPTER_DEFAULT, 0, &d3dAdapterIdentifier);
	SKMAC_ASSERT(!FAILED(hResult));
	m_AdapterName = d3dAdapterIdentifier.DeviceName;

	for (unsigned int i = 0; i < sizeof(ms_dwTextureFormatType) / sizeof(DWORD); i++)
	{
		if (d3dDisplayMode.Format == ms_dwTextureFormatType[i])
		{
			m_uiDisplayFormat = i;
			m_uiBufferFormat = i;
			break;
		}
	}
	m_uiDepthStencilFormat = SFT_D24S8;

	m_Present.Windowed = m_bWindowed;
	m_Present.BackBufferCount = 1;
	m_Present.BackBufferFormat = d3dDisplayMode.Format;
	m_Present.EnableAutoDepthStencil = TRUE;
	m_Present.AutoDepthStencilFormat = D3DFMT_D24S8;
	m_Present.SwapEffect = D3DSWAPEFFECT_DISCARD;


	m_Present.hDeviceWindow = m_hMainWindow;

	m_Present.BackBufferWidth = uiScreenWidth;
	m_Present.BackBufferHeight = uiScreenHeight;
	m_Present.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	m_Present.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;


	for (int uiMultiSampleTypes = MS_MAX - 1; uiMultiSampleTypes >= MS_NONE; uiMultiSampleTypes--)
	{
		hResult = ms_pMain->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
			D3DFMT_A8R8G8B8, bIsWindowed, (D3DMULTISAMPLE_TYPE)ms_dwMultiSampleTypes[uiMultiSampleTypes], NULL);
		if (SUCCEEDED(hResult))
		{
			m_uiMaxMultisample = uiMultiSampleTypes;
			break;
		}
	}

	if (uiMultisample > m_uiMaxMultisample)
	{
		m_Present.MultiSampleType = (D3DMULTISAMPLE_TYPE)ms_dwMultiSampleTypes[m_uiMaxMultisample];
		m_uiCurMultisample = m_uiMaxMultisample;
	}
	else
	{
		m_uiCurMultisample = uiMultisample;
		m_Present.MultiSampleType = (D3DMULTISAMPLE_TYPE)ms_dwMultiSampleTypes[m_uiCurMultisample];
	}

	m_uiCurRTMultisampler = m_uiCurMultisample;
	m_dwMultisampleQuality = 0;
	hResult = ms_pMain->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		D3DFMT_A8R8G8B8, bIsWindowed, (D3DMULTISAMPLE_TYPE)ms_dwMultiSampleTypes[m_uiCurMultisample], &m_dwMultisampleQuality);
	SKMAC_ASSERT(!FAILED(hResult));

	m_Present.MultiSampleQuality = m_dwMultisampleQuality - 1;

	struct DeviceType
	{
		D3DDEVTYPE DevType;
		DWORD dwBehavior;
	};
	DeviceType device_type[] =
	{
		{ D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING },
		{ D3DDEVTYPE_REF, D3DCREATE_SOFTWARE_VERTEXPROCESSING }
	};
	unsigned int type;
	for (type = 0; type < DT_MAX; type++)
	{
		if (SKResourceManager::ms_bRenderThread)
		{
			device_type[type].dwBehavior |= D3DCREATE_MULTITHREADED;
		}
		hResult = ms_pMain->CreateDevice(D3DADAPTER_DEFAULT, device_type[type].DevType,
			m_hMainWindow, device_type[type].dwBehavior, &m_Present, &m_pDevice);
		if (SUCCEEDED(hResult))
		{
			m_uiDevType = type;
			break;
		}
	}
	SKMAC_ASSERT(type == 0);
	m_pMainChain = NULL;
	hResult = m_pDevice->GetSwapChain(0, &m_pMainChain);
	SKMAC_ASSERT(m_pMainChain);
	SKMAC_ASSERT(!FAILED(hResult));
	hResult = m_pDevice->GetDepthStencilSurface(&m_pMainDepthStencilBuffer);
	SKMAC_ASSERT(m_pMainDepthStencilBuffer);
	SKMAC_ASSERT(!FAILED(hResult));
	D3DCAPS9 d3dCaps;
	hResult = ms_pMain->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dCaps);
	SKMAC_ASSERT(d3dCaps.MaxSimultaneousTextures > 1);
	SKMAC_ASSERT(!FAILED(hResult));
	m_uiMaxAnisotropy = d3dCaps.MaxAnisotropy;
	if (uiAnisotropy > m_uiMaxAnisotropy)
		m_uiCurAnisotropy = m_uiMaxAnisotropy;
	else
	{
		m_uiCurAnisotropy = uiAnisotropy;
	}


	m_uiMaxTexture = 16;
	m_uiMaxRTNum = d3dCaps.NumSimultaneousRTs;
	m_uiMaxVTexture = 4;
	m_uiMaxGTexture = 0;
	m_uiMaxDTexture = 0;
	m_uiMaxHTexture = 0;
	m_uiMaxTextureHeight = d3dCaps.MaxTextureHeight;
	m_uiMaxTextureWidth = d3dCaps.MaxTextureWidth;
	m_uiMaxUseClipPlane = d3dCaps.MaxUserClipPlanes;


	m_pChain = NULL;
	m_pChainnDepthStencilBuffer = NULL;
	if ((m_iNumChildWindow > 0) && m_bWindowed)
	{
		m_pChain = SK_NEW LPDIRECT3DSWAPCHAIN9[m_iNumChildWindow];
		m_pChainnDepthStencilBuffer = SK_NEW LPDIRECT3DSURFACE9[m_iNumChildWindow];
		for (int i = 0; i < m_iNumChildWindow; i++)
		{
			m_pChain[i] = NULL;
			m_pChainnDepthStencilBuffer[i] = NULL;
		}
		for (int i = 0; i < m_iNumChildWindow; i++)
		{
			m_Present.hDeviceWindow = m_pChildWindowInfo[i].m_hHwnd;
			m_Present.BackBufferWidth = m_pChildWindowInfo[i].m_uiWidth;
			m_Present.BackBufferHeight = m_pChildWindowInfo[i].m_uiHeight;
			hResult = m_pDevice->CreateAdditionalSwapChain(&m_Present, &m_pChain[i]);
			SKMAC_ASSERT(!FAILED(hResult));
			if (m_pChildWindowInfo[i].m_bDepth)
			{
				hResult = m_pDevice->CreateDepthStencilSurface(m_pChildWindowInfo[i].m_uiWidth, m_pChildWindowInfo[i].m_uiHeight, D3DFMT_D24S8,
					m_Present.MultiSampleType, m_Present.MultiSampleQuality, FALSE, &m_pChainnDepthStencilBuffer[i], NULL);
				SKMAC_ASSERT(!FAILED(hResult));
			}
		}
	}
	SetDefaultValue();
	for (unsigned int i = 0; i < m_uiMaxTexture; i++)
	{
		m_bSRGB[i] = false;
	}
	SKMemset(m_fFloatShaderBuffer, 0, sizeof(SKREAL)* MAX_SHADER_BUFFER);
	SKMemset(m_iIntShaderBuffer, 0, sizeof(int)* MAX_SHADER_BUFFER);
	SKMemset(m_bBoolShaderBuffer, 0, sizeof(bool)* MAX_SHADER_BUFFER);
	SKMemset(m_pTexAllStateBuffer, 0, sizeof(SKTexAllState *)* MAX_TEXTURE_BUFFER);
}
SKDX9Renderer::~SKDX9Renderer()
{

	ReleaseDefaultValue();
	SKMAC_RELEASE(m_pMainDepthStencilBuffer);
	SKMAC_RELEASE(m_pMainChain);
	for (int i = 0; i < m_iNumChildWindow; i++)
	{
		SKMAC_RELEASE(m_pChainnDepthStencilBuffer[i]);
		SKMAC_RELEASE(m_pChain[i]);
	}
	SKMAC_DELETEA(m_pChain);
	SKMAC_DELETEA(m_pChainnDepthStencilBuffer);

	SKMAC_RELEASE(m_pDevice);
}
IMPLEMENT_PRIORITY(SKDX9Renderer)
IMPLEMENT_INITIAL_ONLY_BEGIN(SKDX9Renderer)
ADD_PRIORITY(SKResourceManager);
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
ADD_TERMINAL_FUNCTION(TerminalDefaultState);
IMPLEMENT_INITIAL_ONLY_END
bool SKDX9Renderer::InitialDefaultState()
{
	SKMAC_RELEASE(ms_pMain);
	ms_pMain = Direct3DCreate9(D3D_SDK_VERSION);
	SKMAC_ASSERT(ms_pMain);
	ms_uiAdapterNum = ms_pMain->GetAdapterCount();
	D3DADAPTER_IDENTIFIER9 d3dAdapterIdentifier;
	D3DDISPLAYMODE d3ddspmd;
	D3DCAPS9 d3dCaps;
	HRESULT hResult = NULL;
	for (unsigned int nAdapter = 0; nAdapter < ms_uiAdapterNum; nAdapter++)
	{
		hResult = ms_pMain->GetAdapterIdentifier(nAdapter, 0, &d3dAdapterIdentifier);
		SKMAC_ASSERT(!FAILED(hResult));
		ms_AdapterInfo[nAdapter].AdapterName = d3dAdapterIdentifier.DeviceName;
		unsigned int DisplayModeNumA8R8G8B8 = 0;
		DisplayModeNumA8R8G8B8 = ms_pMain->GetAdapterModeCount(nAdapter, (D3DFORMAT)ms_dwTextureFormatType[SKRenderer::SFT_A8R8G8B8]);
		ms_AdapterInfo[nAdapter].uiDisplayModeNum = DisplayModeNumA8R8G8B8;
		ms_AdapterInfo[nAdapter].DisplayMode.Clear();
		ms_AdapterInfo[nAdapter].DisplayMode.SetBufferNum(DisplayModeNumA8R8G8B8);
		for (UINT nMode = 0; nMode < DisplayModeNumA8R8G8B8; nMode++)
		{
			TCHAR temp[20];
			hResult = ms_pMain->EnumAdapterModes(nAdapter, (D3DFORMAT)ms_dwTextureFormatType[SKRenderer::SFT_A8R8G8B8], nMode, &d3ddspmd);
			SKMAC_ASSERT(!FAILED(hResult));
			ms_AdapterInfo[nAdapter].DisplayMode[nMode].uiBitFormat = SKRenderer::SFT_A8R8G8B8;
			ms_AdapterInfo[nAdapter].DisplayMode[nMode].uiHeight = d3ddspmd.Height;
			ms_AdapterInfo[nAdapter].DisplayMode[nMode].uiWidth = d3ddspmd.Width;
			ms_AdapterInfo[nAdapter].DisplayMode[nMode].uiRefreshRate = d3ddspmd.RefreshRate;
			sprintf_s(temp, "%d * %d * 32", d3ddspmd.Width, d3ddspmd.Height);
			ms_AdapterInfo[nAdapter].DisplayMode[nMode].StringExpress = temp;
		}
		for (unsigned int DevType = 0; DevType < DT_MAX; DevType++)
		{
			hResult = ms_pMain->GetDeviceCaps(nAdapter, (D3DDEVTYPE)ms_dwDevTypes[DevType], &d3dCaps);
			SKMAC_ASSERT(!FAILED(hResult));
			ms_AdapterInfo[nAdapter].DeviceInfo[DevType].fMaxAnisotropy = d3dCaps.MaxAnisotropy;
			ms_AdapterInfo[nAdapter].DeviceInfo[DevType].PixelShaderVersion = d3dCaps.PixelShaderVersion;
			ms_AdapterInfo[nAdapter].DeviceInfo[DevType].uiMaxTexture = d3dCaps.MaxSimultaneousTextures;
			for (unsigned int uiMultiSampleTypes = MS_MAX - 1; uiMultiSampleTypes >= MS_NONE; uiMultiSampleTypes--)
			{
				hResult = ms_pMain->CheckDeviceMultiSampleType(nAdapter, (D3DDEVTYPE)ms_dwDevTypes[DevType],
					D3DFMT_A8R8G8B8, false, (D3DMULTISAMPLE_TYPE)ms_dwMultiSampleTypes[uiMultiSampleTypes], NULL);
				if (SUCCEEDED(hResult))
				{
					ms_AdapterInfo[nAdapter].DeviceInfo[DevType].uiMaxMultisample = uiMultiSampleTypes;
					break;
				}
			}
			ms_AdapterInfo[nAdapter].DeviceInfo[DevType].uiMaxTextureHeight = d3dCaps.MaxTextureHeight;
			ms_AdapterInfo[nAdapter].DeviceInfo[DevType].uiMaxTextureWidth = d3dCaps.MaxTextureWidth;
			ms_AdapterInfo[nAdapter].DeviceInfo[DevType].uiMaxUseClipPlane = d3dCaps.MaxUserClipPlanes;
			ms_AdapterInfo[nAdapter].DeviceInfo[DevType].VertexShaderVersion = d3dCaps.VertexShaderVersion;
			ms_AdapterInfo[nAdapter].DeviceInfo[DevType].uiMaxRTNum = d3dCaps.NumSimultaneousRTs;
		}
	}

	SKString RenderAPIPre = SKResourceManager::GetRenderTypeShaderPath(RAT_DIRECTX9);
	SKFile IncludeShaderFile;
	SKString IncludeShaderPath = SKResourceManager::ms_ShaderPath + RenderAPIPre + _T("SKShader.txt");
	if (!IncludeShaderFile.Open(IncludeShaderPath.GetBuffer(), SKFile::OM_RB))
	{
		return false;
	}
	unsigned int uiIncludeSize = IncludeShaderFile.GetFileSize();
	ms_IncludeShader.SetTCHARBufferNum(uiIncludeSize);
	if (!IncludeShaderFile.Read(ms_IncludeShader.GetBuffer(), uiIncludeSize, 1))
	{
		return false;
	}


	SKFile DynamicShaderFile;
	SKString DynamicShaderPath = SKResourceManager::ms_ShaderPath + RenderAPIPre + _T("SKDymaticSKShader.txt");
	if (!DynamicShaderFile.Open(DynamicShaderPath.GetBuffer(), SKFile::OM_RB))
	{
		return false;
	}
	unsigned int uiSize = DynamicShaderFile.GetFileSize();
	SKString VDynamicShaderString;
	VDynamicShaderString.SetTCHARBufferNum(uiSize);
	if (!DynamicShaderFile.Read(VDynamicShaderString.GetBuffer(), uiSize, 1))
	{
		return false;
	}
	ms_DynamicShader.Format(VDynamicShaderString.GetBuffer(), SKResourceManager::GetGpuSkinBoneNum() * 3);

	ms_pDx9IncludeShader = SK_NEW SKDx9ShaderInclude();
	SKMAC_ASSERT(ms_pDx9IncludeShader)
		return 1;
}
bool SKDX9Renderer::TerminalDefaultState()
{
	SKMAC_DELETE(ms_pDx9IncludeShader);
	SKMAC_RELEASE(ms_pMain);
	return 1;
}
bool SKDX9Renderer::UseWindow(int uiWindowID)
{
	if (!SKRenderer::UseWindow(uiWindowID))
		return 0;
	UseWindowEx(uiWindowID);
	// 	LPDIRECT3DSURFACE9 pBack=NULL;
	// 	if (uiWindowID == - 1)
	// 	{
	// 		hResult = m_pMainChain->GetBackBuffer(0,D3DBACKBUFFER_TYPE_MONO,&pBack);
	// 	}
	// 	else
	// 	{
	// 		hResult = m_pChain[uiWindowID]->GetBackBuffer(0,D3DBACKBUFFER_TYPE_MONO,&pBack);
	// 	}
	// 	
	// 	SKMAC_ASSERT(!FAILED(hResult));
	// 	m_pDevice->SetRenderTarget(0, pBack);
	// 	SKMAC_RELEASE(pBack);
	return 1;
}
bool SKDX9Renderer::CooperativeLevel()
{
	HRESULT hResult = NULL;
	hResult = m_pDevice->TestCooperativeLevel();

	if (hResult == D3DERR_DEVICELOST)
	{
		DeviceLost();
		return false;
	}

	if (hResult == D3DERR_DEVICENOTRESET)
	{

		ResetDevice();
	}
	return true;
}
bool SKDX9Renderer::BeginRendering()
{
	SKRenderer::BeginRendering();

	BeginRender();
	// 	hResult = m_pDevice->BeginScene();
	// 	SKMAC_ASSERT(!FAILED(hResult));
	return true;
}
bool SKDX9Renderer::EndRendering()
{
	SKRenderer::EndRendering();
	EndRender();
	// 	m_pDevice->EndScene();
	// 	if (m_bWindowed && (m_iNumChildWindow > 0)) 
	// 	{
	// 		hResult = m_pChain[m_iCurWindowID]->Present(NULL, NULL, NULL, NULL, 0);
	// 		SKMAC_ASSERT(!FAILED(hResult));	
	// 	}
	// 	else 
	// 	{
	// 		hResult = m_pMainChain->Present(NULL, NULL, NULL, NULL,0);
	// 		SKMAC_ASSERT(!FAILED(hResult));	
	// 	}
	return true;
}
void SKDX9Renderer::DeviceLost()
{
	SKMAC_RELEASE(m_pMainChain);
	SKMAC_RELEASE(m_pMainDepthStencilBuffer);
	for (int i = 0; i < m_iNumChildWindow; i++)
	{
		SKMAC_RELEASE(m_pChain[i]);
		SKMAC_RELEASE(m_pChainnDepthStencilBuffer[i]);
	}
	for (unsigned int i = 0; i < SKBind::ms_BindArray.GetNum(); i++)
	{
		VS2DTexture * p2DTexture = DynamicCast<VS2DTexture>(SKBind::ms_BindArray[i]);
		if (p2DTexture)
		{
			if (p2DTexture->IsRenderTarget() || !p2DTexture->IsStatic())
			{
				if (!p2DTexture->ReleaseResource())
				{
					SKMAC_ASSERT(0);
				}
			}
		}
		SKCubeTexture * pCubTexture = DynamicCast<SKCubeTexture>(SKBind::ms_BindArray[i]);
		if (pCubTexture)
		{
			if (pCubTexture->IsRenderTarget() || !pCubTexture->IsStatic())
			{
				if (!pCubTexture->ReleaseResource())
				{
					SKMAC_ASSERT(0);
				}
			}
		}
		SKVertexBuffer * pVertexBuffer = DynamicCast<SKVertexBuffer>(SKBind::ms_BindArray[i]);
		if (pVertexBuffer)
		{
			if (!pVertexBuffer->ReleaseResource())
			{
				SKMAC_ASSERT(0);
			}
		}

		SKIndexBuffer * pIndexBuffer = DynamicCast<SKIndexBuffer>(SKBind::ms_BindArray[i]);
		if (pIndexBuffer)
		{
			if (!pIndexBuffer->ReleaseResource())
			{
				SKMAC_ASSERT(0);
			}
		}

		SKDepthStencil * pDepthStencilBuffer = DynamicCast<SKDepthStencil>(SKBind::ms_BindArray[i]);
		if (pDepthStencilBuffer)
		{
			if (!pDepthStencilBuffer->ReleaseResource())
			{
				SKMAC_ASSERT(0);
			}
		}
		SKRenderTarget * pRenderTarget = DynamicCast<SKRenderTarget>(SKBind::ms_BindArray[i]);
		if (pRenderTarget)
		{
			if (!pRenderTarget->ReleaseResource())
			{
				SKMAC_ASSERT(0);
			}
		}
	}

}
void SKDX9Renderer::ResetDevice()
{

	HRESULT hResult = NULL;
	m_Present.BackBufferWidth = m_uiScreenWidth;
	m_Present.BackBufferHeight = m_uiScreenHeight;
	hResult = m_pDevice->Reset(&m_Present);
	SKMAC_ASSERT(SUCCEEDED(hResult));

	hResult = m_pDevice->GetSwapChain(0, &m_pMainChain);
	SKMAC_ASSERT(m_pMainChain);
	SKMAC_ASSERT(!FAILED(hResult));

	hResult = m_pDevice->GetDepthStencilSurface(&m_pMainDepthStencilBuffer);
	SKMAC_ASSERT(m_pMainDepthStencilBuffer);
	SKMAC_ASSERT(!FAILED(hResult));

	if (m_bWindowed && m_iNumChildWindow > 0)
	{
		for (int i = 0; i < m_iNumChildWindow; i++)
		{
			m_Present.hDeviceWindow = m_pChildWindowInfo[i].m_hHwnd;
			m_Present.BackBufferWidth = m_pChildWindowInfo[i].m_uiWidth;
			m_Present.BackBufferHeight = m_pChildWindowInfo[i].m_uiHeight;
			hResult = m_pDevice->CreateAdditionalSwapChain(&m_Present, &m_pChain[i]);
			SKMAC_ASSERT(!FAILED(hResult));

			if (m_pChildWindowInfo[i].m_bDepth)
			{
				hResult = m_pDevice->CreateDepthStencilSurface(m_pChildWindowInfo[i].m_uiWidth, m_pChildWindowInfo[i].m_uiHeight, D3DFMT_D24S8,
					m_Present.MultiSampleType, m_Present.MultiSampleQuality, FALSE, &m_pChainnDepthStencilBuffer[i], NULL);
				SKMAC_ASSERT(!FAILED(hResult));
			}
		}
	}
	RenderStateToDefault();
	for (unsigned int i = 0; i < m_uiMaxTexture; i++)
	{
		if (m_bSRGB[i])
		{
			m_bSRGB[i] = false;
			SetSamplerState(i, D3DSAMP_SRGBTEXTURE, 0);
		}
	}
}
bool SKDX9Renderer::ChangeScreenSize(unsigned int uiWidth, unsigned int uiHeight, bool bWindow)
{
	if (!SKRenderer::ChangeScreenSize(uiWidth, uiHeight, bWindow))
	{
		return false;
	}
	m_Present.Windowed = bWindow;
	if (m_Present.Windowed)
	{
		m_Present.FullScreen_RefreshRateInHz = 0;

	}
	else
	{

		m_Present.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	}

	DeviceLost();
	ResetDevice();
	return true;
}
void * SKDX9Renderer::Lock(SKVertexBuffer * pVertexBuffer)
{
	if (!pVertexBuffer)
		return NULL;
	// 	if(!pVertexBuffer->GetPositionDate(0))
	// 		return NULL;

	if (pVertexBuffer->IsStatic())
		return NULL;

	if (!LoadVBuffer(pVertexBuffer))
		return NULL;

	SKVBufferID* pVBufferID = (SKVBufferID *)pVertexBuffer->GetIdentifier();

	if (!pVBufferID->m_pVertexBuffer)
	{
		return NULL;
	}

	void *pResource = NULL;
	HRESULT hResult = NULL;
	hResult = pVBufferID->m_pVertexBuffer->Lock(0,
		pVertexBuffer->GetOneVertexSize() * pVertexBuffer->GetVertexNum(), (void**)(&pResource), ms_dwLockFlag[pVertexBuffer->GetLockFlag()]);
	SKMAC_ASSERT(!FAILED(hResult));
	return pResource;
}
void SKDX9Renderer::UnLock(SKVertexBuffer * pVertexBuffer)
{
	if (!pVertexBuffer)
		return;
	// 	if(!pVertexBuffer->GetPositionDate(0))
	// 		return;

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
	HRESULT hResult = NULL;
	hResult = pVBufferID->m_pVertexBuffer->Unlock();
	SKMAC_ASSERT(!FAILED(hResult));
	return;
}

void * SKDX9Renderer::Lock(SKIndexBuffer * pIndexBuffer)
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
	void *pResource = NULL;
	HRESULT hResult = NULL;
	hResult = pIBufferID->m_IndexBuffer->Lock(0,
		pIndexBuffer->GetByteSize(), (void**)(&pResource), ms_dwLockFlag[pIndexBuffer->GetLockFlag()]);
	SKMAC_ASSERT(!FAILED(hResult));
	return pResource;
}
void SKDX9Renderer::UnLock(SKIndexBuffer * pIndexBuffer)
{
	if (!pIndexBuffer)
		return;
	// 	if(!pIndexBuffer->GetDate())
	// 		return ;
	if (pIndexBuffer->IsStatic())
		return;
	SKIBufferID* pIBufferID = (SKIBufferID *)pIndexBuffer->GetIdentifier();
	if (!pIBufferID)
	{
		return;
	}
	if (!pIBufferID->m_IndexBuffer)
		return;
	HRESULT hResult = NULL;
	hResult = pIBufferID->m_IndexBuffer->Unlock();
	SKMAC_ASSERT(!FAILED(hResult));
	return;
}
void * SKDX9Renderer::Lock(SKTexture * pTexture, unsigned int uiLevel, unsigned int uiFace)
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
		D3DLOCKED_RECT LockRect;
		LPDIRECT3DTEXTURE9 pDXTexture = (LPDIRECT3DTEXTURE9)pTextureID->m_pTexture;
		hResult = pDXTexture->LockRect(uiLevel, &LockRect, 0, ms_dwLockFlag[pTexture->GetLockFlag()]);
		SKMAC_ASSERT(!FAILED(hResult));
		return LockRect.pBits;
	}
	else if (pTexture->GetTexType() == SKTexture::TT_3D)
	{
		LPDIRECT3DVOLUMETEXTURE9 pDXTexture = (LPDIRECT3DVOLUMETEXTURE9)pTextureID->m_pTexture;

		D3DLOCKED_BOX kLockBox;
		hResult = pDXTexture->LockBox(uiLevel, &kLockBox, 0, ms_dwLockFlag[pTexture->GetLockFlag()]);
		SKMAC_ASSERT(!FAILED(hResult));
		return kLockBox.pBits;
	}
	else if (pTexture->GetTexType() == SKTexture::TT_CUBE)
	{
		if (uiFace > SKCubeTexture::F_MAX)
			return NULL;
		if (((SKCubeTexture*)pTexture)->IsRenderTarget())
		{
			return NULL;
		}
		D3DLOCKED_RECT LockRect;
		LPDIRECT3DCUBETEXTURE9 pDXTexture = (LPDIRECT3DCUBETEXTURE9)pTextureID->m_pTexture;
		hResult = pDXTexture->LockRect((D3DCUBEMAP_FACES)ms_dwCubeMapFace[uiFace], uiLevel, &LockRect, NULL, ms_dwLockFlag[pTexture->GetLockFlag()]);
		SKMAC_ASSERT(!FAILED(hResult));
		return LockRect.pBits;

	}
	else
	{
		SKMAC_ASSERT(0);
		return NULL;
	}

}
void SKDX9Renderer::UnLock(SKTexture * pTexture, unsigned int uiLevel, unsigned int uiFace)
{
	if (!pTexture)
		return;
	// 	if(!pTexture->GetBuffer(0))
	// 		return ;
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
		LPDIRECT3DTEXTURE9 pDXTexture = (LPDIRECT3DTEXTURE9)pTextureID->m_pTexture;

		hResult = pDXTexture->UnlockRect(uiLevel);
		SKMAC_ASSERT(!FAILED(hResult));
		return;
	}
	else if (pTexture->GetTexType() == SKTexture::TT_3D)
	{
		LPDIRECT3DVOLUMETEXTURE9 pDXTexture = (LPDIRECT3DVOLUMETEXTURE9)pTextureID->m_pTexture;
		hResult = pDXTexture->UnlockBox(uiLevel);
		SKMAC_ASSERT(!FAILED(hResult));
		return;
	}
	else if (pTexture->GetTexType() == SKTexture::TT_CUBE)
	{
		if (uiFace > SKCubeTexture::F_MAX)
			return;
		if (((SKCubeTexture*)pTexture)->IsRenderTarget())
		{
			return;
		}
		LPDIRECT3DCUBETEXTURE9 pDXTexture = (LPDIRECT3DCUBETEXTURE9)pTextureID->m_pTexture;

		hResult = pDXTexture->UnlockRect((D3DCUBEMAP_FACES)ms_dwCubeMapFace[uiFace], uiLevel);
		SKMAC_ASSERT(!FAILED(hResult));
		return;

	}
	else
	{
		SKMAC_ASSERT(0);
		return;
	}
	return;
}
void * SKDX9Renderer::Lock(SKRenderTarget * pRenderTarget)
{
	return NULL;
}
void SKDX9Renderer::UnLock(SKRenderTarget * pRenderTarget)
{

}

void * SKDX9Renderer::Lock(SKDepthStencil * pDepthStencil)
{
	return NULL;
}
void SKDX9Renderer::UnLock(SKDepthStencil * pDepthStencil)
{

}
void SKDX9Renderer::GetIncludeShader(SKString &OutString)
{
	OutString = ms_IncludeShader;
}

void SKDX9Renderer::GetDynamicShader(SKString &OutString)
{
	OutString = ms_DynamicShader;
}
const SKAdapterInfo* SKDX9Renderer::GetAdapterInfo(unsigned int & uiAdapterNum)
{
	uiAdapterNum = ms_uiAdapterNum;
	return ms_AdapterInfo;
}
bool SKDX9Renderer::IsSupportMulBufferSwtich()const
{
	return false;
}
bool SKDX9Renderer::IsSupportGS()const
{
	return false;
}



