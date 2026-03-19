#include "SKSceneMap.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
#include "SKSceneManager.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKSceneMap,SKObject)
BEGIN_ADD_PROPERTY(SKSceneMap,SKObject)
REGISTER_PROPERTY(m_Name, Name, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_pScene, Scene, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_NO_USE_GC)
REGISTER_PROPERTY(m_pActorArray, ActorArray, SKProperty::F_SAVE_LOAD_CLONE  | SKProperty::F_REFLECT_NAME)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKSceneMap)
ADD_INITIAL_FUNCTION_WITH_PRIORITY(InitialDefaultState)
ADD_TERMINAL_FUNCTION_WITH_PRIORITY(TerminalDefaultState)
IMPLEMENT_INITIAL_END
SKSceneMapPtr SKSceneMap::Default = NULL;
bool SKSceneMap::ms_bIsEnableASYNLoader = true;
bool SKSceneMap::ms_bIsEnableGC = false;
SKSceneMap::SKSceneMap(const TCHAR * pName)
{
	m_Name = pName;
	m_pScene = SK_NEW SKScene();
	
}
SKSceneMap::~SKSceneMap()
{
	m_pScene = NULL;
}
SKSceneMap::SKSceneMap()
{
	
}
void SKSceneMap::OnDestroy()
{
	m_pScene = NULL;
}
bool SKSceneMap::InitialDefaultState()
{
	Default = NULL;// SK_NEW SKSceneMap();
	return true;
}
bool SKSceneMap::TerminalDefaultState()
{
	Default = NULL;
	return true;
}
void SKSceneMap::AddActor(SKActor * pActor)
{

	if (pActor)
	{
		for (unsigned int i = 0 ; i < m_pActorArray.GetNum() ; i++)
		{
			if (pActor == m_pActorArray[i])
			{
				pActor->m_pSceneMap = this;
				return ;
			}
		}
		if (pActor->m_pSceneMap)
		{
			pActor->m_pSceneMap->DeleteActor(pActor);
		}
		
		m_pActorArray.AddElement(pActor);

		m_pScene->AddObject(pActor->GetActorNode());

		pActor->AddToSceneMap(this);
	}
}
SKActor *SKSceneMap::GetActor(SKUsedName Name)
{
	for (unsigned int i = 0 ; i < m_pActorArray.GetNum() ; i++)
	{
		if (m_pActorArray[i]->m_ActorName == Name)
		{
			
			return m_pActorArray[i];
		}
	}	
	return NULL;
}

void SKSceneMap::DeleteActor(SKActor * pActor)
{

	for (unsigned int i = 0 ; i < m_pActorArray.GetNum() ; i++)
	{
		if (m_pActorArray[i] == pActor)
		{
			if (m_pActorArray[i]->GetOwner())
			{
				m_pActorArray[i]->GetOwner()->DeleteChildActor(m_pActorArray[i]);
			}
			else
			{
				m_pScene->DeleteObject(m_pActorArray[i]->GetActorNode());
			}
			m_pActorArray[i]->AddToSceneMap(NULL);
			m_pActorArray.Erase(i);
			return ;
		}
	}
}
bool SKSceneMap::PostLoad(void * pDate)
{
	SKObject::PostLoad(pDate);
	//SKSceneManager::ms_pSceneManager->AddScene(m_pScene);
	return  true;
}