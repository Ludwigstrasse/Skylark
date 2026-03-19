#include "SKCaptureTexAllState.h"
#include "SKResourceManager.h"
#include "SKImage.h"
#include "SKGraphicInclude.h"
#include "SKSceneManager.h"
#include "SKRenderThread.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKCaptureTexAllState,SKTexAllState)
BEGIN_ADD_PROPERTY(SKCaptureTexAllState,SKTexAllState)
REGISTER_PROPERTY(m_ViewCaptureName,ViewCaptureName,SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKCaptureTexAllState)
IMPLEMENT_INITIAL_END

SKCaptureTexAllState::SKCaptureTexAllState()
{
	m_bDynamic = true;
	m_pStaticTexture = NULL;
	m_uiMipLevel = 1;
}
SKCaptureTexAllState::~SKCaptureTexAllState()
{
	m_pStaticTexture = NULL;
	if (SKSceneManager::ms_pSceneManager)
	{
		SKCaptureViewFamily * pCaptureViewFamily = DynamicCast<SKCaptureViewFamily>(SKSceneManager::ms_pSceneManager->GetViewFamily(m_ViewCaptureName));

		if (pCaptureViewFamily)
		{
			pCaptureViewFamily->m_pTexOwner = NULL;
		}
	}
	
	
	
}
bool SKCaptureTexAllState::SetViewCapture(const SKString & ViewCaptureName)
{
	
	m_ViewCaptureName = ViewCaptureName;

	SKCaptureViewFamily * pCaptureViewFamily = DynamicCast<SKCaptureViewFamily>(SKSceneManager::ms_pSceneManager->GetViewFamily(ViewCaptureName));

	if (!pCaptureViewFamily)
	{
		return false;
	}
	SKTexture * pTexture =  pCaptureViewFamily->GetTexture();

	SKMAC_ASSERT(pTexture);

	m_pTex = pTexture;
	ForceUpdate(!m_bDynamic);
	return true;
}
bool SKCaptureTexAllState::PostLoad(void * pDate)
{
	SKTexAllState::PostLoad(pDate);
	if (!m_pTex)
	{
		SetViewCapture(m_ViewCaptureName);
	}
	
	return true;
}
bool SKCaptureTexAllState::PostClone(SKObject * pObjectSrc)
{
	SKTexAllState::PostClone(pObjectSrc);
	SetViewCapture(m_ViewCaptureName);
	return true;
}
bool SKCaptureTexAllState::BeforeSave( void * pDate)
{
	SKTexAllState::BeforeSave(pDate);
	if (m_bDynamic)
	{
		m_pTex = NULL;
	}
	return true;
}
bool SKCaptureTexAllState::PostSave( void * pDate)
{
	SKTexAllState::PostSave(pDate);
	if (m_bDynamic)
	{
		SetViewCapture(m_ViewCaptureName);
	}
	return true;
}
void SKCaptureTexAllState::SetMipLevel(unsigned int uiMipLevel)
{
	m_uiMipLevel = uiMipLevel;
	ForceUpdate(!m_bDynamic);
}
SKCaptureViewFamily * SKCaptureTexAllState::GetViewFamily()
{
	SKCaptureViewFamily * pCaptureViewFamily = DynamicCast<SKCaptureViewFamily>(SKSceneManager::ms_pSceneManager->GetViewFamily(m_ViewCaptureName));


	if (!pCaptureViewFamily)
	{
		return NULL;
	}

	return pCaptureViewFamily;
}
void SKCaptureTexAllState::ForceUpdate(bool OnlyUpdateOneTime)
{
	SKCaptureViewFamily * pCaptureViewFamily = DynamicCast<SKCaptureViewFamily>(SKSceneManager::ms_pSceneManager->GetViewFamily(m_ViewCaptureName));


	if (!pCaptureViewFamily)
	{
		return ;
	}
	m_bDynamic = !OnlyUpdateOneTime;

	pCaptureViewFamily->m_OnlyUpdateOneTime = OnlyUpdateOneTime;
	pCaptureViewFamily->m_bEnable = true;
	pCaptureViewFamily->m_pTexOwner = this;

}
void SKCaptureTexAllState::CreateStaticTexture()
{
	SKCaptureViewFamily * pCaptureViewFamily = DynamicCast<SKCaptureViewFamily>(SKSceneManager::ms_pSceneManager->GetViewFamily(m_ViewCaptureName));
	SKMAC_ASSERT(pCaptureViewFamily);
	if (!m_bDynamic)
	{
		SKTexture * pTexture =  pCaptureViewFamily->GetTexture();
		unsigned int uiTextureType = pTexture->GetTexType();
		if (uiTextureType == SKTexture::TT_2D)
		{
			m_pStaticTexture = SK_NEW VS2DTexture(pTexture->GetWidth(0),pTexture->GetHeight(0),pTexture->GetFormatType(),m_uiMipLevel,1);
		}
		else if (uiTextureType == SKTexture::TT_CUBE)
		{
			m_pStaticTexture = SK_NEW SKCubeTexture(pTexture->GetWidth(0),pTexture->GetFormatType(),m_uiMipLevel,1);
		}
		m_pStaticTexture->CreateRAMDate();
		m_uiMipLevel = m_pStaticTexture->GetMipLevel();
		m_pTex = m_pStaticTexture;
	}
}
void SKCaptureTexAllState::NotifyEndDraw()
{
	CreateStaticTexture();
	SKCaptureViewFamily * pCaptureViewFamily = DynamicCast<SKCaptureViewFamily>(SKSceneManager::ms_pSceneManager->GetViewFamily(m_ViewCaptureName));
	SKMAC_ASSERT(pCaptureViewFamily);
	if (!m_bDynamic)
	{
		SKTexture * pSourceTexture =  pCaptureViewFamily->GetTexture();
		
		if (pSourceTexture->GetTexType() == SKTexture::TT_2D)
		{
			SKTexture * pDestTexture = m_pStaticTexture;
			ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(SKUpdate2DTextureMipCommand,
				SKTexture *,pSourceTexture,pSourceTexture,SKTexture *,pDestTexture,pDestTexture,
			{
				void * SourceBuffer = pSourceTexture->Lock(0);
				unsigned char * pDestBuffer = pDestTexture->GetBuffer(0);
				SKMemcpy((void *) pDestBuffer,SourceBuffer,pSourceTexture->GetByteSize(0));
				pSourceTexture->UnLock(0);

				unsigned char * pLast = pDestBuffer;
				for (unsigned int i = 1 ; i < pDestTexture->GetMipLevel() ; i++)
				{
					unsigned char * pNow = (unsigned char *)pDestTexture->GetBuffer(i);

					SKResourceManager::GetNextMipDate(pLast,pDestTexture->GetWidth(i - 1),pDestTexture->GetHeight(i - 1),pNow,pDestTexture->GetChannelPerPixel());
		
					pLast = (unsigned char *)pDestTexture->GetBuffer(i);
				}
			})
				void * SourceBuffer = pSourceTexture->Lock(0);
				unsigned char * pDestBuffer = pDestTexture->GetBuffer(0);
				SKMemcpy((void *) pDestBuffer,SourceBuffer,pSourceTexture->GetByteSize(0));
				pSourceTexture->UnLock(0);

				unsigned char * pLast = pDestBuffer;
				for (unsigned int i = 1 ; i < pDestTexture->GetMipLevel() ; i++)
				{
					unsigned char * pNow = (unsigned char *)pDestTexture->GetBuffer(i);

					SKResourceManager::GetNextMipDate(pLast,pDestTexture->GetWidth(i - 1),pDestTexture->GetHeight(i - 1),pNow,pDestTexture->GetChannelPerPixel());

					pLast = (unsigned char *)pDestTexture->GetBuffer(i);
				}
				
			ENQUEUE_UNIQUE_RENDER_COMMAND_END
		}
		else if (pSourceTexture->GetTexType() == SKTexture::TT_CUBE)
		{
			SKTexture * Temp = m_pStaticTexture;
			SKCubeTexture * pDestTexture = (SKCubeTexture *)Temp;
			ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(SKUpdateCubeTextureMipCommand,
				SKTexture *,pSourceTexture,pSourceTexture,SKCubeTexture *,pDestTexture,pDestTexture,
			{
				for (unsigned int i = 0 ; i < SKCubeTexture::F_MAX ; i++)
				{
					void * SourceBuffer = pSourceTexture->Lock(0,i);
					unsigned char * pDestBuffer = pDestTexture->GetFaceBuffer(0,i);
					SKMemcpy((void *) pDestBuffer,SourceBuffer,pDestTexture->GetFaceByteSize(0));
					pSourceTexture->UnLock(0,i);
				}
				
				for (unsigned int s = 0 ; s < SKCubeTexture::F_MAX ; s++)
				{
					unsigned char * pLast = pDestTexture->GetFaceBuffer(0,s);

					for (unsigned int i = 1 ; i < pDestTexture->GetMipLevel() ; i++)
					{
						unsigned char * pNow = (unsigned char *)pDestTexture->GetFaceBuffer(i,s);

						SKResourceManager::GetNextMipDate(pLast,pDestTexture->GetWidth(i - 1),pDestTexture->GetHeight(i - 1),pNow,pDestTexture->GetChannelPerPixel());

						pLast = (unsigned char *)pDestTexture->GetFaceBuffer(i,s);
					}
				}
			})
				for (unsigned int i = 0 ; i < SKCubeTexture::F_MAX ; i++)
				{
					void * SourceBuffer = pSourceTexture->Lock(0,i);
					unsigned char * pDestBuffer = pDestTexture->GetFaceBuffer(0,i);
					SKMemcpy((void *) pDestBuffer,SourceBuffer,pDestTexture->GetFaceByteSize(0));
					pSourceTexture->UnLock(0,i);
				}

				for (unsigned int s = 0 ; s < SKCubeTexture::F_MAX ; s++)
				{
					unsigned char * pLast = pDestTexture->GetFaceBuffer(0,s);

					for (unsigned int i = 1 ; i < pDestTexture->GetMipLevel() ; i++)
					{
						unsigned char * pNow = (unsigned char *)pDestTexture->GetFaceBuffer(i,s);

						SKResourceManager::GetNextMipDate(pLast,pDestTexture->GetWidth(i - 1),pDestTexture->GetHeight(i - 1),pNow,pDestTexture->GetChannelPerPixel());

						pLast = (unsigned char *)pDestTexture->GetFaceBuffer(i,s);
					}
				}

			ENQUEUE_UNIQUE_RENDER_COMMAND_END
		}

		
	}
}