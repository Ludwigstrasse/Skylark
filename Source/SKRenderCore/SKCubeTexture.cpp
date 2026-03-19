#include "SKCubeTexture.h"
#include "SKGraphicInclude.h"
#include "SKRenderTarget.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKCubeTexture,SKTexture)
BEGIN_ADD_PROPERTY(SKCubeTexture,SKTexture)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKCubeTexture)
IMPLEMENT_INITIAL_END
SKCubeTexture::SKCubeTexture(unsigned int uiWidth,unsigned int uiFormatType,
							 unsigned int uiMipLevel,bool bIsStatic)
						 :SKTexture(uiFormatType,uiWidth,uiWidth,1,uiMipLevel,bIsStatic)
{
	
	SetMipLevel();
	m_bRenderTarget = false;
	m_bRtUsed = false;
	for (unsigned int i = 0; i < F_MAX; i++)
	{
		m_pCurRenderTarget[i] = NULL;
	}
}
SKCubeTexture::SKCubeTexture(VS2DTexture * pTexture[SKCubeTexture::F_MAX])
{
	m_bRenderTarget = false;
	bool bFind = false;
	unsigned int uiWidth = 0;
	unsigned int uiHeight = 0;
	unsigned int uiFormat = 0;
	unsigned int uiMipLevel = 0;
	bool bIsRenderTarge = false;
	bool bStatic = true;
	for (unsigned int i = 0 ; i < SKCubeTexture::F_MAX ;i++)
	{
		if (pTexture[i])
		{
			if (!bFind)
			{
				uiWidth = pTexture[i]->GetWidth(0);
				uiHeight = pTexture[i]->GetHeight(0);
				SKMAC_ASSERT(uiWidth == uiHeight);
				uiFormat = pTexture[i]->GetFormatType();
				bIsRenderTarge = pTexture[i]->IsRenderTarget();
				uiMipLevel = pTexture[i]->GetMipLevel();
				bStatic = pTexture[i]->IsStatic();
				bFind = true;

				
			}
			else
			{
				if (uiWidth != pTexture[i]->GetWidth(0) 
					|| uiFormat != pTexture[i]->GetFormatType() 
					|| bIsRenderTarge != pTexture[i]->IsRenderTarget()
					|| uiMipLevel != pTexture[i]->GetMipLevel()
					|| bStatic != pTexture[i]->IsStatic())
				{
					SKMAC_ASSERT(0);
				}
			}
					
		}
	}

	

	SKMAC_ASSERT(uiWidth);
	SKMAC_ASSERT(uiFormat < SKRenderer::SFT_MAX);
	m_uiFormatType = uiFormat;
	m_uiHeight = uiWidth;
	m_uiWidth = uiWidth;
	m_uiLenght = 1;


	m_bIsStatic = true;
	m_uiMipLevel = uiMipLevel;

	if (!bIsRenderTarge)
	{		
		SetMipLevel();

		CreateRAMDate();
		for (unsigned int i = 0 ; i < F_MAX ;i++)
		{
			if (pTexture[i])
			{
				for (unsigned int j = 0 ; j < m_uiMipLevel ; j++)
				{
					SKMemcpy(GetFaceBuffer(j,i),pTexture[i]->GetBuffer(j),pTexture[i]->GetByteSize(j));
				}
				
			}
			else
			{
				for (unsigned int j = 0 ; j < m_uiMipLevel ; j++)
				{
					SKMemset(GetFaceBuffer(j,i),0,GetFaceByteSize(j));
				}
			}
		}
		
		
	}
	else
	{
		SKMAC_ASSERT(0);
	}
	m_bRtUsed = false;
	for (unsigned int i = 0; i < F_MAX; i++)
	{
		m_pCurRenderTarget[i] = NULL;
	}
}
SKCubeTexture::SKCubeTexture(unsigned int uiWidth,unsigned int uiFormatType,unsigned int uiMipLevel)
:SKTexture(uiFormatType,uiWidth,uiWidth,1,uiMipLevel,true)
{

	m_bRenderTarget = true;
	m_bRtUsed = false;
	for (unsigned int i = 0; i < F_MAX; i++)
	{
		m_pCurRenderTarget[i] = NULL;
	}
}
SKCubeTexture::~SKCubeTexture()
{

}
SKCubeTexture::SKCubeTexture()
{

}
unsigned int SKCubeTexture::GetByteSize(unsigned int uiLevel)const
{
	return GetFaceByteSize(uiLevel) * F_MAX;
}
unsigned char * SKCubeTexture::GetFaceBuffer(unsigned int uiLevel,unsigned int uiFace)const
{
	if(uiLevel < m_uiMipLevel && uiFace < F_MAX)
	{
		if (IsCompress())
		{
			return GetBuffer(uiLevel) + SKRenderer::ms_pRenderer->GetTextureCompressLen(GetWidth(uiLevel)) 
					* SKRenderer::ms_pRenderer->GetTextureCompressLen(GetHeight(uiLevel)) 
					* uiFace * SKRenderer::GetBytesPerPixel(m_uiFormatType);
		}
		else
		{
			return GetBuffer(uiLevel) + GetWidth(uiLevel) * GetHeight(uiLevel) * uiFace * SKRenderer::GetBytesPerPixel(m_uiFormatType);
		}
		
	}
	else
		return NULL;
}
void SKCubeTexture::SetMipLevel()
{
	SKMAC_ASSERT(m_uiWidth && m_uiHeight);
	SKMAC_ASSERT(IsTwoPower(m_uiWidth));
	unsigned int uiWidthLevel;
	unsigned int uiWidth = m_uiWidth;
	_asm bsr eax, uiWidth
	_asm mov uiWidthLevel, eax


	unsigned int uiCurMipLevel = uiWidthLevel + 1;
	if (!m_uiMipLevel || uiCurMipLevel < m_uiMipLevel)
	{
		m_uiMipLevel = uiCurMipLevel;
	}
	
}