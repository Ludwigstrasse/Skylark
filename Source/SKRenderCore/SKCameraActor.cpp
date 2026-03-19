#include "SKCameraActor.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
#include "SKWorld.h"
#include "SKViewFamily.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKViewFamliyInfo,SKObject)
BEGIN_ADD_PROPERTY(SKViewFamliyInfo,SKObject)
REGISTER_PROPERTY(m_SceneMapName,SceneMapName,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiWidth,Width,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiHeight,Height,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiCaptureViewFamilyType,CaptureViewFamilyType,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_ViewFamilyName,ViewFamilyName,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pPostEffect, PostEffect, SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_RenderMethodRTTIName, RenderMethodRTTIName, SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKViewFamliyInfo)
IMPLEMENT_INITIAL_END
SKViewFamliyInfo::~SKViewFamliyInfo()
{

}
SKViewFamliyInfo::SKViewFamliyInfo()
{
	
}
void SKViewFamliyInfo::GetScene()
{

	m_pViewFamily->ClearAllScene();
	if (m_SceneMapName.GetNum() > 0)
	{
		for (unsigned int i = 0 ; i < m_SceneMapName.GetNum() ; i++)
		{
			SKSceneMap * pSceneMap = SKWorld::ms_pWorld->GetScene(m_SceneMapName[i]);
			if (pSceneMap)
			{
				m_pViewFamily->AddScene(pSceneMap->m_pScene);
			}			
		}
	}
	else
	{
		SKSceneMap * pSceneMap = SKWorld::ms_pWorld->GetScene(SKUsedName::ms_cMainScene);
		m_pViewFamily->AddScene(pSceneMap->m_pScene);
		
	}
	
}
IMPLEMENT_RTTI(SKCameraActor,SKActor)
BEGIN_ADD_PROPERTY(SKCameraActor,SKActor)
REGISTER_PROPERTY(m_ViewFamliyInfoArray,ViewFamliyInfoArray,SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_NO_USE_GC)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKCameraActor)
IMPLEMENT_INITIAL_END
SKCameraActor::SKCameraActor()
{
	
}
SKCameraActor::~SKCameraActor()
{

}
void SKCameraActor::Update(double dAppTime)
{
	SKActor::Update(dAppTime);
	for (unsigned int i = 0 ; i < m_ViewFamliyInfoArray.GetNum() ; i++)
	{
		m_ViewFamliyInfoArray[i]->GetScene();
		
	}
}
void SKCameraActor::CreateDefaultComponentNode()
{
	m_pNode = SKNodeComponent::CreateComponet<SKCamera>();
}
void SKCameraActor::AddCustomCullPlane(const SKPlane3& P)
{
	GetTypeNode()->AddCustomCullPlane(P);
}
void SKCameraActor::ClearCustomCullPlane()
{
	GetTypeNode()->ClearCustomCullPlane();
}