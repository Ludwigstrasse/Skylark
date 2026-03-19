#include "SKAdditiveBlend.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKAdditiveBlend, SKAnimBlendFunction)
BEGIN_ADD_PROPERTY(SKAdditiveBlend, SKAnimBlendFunction)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKAdditiveBlend)
IMPLEMENT_INITIAL_END
SKAdditiveBlend::~SKAdditiveBlend()
{

}
SKAdditiveBlend::SKAdditiveBlend()
{

}
SKAdditiveBlend::SKAdditiveBlend(const SKUsedName & ShowName, SKAnimTree * pAnimTree)
:SKAnimBlendFunction(ShowName, pAnimTree)
{
	SKString InputName0 = _T("Delta");
	SKInputNode * pInputNode = NULL;
	pInputNode = SK_NEW SKInputNode(SKPutNode::AVT_ANIM, InputName0, this);
	SKMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);


	SKString InputName1 = _T("BlendAnim");
	pInputNode = NULL;
	pInputNode = SK_NEW SKInputNode(SKPutNode::AVT_ANIM, InputName1, this);
	SKMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);
}
void SKAdditiveBlend::AddInputNode()
{
	return;

}
void SKAdditiveBlend::DeleteInputNode()
{
	return;
}
bool SKAdditiveBlend::ComputeOutBoneMatrix(double dAppTime)
{

	SKInputNode* pInputNode1 = GetInputNode(0);
	SKInputNode* pInputNode2 = GetInputNode(1);
	if (pInputNode1->GetOutputLink() && pInputNode2->GetOutputLink())
	{
		SKAnimFunction *pAnimBaseFunction1 = (SKAnimFunction *)pInputNode1->GetOutputLink()->GetOwner();
		SKAnimFunction *pAnimBaseFunction2 = (SKAnimFunction *)pInputNode2->GetOutputLink()->GetOwner();
		AdditiveBlend(this, pAnimBaseFunction1, pAnimBaseFunction2);
		m_RootAtom = pAnimBaseFunction1->m_RootAtom;
	}
	else if (pInputNode1->GetOutputLink())
	{
		SKAnimFunction *pAnimBaseFunction1 = (SKAnimFunction *)pInputNode1->GetOutputLink()->GetOwner();

		AdditiveBlend(this, pAnimBaseFunction1,NULL);
		m_RootAtom = pAnimBaseFunction1->m_RootAtom;
	}
	else if (pInputNode2->GetOutputLink())
	{
		SKMAC_ASSERT(0);
	}
	return 1;
}
