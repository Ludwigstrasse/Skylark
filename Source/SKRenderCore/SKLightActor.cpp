#include "SKLightActor.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
#include "SKDirectionLight.h"
#include "SKPointLight.h"
#include "SKSpotLight.h"
#include "SKWorld.h"
#include "SKSkyLight.h"
using namespace SKEngine2;

IMPLEMENT_RTTI_NoCreateFun(SKLightActor,SKActor)
BEGIN_ADD_PROPERTY(SKLightActor,SKActor)
REGISTER_PROPERTY(m_ShadowSceneNameArray, ShadowSceneNameArray, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(SKLightActor)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
SKLightActor::SKLightActor()
{

}
SKLightActor::~SKLightActor()
{

}
void SKLightActor::Update(double dAppTime)
{
	if (m_ShadowSceneNameArray.GetNum() && ((SKLight *)(m_pNode.GetObject()))->GetLightType() != SKLight::LT_SKY)
	{
		SKLocalLight * pLocalLight = (SKLocalLight *)(m_pNode.GetObject());
		pLocalLight->ClearAllShadowScene();
		for (unsigned int i = 0; i < m_ShadowSceneNameArray.GetNum(); i++)
		{
			SKSceneMap *pSceneMap = SKWorld::ms_pWorld->GetScene(m_ShadowSceneNameArray[i]);
			pLocalLight->AddShadowScene(pSceneMap->GetScene());
		}
	}
}
void SKLightActor::AddToSceneMap(SKSceneMap * pSceneMap)
{
	SKActor::AddToSceneMap(pSceneMap);
	if (pSceneMap)
	{
		m_ShadowSceneNameArray.AddElement(m_pSceneMap->m_Name);
	}
	else
	{
		m_ShadowSceneNameArray.Clear();
	}
}
IMPLEMENT_RTTI(SKDirectionLightActor,SKLightActor)
BEGIN_ADD_PROPERTY(SKDirectionLightActor,SKLightActor)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKDirectionLightActor)
IMPLEMENT_INITIAL_END
SKDirectionLightActor::SKDirectionLightActor()
{
	
}
SKDirectionLightActor::~SKDirectionLightActor()
{

}
void SKDirectionLightActor::CreateDefaultComponentNode()
{
	m_pNode = SKNodeComponent::CreateComponet<SKDirectionLight>();
}

IMPLEMENT_RTTI(SKPointLightActor,SKLightActor)
BEGIN_ADD_PROPERTY(SKPointLightActor,SKLightActor)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKPointLightActor)
IMPLEMENT_INITIAL_END
SKPointLightActor::SKPointLightActor()
{

}
SKPointLightActor::~SKPointLightActor()
{

}
void SKPointLightActor::CreateDefaultComponentNode()
{
	m_pNode = SKNodeComponent::CreateComponet<SKPointLight>();
}
IMPLEMENT_RTTI(SKSpotLightActor,SKLightActor)
BEGIN_ADD_PROPERTY(SKSpotLightActor,SKLightActor)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKSpotLightActor)
IMPLEMENT_INITIAL_END
SKSpotLightActor::SKSpotLightActor()
{

}
SKSpotLightActor::~SKSpotLightActor()
{

}
void SKSpotLightActor::CreateDefaultComponentNode()
{
	m_pNode = SKNodeComponent::CreateComponet<SKSpotLight>();
}
IMPLEMENT_RTTI(SKSkyLightActor,SKLightActor)
BEGIN_ADD_PROPERTY(SKSkyLightActor,SKLightActor)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKSkyLightActor)
IMPLEMENT_INITIAL_END
SKSkyLightActor::SKSkyLightActor()
{

}
SKSkyLightActor::~SKSkyLightActor()
{

}
void SKSkyLightActor::CreateDefaultComponentNode()
{
	m_pNode = SKNodeComponent::CreateComponet<SKSkyLight>();
}

