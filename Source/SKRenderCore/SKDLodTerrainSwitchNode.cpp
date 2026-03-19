#include "SKDLodTerrainSwitchNode.h"
#include "SKDLodTerrainNode.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKDLodTerrainSwitchNode,SKSwitchNode)
BEGIN_ADD_PROPERTY(SKDLodTerrainSwitchNode,SKSwitchNode)
REGISTER_PROPERTY(m_uiIndexXInTerrain,IndexXInTerrain,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiIndexZInTerrain,IndexZInTerrain,SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKDLodTerrainSwitchNode)
IMPLEMENT_INITIAL_END
SKDLodTerrainSwitchNode::SKDLodTerrainSwitchNode(unsigned int uiIndexXInTerrain,unsigned int uiIndexZInTerrain)
{
	m_uiIndexXInTerrain = uiIndexXInTerrain;
	m_uiIndexZInTerrain = uiIndexZInTerrain;
}
SKDLodTerrainSwitchNode::SKDLodTerrainSwitchNode()
{
	
}
SKDLodTerrainSwitchNode::~SKDLodTerrainSwitchNode()
{

}
void SKDLodTerrainSwitchNode::UpDateView(SKCuller & Culler,double dAppTime)
{
	SKSwitchNode::UpDateView(Culler,dAppTime);
	SKCamera * pCamera = Culler.GetCamera();
	if (!pCamera)
	{
		return;
	}
	if (Culler.GetCullerType() == SKCuller::CUT_MAIN)
	{
		SKDLodTerrainNode * pTerrainNode = DynamicCast<SKDLodTerrainNode>(m_pParent);
		if (!pTerrainNode)
		{
			return ;
		}
		SKTransform Tran = pTerrainNode->GetWorldTransform();
		SKVector3 Loc = pCamera->GetWorldTranslate() * Tran.GetCombineInverse();
		unsigned int uiLength = 1 << pTerrainNode->GetTesselationLevel();
		SKVector3 Pos;
		unsigned int uiIndenX = uiLength * m_uiIndexXInTerrain;
		unsigned int uiIndenZ = uiLength * m_uiIndexZInTerrain;
		Pos.x = SKTerrainNode::WIDTH_SCALE * (uiIndenX + (uiLength >> 1)) * 1.0f;
		Pos.z = SKTerrainNode::WIDTH_SCALE * (uiIndenZ + (uiLength >> 1)) * 1.0f;
		Pos.y = pTerrainNode->GetHeight(uiIndenX,uiIndenZ);

		SKVector3 Length = Loc - Pos;
		SKREAL fSqrLen = Length.GetSqrLength();
		SKREAL fDLodScale = pTerrainNode->GetDLodScale();
		for (unsigned int i = 0 ; i < m_pChild.GetNum() ; i++)
		{
			m_uiActiveNode = i;
			if (fSqrLen < (1 << i) * fDLodScale)
			{		
				break;
			}
		}
	}


}
