#include "SKDepthStencil.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI_NoCreateFun(SKDepthStencil,SKBind)
BEGIN_ADD_PROPERTY(SKDepthStencil,SKBind)
REGISTER_PROPERTY(m_uiWidth,Width,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiHeight,Height,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiMulSample,MulSample,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiFormatType,FormatType,SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(SKDepthStencil)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
SKDepthStencil::SKDepthStencil()
{
	m_bUsed = false;
}
SKDepthStencil::SKDepthStencil(unsigned int uiWidth,unsigned int uiHeight,
							   unsigned int uiMulSample,unsigned int uiFormatType)
{
	m_uiWidth = uiWidth;
	m_uiHeight = uiHeight;
	SKMAC_ASSERT(uiMulSample < SKRenderer::MS_MAX)
	SKMAC_ASSERT(uiFormatType < SKRenderer::SFT_MAX)
	m_uiMulSample = uiMulSample;
	m_uiFormatType = uiFormatType;
	m_bIsStatic = 1;
	m_bUsed = false;
}
SKDepthStencil::~SKDepthStencil()
{
	ReleaseResource();
}
bool SKDepthStencil::OnLoadResource(SKResourceIdentifier *&pID)
{
	if(!m_pUser)
		return 0;
	if(!m_pUser->OnLoadDepthStencil (this,pID))
		return 0;
	return 1;
}
bool SKDepthStencil::OnReleaseResource(SKResourceIdentifier *pID)
{
	if(!m_pUser)
		return 0;
	if(!m_pUser->OnReleaseDepthStencil(pID))
		return 0;
	return 1;
}