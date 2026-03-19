#include "SKOneParamMorphBlend.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKOneParamMorphBlend, SKMorphBlendFunction)
BEGIN_ADD_PROPERTY(SKOneParamMorphBlend, SKMorphBlendFunction)
REGISTER_PROPERTY(m_fParamMax, ParamMax, SKProperty::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_fParamMin, ParamMin, SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKOneParamMorphBlend)
IMPLEMENT_INITIAL_END
SKOneParamMorphBlend::~SKOneParamMorphBlend()
{

}
SKOneParamMorphBlend::SKOneParamMorphBlend()
{
	m_fParam = 0.0f;
	m_fParamMax = 1.0f;
	m_fParamMin = -1.0f;
	m_pMorphBaseFunction1 = NULL;
	m_pMorphBaseFunction2 = NULL;
	m_fWeight = 0.0f;
}
SKOneParamMorphBlend::SKOneParamMorphBlend(const SKUsedName & ShowName, SKMorphTree * pMorphTree)
:SKMorphBlendFunction(ShowName, pMorphTree)
{
	m_fParam = 0.0f;
	m_fParamMax = 1.0f;
	m_fParamMin = -1.0f;

	m_pMorphBaseFunction1 = NULL;
	m_pMorphBaseFunction2 = NULL;
	m_fWeight = 0.0f;
	SKString InputName0 = _T("Child0");
	SKInputNode * pInputNode = NULL;
	pInputNode = SK_NEW SKInputNode(SKPutNode::AVT_ANIM, InputName0, this);
	SKMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);


	SKString InputName1 = _T("Child1");
	pInputNode = NULL;
	pInputNode = SK_NEW SKInputNode(SKPutNode::AVT_ANIM, InputName1, this);
	SKMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);

}
void SKOneParamMorphBlend::AddInputNode()
{
	SKString InputName = _T("Child");
	SKString ID = IntToString(m_pInput.GetNum());
	InputName += ID;
	SKInputNode * pInputNode = NULL;
	pInputNode = SK_NEW SKInputNode(SKPutNode::AVT_ANIM, InputName, this);
	SKMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);

}
void SKOneParamMorphBlend::DeleteInputNode()
{
	if (m_pInput.GetNum() > 2)
	{
		SKInputNode * pInputNode = m_pInput[m_pInput.GetNum() - 1];

		m_pInput.Erase(m_pInput.GetNum() - 1);
		if (!pInputNode)
		{
			return;
		}
		SKMAC_DELETE(pInputNode);
	}
}
bool SKOneParamMorphBlend::ComputePara(double dAppTime)
{
	if (m_fParam < m_fParamMin)
	{
		m_fParam = m_fParamMin;
	}
	else if (m_fParam > m_fParamMax)
	{
		m_fParam = m_fParamMax;
	}
	return 1;
}
