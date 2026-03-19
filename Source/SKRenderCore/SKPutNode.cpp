#include "SKPutNode.h"
#include "SKShaderFunction.h"
#include "SKStream.h"
#include "SKGraphicInclude.h"
using namespace SKEngine2;
IMPLEMENT_RTTI_NoCreateFun(SKPutNode,SKObject)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(SKPutNode)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(SKPutNode,SKObject)
REGISTER_PROPERTY(m_NodeName,NodeName,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_uiValueType,ValueType,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_pOwner,Owner,SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
SKPutNode::SKPutNode(unsigned int uiValueType,const SKUsedName & NodeName,SKObject * pObject)
{
	m_pOwner = pObject;
	m_NodeName = NodeName;
	m_uiValueType = uiValueType;
}
SKPutNode::~SKPutNode()
{
	m_pOwner = NULL;
}
SKPutNode::SKPutNode()
{
	m_pOwner = NULL;
	m_NodeName = _T("");
	m_uiValueType = VT_1;

}
IMPLEMENT_RTTI(SKInputNode,SKPutNode)
BEGIN_ADD_PROPERTY(SKInputNode,SKPutNode)
REGISTER_PROPERTY(m_pOutputLink,OutputLink,SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKInputNode)
IMPLEMENT_INITIAL_END

SKInputNode::SKInputNode(unsigned int uiValueType,const SKUsedName &NodeName,SKObject * pObject)
			:SKPutNode(uiValueType,NodeName,pObject)
{
	m_pOutputLink = NULL;

}
SKInputNode::~SKInputNode()
{
	m_pOutputLink = NULL;
}
SKInputNode::SKInputNode()
{
	m_pOutputLink = NULL;
}


IMPLEMENT_RTTI(SKOutputNode,SKPutNode)
BEGIN_ADD_PROPERTY(SKOutputNode,SKPutNode)
REGISTER_PROPERTY(m_pInputLink,InputLink,SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKOutputNode)
IMPLEMENT_INITIAL_END


SKOutputNode::SKOutputNode(unsigned int uiValueType,const SKUsedName &NodeName,SKObject * pObject)
			:SKPutNode(uiValueType,NodeName,pObject)
{

	m_pInputLink.Clear();
}
SKOutputNode::~SKOutputNode()
{
	m_pInputLink.Clear();
}
SKOutputNode::SKOutputNode()
{
	m_pInputLink.Clear();
}
bool SKOutputNode::CheckIsConnection(SKInputNode * pInputNode)const
{
	if(!pInputNode)
		return 0;
	for(unsigned int i = 0 ; i < m_pInputLink.GetNum() ; i++)
	{
		if(m_pInputLink[i] == pInputNode)
			return 0;
	
	}
	return 1;
}
bool SKInputNode::Connection(SKOutputNode * pOutputNode,bool bIsJudgeType)
{
	if(!pOutputNode)
		return 0;
	if(bIsJudgeType)
	{
		if (m_uiValueType != pOutputNode->GetValueType())
		{
			return 0;
		}
	}
	m_pOutputLink = pOutputNode;
	if(pOutputNode->CheckIsConnection(this))
		pOutputNode->m_pInputLink.AddElement(this);
	return 1;
}
bool SKOutputNode::Connection(SKInputNode * pInputNode,bool bIsJudgeType)
{
	if(!pInputNode)
		return 0;
	if(bIsJudgeType)
	{
		if (m_uiValueType != pInputNode->GetValueType())
		{
			return 0;
		}
	}
	if(CheckIsConnection(pInputNode))
		m_pInputLink.AddElement(pInputNode);
	pInputNode->m_pOutputLink = this;
	return 1;
}
void SKInputNode::UnConnection()
{
	if(!m_pOutputLink)
		return;
	for(unsigned int i = 0 ; i < m_pOutputLink->m_pInputLink.GetNum() ; i++)
	{
		if(m_pOutputLink->m_pInputLink[i] == this)
		{
			m_pOutputLink->m_pInputLink.Erase(i);
			break;
		}
	}
	m_pOutputLink = NULL;
}
void SKOutputNode::UnConnection()
{
	if(!m_pInputLink.GetNum())
		return ;
	for(unsigned int i = 0 ; i < m_pInputLink.GetNum() ; i++)
	{
		if(m_pInputLink[i])
			m_pInputLink[i]->m_pOutputLink = NULL;
	}
	m_pInputLink.Clear();
}

