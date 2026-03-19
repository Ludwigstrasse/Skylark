#include "SKRenderTarget.h"
#include "SKVertexFormat.h"
#include "SKGraphicInclude.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKRenderTarget,SKBind)
BEGIN_ADD_PROPERTY(SKRenderTarget,SKBind)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKRenderTarget)
IMPLEMENT_INITIAL_END
SKRenderTarget::SKRenderTarget()
{
	m_uiWidth = 512;
	m_uiHeight = 512;
	m_uiFormatType = SKRenderer::SFT_A8R8G8B8;
	m_uiMulSample = SKRenderer::MS_NONE;
	m_pCreateBy = NULL;
	m_bUsed = false;
}
SKRenderTarget::SKRenderTarget(unsigned int uiWidth, unsigned int uiHeight,
							   unsigned int uiFormatType, unsigned int uiMulSample)
{
	m_uiWidth = uiWidth;
	m_uiHeight = uiHeight;
	m_uiFormatType = uiFormatType;
	m_uiMulSample = uiMulSample;
	m_pCreateBy = NULL;
	m_uiLevel = 0;
	m_uiParam = 0;
	m_bUsed = false;
}
SKRenderTarget::SKRenderTarget(SKTexture * pCreateBy,unsigned int uiMulSample,unsigned int uiLevel,unsigned int Param)
{
	if (pCreateBy->GetTexType() == SKTexture::TT_2D && ((VS2DTexture *)pCreateBy)->IsRenderTarget())
	{
		SKMAC_ASSERT(((VS2DTexture *)pCreateBy)->IsRenderTarget());
		((VS2DTexture *)pCreateBy)->m_pCurRenderTarget = this;
	}
	else if (pCreateBy->GetTexType() == SKTexture::TT_CUBE && ((SKCubeTexture *)pCreateBy)->IsRenderTarget())
	{
		SKMAC_ASSERT(((VS2DTexture *)pCreateBy)->IsRenderTarget());
		((SKCubeTexture *)pCreateBy)->m_pCurRenderTarget[Param] = this;
	}
	else
	{
		SKMAC_ASSERT(0);
	}
	m_uiFormatType = pCreateBy->GetFormatType();
	m_uiMulSample = uiMulSample;
	m_pCreateBy = pCreateBy;
	m_uiLevel = uiLevel;
	m_uiParam = Param;
	m_uiWidth = m_pCreateBy->GetWidth(m_uiLevel);
	m_uiHeight = m_pCreateBy->GetHeight(m_uiLevel);
	m_bUsed = false;
}
SKRenderTarget::~SKRenderTarget()
{
	ReleaseResource();
	m_pCreateBy = NULL;
}
bool SKRenderTarget::OnLoadResource(SKResourceIdentifier *&pID)
{
	if(!m_pUser)
		return 0;
	if(!m_pUser->OnLoadRenderTarget(this,pID))
		return 0;
	return 1;
}
bool SKRenderTarget::OnReleaseResource(SKResourceIdentifier *pID)
{
	if(!m_pUser)
		return 0;
	if(!m_pUser->OnReleaseRenderTarget(pID))
		return 0;
	return 1;
}