#include "SKSkelectonMeshComponent.h"
#include "SKSkelectonMeshNode.h"
#include "SKGraphicInclude.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKSkelectonMeshComponent, SKMeshComponent)
BEGIN_ADD_PROPERTY(SKSkelectonMeshComponent, SKMeshComponent)
REGISTER_PROPERTY(m_pSkelectonMeshResource, SkelectonMeshResource, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKSkelectonMeshComponent)
IMPLEMENT_INITIAL_END
SKSkelectonMeshComponent::SKSkelectonMeshComponent()
{
	m_bIsStatic = false;
	m_pSkelectonMeshResource = SKResourceManager::ms_DefaultSkelectonMeshNodeResource;
}
SKSkelectonMeshComponent::~SKSkelectonMeshComponent()
{

}
bool SKSkelectonMeshComponent::BeforeSave(void * pDate)
{
	if (m_pSkelectonMeshResource == SKResourceManager::ms_DefaultSkelectonMeshNodeResource)
	{
		m_pSkelectonMeshResource = NULL;
	}
	return true;
}
void SKSkelectonMeshComponent::SetSkelectonMeshResource(SKSkelectonMeshNodeR * pSkelectonMeshResource)
{
	if (m_pSkelectonMeshResource == pSkelectonMeshResource)
	{
		return;
	}
	m_pSkelectonMeshResource = pSkelectonMeshResource;
	m_UseID.Clear();
	PostCreate();
}
void SKSkelectonMeshComponent::PostCreate()
{
	if (!m_pSkelectonMeshResource)
	{
		return;
	}
	LoadedEvent(NULL);
	if (!m_pSkelectonMeshResource->IsLoaded())
	{
		m_pSkelectonMeshResource->AddLoadEventObject(this);
	}
}
void SKSkelectonMeshComponent::LoadedEvent(SKResourceProxyBase * pResourceProxy, int Data)
{
	if (m_pSkelectonMeshResource)
	{
		if (m_pNode)
		{
			m_pNode->SetParent(NULL);
			m_pSaveNode = m_pNode;
			SKResourceManager::GetDelayUpdateObjectOneFrame().AddMethod<SKMeshComponent, &SKMeshComponent::DelayUpdate>(this);
		}	
		m_pNode = (SKModelMeshNode *)SKObject::CloneCreateObject(m_pSkelectonMeshResource->GetResource());
		m_pNode->SetParent(this);
		if (m_pSkelectonMeshResource->IsLoaded())
		{
			ResetUseID();
		}
		SetPostLoadNodeParam();
	}
}
void SKSkelectonMeshComponent::SetIsDrawSkelecton(bool bIsDrawSkelecton)
{
	SKMeshNode * pMeshNode = m_pNode;
	if (pMeshNode)
	{
		((SKSkelectonMeshNode *)pMeshNode)->SetIsDrawSkelecton(bIsDrawSkelecton);
	}
	
}
bool SKSkelectonMeshComponent::PlayAnim(const SKString & AnimName, SKREAL fRatio, unsigned int uiRepeatType)
{
	SKMeshNode * pMeshNode = m_pNode;
	if (pMeshNode)
	{
		((SKSkelectonMeshNode *)pMeshNode)->PlayAnim(AnimName,fRatio,uiRepeatType);
	}
	return true;
}
SKSocketNode * SKSkelectonMeshComponent::GetSocketNode(const SKUsedName & SocketName)
{
	SKSkelectonMeshNode * pSkelectonNode = (SKSkelectonMeshNode *)m_pNode.GetObject();
	if (pSkelectonNode)
	{
		return pSkelectonNode->GetSocket(SocketName);
	}
	return NULL;
}
void SKSkelectonMeshComponent::SetAnimTreeNodePara(const SKUsedName & ShowName, void * pPara)
{

	SKSkelectonMeshNode * pSkelectonNode = (SKSkelectonMeshNode *)m_pNode.GetObject();
	if (pSkelectonNode)
	{
		return pSkelectonNode->SetAnimTreeNodePara(ShowName, pPara);
	}
}
bool SKSkelectonMeshComponent::PostLoad(void * pDate)
{
	if (!SKMeshComponent::PostLoad(pDate))
	{
		return false;
	}
	PostCreate();
	return true;
}
