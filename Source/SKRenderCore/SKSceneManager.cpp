#include "SKSceneManager.h"
#include "SKGraphicInclude.h"
#include "SKUpdateThread.h"
#include "SKTimer.h"
#include "SKProfiler.h"
using namespace SKEngine2;
SKSceneManager * SKSceneManager::ms_pSceneManager = NULL;
DECLEAR_TIME_PROFILENODE(SKSceneManagerUpdate,Update)
DECLEAR_TIME_PROFILENODE(SKSceneManagerDraw,Update)
DECLEAR_TIME_PROFILENODE(ViewFamilyUpdate,SKSceneManagerDraw);
DECLEAR_TIME_PROFILENODE(ViewFamilyDraw, SKSceneManagerDraw);
SKSceneManager::SKSceneManager()
{
	
	ms_pSceneManager = this;
}
SKSceneManager::~SKSceneManager()
{
	
}

void SKSceneManager::Update(double dAppTime)
{
	ADD_TIME_PROFILE(SKSceneManagerUpdate)
	for (unsigned int i = 0 ;i < m_pScene.GetNum() ; i++)
	{
		m_pScene[i]->Update(dAppTime);
	}
	if (SKResourceManager::ms_bUpdateThread)
	{
		SKUpdateThreadSys::ms_pUpdateThreadSys->Begin();
/*		double Time = SKTimer::ms_pTimer->GetGamePlayTime();*/
		SKUpdateThreadSys::ms_pUpdateThreadSys->ExChange();
// 		Time = SKTimer::ms_pTimer->GetGamePlayTime() - Time;
// 		SKOutPutDebugString("Time %f\n",Time);
	}
	
	for (unsigned int i = 0 ;i < m_pScene.GetNum() ; i++)
	{
		m_pScene[i]->CollectUpdateInfo();
	}
}

void SKSceneManager::Draw(double dAppTime)
{
	ADD_TIME_PROFILE(SKSceneManagerDraw)
	for (unsigned int i = 0 ; i < m_pViewFamily.GetNum() ;i++)
	{
		if (m_pViewFamily[i] && m_pViewFamily[i]->m_bEnable)
		{
			{
				ADD_TIME_PROFILE(ViewFamilyUpdate)
				m_pViewFamily[i]->Update(dAppTime);
			}

			{
				ADD_TIME_PROFILE(ViewFamilyDraw)
				m_pViewFamily[i]->Draw(dAppTime);
			}
			
		}
		
	}
	for (unsigned int i = 0 ;i < m_pScene.GetNum() ; i++)
	{
		for (unsigned int j = 0 ; j < m_pScene[i]->GetAllLightNum() ;j++)
		{
			SKLocalLight * pLight = DynamicCast<SKLocalLight>(m_pScene[i]->GetAllLight(j));
			if (pLight)
			{
				pLight->DrawLightMaterial(dAppTime);
				pLight->DisableNoDepandenceShadowMap(dAppTime);
			}
			
		}

	}
}
SKViewFamily * SKSceneManager::GetViewFamily(const SKString & ViewFamilyName)const
{
	for (unsigned int i = 0 ; i < m_pViewFamily.GetNum() ;i++)
	{
		if (m_pViewFamily[i]->m_ViewFamilyName == ViewFamilyName)
		{
			return m_pViewFamily[i];
		}
	}
	return NULL;
}
