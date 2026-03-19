#include "SKStaticMeshComponent.h"
#include "SKStaticMeshNode.h"
#include "SKGraphicInclude.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKStaticMeshComponent, SKMeshComponent)
BEGIN_ADD_PROPERTY(SKStaticMeshComponent, SKMeshComponent)
REGISTER_PROPERTY(m_pStaticMeshResource, StaticMeshResource, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKStaticMeshComponent)
IMPLEMENT_INITIAL_END
SKStaticMeshComponent::SKStaticMeshComponent()
{
	m_pStaticMeshResource = SKResourceManager::ms_DefaultStaticMeshNodeResource;
}
SKStaticMeshComponent::~SKStaticMeshComponent()
{

}
bool SKStaticMeshComponent::BeforeSave(void * pDate)
{
	if (m_pStaticMeshResource == SKResourceManager::ms_DefaultStaticMeshNodeResource)
	{
		m_pStaticMeshResource = NULL;
	}
	return true;
}
void SKStaticMeshComponent::SetStaticMeshResouce(SKStaticMeshNodeR * pStaticMeshResource)
{
	if (m_pStaticMeshResource == pStaticMeshResource)
	{
		return;
	}
	m_pStaticMeshResource = pStaticMeshResource;
	m_UseID.Clear();
	PostCreate();
}
void SKStaticMeshComponent::PostCreate()
{
	if (!m_pStaticMeshResource)
	{
		return;
	}
	LoadedEvent(NULL);

	if (!m_pStaticMeshResource->IsLoaded())
	{
		m_pStaticMeshResource->AddLoadEventObject(this);
	}

}
void SKStaticMeshComponent::LoadedEvent(SKResourceProxyBase * pResourceProxy, int Data)
{
	if (m_pStaticMeshResource)
	{
		if (m_pNode)
		{
			m_pNode->SetParent(NULL);

			m_pSaveNode = m_pNode;
			SKResourceManager::GetDelayUpdateObjectOneFrame().AddMethod<SKMeshComponent, &SKMeshComponent::DelayUpdate>(this);
		}
		m_pNode = (SKModelMeshNode *)SKObject::CloneCreateObject(m_pStaticMeshResource->GetResource());
		m_pNode->SetParent(this);
		if (m_pStaticMeshResource->IsLoaded())
		{
			ResetUseID();
		}
		SetPostLoadNodeParam();
		m_bIsStatic = !m_pNode->IsDynamic();
	}
}
bool SKStaticMeshComponent::PostLoad(void * pDate)
{
	if (!SKMeshComponent::PostLoad(pDate))
	{
		return false;
	}
	PostCreate();
	return true;
}
