#include "SKModelSwitchNode.h"
#include "SKGraphicInclude.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKModelSwitchNode,SKSwitchNode)
BEGIN_ADD_PROPERTY(SKModelSwitchNode,SKSwitchNode)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKModelSwitchNode)
IMPLEMENT_INITIAL_END
SKModelSwitchNode::SKModelSwitchNode()
{

}
SKModelSwitchNode::~SKModelSwitchNode()
{

}
SKGeometryNode * SKModelSwitchNode::GetGeometryNode(unsigned int uiLodLevel)
{
	if (uiLodLevel >= m_pChild.GetNum())
	{
		return NULL;
	}
	else
	{
		SKGeometryNode * pGeometryNode = DynamicCast<SKGeometryNode>(m_pChild[uiLodLevel]);
		return pGeometryNode;
	}
}
void SKModelSwitchNode::UpDateView(SKCuller & Culler, double dAppTime)
{
	SKSwitchNode::UpDateView(Culler, dAppTime);
	SKCamera * pCamera = Culler.GetCamera();
	if (!pCamera)
	{
		return;
	}
	if (Culler.GetCullerType() == SKCuller::CUT_MAIN)
	{
		SKREAL ZFar = pCamera->GetZFar();
		SKVector3 DistVector = pCamera->GetWorldTranslate() - GetWorldTranslate();
		SKREAL Dist = DistVector.GetLength();
		SKREAL LastTemp = ZFar * 0.5f;
		SKREAL CurTemp = LastTemp;

		m_uiActiveNode = 0;
		while (true)
		{
			if (Dist > CurTemp)
			{		
				if (m_uiActiveNode + 1 >= m_pChild.GetNum() || m_pChild[m_uiActiveNode + 1] == NULL)
				{
					break;
				}
				LastTemp = LastTemp * 0.5f;
				CurTemp = CurTemp + LastTemp;
				m_uiActiveNode++;
			}
			else
			{
				break;
			}
		}
	}


}