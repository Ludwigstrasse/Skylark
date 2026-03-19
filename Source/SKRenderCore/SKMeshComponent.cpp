#include "SKMeshComponent.h"
#include "SKGraphicInclude.h"
using namespace SKEngine2;
IMPLEMENT_RTTI_NoCreateFun(SKMeshComponent, SKNodeComponent)
BEGIN_ADD_PROPERTY(SKMeshComponent, SKNodeComponent)
REGISTER_PROPERTY(m_pNode, pNode, SKProperty::F_CLONE | SKProperty::F_NO_USE_GC)
REGISTER_PROPERTY(m_UseID, UseID, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_bCastShadow, bCastShadow, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(SKMeshComponent)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
SKMeshComponent::SKMeshComponent()
{
	m_pNode = NULL;
	m_bCastShadow = true;
}
SKMeshComponent::~SKMeshComponent()
{

}
void SKMeshComponent::CreateLocalAABB()
{
	if (!m_pNode)
	{
		return;
	}

	m_pNode->CreateLocalAABB();

}
void SKMeshComponent::UpdateWorldBound(double dAppTime)
{
	if (!m_pNode)
	{
		return;
	}

	m_WorldBV = m_pNode->GetWorldAABB();
	if (m_pParent)
	{
		m_pParent->m_bIsChanged = true;;
	}
	
}
void SKMeshComponent::UpdateNodeAll(double dAppTime)
{

	if (dAppTime > 0.0f)
	{
		UpdateController(dAppTime);
	}

	UpdateTransform(dAppTime);


	if (m_pNode)
		m_pNode->UpdateAll(dAppTime);


	UpdateLightState(dAppTime);

	UpdateCameraState(dAppTime);

	if (m_bIsChanged)
	{
		UpdateWorldBound(dAppTime);
	}
	m_bIsChanged = false;

}
void SKMeshComponent::ComputeNodeVisibleSet(SKCuller & Culler, bool bNoCull, double dAppTime)
{

	UpDateView(Culler, dAppTime);

	if (m_pNode)
	{
		//set use material before cull geometry
		SKGeometryNode * pGeometryNode = m_pNode->GetGeometryNode(0);
		for (unsigned int i = 0; i < pGeometryNode->GetNormalGeometryNum() && i < m_UseID.GetNum(); i++)
		{
			pGeometryNode->GetGeometry(i)->SetUseMaterialInstance(m_UseID[i]);
		}

		m_pNode->ComputeVisibleSet(Culler, bNoCull, dAppTime);

		
	}

}

void SKMeshComponent::SetIsVisibleUpdate(bool bIsVisibleUpdate)
{
	if (m_pNode)
	{
		m_pNode->SetIsVisibleUpdate(bIsVisibleUpdate);

	}

	m_bIsVisibleUpdate = bIsVisibleUpdate;
	if (!m_bIsVisibleUpdate)
	{
		m_bEnable = true;
	}
}
void SKMeshComponent::SetIsDrawBoundVolume(bool bIsDrawBoundVolume)
{
	if (m_pNode)
	{
		m_pNode->SetIsDrawBoundVolume(bIsDrawBoundVolume);
	}
}
void SKMeshComponent::SetMorphTreeNodePara(const SKUsedName & ShowName, void * pPara)
{
	SKModelMeshNode * pModelMeshNode = DynamicCast<SKModelMeshNode>(m_pNode);
	if (pModelMeshNode)
	{
		return pModelMeshNode->SetMorphTreeNodePara(ShowName, pPara);
	}
}
void SKMeshComponent::CastShadow(bool bCastShadow)
{
	if (m_pNode)
	{
		m_bCastShadow = bCastShadow;
		m_pNode->m_bCastShadow = bCastShadow;
	}
}
void SKMeshComponent::SetPostLoadNodeParam()
{
	CastShadow(m_bCastShadow);
}
void SKMeshComponent::ResetUseID()
{
	//get lod 0
	if (m_UseID.GetNum() == 0)
	{
		SKGeometryNode * pGeometryNode = m_pNode->GetGeometryNode(0);
		m_UseID.SetBufferNum(pGeometryNode->GetNormalGeometryNum());
		for (unsigned int i = 0; i < m_UseID.GetNum(); i++)
		{
			m_UseID[i] = 0;
		}
	}
}
SKMaterialInstance * SKMeshComponent::GetUseMaterialInstance(unsigned int SubMeshID)const
{
	if (SubMeshID >= m_UseID.GetNum())
	{
		return NULL;
	}
	SKGeometryNode * pGeometryNode = m_pNode->GetGeometryNode(0);
	
	if (SubMeshID >= pGeometryNode->GetNormalGeometryNum())
	{
		return NULL;
	}

	return pGeometryNode->GetGeometry(SubMeshID)->GetUseMaterialInstance();
}

SKMaterialInstance * SKMeshComponent::GetMaterialInstance(unsigned int SubMeshID, unsigned int UseID)const
{

	if (SubMeshID >= m_UseID.GetNum())
	{
		return NULL;
	}
	SKGeometryNode * pGeometryNode = m_pNode->GetGeometryNode(0);

	if (SubMeshID >= pGeometryNode->GetNormalGeometryNum())
	{
		return NULL;
	}

	return pGeometryNode->GetGeometry(SubMeshID)->GetMaterialInstance(UseID);
}

bool SKMeshComponent::SetUseMaterialInstance(unsigned int SubMeshID, unsigned int UseID)
{
	if (SubMeshID >= m_UseID.GetNum())
	{
		return false;
	}
	m_UseID[SubMeshID] = UseID;
	return true;
}
unsigned int SKMeshComponent::GetSubMeshNum()
{
	SKGeometryNode * pGeometryNode = m_pNode->GetGeometryNode(0);

	return pGeometryNode->GetNormalGeometryNum();
}
void SKMeshComponent::DelayUpdate()
{
	m_pSaveNode = NULL;
}