#include "SKMorphBaseFunction.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI_NoCreateFun(SKMorphBaseFunction,SKController)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(SKMorphBaseFunction)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(SKMorphBaseFunction,SKController)
REGISTER_PROPERTY(m_ShowName,ShowName,SKProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_pInput,pInput,SKProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_pOutput,Output,SKProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_pOwner,Owner,SKProperty::F_SAVE_LOAD_CLONE);
END_ADD_PROPERTY
SKMorphBaseFunction::SKMorphBaseFunction()
{
	m_pInput.Clear();
	m_pOutput.Clear();
	m_pOwner = NULL;
	m_bIsVisited = 0;
}
SKMorphBaseFunction::SKMorphBaseFunction(const SKUsedName & ShowName,SKMorphTree * pMorphTree)
{
	m_ShowName = ShowName;
	m_pInput.Clear();
	m_pOutput.Clear();
	SKMAC_ASSERT(pMorphTree);
	m_pOwner = pMorphTree;
	m_pOwner->AddMorphFunction(this);
	m_bIsVisited = 0;

}
SKMorphBaseFunction::~SKMorphBaseFunction()
{
	for(unsigned int i = 0 ; i < m_pInput.GetNum() ; i++)
	{
		SKMAC_DELETE(m_pInput[i]);
	}
	for(unsigned int i = 0 ; i < m_pOutput.GetNum() ; i++)
	{
		SKMAC_DELETE(m_pOutput[i]);
	}
	m_pOwner->DeleteMorphFunction(this);
}
bool SKMorphBaseFunction::Update(double dAppTime)
{
	if(!SKController::Update(dAppTime))
		return false;
	if(m_bIsVisited)
		return false;
	m_bIsVisited = 1;
	for (unsigned int i = 0 ; i < m_pInput.GetNum() ;i++)
	{
		if(m_pInput[i]->GetOutputLink())
		{
			SKMorphBaseFunction *pMorphBaseFunction = (SKMorphBaseFunction *)m_pInput[0]->GetOutputLink()->GetOwner();
			if(pMorphBaseFunction)
			{
				pMorphBaseFunction->Update(dAppTime);
			}
		}
	}
	return true;

}
SKInputNode * SKMorphBaseFunction::GetInputNode(unsigned int uiNodeID)const
{
	if(uiNodeID >=m_pInput.GetNum())
		return NULL;
	return m_pInput[uiNodeID];

}
SKInputNode * SKMorphBaseFunction::GetInputNode(const SKString & NodeName)const
{
	for (unsigned int i  =0 ; i < m_pInput.GetNum(); i++)
	{
		if(m_pInput[i]->GetNodeName() == NodeName)
			return m_pInput[i];
	}
	return NULL;
}

SKOutputNode * SKMorphBaseFunction::GetOutputNode(unsigned int uiNodeID)const
{
	if(uiNodeID >=m_pOutput.GetNum())
		return NULL;
	return m_pOutput[uiNodeID];
}
SKOutputNode * SKMorphBaseFunction::GetOutputNode(const SKString & NodeName)const
{
	for (unsigned int i  =0 ; i < m_pInput.GetNum(); i++)
	{
		if(m_pOutput[i]->GetNodeName() == NodeName)
			return m_pOutput[i];
	}
	return NULL;
}
void SKMorphBaseFunction::ClearFlag()
{
	m_bIsVisited = 0;
}