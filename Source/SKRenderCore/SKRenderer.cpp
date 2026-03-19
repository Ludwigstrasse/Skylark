#include "SKRenderer.h"
#include "SKFont.h"
#include "SKVShader.h"
#include "SKPShader.h"
#include "SKGShader.h"
#include "SKDShader.h"
#include "SKHShader.h"
#include "SKVertexBuffer.h"
#include "SKGeometry.h"
#include "SKTexture.h"
#include "SK2DTexture.h"
#include "SKTexAllState.h"
#include "SKMaterial.h"
#include "SKDepthStencil.h"
#include "SKCubeTexture.h"
#include "SKVertexFormat.h"
#include "SKMeshDate.h"
#include "SKShaderStringFactory.h"
#include "SKAABB3.h"
#include "SKOBB3.h"
#include "SKSphere3.h"
#include "SKTriangleSet.h"
#include "SKLineSet.h"
#include "SKGraphicInclude.h"
#include "SKRenderTarget.h"
#include "SKProfiler.h"
using namespace SKEngine2;
SKRenderer * SKRenderer::ms_pRenderer = NULL;

SKString SKRenderer::ms_VShaderProgramMain = _T("SKMain");
SKString SKRenderer::ms_PShaderProgramMain = _T("PSMain");
SKString SKRenderer::ms_GShaderProgramMain = _T("GSMain");
unsigned int SKRenderer::ms_uiBytesPerPixel[SFT_MAX] =
{
	4,  
	2,
	4,
	4,
	16,
	8,
	4,
	2,
	4,
	4,
	4,
	8,
	2,
	1,
	16,
	16
};
unsigned int SKRenderer::ms_uiChannelPerPixel[SFT_MAX] =
{
	4,  
	1,
	2,
	1,
	4,
	4,
	2,
	1,
	1,
	4,
	2,
	2,
	3,
	1,
	4,
	4
};
SKRenderer::SCREEN_QUAD_TYPE SKRenderer::ms_FullScreen[4] = 
{
	{SKVector3(-1.0f,  1.0f, 0.0f)},
	{SKVector3( 1.0f,  1.0f, 0.0f)},
	{SKVector3( 1.0f, -1.0f, 0.0f)},
	{SKVector3(-1.0f, -1.0f, 0.0f)}	
	
};
SKUSHORT_INDEX SKRenderer::ms_FullScreenI[6] = { 0, 1, 3, 1, 2, 3 };
unsigned int SKRenderer::GetBytesPerPixel(unsigned int uiFormatType)
{
	if (uiFormatType >= SFT_MAX)
	{
		return 0;
	}
	return ms_uiBytesPerPixel[uiFormatType];
}
unsigned int SKRenderer::GetChannelPerPixel(unsigned int uiFormatType)
{
	if (uiFormatType >= SFT_MAX)
	{
		return 0;
	}
	return ms_uiChannelPerPixel[uiFormatType];
}
const SKString & SKRenderer::GetVShaderProgramMain()
{
	return ms_VShaderProgramMain;
}
const SKString & SKRenderer::GetPShaderProgramMain()
{
	return ms_PShaderProgramMain;
}
const SKString & SKRenderer::GetGShaderProgramMain()
{
	return ms_GShaderProgramMain;
}
SKRenderer::~SKRenderer()
{
	SKMAC_DELETEA(m_pChildWindowInfo);
	m_pVertexBuffer = NULL;
	m_pVertexFormat = NULL;
	m_pIndexBuffer = NULL;
	m_pVShader = NULL;
	m_pPshader = NULL;
	m_pGShader = NULL;
	m_pDShader = NULL;
	m_pHShader = NULL;
	m_pFont		= NULL;
	m_pQuadVertexFormat = NULL;

	m_pBlendState = NULL;
	m_pDepthStencilState = NULL;
	m_pRasterizerState = NULL;

}
void SKRenderer::RenderStateToDefault()
{
	SKMAC_ASSERT(m_uiMaxTexture);

	for (unsigned int i = 0; i < m_uiMaxTexture; i++)
	{
		m_pPTex[i] = NULL;

		m_pPSamplerState[i] = (SKSamplerState *)SKSamplerState::GetDefault();
		SetSamplerState(NULL, i, true);
	}

	SKMAC_ASSERT(m_uiMaxVTexture);
	for (unsigned int i = 0; i < m_uiMaxVTexture; i++)
	{
		m_pVTex[i] = NULL;
		m_pVSamplerState[i] = (SKSamplerState *)SKSamplerState::GetDefault();
		SetVSamplerState(NULL, i, true);
	}

	for (unsigned int i = 0; i < m_uiMaxGTexture; i++)
	{
		m_pGTex[i] = NULL;
		m_pGSamplerState[i] = (SKSamplerState *)SKSamplerState::GetDefault();
		SetGSamplerState(NULL, i, true);
	}

	for (unsigned int i = 0; i < m_uiMaxDTexture; i++)
	{
		m_pDTex[i] = NULL;
		m_pDSamplerState[i] = (SKSamplerState *)SKSamplerState::GetDefault();
		SetDSamplerState(NULL, i, true);
	}

	for (unsigned int i = 0; i < m_uiMaxHTexture; i++)
	{
		m_pHTex[i] = NULL;
		m_pHSamplerState[i] = (SKSamplerState *)SKSamplerState::GetDefault();
		SetHSamplerState(NULL, i, true);
	}

	m_pBlendState = (SKBlendState *)SKBlendState::GetDefault();
	m_pDepthStencilState = (SKDepthStencilState *)SKDepthStencilState::GetDefault();
	m_pRasterizerState = (SKRasterizerState *)SKRasterizerState::GetDefault();
	SetDepthStencilState(NULL, true);
	SetBlendState(NULL, true);
	SetRasterizerState(NULL, true);
	m_bClipPlaneEnable = false;
	m_bScissorRectEnable = false;
 	m_pVertexFormat = NULL;
	m_pVertexBuffer = NULL;
	m_pIndexBuffer = NULL;
	m_pPshader = NULL;
	m_pVShader = NULL;
	m_pGShader = NULL;
	m_pDShader = NULL;
	m_pHShader = NULL;
	SetTexture(SKTexAllState::GetDefalut()->m_pTex, 0);
}
bool SKRenderer::SetDefaultValue()
{
	SKMAC_ASSERT(m_uiMaxTexture);

	for(unsigned int i = 0 ; i < m_uiMaxTexture ; i++)
	{
		m_pPTex[i] = NULL;

		m_pPSamplerState[i] = (SKSamplerState *)SKSamplerState::GetDefault();
		SetSamplerState(NULL, i, true);
	}
	SKMAC_ASSERT(m_uiMaxVTexture);

	for(unsigned int i = 0 ; i < m_uiMaxVTexture ; i++)
	{
		m_pVTex[i] = NULL;
		m_pVSamplerState[i] = (SKSamplerState *)SKSamplerState::GetDefault();
		SetVSamplerState(NULL, i, true);
	}

	for (unsigned int i = 0; i < m_uiMaxGTexture; i++)
	{
		m_pGTex[i] = NULL;
		m_pGSamplerState[i] = (SKSamplerState *)SKSamplerState::GetDefault();
		SetGSamplerState(NULL, i, true);
	}

	for (unsigned int i = 0; i < m_uiMaxDTexture; i++)
	{
		m_pDTex[i] = NULL;
		m_pDSamplerState[i] = (SKSamplerState *)SKSamplerState::GetDefault();
		SetDSamplerState(NULL, i, true);
	}

	for (unsigned int i = 0; i < m_uiMaxHTexture; i++)
	{
		m_pHTex[i] = NULL;
		m_pHSamplerState[i] = (SKSamplerState *)SKSamplerState::GetDefault();
		SetHSamplerState(NULL, i, true);
	}
	m_pBlendState = (SKBlendState *)SKBlendState::GetDefault();
	m_pDepthStencilState = (SKDepthStencilState *)SKDepthStencilState::GetDefault();
	m_pRasterizerState = (SKRasterizerState *)SKRasterizerState::GetDefault();
 	SetDepthStencilState(NULL, true);
 	SetBlendState(NULL, true);
 	SetRasterizerState(NULL, true);
	m_bClipPlaneEnable = false;
	m_bScissorRectEnable = false;

	

	m_pFont = SKResourceManager::LoadASYNFont(_T("Arial"),false);

	SetTexture(SKTexAllState::GetDefalut()->m_pTex,0);

	SKArray<SKVertexFormat::VERTEXFORMAT_TYPE> ForamtArray;
	SKVertexFormat::VERTEXFORMAT_TYPE Pos;
	Pos.DateType = SKDataBuffer::DT_FLOAT32_3;
	Pos.OffSet = 0;
	Pos.Semantics = SKVertexFormat::VF_POSITION;
	Pos.SemanticsIndex = 0;
	ForamtArray.AddElement(Pos);

	SKVertexFormat::VERTEXFORMAT_TYPE TextureCode;
	TextureCode.DateType = SKDataBuffer::DT_FLOAT32_2;
	TextureCode.OffSet = 12;
	TextureCode.Semantics = SKVertexFormat::VF_TEXCOORD;
	TextureCode.SemanticsIndex = 0;
	ForamtArray.AddElement(TextureCode);
	m_pQuadVertexFormat = SKResourceManager::LoadVertexFormat(NULL, &ForamtArray);

	SKVertexFormat::VERTEXFORMAT_TYPE ColorCode;
	ColorCode.DateType = SKDataBuffer::DT_COLOR;
	ColorCode.OffSet = 20;
	ColorCode.Semantics = SKVertexFormat::VF_COLOR;
	ColorCode.SemanticsIndex = 0;
	ForamtArray.AddElement(ColorCode);
	m_pQuadVertexFormat = SKResourceManager::LoadVertexFormat(NULL, &ForamtArray);
	SKResourceManager::LoadDefaultResource(GetRendererType());
	//SKCustomMaterial::LoadAllDeviceShader();
	
	return 1;
}
bool SKRenderer::SetRenderTargets(SKRenderTarget * pRenderTargets[], unsigned int uiNum)
{
	if (uiNum > 0)
	{
		if (pRenderTargets[0] == NULL)
		{
			return false;
		}
		for (unsigned int i = 0; i < uiNum; i++)
		{
			SetRenderTarget(pRenderTargets[i], i);
		}
		return true;
	}
	return false;

}
bool SKRenderer::EndRenderTargets(SKRenderTarget * pRenderTargets[], unsigned int uiNum)
{
	if (uiNum > 0)
	{
		if (pRenderTargets[0] == NULL)
		{
			return false;
		}
		for (unsigned int i = 0; i < uiNum; i++)
		{
			EndRenderTarget(pRenderTargets[i], i);
		}
		return true;
	}
	return false;
}
SKRenderer::ChildWindowInfo * SKRenderer::GetChildWindowInfo(int uiID)
{
	if (!m_bWindowed)
		return NULL;
	else if (uiID >= m_iNumChildWindow)
		return NULL;
	else if (uiID < 0)
	{
		return NULL;
	}
	return &m_pChildWindowInfo[uiID];
}

bool SKRenderer::ReleaseDefaultValue()
{
	ReleaseBindObjectResource();


	return 1;
}
bool SKRenderer::ReleaseBindObjectResource()
{
	for (unsigned int i = 0; i < SKBind::ms_BindArray.GetNum(); i++)
	{
		SKBind * pBind = SKBind::ms_BindArray[i];
		if (!pBind)
		{
			continue;
		}
		if (!pBind->ReleaseResource())
		{
			SKMAC_ASSERT(0);
			return false;
		}
	}
	return true;
}

void SKRenderer::SetUseState(SKRenderState & RenderState, unsigned int uiRenderStateInheritFlag)
{
	m_UseState.GetAll(&RenderState);
	m_uiRenderStateInheritFlag = uiRenderStateInheritFlag;
}
void SKRenderer::ClearUseState()
{
	m_uiRenderStateInheritFlag = 0;
}
void SKRenderer::SetRenderState(SKRenderState & RenderState)
{

	SetBlendState(RenderState.m_pBlendState);

	SetDepthStencilState(RenderState.m_pDepthStencilState);

	SetRasterizerState(RenderState.m_pRasterizerState);

	if (RenderState.m_RasterizerDesc.m_bClipPlaneEnable && RenderState.m_Plane.GetNum() > 0)
	{
		m_bClipPlaneEnable = true;
		SetClipPlane(RenderState.m_Plane, m_bClipPlaneEnable);
	}
	else
	{
		if (m_bClipPlaneEnable)
		{
			m_bClipPlaneEnable = false;
			SetClipPlane(RenderState.m_Plane, m_bClipPlaneEnable);
		}
	}
	if (RenderState.m_RasterizerDesc.m_bScissorTestEnable && RenderState.m_RectArray.GetNum() > 0)
	{
		m_bScissorRectEnable = true;
		SetScissorRect(RenderState.m_RectArray, m_bScissorRectEnable);
	}
	else
	{
		if (m_bScissorRectEnable)
		{
			m_bScissorRectEnable = false;
			SetScissorRect(RenderState.m_RectArray, m_bScissorRectEnable);
		}
	}


}
bool SKRenderer::SetVShaderProgram(SKVShader*	pVShaderProgram)
{
	if (!pVShaderProgram)
		return 0;
	if (!LoadVShaderProgram(pVShaderProgram))
		return 0;

	/*SKResourceIdentifier * pID = pVShaderProgram->GetIdentifier(this);
	if(!pID)
	return 0;*/

	return 1;
}
bool SKRenderer::SetGShaderProgram(SKGShader* pGShaderProgram)
{
	if (!pGShaderProgram)
		return 0;
	if (!LoadGShaderProgram(pGShaderProgram))
		return 0;
	return 1;
}
bool SKRenderer::SetDShaderProgram(SKDShader* pDShaderProgram)
{
	if (!pDShaderProgram)
		return 0;
	if (!LoadDShaderProgram(pDShaderProgram))
		return 0;
	return 1;
}
bool SKRenderer::SetHShaderProgram(SKHShader* pHShaderProgram)
{
	if (!pHShaderProgram)
		return 0;
	if (!LoadHShaderProgram(pHShaderProgram))
		return 0;
	return 1;
}
bool SKRenderer::SetPShaderProgram(SKPShader*	pPShaderProgram)
{
	if (!pPShaderProgram)
		return 0;
	if (!LoadPShaderProgram(pPShaderProgram))
		return 0;

	/*SKResourceIdentifier * pID = pPShaderProgram->GetIdentifier(this);
	if(!pID)
	return 0;*/


	return 1;
}
void SKRenderer::EnableTextureRTUsed(SKTexture * pTexture)
{
	if (!pTexture)
	{
		return;
	}
	if (pTexture->GetTexType() == SKTexture::TT_2D)
	{
		((VS2DTexture *)pTexture)->m_bRtUsed = true;
	}
	else if (pTexture->GetTexType() == SKTexture::TT_CUBE)
	{
		((VS2DTexture *)pTexture)->m_bRtUsed = true;
	}
}
void SKRenderer::DisableTextureRTUsed(SKTexture * pTexture)
{
	if (!pTexture)
	{
		return;
	}
	if (pTexture->GetTexType() == SKTexture::TT_2D)
	{
		((VS2DTexture *)pTexture)->m_bRtUsed = false;
	}
	else if (pTexture->GetTexType() == SKTexture::TT_CUBE)
	{
		((VS2DTexture *)pTexture)->m_bRtUsed = false;
	}
}
bool SKRenderer::SetRenderTarget(SKRenderTarget *pRenderTarget, unsigned int i)
{
	if (!pRenderTarget)
	{
		return 0;
	}
	if (i >= m_uiMaxRTNum)
	{
		return 0;
	}

	if (!LoadRenderTarget(pRenderTarget))
		return 0;
	SKTexture * pTexture = pRenderTarget->GetCreateBy();
	if (pTexture->HasAnySlot())
	{
		SKMAC_ASSERT(0);
		return 0;
	}
	if (m_uiCurRTWidth != pRenderTarget->GetWidth()
		|| m_uiCurRTHeight != pRenderTarget->GetHeight()
		|| m_uiCurRTMultisampler != pRenderTarget->GetMulSample())
	{
		if (i == 0 && m_uiBackMacthType == BMT_NONE)
		{
			m_uiBackMacthType = BMT_RENDER_TARGET;
			m_uiCurRTWidth = pRenderTarget->GetWidth();
			m_uiCurRTHeight = pRenderTarget->GetHeight();
			m_uiCurRTMultisampler = pRenderTarget->GetMulSample();
		}
		else
		{
			SKMAC_ASSERT(false);
			return false;
		}
		
	}
	
	EnableTextureRTUsed(pTexture);
	return 1;
}
bool SKRenderer::EndRenderTarget(SKRenderTarget *pRenderTarget, unsigned int i)
{
	if (!pRenderTarget)
		return 0;

	if (i >= m_uiMaxRTNum)
	{
		return 0;
	}

	SKTexture * pTexture = pRenderTarget->GetCreateBy();
	if (pTexture->HasAnySlot())
	{
		SKMAC_ASSERT(0);
		return 0;
	}
	if (!LoadRenderTarget(pRenderTarget))
		return 0;
	m_uiCurRTWidth = m_uiScreenWidth;
	m_uiCurRTHeight = m_uiScreenHeight;
	m_uiCurRTMultisampler = m_uiCurMultisample;
	DisableTextureRTUsed(pTexture);
	if (m_uiBackMacthType == BMT_RENDER_TARGET)
	{
		m_uiBackMacthType = BMT_NONE;
	}
	return 1;
}
bool SKRenderer::SetDepthStencilBuffer(SKDepthStencil * pDepthStencilBuffer)
{
	if (!pDepthStencilBuffer)
		return 0;
	if (m_uiCurRTWidth != pDepthStencilBuffer->GetWidth()
		|| m_uiCurRTHeight != pDepthStencilBuffer->GetHeight()
		|| m_uiCurRTMultisampler != pDepthStencilBuffer->GetMulSample())
	{
		SKMAC_ASSERT(false);
		return false;	
	}
	if (!LoadDepthStencil(pDepthStencilBuffer))
		return 0;
	return 1;
}
bool SKRenderer::EndDepthStencilBuffer(SKDepthStencil * pDepthStencilBuffer)
{
	if (!pDepthStencilBuffer)
		return 0;

	if (!LoadDepthStencil(pDepthStencilBuffer))
		return 0;
	return 1;
}
unsigned int SKRenderer::SetVBuffer(SKVertexBuffer * pVBuffer)
{
	if (m_pVertexBuffer == pVBuffer)
	{
		if (!pVBuffer)
		{
			return FRI_SAMERESOURCE;
		}
		else if (pVBuffer  && pVBuffer->GetSwapChainNum() == 1)
		{
			return FRI_SAMERESOURCE;
		}
	}

	if (!pVBuffer)
	{
		m_pVertexBuffer = NULL;
		SetVertexFormat(NULL);
		return FRI_FAIL;
	}
	if (!LoadVBuffer(pVBuffer))
	{
		m_pVertexBuffer = NULL;
		SetVertexFormat(NULL);
		return FRI_FAIL;
	}
	SetVertexFormat(pVBuffer->GetVertexFormat());
	m_pVertexBuffer = pVBuffer;

	return FRI_SUCCESS;
}
unsigned int SKRenderer::SetVertexFormat(SKVertexFormat * pVertexFormat)
{
	if (m_pVertexFormat == pVertexFormat)
		return FRI_SAMERESOURCE;
	m_pVertexFormat = pVertexFormat;
	if (!pVertexFormat)
	{
		return FRI_FAIL;
	}
	return FRI_SUCCESS;
}
bool SKRenderer::CopyResourceBuffer(VS2DTexture * pSource, SKCubeTexture * pDest, unsigned int uiFace)
{
	if (!pSource || !pDest || uiFace >= SKCubeTexture::F_MAX)
	{
		return false;
	}
	if (pSource->GetWidth(0) != pSource->GetHeight(0) && pSource->GetWidth(0) != pDest->GetWidth(0))
	{
		return false;
	}
	if (pSource->GetMipLevel() != pDest->GetMipLevel())
	{
		return false;
	}
	if (pSource->GetFormatType() != pDest->GetFormatType())
	{
		return false;
	}
	return true;
}
unsigned int SKRenderer::SetIBuffer(SKIndexBuffer* pIBuffer)
{
	if (m_pIndexBuffer == pIBuffer)
	{
		if (!pIBuffer)
		{
			return FRI_SAMERESOURCE;
		}
		else if (pIBuffer && pIBuffer->GetSwapChainNum() == 1)
		{
			return FRI_SAMERESOURCE;
		}

	}
	if (!pIBuffer)
	{
		m_pIndexBuffer = NULL;
		return FRI_FAIL;
	}
	if (!LoadIBuffer(pIBuffer))
	{
		m_pIndexBuffer = NULL;
		return FRI_FAIL;
	}

	m_pIndexBuffer = pIBuffer;

	return FRI_SUCCESS;
}

bool SKRenderer::LoadVShaderProgram(SKVShader * pVShaderProgram)
{
	if (!pVShaderProgram)
		return 0;

	return pVShaderProgram->LoadResource(this);
}
bool SKRenderer::ReleaseVShaderProgram(SKVShader * pVShaderProgram)
{


	if (!pVShaderProgram)
		return 0;
	return pVShaderProgram->ReleaseResource();
}

bool SKRenderer::LoadPShaderProgram(SKPShader* pPShaderProgram)
{
	if (!pPShaderProgram)
		return 0;

	return pPShaderProgram->LoadResource(this);

}
bool SKRenderer::ReleasePShaderProgram(SKPShader* pPShaderProgram)
{

	if (!pPShaderProgram)
		return 0;

	return pPShaderProgram->ReleaseResource();

}
bool SKRenderer::LoadGShaderProgram(SKGShader * pGShaderProgram)
{
	if (!pGShaderProgram)
		return 0;

	return pGShaderProgram->LoadResource(this);
}
bool SKRenderer::ReleaseGShaderProgram(SKGShader * pGShaderProgram)
{
	if (!pGShaderProgram)
		return 0;

	return pGShaderProgram->ReleaseResource();
}
bool SKRenderer::LoadDShaderProgram(SKDShader * pDShaderProgram)
{
	if (!pDShaderProgram)
		return 0;

	return pDShaderProgram->LoadResource(this);
}
bool SKRenderer::ReleaseDShaderProgram(SKDShader * pDShaderProgram)
{
	if (!pDShaderProgram)
		return 0;

	return pDShaderProgram->ReleaseResource();
}
bool SKRenderer::LoadHShaderProgram(SKHShader * pHShaderProgram)
{
	if (!pHShaderProgram)
		return 0;

	return pHShaderProgram->LoadResource(this);
}
bool SKRenderer::ReleaseHShaderProgram(SKHShader * pHShaderProgram)
{
	if (!pHShaderProgram)
		return 0;

	return pHShaderProgram->ReleaseResource();
}
bool SKRenderer::LoadTexture(SKTexture * pTexture)
{
	if (!pTexture)
		return 0;

	return pTexture->LoadResource(this);
}
bool SKRenderer::ReleaseTexture(SKTexture * pTexture)
{

	if (!pTexture)
		return 0;

	return pTexture->ReleaseResource();

}
bool SKRenderer::LoadDepthStencil(SKDepthStencil * pDepthStencil)
{
	if (!pDepthStencil)
		return 0;

	return pDepthStencil->LoadResource(this);
}
bool SKRenderer::ReleaseDepthStencil(SKDepthStencil * pDepthStencil)
{
	if (!pDepthStencil)
		return 0;

	return pDepthStencil->ReleaseResource();
}
bool SKRenderer::LoadRenderTarget(SKRenderTarget  * pRenderTarget)
{
	if (!pRenderTarget)
	{
		return 0;
	}
	pRenderTarget->LoadResource(this);
	return 1;
}
bool SKRenderer::ReleaseRenderTarget(SKRenderTarget * pRenderTarget)
{
	if (!pRenderTarget)
	{
		return 0;
	}
	pRenderTarget->ReleaseResource();
	return 1;
}
bool SKRenderer::LoadBlendState(SKBlendState * pBlendState)
{
	if (!pBlendState)
	{
		return 0;
	}
	if (GetRendererType() == RAT_DIRECTX9)
	{
		return 1;
	}
	pBlendState->LoadResource(this);
	return 1;
}
bool SKRenderer::ReleaseBlendState(SKBlendState * pBlendState)
{
	if (!pBlendState)
	{
		return 0;
	}
	pBlendState->ReleaseResource();
	return 1;
}

bool SKRenderer::LoadDepthStencilState(SKDepthStencilState * pDepthStencilState)
{
	if (!pDepthStencilState)
	{
		return 0;
	}
	if (GetRendererType() == RAT_DIRECTX9)
	{
		return 1;
	}
	pDepthStencilState->LoadResource(this);
	return 1;
}
bool SKRenderer::ReleaseDepthStencilState(SKDepthStencilState * pDepthStencilState)
{
	if (!pDepthStencilState)
	{
		return 0;
	}
	pDepthStencilState->ReleaseResource();
	return 1;
}

bool SKRenderer::LoadRasterizerState(SKRasterizerState * pRasterizerState)
{
	if (!pRasterizerState)
	{
		return 0;
	}
	if (GetRendererType() == RAT_DIRECTX9)
	{
		return 1;
	}
	pRasterizerState->LoadResource(this);
	return 1;
}
bool SKRenderer::ReleaseRasterizerState(SKRasterizerState * pRasterizerState)
{
	if (!pRasterizerState)
	{
		return 0;
	}
	pRasterizerState->ReleaseResource();
	return 1;
}
bool SKRenderer::LoadSamplerState(SKSamplerState * pSamplerState)
{
	if (!pSamplerState)
	{
		return 0;
	}
	if (GetRendererType() == RAT_DIRECTX9)
	{
		return 1;
	}
	pSamplerState->LoadResource(this);
	return 1;
}
bool SKRenderer::ReleaseSamplerState(SKSamplerState * pSamplerState)
{
	if (!pSamplerState)
	{
		return 0;
	}
	pSamplerState->ReleaseResource();
	return 1;
}
bool SKRenderer::LoadVBuffer(SKVertexBuffer * pVBuffer)
{
	if (!pVBuffer)
		return 0;

	return pVBuffer->LoadResource(this);
}
bool SKRenderer::ReleaseVBuffer(SKVertexBuffer * pVBuffer)
{

	if (!pVBuffer)
		return 0;

	return pVBuffer->ReleaseResource();
}
bool SKRenderer::LoadIBuffer(SKIndexBuffer* pIBuffer)
{
	if (!pIBuffer)
		return 0;

	return pIBuffer->LoadResource(this);

}
bool SKRenderer::ReleaseIBuffer(SKIndexBuffer* pIBuffer)
{
	if (!pIBuffer)
		return 0;

	return pIBuffer->ReleaseResource();

}
unsigned int SKRenderer::SetVShader(SKVShader * pVShader)
{
	return FRI_SUCCESS;
}
unsigned int SKRenderer::SetPShader(SKPShader * pPShader)
{

	return FRI_SUCCESS;
}
unsigned int SKRenderer::SetGShader(SKGShader * pGShader)
{

	return FRI_SUCCESS;
}
unsigned int SKRenderer::SetDShader(SKDShader * pDShader)
{

	return FRI_SUCCESS;
}
unsigned int SKRenderer::SetHShader(SKHShader * pHShader)
{

	return FRI_SUCCESS;
}
unsigned int SKRenderer::SetMesh(SKMeshDate *pMeshDate)
{

	SKMAC_ASSERT(pMeshDate);
	if (!pMeshDate)
	{
		return FRI_FAIL;
	}


	if (!SetVBuffer(pMeshDate->GetVertexBuffer()))
		return 0;
	if (!SetIBuffer(pMeshDate->GetIndexBuffer()))
		return 0;
	return FRI_SUCCESS;

}
bool SKRenderer::DrawMesh(SKGeometry * pGeometry, SKRenderState * pRenderState, SKVShader * pVShader, SKPShader * pPShader, SKGShader * pGShader)
{

	if (!pGeometry || !pGeometry->GetMeshDate())
		return 0;

	m_LocalRenderState.GetAll(pRenderState);

	if (pGeometry->IsSwapCull())
	{
		m_LocalRenderState.SwapCull();
	}
	if (m_uiRenderStateInheritFlag)
	{

		m_LocalRenderState.Inherit(&m_UseState, m_uiRenderStateInheritFlag);


	}
	SetRenderState(m_LocalRenderState);
	if (!SetVShader(pVShader))
	{
		return 0;
	}
	SetGShader(pGShader);

	if (!SetPShader(pPShader))
	{
		return 0;
	}
	
	if (SetMesh(pGeometry->GetMeshDate()) == FRI_FAIL)
		return 0;

	return 1;
}
void SKRenderer::SetVTexAllState(SKTexAllState * pTexAllState, unsigned int i)
{
	if (i >= m_uiMaxVTexture)
		return;
	if (pTexAllState)
	{
		SetVTexture(pTexAllState->m_pTex, i);
		SetVSamplerState(pTexAllState->m_pSamplerState, i);
	}
	else
	{
		SetVTexture(NULL, i);
		SetVSamplerState(NULL, i);
	}
}
unsigned int SKRenderer::SetGSamplerState(SKSamplerState * pSamplerState, unsigned int i, bool bForceSet)
{
	if (!pSamplerState)
	{
		pSamplerState = (SKSamplerState *)SKSamplerState::GetDefault();
	}
	if (!m_pGSamplerState[i])
	{
		m_pGSamplerState[i] = (SKSamplerState *)SKSamplerState::GetDefault();
	}
	if (m_pGSamplerState[i] == pSamplerState && bForceSet == false)
	{
		return FRI_SAMERESOURCE;
	}
	if (!LoadSamplerState(pSamplerState))
	{
		return FRI_FAIL;
	}

	return FRI_SUCCESS;
}
unsigned int SKRenderer::SetDSamplerState(SKSamplerState * pSamplerState, unsigned int i, bool bForceSet)
{
	if (!pSamplerState)
	{
		pSamplerState = (SKSamplerState *)SKSamplerState::GetDefault();
	}
	if (!m_pDSamplerState[i])
	{
		m_pDSamplerState[i] = (SKSamplerState *)SKSamplerState::GetDefault();
	}
	if (m_pDSamplerState[i] == pSamplerState && bForceSet == false)
	{
		return FRI_SAMERESOURCE;
	}
	if (!LoadSamplerState(pSamplerState))
	{
		return FRI_FAIL;
	}

	return FRI_SUCCESS;
}
unsigned int SKRenderer::SetHSamplerState(SKSamplerState * pSamplerState, unsigned int i, bool bForceSet)
{
	if (!pSamplerState)
	{
		pSamplerState = (SKSamplerState *)SKSamplerState::GetDefault();
	}
	if (!m_pHSamplerState[i])
	{
		m_pHSamplerState[i] = (SKSamplerState *)SKSamplerState::GetDefault();
	}
	if (m_pHSamplerState[i] == pSamplerState && bForceSet == false)
	{
		return FRI_SAMERESOURCE;
	}
	if (!LoadSamplerState(pSamplerState))
	{
		return FRI_FAIL;
	}

	return FRI_SUCCESS;
}
unsigned int SKRenderer::SetVSamplerState(SKSamplerState * pSamplerState, unsigned int i, bool bForceSet)
{
	if (!pSamplerState)
	{
		pSamplerState = (SKSamplerState *)SKSamplerState::GetDefault();
	}
	if (!m_pVSamplerState[i])
	{
		m_pVSamplerState[i] = (SKSamplerState *)SKSamplerState::GetDefault();
	}
	if (m_pVSamplerState[i] == pSamplerState && bForceSet == false)
	{
		return FRI_SAMERESOURCE;
	}
	if (!LoadSamplerState(pSamplerState))
	{
		return FRI_FAIL;
	}

	return FRI_SUCCESS;
}
void SKRenderer::SetGTexAllState(SKTexAllState * pTexAllState, unsigned int i)
{
	SKMAC_ASSERT(i < TEXLEVEL);
	if (i >= m_uiMaxGTexture)
		return;
	if (pTexAllState)
	{
		SetGTexture(pTexAllState->m_pTex, i);
		SetGSamplerState(pTexAllState->m_pSamplerState, i);
	}
	else
	{
		SetGTexture(NULL, i);
		SetGSamplerState(NULL, i);
	}
}
void SKRenderer::SetDTexAllState(SKTexAllState * pTexAllState, unsigned int i)
{
	SKMAC_ASSERT(i < TEXLEVEL);
	if (i >= m_uiMaxDTexture)
		return;
	if (pTexAllState)
	{
		SetDTexture(pTexAllState->m_pTex, i);
		SetDSamplerState(pTexAllState->m_pSamplerState, i);
	}
	else
	{
		SetDTexture(NULL, i);
		SetDSamplerState(NULL, i);
	}
}
void SKRenderer::SetHTexAllState(SKTexAllState * pTexAllState, unsigned int i)
{
	SKMAC_ASSERT(i < TEXLEVEL);
	if (i >= m_uiMaxHTexture)
		return;
	if (pTexAllState)
	{
		SetHTexture(pTexAllState->m_pTex, i);
		SetHSamplerState(pTexAllState->m_pSamplerState, i);
	}
	else
	{
		SetHTexture(NULL, i);
		SetHSamplerState(NULL, i);
	}
}
void SKRenderer::SetTexAllState(SKTexAllState * pTexAllState, unsigned int i)
{
	SKMAC_ASSERT(i < TEXLEVEL);
	if (i >= m_uiMaxTexture)
		return;
	if (pTexAllState)
	{
		SetTexture(pTexAllState->m_pTex, i);
		SetSamplerState(pTexAllState->m_pSamplerState, i);
	}
	else
	{
		SetTexture(NULL, i);
		SetSamplerState(NULL, i);
	}
}
void SKRenderer::SetVSTextureRTTextureUsed(SKTexture * pTexture, unsigned int uiSlot, bool b)
{
	if (!pTexture)
	{
		return;
	}
	pTexture->m_VSTexSlot.Set(uiSlot, b);
}
void SKRenderer::SetPSTextureRTTextureUsed(SKTexture * pTexture, unsigned int uiSlot, bool b)
{
	if (!pTexture)
	{
		return;
	}
	pTexture->m_PSTexSlot.Set(uiSlot, b);
}
void SKRenderer::SetGSTextureRTTextureUsed(SKTexture * pTexture, unsigned int uiSlot, bool b)
{
	if (!pTexture)
	{
		return;
	}
	pTexture->m_GSTexSlot.Set(uiSlot, b);
}
void SKRenderer::SetDSTextureRTTextureUsed(SKTexture * pTexture, unsigned int uiSlot, bool b)
{
	if (!pTexture)
	{
		return;
	}
	pTexture->m_DSTexSlot.Set(uiSlot, b);
}
void SKRenderer::SetHSTextureRTTextureUsed(SKTexture * pTexture, unsigned int uiSlot, bool b)
{
	if (!pTexture)
	{
		return;
	}
	pTexture->m_HSTexSlot.Set(uiSlot, b);
}
bool SKRenderer::CheckIsTextureCanSet(SKTexture * pTexture)
{
	if (!pTexture)
	{
		return true;
	}
	if (pTexture->GetTexType() == SKTexture::TT_2D)
	{
		if (((VS2DTexture*)pTexture)->IsRenderTarget())
		{
			if (((VS2DTexture*)pTexture)->m_bRtUsed == true)
			{
				SKMAC_ASSERT(0);
				return false;
			}
			else
			{
				return true;
			}
		}
	}
	else if (pTexture->GetTexType() == SKTexture::TT_CUBE)
	{
		if (((SKCubeTexture*)pTexture)->m_bRtUsed == true)
		{
			SKMAC_ASSERT(0);
			return false;
		}
		else
		{
			return true;
		}
	}
	return true;
}
unsigned int SKRenderer::SetVTexture(SKTexture* pTexture, unsigned int i)
{

	if (!CheckIsTextureCanSet(pTexture))
	{
		return FRI_FAIL;
	}
	if (m_pVTex[i] == pTexture)
	{
		if (!pTexture)
		{
			return FRI_SAMERESOURCE;
		}
		else if (pTexture && pTexture->GetSwapChainNum() == 1)
		{
			return FRI_SAMERESOURCE;
		}
	}
	if (!pTexture)
	{
		SetVSTextureRTTextureUsed(m_pVTex[i],i,false);
		m_pVTex[i] = NULL;
		return FRI_FAIL;
	}
	if (!LoadTexture(pTexture))
	{
		m_pVTex[i] = NULL;
		return FRI_FAIL;
	}
	SetVSTextureRTTextureUsed(m_pVTex[i],i,false);
	SetVSTextureRTTextureUsed(pTexture,i,true);
	m_pVTex[i] = pTexture;
	return 1;
}
unsigned int SKRenderer::SetDTexture(SKTexture* pTexture, unsigned int i)
{

	SKMAC_ASSERT(i < TEXLEVEL);
	if (!CheckIsTextureCanSet(pTexture))
	{
		return FRI_FAIL;
	}
	if (m_pDTex[i] == pTexture)
	{
		if (!pTexture)
		{
			return FRI_SAMERESOURCE;
		}
		else if (pTexture && pTexture->GetSwapChainNum() == 1)
		{
			return FRI_SAMERESOURCE;
		}

	}
	if (!pTexture)
	{
		SetDSTextureRTTextureUsed(m_pDTex[i], i, false);
		m_pDTex[i] = NULL;
		return FRI_FAIL;
	}
	if (!LoadTexture(pTexture))
	{
		m_pDTex[i] = NULL;
		return FRI_FAIL;
	}

	SetDSTextureRTTextureUsed(m_pDTex[i], i, false);
	SetDSTextureRTTextureUsed(pTexture, i, true);

	m_pDTex[i] = pTexture;
	return 1;
}
unsigned int SKRenderer::SetHTexture(SKTexture* pTexture, unsigned int i)
{

	SKMAC_ASSERT(i < TEXLEVEL);
	if (!CheckIsTextureCanSet(pTexture))
	{
		return FRI_FAIL;
	}
	if (m_pHTex[i] == pTexture)
	{
		if (!pTexture)
		{
			return FRI_SAMERESOURCE;
		}
		else if (pTexture && pTexture->GetSwapChainNum() == 1)
		{
			return FRI_SAMERESOURCE;
		}

	}
	if (!pTexture)
	{
		SetHSTextureRTTextureUsed(m_pHTex[i], i, false);
		m_pHTex[i] = NULL;
		return FRI_FAIL;
	}
	if (!LoadTexture(pTexture))
	{
		m_pHTex[i] = NULL;
		return FRI_FAIL;
	}

	SetHSTextureRTTextureUsed(m_pHTex[i], i, false);
	SetHSTextureRTTextureUsed(pTexture, i, true);

	m_pHTex[i] = pTexture;
	return 1;
}
unsigned int SKRenderer::SetGTexture(SKTexture* pTexture, unsigned int i)
{

	SKMAC_ASSERT(i < TEXLEVEL);
	if (!CheckIsTextureCanSet(pTexture))
	{
		return FRI_FAIL;
	}
	if (m_pGTex[i] == pTexture)
	{
		if (!pTexture)
		{
			return FRI_SAMERESOURCE;
		}
		else if (pTexture && pTexture->GetSwapChainNum() == 1)
		{
			return FRI_SAMERESOURCE;
		}

	}
	if (!pTexture)
	{
		SetGSTextureRTTextureUsed(m_pGTex[i], i, false);
		m_pGTex[i] = NULL;
		return FRI_FAIL;
	}
	if (!LoadTexture(pTexture))
	{
		m_pGTex[i] = NULL;
		return FRI_FAIL;
	}

	SetGSTextureRTTextureUsed(m_pGTex[i], i, false);
	SetGSTextureRTTextureUsed(pTexture, i, true);

	m_pGTex[i] = pTexture;
	return 1;
}
unsigned int SKRenderer::SetTexture(SKTexture* pTexture, unsigned int i)
{

	SKMAC_ASSERT(i < TEXLEVEL);
	if (!CheckIsTextureCanSet(pTexture))
	{
		return FRI_FAIL;
	}
	if (m_pPTex[i] == pTexture)
	{
		if (!pTexture)
		{
			return FRI_SAMERESOURCE;
		}
		else if (pTexture && pTexture->GetSwapChainNum() == 1)
		{
			return FRI_SAMERESOURCE;
		}

	}
	if (!pTexture)
	{
		SetPSTextureRTTextureUsed(m_pPTex[i],i,false);
		m_pPTex[i] = NULL;
		return FRI_FAIL;
	}
	if (!LoadTexture(pTexture))
	{
		m_pPTex[i] = NULL;
		return FRI_FAIL;
	}

	SetPSTextureRTTextureUsed(m_pPTex[i],i,false);
	SetPSTextureRTTextureUsed(pTexture,i,true);
	
	m_pPTex[i] = pTexture;
	return 1;
}
unsigned int SKRenderer::SetSamplerState(SKSamplerState * pSamplerState, unsigned int i, bool bForceSet)
{
	if (!pSamplerState)
	{
		pSamplerState = (SKSamplerState *)SKSamplerState::GetDefault();
	}
	if (!m_pPSamplerState[i])
	{
		m_pPSamplerState[i] = (SKSamplerState *)SKSamplerState::GetDefault();
	}
	if (m_pPSamplerState[i] == pSamplerState && bForceSet == false)
	{
		return FRI_SAMERESOURCE;
	}
	if (!LoadSamplerState(pSamplerState))
	{
		return FRI_FAIL;
	}

	return FRI_SUCCESS;
}
unsigned int SKRenderer::SetBlendState(SKBlendState * pBlendState, bool bForceSet)
{
	if (!pBlendState)
	{
		pBlendState = (SKBlendState *)SKBlendState::GetDefault();
	}
	if (!m_pBlendState)
	{
		m_pBlendState = (SKBlendState *)SKBlendState::GetDefault();
	}
	if (m_pBlendState == pBlendState && !bForceSet)
	{
		return FRI_SAMERESOURCE;
	}
	if (!LoadBlendState(pBlendState))
	{
		return FRI_FAIL;
	}

	return FRI_SUCCESS;
}
unsigned int SKRenderer::SetDepthStencilState(SKDepthStencilState * pDepthStencilState, bool bForceSet)
{
	if (!pDepthStencilState)
	{
		pDepthStencilState = (SKDepthStencilState *)SKDepthStencilState::GetDefault();
	}
	if (!m_pDepthStencilState)
	{
		m_pDepthStencilState = (SKDepthStencilState *)SKDepthStencilState::GetDefault();
	}
	if (m_pDepthStencilState == pDepthStencilState && !bForceSet)
	{
		return FRI_SAMERESOURCE;
	}
	if (!LoadDepthStencilState(pDepthStencilState))
	{
		return FRI_FAIL;
	}

	return FRI_SUCCESS;
}
unsigned int SKRenderer::SetRasterizerState(SKRasterizerState * pRasterizerState, bool bForceSet)
{
	if (!pRasterizerState)
	{
		pRasterizerState = (SKRasterizerState *)SKRasterizerState::GetDefault();
	}
	if (!m_pRasterizerState)
	{
		m_pRasterizerState = (SKRasterizerState *)SKRasterizerState::GetDefault();
	}
	if (m_pRasterizerState == pRasterizerState && !bForceSet)
	{
		return FRI_SAMERESOURCE;
	}
	if (!LoadRasterizerState(pRasterizerState))
	{
		return FRI_FAIL;
	}

	return FRI_SUCCESS;
}
void SKRenderer::SetClipPlane(SKArray<SKPlane3> &Plane, bool bEnable)
{

}
void SKRenderer::SetScissorRect(SKArray<SKRect2> &Rect, bool bEnable)
{

}
SKRenderer::SKRenderer()
{

	m_pChildWindowInfo = NULL;
	m_ClearColor.Set(1.0f, 1.0f, 1.0f, 1.0f);
	m_fClearDepth = 1.0f;
	m_uiClearStencil = 0;

	m_iNumChildWindow = 0;
	m_iCurWindowID = -1;
	m_bIsRendering = false;



	m_pBlendState = (SKBlendState *)SKBlendState::GetDefault();
	m_pDepthStencilState = (SKDepthStencilState *)SKDepthStencilState::GetDefault();
	m_pRasterizerState = (SKRasterizerState *)SKRasterizerState::GetDefault();
	m_bClipPlaneEnable = false;
	m_bScissorRectEnable = false;
	m_pFont = NULL;//SKFont::Default;

	m_pVertexBuffer = NULL;
	m_pVertexFormat = NULL;
	m_pIndexBuffer = NULL;
	m_pVShader = NULL;
	m_pPshader = NULL;
	m_pGShader = NULL;
	m_pDShader = NULL;
	m_pHShader = NULL;
	m_pQuadVertexFormat = NULL;
	m_uiRenderStateInheritFlag = 0;
	SKRenderer::ms_pRenderer = this;
	m_uiBackMacthType = BMT_NONE;
}
bool SKRenderer::UseWindow(int uiWindowID)
{
	if (!m_bWindowed)
		return 0;
	else if (uiWindowID >= m_iNumChildWindow)
		return 0;
	else if (uiWindowID < -1)
	{
		return 0;
	}
	m_iCurWindowID = uiWindowID;
	return 1;
}
bool SKRenderer::BeginRendering()
{
	ClearBuffers();
	m_bIsRendering = true;
	m_FontVertex.Clear();
	return true;
}
bool SKRenderer::EndRendering()
{
	if (m_FontVertex.GetNum() > 0)
	{
		SetViewPort(NULL);
		SKFont * pUseFont = m_pFont->GetResource();
		SKCustomMaterial *pFontMaterial = SKCustomMaterial::GetFont();
		pFontMaterial->SetPShaderTexture(SKUsedName::ms_cPostInputTexture, pUseFont->GetTexAllState());
		pFontMaterial->UseMaterial();
		unsigned int uiLength = m_FontVertex.GetNum() >> 2;
		DrawScreenFont(m_FontVertex.GetBuffer(), uiLength * 4, m_FontIndex.GetBuffer(), uiLength * 2 * 3);
	}	
	m_bIsRendering = false;
	SKMAC_ASSERT(m_uiBackMacthType == BMT_NONE);
	m_uiBackMacthType = BMT_NONE;
	return true;
}
void SKRenderer::DeviceLost()
{

}
void SKRenderer::ResetDevice()
{

}
bool SKRenderer::ChangeScreenSize(unsigned int uiWidth, unsigned int uiHeight, bool bWindow)
{
	if (m_uiScreenHeight == uiHeight && m_uiScreenWidth == uiWidth && m_bWindowed == bWindow)
	{


		return false;
	}
	if (!bWindow)
	{
		unsigned int uiAdpterInfoNum = 0;
		const SKAdapterInfo * pAdapterInfo = GetAdapterInfo(uiAdpterInfoNum);
		if (!pAdapterInfo || !uiAdpterInfoNum)
		{
			return false;
		}
		bool bFlag = false;
		for (unsigned int i = 0; i < pAdapterInfo[0].DisplayMode.GetNum(); i++)
		{
			if (pAdapterInfo[0].DisplayMode[i].uiHeight == uiHeight
				&& pAdapterInfo[0].DisplayMode[i].uiWidth == uiWidth
				&& pAdapterInfo[0].DisplayMode[i].uiBitFormat == SKRenderer::SFT_A8R8G8B8)
			{
				bFlag = true;
				break;
			}
		}
		if (!bFlag)
		{
			return false;
		}
	}


	m_bWindowed = bWindow;
	m_uiScreenHeight = uiHeight;
	m_uiScreenWidth = uiWidth;
	m_uiCurRTWidth = uiWidth;
	m_uiCurRTHeight = uiHeight;
	return true;
}
bool SKRenderer::DrawScreen(SCREEN_QUAD_TYPE ScreenQuad[4])
{
	SetVertexFormat(m_pQuadVertexFormat);
	m_pVertexBuffer = NULL;
	m_pIndexBuffer = NULL;
	return 1;
}
bool SKRenderer::DrawScreen(SCREEN_QUAD_TYPE * pScreenBuffer, unsigned int uiVertexNum,
	SKUSHORT_INDEX * pIndexBuffer, unsigned int uiIndexNum)
{
	SKMAC_ASSERT(pScreenBuffer && uiVertexNum && pIndexBuffer && uiIndexNum);
	if (!pScreenBuffer || !uiVertexNum || !pIndexBuffer || !uiIndexNum)
	{
		return false;
	}
	SetVertexFormat(m_pQuadVertexFormat);
	m_pVertexBuffer = NULL;
	m_pIndexBuffer = NULL;
	return 1;
}
bool SKRenderer::DrawScreenFont(SCREEN_FONT_TYPE * pScreenFontBuffer, unsigned int uiVertexNum,
	SKUSHORT_INDEX * pIndexBuffer, unsigned int uiIndexNum)
{
	SKMAC_ASSERT(pScreenFontBuffer && uiVertexNum && pIndexBuffer && uiIndexNum);
	if (!pScreenFontBuffer || !uiVertexNum || !pIndexBuffer || !uiIndexNum)
	{
		return false;
	}
	SetVertexFormat(m_pQuadVertexFormat);
	m_pVertexBuffer = NULL;
	m_pIndexBuffer = NULL;
	return 1;
}
SKString SKRenderer::Float(unsigned int uiIndex)const
{
	if (uiIndex == FI_1)
	{
		return Float();
	}
	else if (uiIndex == FI_2)
	{
		return Float2();
	}
	else if (uiIndex == FI_3)
	{
		return Float3();
	}
	else if (uiIndex == FI_4)
	{
		return Float4();
	}
	else
		SKMAC_ASSERT(1);
	return SKString();


}
bool SKRenderer::SetViewPort(SKViewPort *pViewPort)
{
	if (!pViewPort)
	{
		m_CurViewPort.Reset();
	}
	return true;
}
void SKRenderer::DrawText(int iX, int iY, const DWORD rColor,
	const TCHAR* acText, ...)
{
	if (!acText || !m_pFont)
		return;
	SKFont * pUseFont = m_pFont->GetResource();
	TCHAR cch[1024];
	char *pArgs;
	pArgs = (char*)&acText + sizeof(acText);
	SKSprintf(cch, 1024, acText, pArgs);
	SKString String = cch;
	
	unsigned int uiLength = String.GetLength();
	unsigned int uiRTWidth = GetCurRTWidth();
	unsigned int uiRTHeight = GetCurRTHeight();
	
	SKREAL InvRTWidth = 1.0f / uiRTWidth;
	SKREAL InvRTHeight = 1.0f / uiRTHeight;
	SKREAL fLeft = iX  * InvRTWidth;
	SKREAL fTop = 1.0f - iY * InvRTHeight;

	fLeft = fLeft * 2.0f - 1.0f;
	fTop = fTop * 2.0f - 1.0f;

	SKREAL fOriginalLeft = fLeft;
	SKREAL fGlyphSizeY = pUseFont->GetTexAllState()->m_pTex->GetHeight(0) * InvRTHeight/* * 2.0f*/;

	SKREAL fTextureWidth = pUseFont->GetTexAllState()->m_pTex->GetWidth(0) * 1.0f;
	
	for (unsigned int i = 0; i < uiLength; i++)
	{
		if (String[i] < SKFont::ASCII_START || String[i] > SKFont::ASCII_END)
		{
			continue;
		}
		if (String[i] == '\n')
		{
			fLeft = fOriginalLeft;
			fTop -= fGlyphSizeY;
			continue;
		}
		unsigned char CharIndex = String[i] - SKFont::ASCII_START;

		SCREEN_FONT_TYPE Vertex[4];
		SKREAL Left = pUseFont->GetFontData(CharIndex);
		SKREAL Right = pUseFont->GetFontData(CharIndex + 1);
		float fGlyphSizeX = (Right - Left) * fTextureWidth * InvRTWidth /**2.0f*/;  // in pixels
		Vertex[0].Point = SKVector3(fLeft, fTop, 0.0f);
		Vertex[1].Point = SKVector3(fLeft + fGlyphSizeX, fTop, 0.0f);
		Vertex[2].Point = SKVector3(fLeft + fGlyphSizeX, fTop - fGlyphSizeY, 0.0f);
		Vertex[3].Point = SKVector3(fLeft, fTop - fGlyphSizeY, 0.0f);

		
		fLeft += fGlyphSizeX;

		Vertex[0].UV = SKVector2(Left,0.0f);
		Vertex[1].UV = SKVector2(Right, 0.0f);
		Vertex[2].UV = SKVector2(Right, 1.0f);
		Vertex[3].UV = SKVector2(Left, 1.0f);;

		Vertex[0].Color = rColor;
		Vertex[1].Color = rColor;
		Vertex[2].Color = rColor;
		Vertex[3].Color = rColor;

		m_FontVertex.AddElement(Vertex[0]);
		m_FontVertex.AddElement(Vertex[1]);
		m_FontVertex.AddElement(Vertex[2]);
		m_FontVertex.AddElement(Vertex[3]);
		
	}
	unsigned int IndexFontNum = m_FontIndex.GetNum() / 6;
	if (IndexFontNum < (m_FontVertex.GetNum() >> 2))
	{
		for (unsigned int i = IndexFontNum; i < (m_FontVertex.GetNum() >> 2); i++)
		{
			unsigned int Index = i * 4;
			m_FontIndex.AddElement(Index);
			m_FontIndex.AddElement(Index + 1);
			m_FontIndex.AddElement(Index + 3);

			m_FontIndex.AddElement(Index + 1);
			m_FontIndex.AddElement(Index + 2);
			m_FontIndex.AddElement(Index + 3);
		}
	}
	
}
void SKRenderer::SetFont(SKFontR * pFont)
{
	if (!pFont)
	{
		return;	
	}
	else
	{
		m_pFont = pFont;
	}
}