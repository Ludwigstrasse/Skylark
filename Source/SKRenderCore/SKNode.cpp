#include "SKNode.h"
#include "SKMath.h"
#include "SKGeometry.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
#include "SKNodeComponent.h"
using namespace SKEngine2;
IMPLEMENT_RTTI_NoCreateFun(SKNode, SKSpatial)
BEGIN_ADD_PROPERTY(SKNode,SKSpatial)
REGISTER_PROPERTY(m_pChild, Child, SKProperty::F_SAVE_LOAD_CLONE  | SKProperty::F_REFLECT_NAME)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(SKNode)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
SKNode::SKNode()
{
	DeleteAllChild();

}
SKNode::~SKNode()
{
	m_pChild.Clear();
}

unsigned int SKNode::AddChild(SKSpatial * pChild)
{
	if(!pChild)
		return SKMAX_INTEGER;
	for (unsigned int i = 0 ; i < m_pChild.GetNum() ; i++)
	{
		if (m_pChild[i] == pChild)
		{
			return i;
		}
	}
	pChild->SetParent(this);	
	m_pChild.AddElement(pChild);
	return m_pChild.GetNum() - 1;
}
unsigned int SKNode::DeleteChild(SKSpatial *pChild)
{
	if(!pChild)
		return SKMAX_INTEGER;
	for(unsigned int i = 0 ; i < m_pChild.GetNum() ; i++)
	{
		
		if(m_pChild[i] == pChild)
		{
			
			pChild->SetParent(NULL);
			m_pChild.Erase(i);
			return i;
		}
	}

	return SKMAX_INTEGER;
}
bool SKNode::DeleteChild(unsigned int i)
{
	if(i >= m_pChild.GetNum())
		return 0;
	SKSpatial * Temp = m_pChild[i];
	
	Temp->SetParent(NULL);
	m_pChild.Erase(i);


	return 1;
}

SKSpatial * SKNode::GetChild(unsigned int i)const
{
	if(i >= (unsigned int)m_pChild.GetNum())
		return NULL;
	return m_pChild[i];
}

void SKNode::DeleteAllChild()
{
	
	for(unsigned int i = 0 ; i < m_pChild.GetNum() ; i++)
	{
		m_pChild[i]->SetParent(NULL);
	}
	m_pChild.Clear();
}
void SKNode::CreateLocalAABB()
{
	for (unsigned int i = 0; i < m_pChild.GetNum(); i++) 
	{ 	
		if(m_pChild[i])
		{
			m_pChild[i]->CreateLocalAABB();
		}

	} 

}
void SKNode::UpdateWorldBound(double dAppTime)
{
	bool bFoundFirstBound = false; 
	for (unsigned int i = 0; i < m_pChild.GetNum(); i++) 
	{ 	
		if(m_pChild[i])
		{
			if(!bFoundFirstBound)
			{
				m_WorldBV = m_pChild[i]->m_WorldBV;
				bFoundFirstBound = true; 
			}
			else
			{

				m_WorldBV = m_WorldBV.MergAABB(m_pChild[i]->m_WorldBV);
			}
		}

	} 
	if (!bFoundFirstBound)
	{
		SKREAL fA[3];
		m_WorldBV.GetfA(fA);
		m_WorldBV.Set(GetWorldTranslate(), fA);
	}
	if (m_pParent)
	{
		m_pParent->m_bIsChanged = true;
	}
	
}
void SKNode::UpdateNodeAll(double dAppTime)
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


	if(m_bIsChanged)
	{
		UpdateWorldBound(dAppTime); 
	}
	m_bIsChanged = false;
	
}
void SKNode::ComputeNodeVisibleSet(SKCuller & Culler,bool bNoCull,double dAppTime)
{

	UpDateView(Culler,dAppTime);

	for (unsigned int i = 0; i < m_pChild.GetNum(); i++)
	{
		if (m_pChild[i])
		{

			m_pChild[i]->ComputeVisibleSet(Culler, bNoCull, dAppTime);

		}

	}
}

void SKNode::SetIsVisibleUpdate(bool bIsVisibleUpdate)
{
	for(unsigned int i = 0 ; i < m_pChild.GetNum() ; i++)
	{

		m_pChild[i]->SetIsVisibleUpdate(bIsVisibleUpdate);

	}
	m_bIsVisibleUpdate = bIsVisibleUpdate;
	if (!m_bIsVisibleUpdate)
	{
		m_bEnable = true;
	}
}
