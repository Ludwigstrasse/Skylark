#include "SKOneParamSmoothAnimBlendSequence.h"
#include "SKGraphicInclude.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKOneParamSmoothAnimBlendSequence, SKOneParamSmoothAnimBlend)
BEGIN_ADD_PROPERTY(SKOneParamSmoothAnimBlendSequence, SKOneParamSmoothAnimBlend)
REGISTER_PROPERTY(m_AnimSequenceFuncArray, AnimSequenceFuncArray, SKProperty::F_SAVE_LOAD_CLONE);
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKOneParamSmoothAnimBlendSequence)
IMPLEMENT_INITIAL_END
SKOneParamSmoothAnimBlendSequence::~SKOneParamSmoothAnimBlendSequence()
{

}
SKOneParamSmoothAnimBlendSequence::SKOneParamSmoothAnimBlendSequence()
{

}
SKOneParamSmoothAnimBlendSequence::SKOneParamSmoothAnimBlendSequence(const SKUsedName & ShowName, SKAnimTree * pAnimTree)
:SKOneParamSmoothAnimBlend(ShowName, pAnimTree)
{
	//no need input
	for (unsigned int i = 0; i < m_pInput.GetNum(); i++)
	{
		SKMAC_DELETE(m_pInput[i]);
	}
	m_pInput.Clear();

}
void SKOneParamSmoothAnimBlendSequence::SetAnimFrequency(unsigned int Index, double Frequency)
{
	if (Index >= m_AnimSequenceFuncArray.GetNum())
	{
		return ;
	}
	m_AnimSequenceFuncArray[Index]->m_dFrequency = Frequency;
}
void SKOneParamSmoothAnimBlendSequence::SetAnimPhase(unsigned int Index, double Phase)
{
	if (Index >= m_AnimSequenceFuncArray.GetNum())
	{
		return;
	}
	m_AnimSequenceFuncArray[Index]->m_dPhase = Phase;
}
bool SKOneParamSmoothAnimBlendSequence::ComputeOutBoneMatrix(double dAppTime)
{
	if (m_AnimSequenceFuncArray.GetNum() == 0)
	{
		return false;
	}
	unsigned int Index1 = 0;
	unsigned int Index2 = 0;
	SKREAL fBlendWeight = 0.0f;
	if (m_AnimSequenceFuncArray.GetNum() > 1)
	{
		SKREAL fInternal = m_fParamMax - m_fParamMin;
		SKREAL fInternalSeg = fInternal / (m_AnimSequenceFuncArray.GetNum() - 1);
		if (fInternalSeg < EPSILON_E4)
		{
			return 0;
		}


		SKREAL fTemp = (m_fParam - m_fParamMin) / fInternalSeg;
		Index1 = (unsigned int)fTemp;
		Index2 = Index1 + 1;
		fBlendWeight = fTemp - (SKREAL)Index1;


		if (Index2 >= m_AnimSequenceFuncArray.GetNum())
		{
			Index2 = m_AnimSequenceFuncArray.GetNum() - 1;
		}

	}

	SKREAL AnimTime1 = m_AnimSequenceFuncArray[Index1]->GetAnimTime();
	SKREAL AnimTime2 = m_AnimSequenceFuncArray[Index2]->GetAnimTime();
	SKREAL BlendTime = AnimTime1 + (AnimTime2 - AnimTime1) * fBlendWeight;
	SKREAL AnimTimeScale1 = AnimTime1 / BlendTime;
	SKREAL AnimTimeScale2 = AnimTime2 / BlendTime;
	m_AnimSequenceFuncArray[Index1]->m_fInnerTimeScale = AnimTimeScale1;
	m_AnimSequenceFuncArray[Index2]->m_fInnerTimeScale = AnimTimeScale2;

	m_AnimSequenceFuncArray[Index1]->ClearFlag();
	m_AnimSequenceFuncArray[Index2]->ClearFlag();
	m_AnimSequenceFuncArray[Index1]->Update(dAppTime);
	m_AnimSequenceFuncArray[Index2]->Update(dAppTime);

	m_AnimSequenceFuncArray[Index1]->m_fInnerTimeScale = 1.0f;
	m_AnimSequenceFuncArray[Index2]->m_fInnerTimeScale = 1.0f;

	SKAnimSequenceFunc* pAnimBaseFunction1 = m_AnimSequenceFuncArray[Index1];
	SKAnimSequenceFunc* pAnimBaseFunction2 = m_AnimSequenceFuncArray[Index2];

	LineBlendTwo(this, pAnimBaseFunction1, pAnimBaseFunction2, fBlendWeight);
	LineBlendTwo(m_RootAtom, pAnimBaseFunction1, pAnimBaseFunction2, fBlendWeight);
	return 1;
}
void SKOneParamSmoothAnimBlendSequence::CreateSlot(unsigned int uiWidth)
{
	m_AnimSequenceFuncArray.Clear();
	for (unsigned int i = 0; i < uiWidth; i++)
	{
		SKAnimSequenceFunc * pAnimSequenceFun = NULL;
		if (GetSkelectonMeshNode())
		{
			pAnimSequenceFun = SK_NEW SKAnimSequenceFunc(GetSkelectonMeshNode());
		}
		else
		{
			pAnimSequenceFun = SK_NEW SKAnimSequenceFunc();
		}
		m_AnimSequenceFuncArray.AddElement(pAnimSequenceFun);
	}
}
void SKOneParamSmoothAnimBlendSequence::SetAnim(unsigned int i, const SKUsedName &AnimName)
{
	if (i >= m_AnimSequenceFuncArray.GetNum())
	{
		return;
	}
	m_AnimSequenceFuncArray[i]->SetAnim(AnimName);
}
bool SKOneParamSmoothAnimBlendSequence::SetObject(SKObject * pObject)
{
	if (SKOneParamSmoothAnimBlend::SetObject(pObject))
	{
		for (unsigned int i = 0; i < m_AnimSequenceFuncArray.GetNum(); i++)
		{
			if (m_AnimSequenceFuncArray[i])
			{
				m_AnimSequenceFuncArray[i]->SetObject(pObject);
			}
		}
		return true;
	}

	return false;
}