#include "SKASYNLoader.h"
#include "SKGraphicInclude.h"
#include "SKImage.h"
#include "SK2DTexture.h"
#include "SKNVCompression.h"
#include "SKTimer.h"
#include "SKProfiler.h"
using namespace SKEngine2;						
DECLEAR_TIME_PROFILENODE(SKASYNLoadManagerUpdate, ApplicationUpdate)
SKResourceJob::SKResourceJob()
{
	m_uiLoadState = LS_PENDING;
	m_uiJobType = JT_TEXTURE;
	m_uiJobState = JS_NONE;
}
SKResourceJob::~SKResourceJob()
{

}
SKFileRJob::SKFileRJob()
{
	m_pBuffer = NULL;
}
SKFileRJob::~SKFileRJob()
{
	SKMAC_DELETEA(m_pBuffer);
}
bool SKFileRJob::Load()
{
	SKFile File;
	if(!File.Open(m_FileName.GetBuffer(),SKFile::OM_RB))
	{
		m_uiLoadState = SKResourceJob::LS_FAIL;
		return 0;
	}

	m_uiSize = File.GetFileSize();
	m_pBuffer = SK_NEW unsigned char[m_uiSize];
	if(!m_pBuffer)
	{
		m_uiLoadState = SKResourceJob::LS_FAIL;
		return 0;
	}
	if(!File.Read(m_pBuffer,m_uiSize,1))
	{
		m_uiLoadState = SKResourceJob::LS_FAIL;
		return 0;
	}
	m_uiLoadState = SKResourceJob::LS_LOADED;

	return true;
}
SKTextureRJob::SKTextureRJob(SKTexAllStateRPtr& pProxy)
{
	m_pProxy = pProxy;

	m_bEngineType = true;
	m_bCompress = 0;
	m_uiCompressType = 0;
	m_bIsNormal = false;
	m_bSRGB = false;
	m_pSamplerState = NULL;
	m_uiJobType = JT_TEXTURE;
}
SKTextureRJob::~SKTextureRJob()
{
	m_pProxy = NULL;

}
SKMaterialRJob::SKMaterialRJob(SKMaterialRPtr& pProxy)
{
	m_pProxy = pProxy;

	m_uiJobType = JT_MATERIAL;
}
SKMaterialRJob::~SKMaterialRJob()
{
	m_pProxy = NULL;
}
SKAnimRJob::SKAnimRJob(SKAnimRPtr& pProxy)
{
	m_pProxy = pProxy;

	m_uiJobType = JT_ANIM;
}
SKAnimRJob::~SKAnimRJob()
{
	m_pProxy = NULL;
}

SKStaticMeshRJob::SKStaticMeshRJob(SKStaticMeshNodeRPtr& pProxy)
{
	m_pProxy = pProxy;

	m_uiJobType = JT_STATIC_MESH;
}
SKStaticMeshRJob::~SKStaticMeshRJob()
{
	m_pProxy = NULL;

}
SKSkelectonMeshRJob::SKSkelectonMeshRJob(SKSkelectonMeshNodeRPtr& pProxy)
{
	m_pProxy = pProxy;

	m_uiJobType = JT_SKELECTON_MESH;
}
SKSkelectonMeshRJob::~SKSkelectonMeshRJob()
{
	m_pProxy = NULL;

}
SKAnimTreeRJob::SKAnimTreeRJob(SKAnimTreeRPtr& pProxy)
{
	m_pProxy = pProxy;

	m_uiJobType = JT_ANIMTREE;
}
SKAnimTreeRJob::~SKAnimTreeRJob()
{
	m_pProxy = NULL;

}
SKMorphTreeRJob::SKMorphTreeRJob(SKMorphTreeRPtr& pProxy)
{
	m_pProxy = pProxy;

	m_uiJobType = JT_MORPHTREE;
}
SKMorphTreeRJob::~SKMorphTreeRJob()
{
	m_pProxy = NULL;

}
SKSceneMapRJob::SKSceneMapRJob(SKSceneMapRPtr& pProxy)
{
	m_pProxy = pProxy;

	m_uiJobType = JT_MAP;
}
SKSceneMapRJob::~SKSceneMapRJob()
{
	m_pProxy = NULL;

}
SKFontRJob::SKFontRJob(SKFontRPtr& pProxy)
{
	m_pProxy = pProxy;

	m_uiJobType = JT_FONT;
}
SKFontRJob::~SKFontRJob()
{
	m_pProxy = NULL;

}
SKActorRJob::SKActorRJob(SKActorRPtr& pProxy)
{
	m_pProxy = pProxy;

	m_uiJobType = JT_ACTOR;
}
SKActorRJob::~SKActorRJob()
{
	m_pProxy = NULL;

}
void SKResourceLoaderThread::Stop()
{
	
	SKThread::Stop();
	
}
SKResourceLoaderThread::SKResourceLoaderThread()
{
	m_pResourceQueue.Clear();
}
SKResourceLoaderThread::~SKResourceLoaderThread()
{
	m_pResourceQueue.Clear();
}
void SKResourceLoaderThread::AddJob(SKResourceJob* m_pJob)
{
	bool IsEmpty = m_pResourceQueue.IsEmpty();
	m_pResourceQueue.Enqueue(m_pJob);

}
void SKResourceLoaderThread::Run()
{
	while(!IsStopTrigger())
	{
		while(!m_pResourceQueue.IsEmpty())
		{
			SKResourceJob* pJob = NULL;
			m_pResourceQueue.Dequeue(pJob);
			if(!pJob->Load())
			{
				SKMAC_ASSERT(0);
			}
		}
	}

}
bool SKMaterialRJob::Process()
{
	if (!m_pBuffer)
	{
		return false;
	}

	SKStream LoadStream;
	LoadStream.NewLoadFromBuffer(m_pBuffer,m_uiSize);
	SKMaterial * pMaterial = (SKMaterial *)LoadStream.GetObjectByRtti(SKMaterial::ms_Type);

	if (pMaterial)
	{
		m_pProxy->SetNewResource(pMaterial);
		m_pProxy->Loaded();
		return true;
	}
	return false;
	
}
bool SKStaticMeshRJob::Process()
{
	if (!m_pBuffer)
	{

		return false;
	}
	SKStream LoadStream;
	LoadStream.NewLoadFromBuffer(m_pBuffer,m_uiSize);
	SKStaticMeshNode * pStaticMesh = (SKStaticMeshNode *)LoadStream.GetObjectByRtti(SKStaticMeshNode::ms_Type);
	if (pStaticMesh)
	{
		m_pProxy->SetNewResource(pStaticMesh);
		m_pProxy->Loaded();
		return true;
	}
	return false;
	
}
bool SKSkelectonMeshRJob::Process()
{
	if (!m_pBuffer)
	{

		return false;
	}
	SKStream LoadStream;
	LoadStream.NewLoadFromBuffer(m_pBuffer,m_uiSize);
	SKSkelectonMeshNode * pSkelectonMesh = (SKSkelectonMeshNode *)LoadStream.GetObjectByRtti(SKSkelectonMeshNode::ms_Type);
	if (pSkelectonMesh)
	{

		m_pProxy->SetNewResource(pSkelectonMesh);
		m_pProxy->Loaded();
		return true;
	}
	return false;

}
bool SKAnimTreeRJob::Process()
{
	if (!m_pBuffer)
	{

		return false;
	}
	SKStream LoadStream;
	LoadStream.NewLoadFromBuffer(m_pBuffer,m_uiSize);
	SKAnimTree * pAnimTree = (SKAnimTree *)LoadStream.GetObjectByRtti(SKAnimTree::ms_Type);
	if (pAnimTree)
	{

		m_pProxy->SetNewResource(pAnimTree);
		m_pProxy->Loaded();
		return true;
	}
	return false;

}
bool SKMorphTreeRJob::Process()
{
	if (!m_pBuffer)
	{

		return false;
	}
	SKStream LoadStream;
	LoadStream.NewLoadFromBuffer(m_pBuffer,m_uiSize);
	SKMorphTree * pMorphTree = (SKMorphTree *)LoadStream.GetObjectByRtti(SKMorphTree::ms_Type);
	if (pMorphTree)
	{

		m_pProxy->SetNewResource(pMorphTree);
		m_pProxy->Loaded();
		return true;
	}
	return false;

}
bool SKActorRJob::Process()
{
	if (!m_pBuffer)
	{

		return false;
	}
	SKStream LoadStream;
	LoadStream.NewLoadFromBuffer(m_pBuffer, m_uiSize);
	SKActor * pActor = (SKActor *)LoadStream.GetObjectByRtti(SKActor::ms_Type);
	if (pActor)
	{

		m_pProxy->SetNewResource(pActor);
		m_pProxy->Loaded();
		return true;
	}
	return false;

}
bool SKSceneMapRJob::Process()
{
	if (!m_pBuffer)
	{

		return false;
	}
	SKStream LoadStream;
	LoadStream.NewLoadFromBuffer(m_pBuffer, m_uiSize);
	SKSceneMap * pMap = (SKSceneMap *)LoadStream.GetObjectByRtti(SKSceneMap::ms_Type);
	if (pMap)
	{
		m_pProxy->SetNewResource(pMap);
		m_pProxy->Loaded();
		return true;
	}
	return false;

}
bool SKFontRJob::Process()
{
	if (!m_pBuffer)
	{

		return false;
	}
	SKStream LoadStream;
	LoadStream.NewLoadFromBuffer(m_pBuffer, m_uiSize);
	SKFont * pFont = (SKFont *)LoadStream.GetObjectByRtti(SKFont::ms_Type);
	if (pFont)
	{
		m_pProxy->SetNewResource(pFont);
		m_pProxy->Loaded();
		return true;
	}
	return false;

}
bool SKAnimRJob::Process()
{
	if (!m_pBuffer)
	{

		return false;
	}
	SKStream LoadStream;
	LoadStream.NewLoadFromBuffer(m_pBuffer,m_uiSize);
	SKAnim * pAnim = (SKAnim *)LoadStream.GetObjectByRtti(SKAnim::ms_Type);

	if (pAnim)
	{
		m_pProxy->SetNewResource(pAnim);
		m_pProxy->Loaded();
		return true;
	}
	return false;
}
SKTexAllState * SKTextureRJob::LoadTexture()
{
	if (!m_pBuffer)
	{
		return NULL;
	}
	SKStream LoadStream;
	LoadStream.NewLoadFromBuffer(m_pBuffer,m_uiSize);
	SKTexAllState * pTexAllState = (SKTexAllState *)LoadStream.GetObjectByRtti(SKTexAllState::ms_Type);
	if (!pTexAllState)
	{

		return NULL;
	}
	if(!pTexAllState->m_pTex)
	{
		SKMAC_DELETE(pTexAllState);

		return NULL;
	}

	return pTexAllState;
}
SKTexAllState * SKTextureRJob::Load2DTexture()
{
	if (!m_pBuffer)
	{
		return NULL;
	}
	SKString Extension; 

	if (!m_FileName.GetExtension(Extension))
	{
		return NULL;
	}
	SKTexAllState * pTexAllState = NULL;


	SKImage *pImage = NULL;

	if (Extension == SKImage::ms_ImageFormat[SKImage::IF_BMP])
	{
		pImage = SK_NEW SKBMPImage();
	}
	else if (Extension == SKImage::ms_ImageFormat[SKImage::IF_TGA])
	{
		pImage = SK_NEW SKTGAImage();
	}
	else 
	{

		return NULL;
	}

	if(!pImage->LoadFromBuffer(m_pBuffer,m_uiSize))
	{
		SKMAC_DELETE(pImage);

		return NULL;
	}

	unsigned int uiWidth = pImage->GetWidth();
	unsigned int uiHeight = pImage->GetHeight();
	if (!uiWidth || !uiHeight)
	{
		SKMAC_DELETE(pImage);

		return NULL;
	}
	if (!IsTwoPower(uiWidth) || !IsTwoPower(uiHeight))
	{
		SKMAC_DELETE(pImage);

		return NULL;
	}
	VS2DTexture * pTexture = NULL;
	pTexture = SK_NEW VS2DTexture(uiWidth, uiHeight, SKRenderer::SFT_A8R8G8B8, 0, 1);
	if(!pTexture)
	{
		SKMAC_DELETE(pImage);

		return NULL;
	}
	pTexture->CreateRAMDate();
	for (unsigned int cy = 0; cy < uiHeight; cy++) 
	{
		for (unsigned int cx = 0; cx < uiWidth; cx++) 
		{

			unsigned uiIndex = cy * uiWidth + cx;
			unsigned char *pBuffer = pTexture->GetBuffer(0,uiIndex);
			const unsigned char * pImageBuffer = pImage->GetPixel(cx,cy);
			if (pImage->GetBPP() == 8)
			{
				pBuffer[0] = pImageBuffer[0];
				pBuffer[1] = pImageBuffer[0];
				pBuffer[2] = pImageBuffer[0];
				pBuffer[3] = 255;
			}
			else if (pImage->GetBPP() == 24)
			{
				pBuffer[0] = pImageBuffer[0];
				pBuffer[1] = pImageBuffer[1];
				pBuffer[2] = pImageBuffer[2];
				pBuffer[3] = 255;
			}
			else if (pImage->GetBPP() == 32)
			{
				pBuffer[0] = pImageBuffer[0];
				pBuffer[1] = pImageBuffer[1];
				pBuffer[2] = pImageBuffer[2];
				pBuffer[3] = pImageBuffer[3];
			}
		} // for
	} // for

	unsigned char * pLast = pTexture->GetBuffer(0);
	for (unsigned int i = 1 ; i < pTexture->GetMipLevel() ; i++)
	{
		unsigned char *pNow = pTexture->GetBuffer(i);

		if(!SKResourceManager::GetNextMipDate(pLast,pTexture->GetWidth(i - 1),pTexture->GetHeight(i - 1),pNow,pTexture->GetChannelPerPixel()))
		{
			SKMAC_DELETE(pTexture);
			SKMAC_DELETE(pImage);

			return NULL;
		}
		pLast = pTexture->GetBuffer(i);
	}
	if (pImage)
	{
		SKMAC_DELETE(pImage);
	}
	pTexAllState = SK_NEW SKTexAllState(pTexture);

	if (m_pSamplerState)
	{
		pTexAllState->SetSamplerState(m_pSamplerState);
	}

	pTexAllState->SetSRGBEable(m_bSRGB);

	return pTexAllState;
}
SKTexAllState * SKTextureRJob::Load2DTextureCompress()
{
	if (!m_pBuffer)
	{
		return NULL;
	}

	if (m_uiCompressType > SKNVCompression::NV_MAX)
	{

		return NULL;
	}

	SKString Extension; 

	if (!m_FileName.GetExtension(Extension))
	{

		return NULL;
	}
	SKTexAllState * pTexAllState = NULL;

	SKImage *pImage = NULL;
	if (Extension == SKImage::ms_ImageFormat[SKImage::IF_BMP])
	{
		pImage = SK_NEW SKBMPImage();
	}
	else if (Extension == SKImage::ms_ImageFormat[SKImage::IF_TGA])
	{
		pImage = SK_NEW SKTGAImage();
	}
	else 
	{

		return NULL;
	}
	if(!pImage->LoadFromBuffer(m_pBuffer,m_uiSize))
	{
		SKMAC_DELETE(pImage);

		return NULL;
	}

	unsigned int uiWidth = pImage->GetWidth();
	unsigned int uiHeight = pImage->GetHeight();
	if (!uiWidth || !uiHeight)
	{
		SKMAC_DELETE(pImage);

		return NULL;
	}
	if (!IsTwoPower(uiWidth) || !IsTwoPower(uiHeight))
	{
		SKMAC_DELETE(pImage);

		return NULL;
	}
	VS2DTexture * pTexture = NULL;
	pTexture = SK_NEW VS2DTexture(uiWidth, uiHeight, SKRenderer::SFT_A8R8G8B8, 0, 1);
	if(!pTexture)
	{
		SKMAC_DELETE(pImage);

		return NULL;
	}
	pTexture->CreateRAMDate();
	for (unsigned int cy = 0; cy < uiHeight; cy++) 
	{
		for (unsigned int cx = 0; cx < uiWidth; cx++) 
		{

			unsigned uiIndex = cy * uiWidth + cx;
			unsigned char *pBuffer = pTexture->GetBuffer(0,uiIndex);
			const unsigned char * pImageBuffer = pImage->GetPixel(cx,cy);
			if (pImage->GetBPP() == 8)
			{
				pBuffer[0] = pImageBuffer[0];
				pBuffer[1] = pImageBuffer[0];
				pBuffer[2] = pImageBuffer[0];
				pBuffer[3] = 255;
			}
			else if (pImage->GetBPP() == 24)
			{
				pBuffer[0] = pImageBuffer[0];
				pBuffer[1] = pImageBuffer[1];
				pBuffer[2] = pImageBuffer[2];
				pBuffer[3] = 255;
			}
			else if (pImage->GetBPP() == 32)
			{
				pBuffer[0] = pImageBuffer[0];
				pBuffer[1] = pImageBuffer[1];
				pBuffer[2] = pImageBuffer[2];
				pBuffer[3] = pImageBuffer[3];
			}
		} // for
	} // for

	unsigned char * pLast = pTexture->GetBuffer(0);
	for (unsigned int i = 1 ; i < pTexture->GetMipLevel() ; i++)
	{
		unsigned char *pNow = pTexture->GetBuffer(i);

		if(!SKResourceManager::GetNextMipDate(pLast,pTexture->GetWidth(i - 1),pTexture->GetHeight(i - 1),pNow,pTexture->GetChannelPerPixel()))
		{
			SKMAC_DELETE(pTexture);
			SKMAC_DELETE(pImage);

			return NULL;
		}
		pLast = pTexture->GetBuffer(i);
	}
	if (pImage)
	{
		SKMAC_DELETE(pImage);
	}

	unsigned int uiCompressFormat = SKNVCompression::NV_ARGB;
	unsigned int uiVSTextureTpye = SKRenderer::SFT_A8R8G8B8;
	if (m_uiCompressType == 0)
	{
		uiCompressFormat = SKNVCompression::NV_ARGB;
		uiVSTextureTpye = SKRenderer::SFT_A8R8G8B8;
	}
	else if (m_uiCompressType == 1)
	{
		uiCompressFormat = SKNVCompression::NV_DXT3;
		uiVSTextureTpye = SKRenderer::SFT_DXT3;
	}
	else if (m_uiCompressType == 2)
	{
		uiCompressFormat = SKNVCompression::NV_DXT5;
		uiVSTextureTpye = SKRenderer::SFT_DXT5;
	}
	VS2DTexture * pNewTexture = SK_NEW VS2DTexture(uiWidth,uiHeight,uiVSTextureTpye,0,1);
	pNewTexture->CreateRAMDate();
	for (unsigned int i = 0 ; i < pTexture->GetMipLevel() ; i++)
	{
		unsigned char *pNow = pTexture->GetBuffer(i);
		SKNVCompression Help;
		Help.Setup(pNow,uiCompressFormat,pTexture->GetWidth(i),pTexture->GetHeight(i),false,m_bIsNormal);
		Help.Process();
		SKMemcpy(pNewTexture->GetBuffer(i),Help.OutputHandler.m_CompressDate.GetBuffer(),Help.OutputHandler.m_CompressDate.GetNum());
	}

	SKMAC_DELETE(pTexture);
	pTexAllState = SK_NEW SKTexAllState(pNewTexture);
	if (m_pSamplerState)
	{
		pTexAllState->SetSamplerState(m_pSamplerState);
	}
	pTexAllState->SetSRGBEable(m_bSRGB);
	return pTexAllState;
}
bool SKTextureRJob::Process()
{
	SKTexAllState * pTex = NULL;
	if (m_bEngineType)
	{
		pTex = LoadTexture();
	}
	else
	{
		if (m_bCompress)
		{
			pTex = Load2DTextureCompress();
		}
		else
		{
			pTex = Load2DTexture();
		}
	}

	if(pTex)
	{
		SKRenderer::ms_pRenderer->LoadTexture(pTex->m_pTex);
		m_pProxy->SetNewResource(pTex);
		m_pProxy->Loaded();
		return true;
	}
	return false;
	
}

SKASYNLoadManager *SKASYNLoadManager::ms_pASYNLoadManager = NULL;
SKASYNLoadManager::SKASYNLoadManager()
{
	
	m_ResourceJobArray.Clear();
	SKMAC_ASSERT(!ms_pASYNLoadManager);
	ms_pASYNLoadManager = this;
}
SKASYNLoadManager::~SKASYNLoadManager()
{

	m_ResourceLoadThread.Start();

	
	m_ResourceLoadThread.Stop();
	
	for (unsigned int i = 0 ; i < m_ResourceJobArray.GetNum() ;i++)
	{
		SKMAC_DELETE(m_ResourceJobArray[i]);
	}
	m_ResourceJobArray.Clear();
}
void SKASYNLoadManager::Update(double AppTime)
{
	ADD_TIME_PROFILE(SKASYNLoadManagerUpdate)
	unsigned int i = 0 ;
	SKREAL t1 = (SKREAL)SKTimer::ms_pTimer->GetGamePlayTime();
	while(i < m_ResourceJobArray.GetNum())
	{
		if (m_ResourceJobArray[i]->m_uiLoadState == SKResourceJob::LS_PENDING)
		{
			i++;
			continue;
		}
		else if (m_ResourceJobArray[i]->m_uiLoadState == SKResourceJob::LS_LOADED)
		{	
			if (m_ResourceJobArray[i]->m_uiJobState == SKResourceJob::JS_DELETE)
			{
				
			}
			else
			{
				SKMAC_ASSERT(SKRenderer::ms_pRenderer);
				if (!m_ResourceJobArray[i]->Process())
				{
					SKMAC_ASSERT(0);
				}
			}
			
			SKMAC_DELETE(m_ResourceJobArray[i]);
			m_ResourceJobArray.Erase(i);				
		}
		
		else
		{
			SKMAC_DELETE(m_ResourceJobArray[i]);
			m_ResourceJobArray.Erase(i);
			SKMAC_ASSERT(0);
		}
		SKREAL t2 = (SKREAL)SKTimer::ms_pTimer->GetGamePlayTime() - t1;
		if (t2 > 10.0f)
		{
			break;
		}
	}
	if (m_ResourceJobArray.GetNum() == 0)
	{
		m_ResourceLoadThread.Suspend();
	}
}
void SKASYNLoadManager::AddTextureLoad(SKTexAllStateRPtr& pTexture,
									   SKFileName & FileName,
									   bool bEngineType,
									   SKSamplerStatePtr pSamplerState,					
									   bool bCompress,
									   unsigned int uiCompressType,
									   bool bIsNormal,bool bSRGB)
{
	SKTextureRJob* pResourceProxyJob = SK_NEW SKTextureRJob(pTexture);
	pResourceProxyJob->m_bIsNormal = bIsNormal;
	pResourceProxyJob->m_bEngineType = bEngineType;
	pResourceProxyJob->m_pSamplerState = pSamplerState;
	pResourceProxyJob->m_bCompress = bCompress;
	pResourceProxyJob->m_uiCompressType = uiCompressType;
	pResourceProxyJob->m_FileName = FileName;
	pResourceProxyJob->m_bSRGB = bSRGB;
	AddJob(pResourceProxyJob);
}
void SKASYNLoadManager::AddMaterial(SKMaterialRPtr& pMaterial,SKFileName & FileName)
{
	SKMaterialRJob* pResourceProxyJob = SK_NEW SKMaterialRJob(pMaterial);
	pResourceProxyJob->m_FileName = FileName;
	AddJob(pResourceProxyJob);
}
void SKASYNLoadManager::AddAnim(SKAnimRPtr& pAnim,SKFileName & FileName)
{
	SKAnimRJob* pResourceProxyJob = SK_NEW SKAnimRJob(pAnim);
	pResourceProxyJob->m_FileName = FileName;
	AddJob(pResourceProxyJob);
}

void SKASYNLoadManager::AddStaticMesh(SKStaticMeshNodeRPtr& pStaticMesh,SKFileName & FileName)
{
	SKStaticMeshRJob * pResourceProxyJob = SK_NEW SKStaticMeshRJob(pStaticMesh);
	pResourceProxyJob->m_FileName = FileName;
	AddJob(pResourceProxyJob);
}
void SKASYNLoadManager::AddSkelectonMesh(SKSkelectonMeshNodeRPtr& pSkelectonMesh,SKFileName & FileName)
{
	SKSkelectonMeshRJob* pResourceProxyJob = SK_NEW SKSkelectonMeshRJob(pSkelectonMesh);
	pResourceProxyJob->m_FileName = FileName;
	AddJob(pResourceProxyJob);
}
void SKASYNLoadManager::AddAnimTree(SKAnimTreeRPtr& pAnimTree,SKFileName & FileName)
{
	SKAnimTreeRJob* pResourceProxyJob = SK_NEW SKAnimTreeRJob(pAnimTree);
	pResourceProxyJob->m_FileName = FileName;
	AddJob(pResourceProxyJob);
}
void SKASYNLoadManager::AddActor(SKActorRPtr& pActor, SKFileName & FileName)
{
	SKActorRJob* pResourceProxyJob = SK_NEW SKActorRJob(pActor);
	pResourceProxyJob->m_FileName = FileName;
	AddJob(pResourceProxyJob);
}
void SKASYNLoadManager::AddFont(SKFontRPtr& pFont, SKFileName & FileName)
{
	SKFontRJob* pResourceProxyJob = SK_NEW SKFontRJob(pFont);
	pResourceProxyJob->m_FileName = FileName;
	AddJob(pResourceProxyJob);
}
void SKASYNLoadManager::AddMap(SKSceneMapRPtr& pMap, SKFileName & FileName)
{
	SKSceneMapRJob* pResourceProxyJob = SK_NEW SKSceneMapRJob(pMap);
	pResourceProxyJob->m_FileName = FileName;
	AddJob(pResourceProxyJob);
}
void SKASYNLoadManager::AddMorphTree(SKMorphTreeRPtr& pMorphTree,SKFileName & FileName)
{
	SKMorphTreeRJob* pResourceProxyJob = SK_NEW SKMorphTreeRJob(pMorphTree);
	pResourceProxyJob->m_FileName = FileName;
	AddJob(pResourceProxyJob);
}
void SKASYNLoadManager::AddJob(SKResourceJob* pResourceProxyJob)
{

	m_ResourceJobArray.AddElement(pResourceProxyJob);
	//m_ResourceLoadThread.AddJob(pResourceProxyJob); //m_ResourceLoadThread ̻߳
	if (m_ResourceJobArray.GetNum() == 1)
	{
		m_ResourceLoadThread.Start();
	}
	m_ResourceLoadThread.AddJob(pResourceProxyJob);
}
void SKASYNLoadManager::DeleteLoadResource(SKFileName & FileName)
{
	for (unsigned int i = 0; i < m_ResourceJobArray.GetNum(); i++)
	{
		if (m_ResourceJobArray[i]->m_uiJobType == SKResourceJob::JT_MAP)
		{
			if (((SKSceneMapRJob *)m_ResourceJobArray[i])->m_FileName == FileName)
			{
				((SKSceneMapRJob *)m_ResourceJobArray[i])->m_uiJobState = SKResourceJob::JS_DELETE;
				return;
			}
		}
	}
}