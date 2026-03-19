#include "SKMeshNode.h"
#include "SKSwitchNode.h"
#include "SKGraphicInclude.h"
#include "SKDebugDraw.h"
#include "SKViewFamily.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKMeshNode,SKNode)
BEGIN_ADD_PROPERTY(SKMeshNode,SKNode)
REGISTER_PROPERTY(m_uiRenderGroup, m_RenderGroup, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_bReceiveShadow, ReceiveShadow, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
REGISTER_PROPERTY(m_bCastShadow, CastShadow, SKProperty::F_SAVE_LOAD_CLONE | SKProperty::F_REFLECT_NAME)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKMeshNode)
IMPLEMENT_INITIAL_END
SKMeshNode::SKMeshNode()
{
	m_bIsDrawBoundVolume = false;
	m_uiRenderGroup = SKCuller::RG_NORMAL;
	m_bIsVisibleUpdate = true;
	m_bReceiveShadow = true;
	m_bCastShadow = true;
	m_bLighted = true;
}
SKMeshNode::~SKMeshNode()
{

}
void SKMeshNode::UpDateView(SKCuller & Culler,double dAppTime)
{
	
	SKNode::UpDateView(Culler,dAppTime);
	SKCamera * pCamera = Culler.GetCamera();
	if (!pCamera)
	{
		return ;
	}
	if (Culler.GetCullerType() == SKCuller::CUT_MAIN)
	{
		if (m_bIsDrawBoundVolume)
		{
			for (unsigned int i = 0 ; i < pCamera->GetViewFamilyNum() ;i++)
			{
				SKViewFamily * pViewFamily = pCamera->GetViewFamily(i);
				if (pViewFamily)
				{
					SKSceneRenderMethod * pRM = pViewFamily->m_pSceneRenderMethod;
					SKDebugDraw * pDebugDraw = pRM->GetDebugDraw(m_uiRenderGroup);
					if (pDebugDraw)
					{
						pDebugDraw->AddDebugLineAABB(m_WorldBV,SKColorRGBA(1.0f,0.0f,0.0f,1.0f).GetDWABGR(),false);
					}
				}
			}		
		}
	}
}
void SKMeshNode::ComputeNodeVisibleSet(SKCuller & Culler,bool bNoCull,double dAppTime)
{
	if (!Culler.CullConditionNode(this))
	{
		SKNode::ComputeNodeVisibleSet(Culler,bNoCull,dAppTime);
	}
}