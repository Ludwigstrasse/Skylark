#include "SKWorld.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
#include "SKResourceManager.h"
#include "SKStaticActor.h"
#include "SKSkelectonActor.h"
#include "SKGraphicInclude.h"
#include "SKSceneManager.h"
#include "SKLightActor.h"
#include "SKProfiler.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKWorld,SKObject)
BEGIN_ADD_PROPERTY(SKWorld,SKObject)
REGISTER_PROPERTY(m_SceneArray, SceneArray, SKProperty::F_REFLECT_NAME)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKWorld)
IMPLEMENT_INITIAL_END
SKWorld * SKWorld::ms_pWorld = NULL;
DECLEAR_TIME_PROFILENODE(SKWorldUpdate, Update)
SKWorld::SKWorld()
{
	SKMAC_ASSERT(ms_pWorld == NULL);
	ms_pWorld = this;
	SKSceneMap * pSceneMap = SK_NEW SKSceneMap("Main");
	m_SceneArray.AddElement(pSceneMap);
	SKResourceManager::AddRootObject(this);
	IncreRef();//default reference = 1 ,ָָͷš
}
SKWorld::~SKWorld()
{
	SKResourceManager::DeleteAllMapResource();
	for (unsigned int i = 0; i < m_SceneArray.GetNum();i++)
	{
		if (m_SceneArray[i])
		{
			m_SceneArray[i]->OnDestroy();
		}
	}
	m_SceneArray.Clear();
	m_ActorArray.Clear();
	SKResourceManager::GCObject();
	SKResourceManager::RunAllGCTask();
}
void SKWorld::DestroyScene(const SKUsedName & Name)
{
	//first scene  must be not delete
	for (unsigned int i = 1; i < m_SceneArray.GetNum(); i++)
	{
		if (m_SceneArray[i]->m_Name == Name)
		{
			for (unsigned int j = 0; j < m_ActorArray.GetNum();)
			{
				if (m_ActorArray[j]->m_pSceneMap == m_SceneArray[i])
				{
					DestroyActor(m_ActorArray[j]);
				}
				else
				{
					j++;
				}
			}
			m_SceneArray[i]->OnDestroy();
			m_SceneArray[i]->SetFlag(OF_PendingKill);
			m_SceneArray.Erase(i);
			return;
		}
	}

}

SKSceneMap* SKWorld::CreateScene(const TCHAR * pName)
{
	for (unsigned int i = 0 ; i < m_SceneArray.GetNum() ;i++)
	{
		if(m_SceneArray[i]->m_Name == pName)
			return NULL;

	}
	SKSceneMap * pSceneMap = SK_NEW SKSceneMap(pName);
	SKResourceManager::AddGCObject(pSceneMap);
	m_SceneArray.AddElement(pSceneMap);
	return pSceneMap;
}
SKSceneMap * SKWorld::GetScene(const SKUsedName & Name)
{
	for (unsigned int i = 0 ; i < m_SceneArray.GetNum() ;i++)
	{
		if(m_SceneArray[i]->m_Name == Name)
			return m_SceneArray[i];

	}
	return NULL;
}
void SKWorld::ProcessInput(unsigned int uiInputType, unsigned int uiEvent, unsigned int uiKey, int x, int y, int z)
{

	for (unsigned int i = 0; i < m_ActorArray.GetNum(); i++)
	{
		if (!m_ActorArray[i]->IsHasFlag(SKObject::OF_PendingKill))
		{
			m_ActorArray[i]->ProcessInput(uiInputType,uiEvent,uiKey,x,y,z);
		}
	}
}
void SKWorld::Update(double dAppTime)
{
	ADD_TIME_PROFILE(SKWorldUpdate)
	static double LastTime = dAppTime;
	double DetTime = dAppTime - LastTime;
	LastTime = dAppTime;

	for (unsigned int i = 0 ; i < m_ActorArray.GetNum() ; i++)
	{
		if (!m_ActorArray[i]->IsHasFlag(SKObject::OF_PendingKill))
		{
			m_ActorArray[i]->Update(dAppTime);
		}
	}
	unsigned int i = 0 ;
	while(i < m_MessageArray.GetNum())
	{
		if (m_MessageArray[i].DispatchTime <= 0.0)
		{
			SKActor *pActor = GetActor(m_MessageArray[i].Receiver);
			if (pActor)
			{
				pActor->HandleMessage(m_MessageArray[i]);
			}
			m_MessageArray.Erase(i);
		}
		else
		{
			m_MessageArray[i].DispatchTime -= DetTime;
			i++;
		}
	}

}
SKActor *SKWorld::GetActor(SKUsedName Name)
{
	for (unsigned int i = 0 ; i < m_ActorArray.GetNum() ; i++)
	{
		if (m_ActorArray[i]->m_ActorName == Name)
		{

			return m_ActorArray[i];
		}
	}	
	return NULL;
}

void SKWorld::DeleteActor(SKActor * pActor)
{

	for (unsigned int i = 0 ; i < m_ActorArray.GetNum() ; i++)
	{
		if (m_ActorArray[i] == pActor)
		{
			m_ActorArray.Erase(i);
			return ;
		}
	}
}
void SKWorld::AddActor(SKActor * pActor)
{
	if (pActor)
	{
		m_ActorArray.AddElement(pActor);
	}
	
}
bool SKWorld::LoadMap(const TCHAR * MapPath, bool IsAsyn)
{
	if (!MapPath)
	{
		return false;
	}
	SKResourceProxyBase * pResouce = SKResourceManager::LoadASYNMap(MapPath, IsAsyn);
	if (!pResouce)
	{
		return false;
	}
	else
	{
		pResouce->AddLoadEventObject(this);
		
	}
	return true;
}
bool SKWorld::UnLoadMap(const TCHAR * MapPath)
{
	if (!MapPath)
	{
		return false;
	}
	SKResourceManager::DeleteMapResource(MapPath);

	return true;
}
SKActor * SKWorld::CreateActor(const TCHAR *  ActorPath, const SKVector3 & Pos, const SKMatrix3X3 & Rotate, const SKVector3 &Scale, SKSceneMap * pSceneMap)
{
	if (!ActorPath)
	{
		return NULL;
	}
	SKActor * pActor = NULL;
	SKResourceProxyBase * pResouce = SKResourceManager::LoadResource(ActorPath,false);
	if (pResouce)
	{
		unsigned int ResourceType = pResouce->GetResourceType();
		if (ResourceType == SKResource::RT_STATIC_MODEL)
		{
			pActor = CreateActor<SKStaticActor>(Pos, Rotate, Scale,pSceneMap);
			((SKStaticMeshComponent *)pActor->GetActorNode())->SetStaticMeshResouce((SKStaticMeshNodeR *)pResouce);
		}
		else if (ResourceType == SKResource::RT_SKELECTON_MODEL)
		{
			pActor = CreateActor<SKSkelectonActor>(Pos, Rotate, Scale, pSceneMap);
			((SKSkelectonMeshComponent *)pActor->GetActorNode())->SetSkelectonMeshResource((SKSkelectonMeshNodeR *)pResouce);
		}
		else if (ResourceType == SKResource::RT_ACTOR)
		{
			if (!pResouce->IsLoaded())
			{
				unsigned int i = 0;
				for (; i < m_SceneArray.GetNum(); i++)
				{
					if (m_SceneArray[i]->m_Name == pSceneMap->m_Name)
						break;
				}
				pResouce->AddLoadEventObject(this,i);
				pActor = NULL;
			}
			else
			{
				pActor = (SKActor *)SKObject::CloneCreateObject(((SKActorR *)pResouce)->GetResource());
				if (pSceneMap)
				{
					pSceneMap->AddActor(pActor);
				}
				else
				{
					m_SceneArray[0]->AddActor(pActor);
				}
			}
		}
	}
	return pActor;
}
void SKWorld::AddSceneMap(SKSceneMap * pSceneMap)
{
	SKResourceManager::AddGCObject(pSceneMap);
	for (unsigned int i = 0; i < pSceneMap->m_pActorArray.GetNum(); i++)
	{
		AddActor(pSceneMap->m_pActorArray[i]);
	}
	m_SceneArray.AddElement(pSceneMap);
}
void SKWorld::LoadedEvent(SKResourceProxyBase * pResourceProxy, int Data)
{
	if (pResourceProxy->GetResourceType() == SKResource::RT_ACTOR)
	{
		SKActor * pActor = (SKActor *)SKObject::CloneCreateObject(((SKActorR *)pResourceProxy)->GetResource());
		m_SceneArray[Data]->AddActor(pActor);
	}
	else if (pResourceProxy->GetResourceType() == SKResource::RT_MAP)
	{
		SKSceneMap * pSceneMap = ((SKResourceProxy<SKSceneMap> *)pResourceProxy)->GetResource();
		AddSceneMap(pSceneMap);
	}

}
void SKWorld::DestroyActor(SKActor * pActor)
{
	pActor->OnDestory();
	pActor->SetFlag(OF_PendingKill);
	DeleteActor(pActor);

}

void SKWorld::AddMessage(const SKMessage & Message)
{
	m_MessageArray.AddElement(Message);
}
void SKWorld::AttachCaptureViewFamilyToCamera(SKCameraActor * pCameraActor,
									 unsigned int uiCaptureViewFamilyType,const SKString & ViewFamilyName,
									 unsigned int uiWidth,unsigned int uiHeight,
									 SKArray<SKString> & SceneMapName,
									 const TCHAR * RenderMethodRTTIName,
									 SKPostEffectSetR*  pPostEffectSet)
{
	SKCamera * pCamera = (SKCamera *)pCameraActor->GetActorNode();
	if (pCamera)
	{
		SKViewFamliyInfo * pInfo = SK_NEW SKViewFamliyInfo();
		
		SKViewFamily * pViewFamily = NULL;
		if (uiCaptureViewFamilyType == SKCaptureViewFamily::VT_CAPTURE_2D_CAPTURE_NORMAL)
		{
			pViewFamily = SK_NEW VS2DCaptureViewFamily(ViewFamilyName,uiWidth,uiHeight,pCamera,pPostEffectSet,RenderMethodRTTIName);

		}
		else if (uiCaptureViewFamilyType == SKCaptureViewFamily::VT_CAPTURE_CUB_CAPTURE_NORMAL)
		{
			pViewFamily = SK_NEW SKCubCaptureViewFamily(ViewFamilyName,uiWidth,uiHeight,pCamera,pPostEffectSet,RenderMethodRTTIName);
		}

		pInfo->m_SceneMapName = SceneMapName;
		pInfo->m_uiCaptureViewFamilyType = uiCaptureViewFamilyType;
		pInfo->m_uiHeight = uiHeight;
		pInfo->m_uiWidth = uiWidth;
		pInfo->m_pPostEffect = pPostEffectSet;
		pInfo->m_ViewFamilyName = ViewFamilyName;
		pInfo->m_pViewFamily = pViewFamily;
		pCameraActor->m_ViewFamliyInfoArray.AddElement(pInfo);
		pInfo->GetScene();
	}
}
void SKWorld::AttachWindowViewFamilyToCamera(SKCameraActor * pCameraActor,
									unsigned int uiWindowViewFamilyType,const SKString & ViewFamilyName,
									SKArray<SKString> & SceneMapName,
									const TCHAR * RenderMethodRTTIName,
									int iWindowID,				
									SKPostEffectSetR* pPostEffectSet)
{
	SKCamera * pCamera = (SKCamera *)pCameraActor->GetActorNode();
	if (pCamera)
	{
		SKViewFamliyInfo * pInfo = SK_NEW SKViewFamliyInfo();

		SKViewFamily * pViewFamily = NULL;
		if (uiWindowViewFamilyType == SKWindowViewFamily::VT_WINDOW_NORMAL)
		{
			pViewFamily= SK_NEW SKWindowViewFamily(ViewFamilyName,pCamera,pPostEffectSet,RenderMethodRTTIName,iWindowID);
			
			
		}
		pInfo->m_SceneMapName = SceneMapName;
		pInfo->m_uiCaptureViewFamilyType = uiWindowViewFamilyType;
		pInfo->m_pPostEffect = pPostEffectSet;
		pInfo->m_ViewFamilyName = ViewFamilyName;
		pInfo->m_pViewFamily = pViewFamily;
		pCameraActor->m_ViewFamliyInfoArray.AddElement(pInfo);
		pInfo->GetScene();
	}
}
void SKWorld::UpdateLightActorScene(SKLightActor * pActor)
{
	SKLight * pLightNode = pActor->GetTypeNode();
	SKLocalLight * pLocalLight = DynamicCast<SKLocalLight>(pLightNode);
	if (pLocalLight)
	{
		pLocalLight->ClearAllShadowScene();
		for (unsigned int i = 0 ; i < m_SceneArray.GetNum() ; i++)
		{
			pLocalLight->AddShadowScene(m_SceneArray[i]->m_pScene);
		}
	}
	
}