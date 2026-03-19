#include "SKOneParamSmoothAnimBlend.h"
#include "SKGraphicInclude.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKOneParamSmoothAnimBlend, SKOneParamAnimBlend)
BEGIN_ADD_PROPERTY(SKOneParamSmoothAnimBlend, SKOneParamAnimBlend)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKOneParamSmoothAnimBlend)
IMPLEMENT_INITIAL_END
SKOneParamSmoothAnimBlend::~SKOneParamSmoothAnimBlend()
{

}
SKOneParamSmoothAnimBlend::SKOneParamSmoothAnimBlend()
{

}
SKOneParamSmoothAnimBlend::SKOneParamSmoothAnimBlend(const SKUsedName & ShowName, SKAnimTree * pAnimTree)
:SKOneParamAnimBlend(ShowName, pAnimTree)
{


}

bool SKOneParamSmoothAnimBlend::ComputeOutBoneMatrix(double dAppTime)
{
	SKREAL fInternal = m_fParamMax - m_fParamMin;
	SKREAL fInternalSeg = fInternal / (m_pInput.GetNum() - 1);
	if (fInternalSeg < EPSILON_E4)
	{
		return 0;
	}
	unsigned int uiIndex1 = 0;
	unsigned int uiIndex2 = 0;

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


	SKREAL fWeight = fTemp - (SKREAL)uiIndex1;


	SKInputNode* pInputNode1 = GetInputNode(uiIndex1);
	SKInputNode* pInputNode2 = GetInputNode(uiIndex2);
	if (pInputNode1->GetOutputLink() && pInputNode2->GetOutputLink())
	{
		SKAnimFunction *pAnimBaseFunction1 = (SKAnimFunction *)pInputNode1->GetOutputLink()->GetOwner();
		SKAnimFunction *pAnimBaseFunction2 = (SKAnimFunction *)pInputNode2->GetOutputLink()->GetOwner();
		LineBlendTwo(this, pAnimBaseFunction1, pAnimBaseFunction2, fWeight);
		LineBlendTwo(m_RootAtom, pAnimBaseFunction1, pAnimBaseFunction2, fWeight);
	}
	else if (pInputNode1->GetOutputLink())
	{
		SKAnimFunction *pAnimBaseFunction1 = (SKAnimFunction *)pInputNode1->GetOutputLink()->GetOwner();

		LineBlendTwo(this, pAnimBaseFunction1, NULL, 0.0f);
		LineBlendTwo(m_RootAtom, pAnimBaseFunction1, NULL, 0.0f);
	}
	else if (pInputNode2->GetOutputLink())
	{
		SKAnimFunction *pAnimBaseFunction2 = (SKAnimFunction *)pInputNode2->GetOutputLink()->GetOwner();
		LineBlendTwo(this, NULL, pAnimBaseFunction2, 0.0f);
		LineBlendTwo(m_RootAtom, NULL, pAnimBaseFunction2, 0.0f);
	}
	return 1;
}
