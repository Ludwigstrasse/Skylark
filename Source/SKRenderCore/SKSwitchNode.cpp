#include "SKSwitchNode.h"
#include "SKGraphicInclude.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKSwitchNode,SKNode)
BEGIN_ADD_PROPERTY(SKSwitchNode,SKNode)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKSwitchNode)
IMPLEMENT_INITIAL_END
SKSwitchNode::SKSwitchNode()
{
	m_uiActiveNode = 0;
}
SKSwitchNode::~SKSwitchNode()
{

}

void SKSwitchNode::ComputeNodeVisibleSet(SKCuller & Culler,bool bNoCull,double dAppTime)
{

	UpDateView(Culler,dAppTime);
	if (m_uiActiveNode < m_pChild.GetNum())
	{
		if(m_pChild[m_uiActiveNode])
		{
			m_pChild[m_uiActiveNode]->ComputeVisibleSet(Culler,bNoCull,dAppTime);

		}
	}
}
void SKSwitchNode::UpdateWorldBound(double dAppTime)
{
	if (m_uiActiveNode < m_pChild.GetNum())
	{
		if(m_pChild[m_uiActiveNode])
			m_WorldBV = m_pChild[m_uiActiveNode]->m_WorldBV; 
		if (m_pParent)
		{
			m_pParent->m_bIsChanged = true;
		}
	}

}
SKSpatial * SKSwitchNode::GetActiveNode()const
{
	if (m_uiActiveNode < m_pChild.GetNum())
	{
		return m_pChild[m_uiActiveNode];
	}
	return NULL;
}

