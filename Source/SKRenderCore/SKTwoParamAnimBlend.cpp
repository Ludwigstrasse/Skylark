#include "SKTwoParamAnimBlend.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKTwoParamAnimBlend, SKAnimBlendFunction)
IMPLEMENT_INITIAL_BEGIN(SKTwoParamAnimBlend)
IMPLEMENT_INITIAL_END
BEGIN_ADD_PROPERTY(SKTwoParamAnimBlend,SKAnimBlendFunction)
REGISTER_PROPERTY(m_fParamMax[0],ParamMaxX,SKProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_fParamMax[1],ParamMaxY,SKProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_fParamMin[0],ParamMinX,SKProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_fParamMin[1],ParamMinY,SKProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiWidth, uiWidth, SKProperty::F_SAVE_LOAD_CLONE);
REGISTER_PROPERTY(m_uiHeight, uiHeight, SKProperty::F_SAVE_LOAD_CLONE);
END_ADD_PROPERTY
SKTwoParamAnimBlend::~SKTwoParamAnimBlend()
{

}
SKTwoParamAnimBlend::SKTwoParamAnimBlend()
{
	m_fParam[0] = m_fParam[1] =0.0f;
	m_fParamMax[0] = m_fParamMax[0] =1.0f;
	m_fParamMin[1] = m_fParamMin[1] =-1.0f;
	m_uiWidth = 0;
	m_uiHeight = 0;
}
SKTwoParamAnimBlend::SKTwoParamAnimBlend(const SKUsedName & ShowName,SKAnimTree * pAnimTree)
:SKAnimBlendFunction(ShowName,pAnimTree)
{
	m_fParam[0] = m_fParam[1] =0.0f;
	m_fParamMax[0] = m_fParamMax[0] =1.0f;
	m_fParamMin[1] = m_fParamMin[1] =-1.0f;
	m_uiWidth = 0;
	m_uiHeight = 0;
	unsigned int BoneNum = GetSkelectonMeshNode()->GetSkelecton()->GetBoneNum();
	m_BlendBoneMatrix[0].SetBufferNum(BoneNum);
	m_BlendBoneMatrix[1].SetBufferNum(BoneNum);
}
bool SKTwoParamAnimBlend::ComputePara(double dAppTime)
{
	for (unsigned int i = 0 ; i < 2 ; i++)
	{
		if (m_fParam[i] < m_fParamMin[i])
		{
			m_fParam[i] = m_fParamMin[i];
		}
		else if (m_fParam[i] > m_fParamMax[i])
		{
			m_fParam[i] = m_fParamMax[i];
		}
	}
	
	return 1;
}
bool SKTwoParamAnimBlend::SetObject(SKObject * pObject)
{
	if (SKAnimBlendFunction::SetObject(pObject))
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
void SKTwoParamAnimBlend::CreateSlot(unsigned int uiWidth, unsigned int uiHeight)
{
	m_pInput.Clear();
	m_uiWidth = uiWidth;
	m_uiHeight = uiHeight;
	for (unsigned int i = 0; i < uiWidth; i++)
	{
		for (unsigned int j = 0; j < uiHeight; j++)
		{
			SKString InputName = _T("Child");
			SKString ID = IntToString(i);
			InputName += ID;
			ID = IntToString(j);
			InputName += ID;
			SKInputNode * pInputNode = NULL;
			pInputNode = SK_NEW SKInputNode(SKPutNode::AVT_ANIM, InputName, this);
			SKMAC_ASSERT(pInputNode);
			m_pInput.AddElement(pInputNode);
		}
	}
}
bool SKTwoParamAnimBlend::ComputeOutBoneMatrix(double dAppTime)
{
	if (m_pInput.GetNum() == 0)
	{
		return false;
	}

	unsigned int Index1[2] = { 0, 0 };
	unsigned int Index2[2] = { 0, 0 };
	SKREAL fWeight[2] = { 0.0f, 0.0f };
	if (m_uiWidth > 1)
	{
		SKREAL fInternal = m_fParamMax[0] - m_fParamMin[0];
		SKREAL fInternalSeg = fInternal / (m_uiWidth - 1);
		if (fInternalSeg < EPSILON_E4)
		{
			return 0;
		}


		SKREAL fTemp = (m_fParam[0] - m_fParamMin[0]) / fInternalSeg;
		Index1[0] = (unsigned int)fTemp;
		Index2[0] = Index1[0] + 1;
		fWeight[0] = fTemp - (SKREAL)Index1[0];
	}
	if (m_uiHeight > 1)
	{
		SKREAL fInternal = m_fParamMax[1] - m_fParamMin[1];
		SKREAL fInternalSeg = fInternal / (m_uiHeight - 1);
		if (fInternalSeg < EPSILON_E4)
		{
			return 0;
		}


		SKREAL fTemp = (m_fParam[1] - m_fParamMin[1]) / fInternalSeg;
		Index1[1] = (unsigned int)fTemp;
		Index2[1] = Index1[1] + 1;
		fWeight[1] = fTemp - (SKREAL)Index1[1];
	}

	SKAnimFunction *pAnimBaseFunction1 = GetAnimFuntion(Index1[1], Index1[0]);
	SKAnimFunction *pAnimBaseFunction2 = GetAnimFuntion(Index1[1], Index2[0]);
	SKAnimFunction *pAnimBaseFunction3 = GetAnimFuntion(Index2[1], Index1[0]);
	SKAnimFunction *pAnimBaseFunction4 = GetAnimFuntion(Index2[1], Index2[0]);





	LineBlendTwo(&m_BlendBoneMatrix[0], pAnimBaseFunction1, pAnimBaseFunction2, fWeight[0]);
	LineBlendTwo(&m_BlendBoneMatrix[1], pAnimBaseFunction3, pAnimBaseFunction4, fWeight[0]);
	LineBlendTwo(&m_BoneOutPut, &m_BlendBoneMatrix[0], &m_BlendBoneMatrix[1], fWeight[1]);

	LineBlendTwo(m_BlendRootMatrix[0], pAnimBaseFunction1, pAnimBaseFunction2, fWeight[0]);
	LineBlendTwo(m_BlendRootMatrix[1], pAnimBaseFunction3, pAnimBaseFunction4, fWeight[0]);
	m_RootAtom.Interpolation(m_BlendRootMatrix[0], m_BlendRootMatrix[1], fWeight[1]);
	return 1;
}
SKAnimFunction * SKTwoParamAnimBlend::GetAnimFuntion(unsigned int i, unsigned int j)
{
	SKInputNode* pInputNode1 = GetInputNode(i * m_uiWidth + j);
	if (pInputNode1)
	{
		SKAnimFunction *pAnimBaseFunction1 = (SKAnimFunction *)pInputNode1->GetOutputLink()->GetOwner();
		return pAnimBaseFunction1;
	}
	return NULL;
}