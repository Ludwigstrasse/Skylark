#include "SKModelMeshNode.h"
#include "SKSwitchNode.h"
#include "SKGraphicInclude.h"
#include "SKModelSwitchNode.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKModelMeshNode,SKMeshNode)
BEGIN_ADD_PROPERTY(SKModelMeshNode,SKMeshNode)
REGISTER_PROPERTY(m_uiLodType,LodType,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pMorphTree, MorphTree, SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKModelMeshNode)
IMPLEMENT_INITIAL_END
SKModelMeshNode::SKModelMeshNode()
{
	m_uiLodType = 0;
	m_pMorphTree = NULL;
	m_pMorphTreeInstance = NULL;
}
SKModelMeshNode::~SKModelMeshNode()
{
	m_pMorphTree = NULL;
	m_pMorphTreeInstance = NULL;
}
SKGeometryNode * SKModelMeshNode::GetGeometryNode(unsigned int uiLodLevel)
{
	if (m_pChild.GetNum() > 1)
	{
		return NULL;
	}
	else
	{
		SKModelSwitchNode* Temp = DynamicCast<SKModelSwitchNode>(m_pChild[0]);
		if (!Temp)
		{
			SKGeometryNode * pGeometryNode = DynamicCast<SKGeometryNode>(m_pChild[0]);
			return pGeometryNode;
		}
		else
		{
			return	Temp->GetGeometryNode(uiLodLevel);
		}
	}
}
SKSwitchNode * SKModelMeshNode::GetDlodNode()const
{
	if (m_pChild.GetNum())
	{
		return DynamicCast<SKSwitchNode>(m_pChild[0]);
	}
	return NULL;
}
void SKModelMeshNode::SetMorphTree(SKMorphTreeR * pMorphTree)
{
	if (pMorphTree)
	{
		m_bIsStatic = false;
		m_pMorphTree = pMorphTree;
		m_pMorphTree->AddLoadEventObject(this);
	}
}
void SKModelMeshNode::LoadedEvent(SKResourceProxyBase * pResourceProxy, int Data)
{

	if (m_pMorphTree == pResourceProxy)
	{
		m_pMorphTreeInstance = (SKMorphTree *)SKObject::CloneCreateObject(m_pMorphTree->GetResource());
		m_pMorphTreeInstance->SetObject(this);
	}
}
bool SKModelMeshNode::PostLoad(void * pDate)
{
	SKMeshNode::PostLoad(pDate);
	if (m_pMorphTree)
	{
		m_pMorphTree->AddLoadEventObject(this);
	}

	return true;
}
bool SKModelMeshNode::PostClone(SKObject * pObjectSrc)
{
	SKMeshNode::PostClone(pObjectSrc);
	if (m_pMorphTree)
	{
		m_pMorphTree->AddLoadEventObject(this);
	}
	return true;
}
void SKModelMeshNode::UpdateController(double dAppTime)
{
	SKMeshNode::UpdateController(dAppTime);

	if (m_pMorphTreeInstance && m_bEnable)
	{
		m_pMorphTreeInstance->Update(dAppTime);
	}
}
void SKModelMeshNode::SetMorphTreeNodePara(const SKUsedName & ShowName, void * pPara)
{
	if (m_pMorphTreeInstance)
	{
		m_pMorphTreeInstance->SetNodePara(ShowName, pPara);
	}
}