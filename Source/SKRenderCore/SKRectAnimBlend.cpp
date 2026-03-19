#include "SKRectAnimBlend.h"
#include "SKGraphicInclude.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKRectAnimBlend,SKTwoParamAnimBlend)
BEGIN_ADD_PROPERTY(SKRectAnimBlend,SKTwoParamAnimBlend)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKRectAnimBlend)
IMPLEMENT_INITIAL_END
SKRectAnimBlend::~SKRectAnimBlend()
{

}
SKRectAnimBlend::SKRectAnimBlend()
{

}
SKRectAnimBlend::SKRectAnimBlend(const SKUsedName & ShowName,SKAnimTree * pAnimTree)
:SKTwoParamAnimBlend(ShowName,pAnimTree)
{


	for (unsigned int i = CL_LEFT_TOP ; i < CL_MAX ; i++)
	{
		SKString InputName = _T("Child");
		SKString ID = IntToString(m_pInput.GetNum()); 
		InputName += ID;
		SKInputNode * pInputNode = NULL;
		pInputNode = SK_NEW SKInputNode(SKPutNode::AVT_ANIM,InputName,this);
		SKMAC_ASSERT(pInputNode);
		m_pInput.AddElement(pInputNode);

	}
	unsigned int BoneNum = GetSkelectonMeshNode()->GetSkelecton()->GetBoneNum();
	m_BlendBoneMatrix[0].SetBufferNum(BoneNum);
	m_BlendBoneMatrix[1].SetBufferNum(BoneNum);
}
bool SKRectAnimBlend::SetObject(SKObject * pObject)
{
	if (SKTwoParamAnimBlend::SetObject(pObject))
	{
		const SKSkelectonMeshNode* pMesh = GetSkelectonMeshNode();
		SKMAC_ASSERT(pMesh);

		SKSkelecton * pSkelecton = pMesh->GetSkelecton();
		SKMAC_ASSERT(pSkelecton);

		unsigned int BoneNum = pSkelecton->GetBoneNum();
		m_BlendBoneMatrix[0].SetBufferNum(BoneNum);
		m_BlendBoneMatrix[1].SetBufferNum(BoneNum);
		return true;
	}


	return false;
}
bool SKRectAnimBlend::ComputeOutBoneMatrix(double dAppTime)
{
	SKREAL fInternal[2];
	for (unsigned int i = 0 ; i < 2 ; i++)
	{
		fInternal[i] = m_fParamMax[i] - m_fParamMin[i];
		
		if (fInternal[i] < EPSILON_E4)
		{
			return 0;
		}
	}
	


	SKREAL fWeight[2];
	for (unsigned int i = 0 ; i < 2 ; i++)
	{
		fWeight[i] = (m_fParam[i] - m_fParamMin[i]) / fInternal[i];
	}

	SKInputNode* pInputNode0 = GetInputNode(0);
	SKInputNode* pInputNode1 = GetInputNode(1);
	SKInputNode* pInputNode2 = GetInputNode(2);
	SKInputNode* pInputNode3 = GetInputNode(3);
	if(pInputNode1->GetOutputLink() && pInputNode2->GetOutputLink() 
		&& pInputNode0->GetOutputLink() && pInputNode3->GetOutputLink())
	{
		SKAnimFunction *pAnimBaseFunction1 = (SKAnimFunction *)pInputNode1->GetOutputLink()->GetOwner();
		SKAnimFunction *pAnimBaseFunction2 = (SKAnimFunction *)pInputNode2->GetOutputLink()->GetOwner();
		SKAnimFunction *pAnimBaseFunction0 = (SKAnimFunction *)pInputNode0->GetOutputLink()->GetOwner();
		SKAnimFunction *pAnimBaseFunction3 = (SKAnimFunction *)pInputNode3->GetOutputLink()->GetOwner();
		LineBlendTwo(&m_BlendBoneMatrix[0],pAnimBaseFunction0,pAnimBaseFunction1,fWeight[0]);	
		LineBlendTwo(&m_BlendBoneMatrix[1],pAnimBaseFunction2,pAnimBaseFunction3,fWeight[0]);
		LineBlendTwo(&m_BoneOutPut,&m_BlendBoneMatrix[0],&m_BlendBoneMatrix[1],fWeight[1]);

		LineBlendTwo(m_BlendRootMatrix[0], pAnimBaseFunction0, pAnimBaseFunction1, fWeight[0]);
		LineBlendTwo(m_BlendRootMatrix[1], pAnimBaseFunction2, pAnimBaseFunction3, fWeight[0]);
		m_RootAtom.Interpolation(m_BlendRootMatrix[0], m_BlendRootMatrix[1], fWeight[1]);
	}
	
	

	return 1;
}