#include "SKOneParamSmoothMorphBlend.h"
#include "SKGraphicInclude.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKOneParamSmoothMorphBlend, SKOneParamMorphBlend)
BEGIN_ADD_PROPERTY(SKOneParamSmoothMorphBlend, SKOneParamMorphBlend)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKOneParamSmoothMorphBlend)
IMPLEMENT_INITIAL_END
SKOneParamSmoothMorphBlend::~SKOneParamSmoothMorphBlend()
{

}
SKOneParamSmoothMorphBlend::SKOneParamSmoothMorphBlend()
{

}
SKOneParamSmoothMorphBlend::SKOneParamSmoothMorphBlend(const SKUsedName & ShowName, SKMorphTree * pMorphTree)
:SKOneParamMorphBlend(ShowName, pMorphTree)
{


}
bool SKOneParamSmoothMorphBlend::Update(double dAppTime)
{
	SKOneParamMorphBlend::Update(dAppTime);

	SKREAL fInternal = m_fParamMax - m_fParamMin;
	SKREAL fInternalSeg = fInternal / (m_pInput.GetNum() - 1);
	if (fInternalSeg < EPSILON_E4)
	{
		return false;
	}

	unsigned int uiIndex1, uiIndex2;
	SKREAL fTemp = (m_fParam - m_fParamMin) / fInternalSeg;
	uiIndex1 = (unsigned int)fTemp;

	if (uiIndex1 >= m_pInput.GetNum() - 1)
	{
		uiIndex2 = uiIndex1;
	}
	else
	{
		uiIndex2 = uiIndex1 + 1;
	}


	m_fWeight = fTemp - (SKREAL)uiIndex1;


	SKInputNode* pInputNode1 = GetInputNode(uiIndex1);
	SKInputNode* pInputNode2 = GetInputNode(uiIndex2);
	if (pInputNode1->GetOutputLink() && pInputNode2->GetOutputLink())
	{
		m_pMorphBaseFunction1 = (SKMorphFunction *)pInputNode1->GetOutputLink()->GetOwner();
		m_pMorphBaseFunction2 = (SKMorphFunction *)pInputNode2->GetOutputLink()->GetOwner();
	}
	else if (pInputNode1->GetOutputLink())
	{
		m_pMorphBaseFunction1 = (SKMorphFunction *)pInputNode1->GetOutputLink()->GetOwner();
		m_pMorphBaseFunction2 = NULL;
	}
	else if (pInputNode2->GetOutputLink())
	{
		m_pMorphBaseFunction2 = (SKMorphFunction *)pInputNode2->GetOutputLink()->GetOwner();
		m_pMorphBaseFunction1 = NULL;
	}
	else
	{
		m_pMorphBaseFunction1 = NULL;
		m_pMorphBaseFunction2 = NULL;
	}
	return true;
}
void SKOneParamSmoothMorphBlend::UpdateVertexDate(unsigned int uiVertexIndex)
{
	SKOneParamMorphBlend::UpdateVertexDate(uiVertexIndex);

	if (m_pMorphBaseFunction1 || m_pMorphBaseFunction2)
	{
		LineBlendTwo(this, m_pMorphBaseFunction1, m_pMorphBaseFunction2, m_fWeight);
	}
	
	return;
}

