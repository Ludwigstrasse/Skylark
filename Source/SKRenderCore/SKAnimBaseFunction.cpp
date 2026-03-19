#include "SKAnimBaseFunction.h"
#include "SKBoneNode.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI_NoCreateFun(SKAnimBaseFunction,SKController)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(SKAnimBaseFunction)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(SKAnimBaseFunction,SKController)
REGISTER_PROPERTY(m_ShowName,ShowName,SKProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_pInput,Input,SKProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_pOutput,pOutput,SKProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_pOwner,pOwner,SKProperty::F_SAVE_LOAD_CLONE);
END_ADD_PROPERTY
SKAnimBaseFunction::SKAnimBaseFunction()
{
	m_pInput.Clear();
	m_pOutput.Clear();
	m_pOwner = NULL;
	m_bIsVisited = 0;
}
SKAnimBaseFunction::SKAnimBaseFunction(const SKUsedName & ShowName,SKAnimTree * pAnimTree)
{
	m_ShowName = ShowName;
	m_pInput.Clear();
	m_pOutput.Clear();
	SKMAC_ASSERT(pAnimTree);
	m_pOwner = pAnimTree;

	m_pOwner->AddAnimFunction(this);
	m_bIsVisited = 0;

}
SKAnimBaseFunction::~SKAnimBaseFunction()
{
	for(unsigned int i = 0 ; i < m_pInput.GetNum() ; i++)
	{
		SKMAC_DELETE(m_pInput[i]);
	}
	for(unsigned int i = 0 ; i < m_pOutput.GetNum() ; i++)
	{
		SKMAC_DELETE(m_pOutput[i]);
	}
	if (m_pOwner)
	{
		m_pOwner->DeleteAnimFunction(this);
	}
	
}
bool SKAnimBaseFunction::Update(double dAppTime)
{
	if (m_bIsVisited)
		return false;
	m_bIsVisited = 1;

	if (!SKController::Update(dAppTime))
		return false;

	return true;
}
SKSkelectonMeshNode * SKAnimBaseFunction::GetSkelectonMeshNode()const
{
	return DynamicCast<SKSkelectonMeshNode>(m_pObject);
}
SKInputNode * SKAnimBaseFunction::GetInputNode(unsigned int uiNodeID)const
{
	if(uiNodeID >=m_pInput.GetNum())
		return NULL;
	return m_pInput[uiNodeID];
	
}
SKInputNode * SKAnimBaseFunction::GetInputNode(const SKString & NodeName)const
{
	for (unsigned int i  =0 ; i < m_pInput.GetNum(); i++)
	{
		if(m_pInput[i]->GetNodeName() == NodeName)
			return m_pInput[i];
	}
	return NULL;
}

SKOutputNode * SKAnimBaseFunction::GetOutputNode(unsigned int uiNodeID)const
{
	if(uiNodeID >=m_pOutput.GetNum())
		return NULL;
	return m_pOutput[uiNodeID];
}
SKOutputNode * SKAnimBaseFunction::GetOutputNode(const SKString & NodeName)const
{
	for (unsigned int i  =0 ; i < m_pInput.GetNum(); i++)
	{
		if(m_pOutput[i]->GetNodeName() == NodeName)
			return m_pOutput[i];
	}
	return NULL;
}

void SKAnimBaseFunction::ClearFlag()
{
	m_bIsVisited = false;
	m_bNoLeafStart = false;
}

bool SKAnimBaseFunction::IsLeafNode()
{
	return false;
}
void  SKAnimBaseFunction::GetLeafArray(SKArray<SKAnimBaseFunction *> & LeafNode)
{
	if (IsLeafNode())
	{
		LeafNode.AddElement(this);
	}
	for (unsigned int i = 0; i < m_pInput.GetNum(); i++)
	{
		if (m_pInput[i]->GetOutputLink())
		{
			SKAnimBaseFunction *pAnimBaseFunction = (SKAnimBaseFunction *)m_pInput[i]->GetOutputLink()->GetOwner();
			if (pAnimBaseFunction )
			{
				pAnimBaseFunction->GetLeafArray(LeafNode);
			}
		}
	}
}
void SKAnimBaseFunction::NoLeafStart()
{
	m_bNoLeafStart = true;
}
void SKAnimBaseFunction::EnableLeafStart()
{
	if (m_bNoLeafStart)
	{
		return;
	}
	else
	{
		m_bStart = false;
	}
}