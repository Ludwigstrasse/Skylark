#include "SKNodeComponent.h"
#include "SKGraphicInclude.h"
#include "SKSkelectonMeshComponent.h"
#include "SKSocketNode.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKNodeComponent, SKNode)
BEGIN_ADD_PROPERTY(SKNodeComponent, SKNode)
REGISTER_PROPERTY(m_AttachSocketName, AttachSocketName, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKNodeComponent)
IMPLEMENT_INITIAL_END
SKNodeComponent::SKNodeComponent()
{
	m_pAttachSocket = NULL;
}
SKNodeComponent::~SKNodeComponent()
{
	
}
void SKNodeComponent::OnDestroy()
{
	
}
void SKNodeComponent::AttachParentSocket(const SKUsedName & AttackSocketName)
{
	SKSkelectonMeshComponent * pParent = DynamicCast<SKSkelectonMeshComponent>(m_pParent);
	if (pParent)
	{
		m_pAttachSocket = pParent->GetSocketNode(AttackSocketName);
		m_AttachSocketName = AttackSocketName;
	}
}
bool SKNodeComponent::PostLoad(void * pDate)
{
	SKSkelectonMeshComponent * pParent = DynamicCast<SKSkelectonMeshComponent>(m_pParent);
	if (pParent)
	{
		m_pAttachSocket = pParent->GetSocketNode(m_AttachSocketName);
	}
	return true;
}
bool SKNodeComponent::PostClone(SKObject * pObjectSrc)
{
	SKSkelectonMeshComponent * pParent = DynamicCast<SKSkelectonMeshComponent>(m_pParent);
	if (pParent)
	{
		m_pAttachSocket = pParent->GetSocketNode(m_AttachSocketName);
	}
	return true;
}
void SKNodeComponent::UpdateLightState(double dAppTime)
{
	if (m_pAllLight.GetNum() > 0)
		m_pAllLight.Clear();
	for (unsigned int i = 0; i < m_pChild.GetNum(); i++)
	{
		if (m_pChild[i])
		{
			if (m_pChild[i]->m_pAllLight.GetNum() > 0)
				m_pAllLight.AddElement(m_pChild[i]->m_pAllLight, 0, m_pChild[i]->m_pAllLight.GetNum() - 1);
		}
	}
}
void SKNodeComponent::UpdateCameraState(double dAppTime)
{
	if (m_pAllCamera.GetNum() > 0)
		m_pAllCamera.Clear();
	for (unsigned int i = 0; i < m_pChild.GetNum(); i++)
	{
		if (m_pChild[i])
		{
			if (m_pChild[i]->m_pAllCamera.GetNum() > 0)
				m_pAllCamera.AddElement(m_pChild[i]->m_pAllCamera, 0, m_pChild[i]->m_pAllCamera.GetNum() - 1);
		}
	}
}
void SKNodeComponent::UpdateNodeAll(double dAppTime)
{

	if (dAppTime > 0.0f)
	{
		UpdateController(dAppTime);
	}


	UpdateTransform(dAppTime);


	for (unsigned int i = 0; i < m_pChild.GetNum(); i++)
	{
		if (m_pChild[i])
			m_pChild[i]->UpdateNodeAll(dAppTime);
	}
	UpdateLightState(dAppTime);
	UpdateCameraState(dAppTime);

	if (m_bIsChanged)
	{
		UpdateWorldBound(dAppTime);
	}
	m_bIsChanged = false;

}
void SKNodeComponent::UpdateTransform(double dAppTime)
{
	if (m_pAttachSocket)
	{

		if (!m_pAttachSocket->m_bIsStatic)
		{
			m_bIsStatic = 0;
		}
		if (m_pAttachSocket->m_bIsChanged)
		{
			m_bIsChanged = true;
		}

		if (m_bIsChanged)
		{

			unsigned int TransFormFlag = ((unsigned int)m_bInheritScale) | ((unsigned int)m_bInheritRotate << 1) | ((unsigned int)m_bInheritTranlate << 2);
			m_World.Product(m_Local, m_pAttachSocket->m_World, TransFormFlag);
		}
	}
	else
	{
		SKNode::UpdateTransform(dAppTime);
		
	}
}

