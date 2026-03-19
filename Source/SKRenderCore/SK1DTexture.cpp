#include "SK1DTexture.h"
#include "SKGraphicInclude.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(VS1DTexture,SKTexture)
BEGIN_ADD_PROPERTY(VS1DTexture,SKTexture)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(VS1DTexture)
IMPLEMENT_INITIAL_END
bool VS1DTexture:: SaveToFile(TCHAR * pSaveName)
{

	if(!m_uiHeight || !m_uiWidth)
		return 0;

	return 1;
}
VS1DTexture::VS1DTexture(unsigned int uiWidth,unsigned int uiFormatType,
						 unsigned int uiMipLevel,bool bIsStatic)
						 :SKTexture(uiFormatType,uiWidth,1,1,uiMipLevel,bIsStatic)
{

	SetMipLevel();

	
}
VS1DTexture::~VS1DTexture()
{

}
VS1DTexture::VS1DTexture()
{

}
void VS1DTexture::SetMipLevel()
{
	SKMAC_ASSERT(m_uiWidth && m_uiHeight);
	SKMAC_ASSERT(IsTwoPower(m_uiWidth));
	unsigned int uiWidthLevel;
	unsigned int uiWidth = m_uiWidth;
	_asm bsr eax, uiWidth
	_asm mov uiWidthLevel, eax


	unsigned int uiCurMipLevel = uiWidthLevel + 1;
	if (!m_uiMipLevel || m_uiMipLevel > uiCurMipLevel)
	{
		m_uiMipLevel = uiCurMipLevel;
	}

}