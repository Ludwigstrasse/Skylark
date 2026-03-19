#include "SKTexture.h"
#include "SKShaderStringFactory.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI_NoCreateFun(SKTexture,SKBind)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(SKTexture)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(SKTexture,SKBind)
REGISTER_PROPERTY(m_uiFormatType,FormatType,SKProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiWidth,Width,SKProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiHeight,Height,SKProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiLenght,Lenght,SKProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiMipLevel,MipLevel,SKProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY_DATA(m_pBufferArray[0], m_BufferSize[0],BufferArray0);
REGISTER_PROPERTY_DATA(m_pBufferArray[1], m_BufferSize[1], BufferArray1);
REGISTER_PROPERTY_DATA(m_pBufferArray[2], m_BufferSize[2], BufferArray2);
REGISTER_PROPERTY_DATA(m_pBufferArray[3], m_BufferSize[3], BufferArray3);
REGISTER_PROPERTY_DATA(m_pBufferArray[4], m_BufferSize[4], BufferArray4);
REGISTER_PROPERTY_DATA(m_pBufferArray[5], m_BufferSize[5], BufferArray5);
REGISTER_PROPERTY_DATA(m_pBufferArray[6], m_BufferSize[6], BufferArray6);
REGISTER_PROPERTY_DATA(m_pBufferArray[7], m_BufferSize[7], BufferArray7);
REGISTER_PROPERTY_DATA(m_pBufferArray[8], m_BufferSize[8], BufferArray8);
REGISTER_PROPERTY_DATA(m_pBufferArray[9], m_BufferSize[9], BufferArray9);
REGISTER_PROPERTY_DATA(m_pBufferArray[10], m_BufferSize[10], BufferArray10);
REGISTER_PROPERTY_DATA(m_pBufferArray[11], m_BufferSize[11], BufferArray11);
REGISTER_PROPERTY_DATA(m_pBufferArray[12], m_BufferSize[12], BufferArray12);
REGISTER_PROPERTY_DATA(m_pBufferArray[13], m_BufferSize[13], BufferArray13);
END_ADD_PROPERTY
SKTexture::SKTexture(unsigned int uiFormatType,unsigned int uiWidth,
					 unsigned int uiHeight,unsigned int uiLength,unsigned int uiMipLevel,bool bIsStatic)
{
	SKMAC_ASSERT(uiWidth && uiHeight);
	SKMAC_ASSERT(uiFormatType < SKRenderer::SFT_MAX);
	m_uiFormatType = uiFormatType;
	m_uiHeight = uiHeight;
	m_uiWidth = uiWidth;
	m_uiLenght = uiLength;
	for (unsigned int i = 0 ; i < MAX_MIP_LEVEL ; i++)
	{
		for (unsigned int j = 0 ; j < 6 ; j++)
		{
			m_pLockDate[i][j] = NULL;
		}
	}
	for (unsigned int i = 0; i < MAX_MIP_LEVEL; i++)
	{
		m_pBufferArray[i] = NULL;
		m_BufferSize[i] = 0;
	}
	m_bIsStatic = bIsStatic;
	m_uiMipLevel = uiMipLevel;
	m_bSRGB = false;
	
}
SKTexture::SKTexture()
{
	for (unsigned int i = 0 ; i < MAX_MIP_LEVEL ; i++)
	{
		for (unsigned int j = 0 ; j < 6 ; j++)
		{
			m_pLockDate[i][j] = NULL;
		}
	}
	for (unsigned int i = 0; i < MAX_MIP_LEVEL; i++)
	{
		m_pBufferArray[i] = NULL;
		m_BufferSize[i] = 0;
	}
	m_bSRGB = false;
}
SKTexture::~SKTexture()
{
	ReleaseResource();
	for (unsigned int i = 0; i < MAX_MIP_LEVEL; i++)
	{
		SKMAC_DELETEA(m_pBufferArray[i]);
		m_BufferSize[i] = 0;
	}
}

void SKTexture::ClearInfo()
{
	if(m_uiSwapChainNum == m_InfoArray.GetNum())
	{
		if (m_uiMemType == MT_VRAM)
		{
			for (unsigned int i = 0 ; i < MAX_MIP_LEVEL ; i++)
			{
				SKMAC_DELETEA(m_pBufferArray[i]);
				m_BufferSize[i] = 0;
			}
		}
	}

}
bool SKTexture::LoadResource(SKRenderer * pRender)
{
	if (!SKBind::LoadResource(pRender))
	{
		return false;
	}

	return true;
}
bool SKTexture::OnLoadResource(SKResourceIdentifier *&pID)
{
	if(!m_pUser)
		return 0;
	if(!m_pUser->OnLoadTexture(this,pID))
		return 0;
	return 1;

}
bool SKTexture::OnReleaseResource(SKResourceIdentifier *pID)
{
	if(!m_pUser)
		return 0;
	if(!m_pUser->OnReleaseTexture(pID))
		return 0;
	return 1;
}
void *SKTexture::Lock(unsigned int uiLevel,unsigned int uiFace)
{
	if (m_pLockDate[uiLevel][uiFace] || m_bIsStatic)
	{
		return NULL;
	}
	if (m_pUser)
	{
		m_pLockDate[uiLevel][uiFace] = m_pUser->Lock(this,uiLevel,uiFace);
	}

	return m_pLockDate[uiLevel][uiFace];
}
void SKTexture::UnLock(unsigned int uiLevel,unsigned int uiFace)
{
	if (!m_pLockDate[uiLevel][uiFace] || m_bIsStatic)
	{
		return;
	}
	if (m_pUser)
	{
		m_pUser->UnLock(this,uiLevel,uiFace);
	}
	m_pLockDate[uiLevel][uiFace] = NULL;
}
void SKTexture::CreateRAMDate()
{
	for (unsigned int i = 0 ; i < m_uiMipLevel ; i++)
	{
		SKMAC_DELETEA(m_pBufferArray[i]);
		m_BufferSize[i] = GetByteSize(i);
		if (m_BufferSize[i] == 0)
		{
			continue;
		}
		m_pBufferArray[i] = SK_NEW unsigned char[m_BufferSize[i]];
	}
}
unsigned char * SKTexture::GetBuffer(unsigned int uiLevel, unsigned int i)const
{
	if (IsCompress())
	{
		return NULL;
	}
	return m_pBufferArray[uiLevel] + i * SKRenderer::GetBytesPerPixel(m_uiFormatType);
}
unsigned int SKTexture::GetByteSize(unsigned int uiLevel)const
{
	if (uiLevel > m_uiMipLevel)
	{
		return 0;
	}
	if (IsCompress())
	{
		return SKRenderer::GetBytesPerPixel(m_uiFormatType)
			* SKRenderer::ms_pRenderer->GetTextureCompressLen(GetWidth(uiLevel))
			* SKRenderer::ms_pRenderer->GetTextureCompressLen(GetHeight(uiLevel))
			* SKRenderer::ms_pRenderer->GetTextureCompressLen(GetLength(uiLevel));
	}
	return SKRenderer::GetBytesPerPixel(m_uiFormatType) * GetWidth(uiLevel) * GetHeight(uiLevel) * GetLength(uiLevel);
}
unsigned int SKTexture::GetMemPitch(unsigned int uiLevel)const
{
	if (uiLevel > m_uiMipLevel)
	{
		return 0;
	}
	if (IsCompress())
	{
		return SKRenderer::GetBytesPerPixel(m_uiFormatType)
			* SKRenderer::ms_pRenderer->GetTextureCompressLen(GetWidth(uiLevel));
	}
	return SKRenderer::GetBytesPerPixel(m_uiFormatType) * GetWidth(uiLevel);
}
unsigned int SKTexture::GetMemSlicePitch(unsigned int uiLevel)const
{
	if (uiLevel > m_uiMipLevel)
	{
		return 0;
	}
	if (IsCompress())
	{
		return SKRenderer::GetBytesPerPixel(m_uiFormatType)
			* SKRenderer::ms_pRenderer->GetTextureCompressLen(GetWidth(uiLevel))
			* SKRenderer::ms_pRenderer->GetTextureCompressLen(GetHeight(uiLevel));
	}
	return SKRenderer::GetBytesPerPixel(m_uiFormatType) * GetWidth(uiLevel) * GetHeight(uiLevel);
}
unsigned int SKTexture::GetWidth(unsigned int uiLevel)const
{
	if (uiLevel > m_uiMipLevel)
	{
		return 0;
	}
	unsigned int uiWidth = m_uiWidth >> uiLevel;
	if (!uiWidth)
	{
		uiWidth = 1;
	}
	return uiWidth;
}
unsigned int SKTexture::GetHeight(unsigned int uiLevel)const
{
	if (uiLevel > m_uiMipLevel)
	{
		return 0;
	}

	unsigned int uiHeight = m_uiHeight >> uiLevel;
	if (!uiHeight)
	{
		uiHeight = 1;
	}

	return uiHeight;

}
unsigned int SKTexture::GetLength(unsigned int uiLevel)const
{
	if (uiLevel > m_uiMipLevel)
	{
		return 0;
	}
	unsigned int uiLength = m_uiLenght >> uiLevel;
	if (!uiLength)
	{
		uiLength = 1;
	}
	return uiLength;
}
void SKTexture::ClearAllSlot()
{
	
	for (unsigned int i = 0; i < 16; i++)
	{
		if (m_VSTexSlot[i])
		{
			SKRenderer::ms_pRenderer->SetVTexture(NULL, i);
		}
		if (m_PSTexSlot[i])
		{
			SKRenderer::ms_pRenderer->SetTexture(NULL, i);
		}
		if (m_GSTexSlot[i])
		{
			SKRenderer::ms_pRenderer->SetGTexture(NULL, i);
		}
		if (m_DSTexSlot[i])
		{
			SKRenderer::ms_pRenderer->SetDTexture(NULL, i);
		}
		if (m_HSTexSlot[i])
		{
			SKRenderer::ms_pRenderer->SetHTexture(NULL, i);
		}
	}
	m_VSTexSlot.ClearAll();
	m_PSTexSlot.ClearAll();
	m_GSTexSlot.ClearAll();
	m_DSTexSlot.ClearAll();
	m_HSTexSlot.ClearAll();

}
bool SKTexture::HasAnySlot()
{
	return m_VSTexSlot.HasValue() || m_PSTexSlot.HasValue() || m_GSTexSlot.HasValue() || m_DSTexSlot.HasValue() || m_HSTexSlot.HasValue();
}