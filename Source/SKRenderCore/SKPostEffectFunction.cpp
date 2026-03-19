#include "SKPostEffectFunction.h"
#include "SKPostEffectSet.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI_NoCreateFun(SKPostEffectFunction,SKObject)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(SKPostEffectFunction)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(SKPostEffectFunction,SKObject)
REGISTER_PROPERTY(m_ShowName,ShowName,SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_bEnable,Enable,SKProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_pOwner,Owner,SKProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_pInput,Input,SKProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_pOutput,Output,SKProperty::F_SAVE_LOAD_CLONE);
END_ADD_PROPERTY
SKPostEffectFunction::SKPostEffectFunction()
{
	m_pInput.Clear();
	m_pOutput.Clear();
	m_bIsVisited = 0;
	m_pOwner = NULL;
	m_bEnable = true;
	m_bLastOne = false;
}
SKPostEffectFunction::SKPostEffectFunction(const SKUsedName & ShowName,SKPostEffectSet * pPostEffectSet)
{
	m_ShowName = ShowName;
	m_pInput.Clear();
	m_pOutput.Clear();
	SKMAC_ASSERT(pPostEffectSet);
	m_pOwner = pPostEffectSet;
	m_pOwner->AddPostEffectFunction(this);
	m_bEnable = true;
}
SKPostEffectFunction::~SKPostEffectFunction()
{
	for(unsigned int i = 0 ; i < m_pInput.GetNum() ; i++)
	{
		SKMAC_DELETE(m_pInput[i]);
	}
	for(unsigned int i = 0 ; i < m_pOutput.GetNum() ; i++)
	{
		SKMAC_DELETE(m_pOutput[i]);
	}
	m_pOwner->DeletePostEffectFunction(this);
	m_pPostEffectRender = NULL;
}

SKInputNode * SKPostEffectFunction::GetInputNode(unsigned int uiNodeID)const
{
	if(uiNodeID >=m_pInput.GetNum())
		return NULL;
	return m_pInput[uiNodeID];
}
SKInputNode * SKPostEffectFunction::GetInputNode(const SKString & NodeName)const
{
	for (unsigned int i  =0 ; i < m_pInput.GetNum(); i++)
	{
		if(m_pInput[i]->GetNodeName() == NodeName)
			return m_pInput[i];
	}
	return NULL;
}

SKOutputNode * SKPostEffectFunction::GetOutputNode(unsigned int uiNodeID)const
{
	if(uiNodeID >=m_pOutput.GetNum())
		return NULL;
	return m_pOutput[uiNodeID];
}
SKOutputNode * SKPostEffectFunction::GetOutputNode(const SKString & NodeName)const
{
	for (unsigned int i  =0 ; i < m_pInput.GetNum(); i++)
	{
		if(m_pOutput[i]->GetNodeName() == NodeName)
			return m_pOutput[i];
	}
	return NULL;
}
bool SKPostEffectFunction::ClearFlag()
{
	if(m_bIsVisited == 0)
		return 1;
	else
	{
		m_bIsVisited = 0;
		for(unsigned int i = 0 ; i < m_pInput.GetNum(); i++)
		{
			if(m_pInput[i]->GetOutputLink() == NULL)
				continue;
			else
			{
				((SKPostEffectFunction *)m_pInput[i]->GetOutputLink()->GetOwner())->ClearFlag();

			}

		}
		return 1;
	}
}