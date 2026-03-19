#include "SKOneParamImmediateAnimBlend.h"
#include "SKGraphicInclude.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKOneParamImmediateAnimBlend, SKOneParamAnimBlend)
BEGIN_ADD_PROPERTY(SKOneParamImmediateAnimBlend, SKOneParamAnimBlend)
REGISTER_PROPERTY(m_NodeCrossFadingTime, NodeCrossFadingTime, SKProperty::F_SAVE_LOAD_CLONE);
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKOneParamImmediateAnimBlend)
IMPLEMENT_INITIAL_END
SKOneParamImmediateAnimBlend::~SKOneParamImmediateAnimBlend()
{

}
SKOneParamImmediateAnimBlend::SKOneParamImmediateAnimBlend()
{
	m_uiLastAnimIndex = SKMAX_INTEGER;
	m_fCrossFadingTime = 0.0f;
	m_uiCurAnimIndex = 0;
}
SKOneParamImmediateAnimBlend::SKOneParamImmediateAnimBlend(const SKUsedName & ShowName, SKAnimTree * pAnimTree)
:SKOneParamAnimBlend(ShowName, pAnimTree)
{

	m_NodeCrossFadingTime.AddElement(0.0f);
	m_NodeCrossFadingTime.AddElement(0.0f);
	m_uiLastAnimIndex = SKMAX_INTEGER;
	m_fCrossFadingTime = 0.0f;
}
void SKOneParamImmediateAnimBlend::AddInputNode()
{
	SKString InputName = _T("Child");
	SKString ID = IntToString(m_pInput.GetNum());
	InputName += ID;
	SKInputNode * pInputNode = NULL;
	pInputNode = SK_NEW SKInputNode(SKPutNode::AVT_ANIM, InputName, this);
	SKMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);
	m_NodeCrossFadingTime.AddElement(0.0f);
}
void SKOneParamImmediateAnimBlend::DeleteInputNode()
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
		m_NodeCrossFadingTime.Erase(m_NodeCrossFadingTime.GetNum() - 1);
	}
}
bool SKOneParamImmediateAnimBlend::Update(double dAppTime)
{
	if (!SKAnimBaseFunction::Update(dAppTime))
	{
		return 0;

	}
	if (!ComputePara(dAppTime))
		return 0;
	

	SKREAL fInternal = m_fParamMax - m_fParamMin;
	SKREAL fInternalSeg = fInternal / (m_pInput.GetNum() - 1);
	if (fInternalSeg < EPSILON_E4)
	{
		return 0;
	}


	SKREAL fTemp = (m_fParam - m_fParamMin) / fInternalSeg;
	m_uiCurAnimIndex = (unsigned int)fTemp;
	if (m_uiLastAnimIndex == SKMAX_INTEGER)
	{
		m_uiLastAnimIndex = m_uiCurAnimIndex;
	}
	if (m_uiLastAnimIndex != m_uiCurAnimIndex && m_fCrossFadingTime < 0.00001f)
	{
		SKArray<SKAnimBaseFunction *> CurLeafNodeArray;
		if (m_pInput[m_uiCurAnimIndex]->GetOutputLink())
		{
			SKAnimBaseFunction *pAnimBaseFunction = (SKAnimBaseFunction *)m_pInput[m_uiCurAnimIndex]->GetOutputLink()->GetOwner();
			pAnimBaseFunction->GetLeafArray(CurLeafNodeArray);
		}	
		SKArray<SKAnimBaseFunction *> LastLeafNodeArray;
		if (m_pInput[m_uiLastAnimIndex]->GetOutputLink())
		{
			SKAnimBaseFunction *pAnimBaseFunction = (SKAnimBaseFunction *)m_pInput[m_uiLastAnimIndex]->GetOutputLink()->GetOwner();
			pAnimBaseFunction->GetLeafArray(LastLeafNodeArray);
		}
		if (LastLeafNodeArray.GetNum() && CurLeafNodeArray.GetNum())
		{
			for (unsigned int i = 0; i < CurLeafNodeArray.GetNum(); i++)
			{
				bool bFound = false;
				for (unsigned int j = 0; j < LastLeafNodeArray.GetNum(); j++)
				{
					if (CurLeafNodeArray[i] == LastLeafNodeArray[j])
					{
						CurLeafNodeArray[i]->NoLeafStart();
						bFound = true;
						break;
					}
				}
				if (bFound == false)
				{
					CurLeafNodeArray[i]->EnableLeafStart();
				}
			}
		}
		
	}
	for (unsigned int i = 0; i < m_pInput.GetNum(); i++)
	{
		if (m_pInput[i]->GetOutputLink() )
		{
			SKAnimBaseFunction *pAnimBaseFunction = (SKAnimBaseFunction *)m_pInput[i]->GetOutputLink()->GetOwner();
			if (pAnimBaseFunction)
			{
				if (i == m_uiLastAnimIndex || i == m_uiCurAnimIndex)
				{
					pAnimBaseFunction->Update(dAppTime);
				}
			}
		}
	}

	if (!ComputeOutBoneMatrix(dAppTime))
		return 0;
	return 1;
}
bool SKOneParamImmediateAnimBlend::ComputeOutBoneMatrix(double dAppTime)
{
	


	if (m_uiLastAnimIndex != m_uiCurAnimIndex)
	{
		if (m_fCrossFadingTime < m_NodeCrossFadingTime[m_uiCurAnimIndex])
		{
			SKREAL fWeight = 0.0f;
// 			if (m_NodeCrossFadingTime[uiIndex1] > EPSILON_E4)
// 			{
			fWeight = m_fCrossFadingTime / m_NodeCrossFadingTime[m_uiCurAnimIndex];
		/*	}*/

			SKInputNode* pInputNode1 = GetInputNode(m_uiLastAnimIndex);
			SKInputNode* pInputNode2 = GetInputNode(m_uiCurAnimIndex);
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
				LineBlendTwo(this, NULL, pAnimBaseFunction2,0.0f);
				LineBlendTwo(m_RootAtom, NULL, pAnimBaseFunction2, 0.0f);
			}
			m_fCrossFadingTime += (SKREAL)m_dIntervalTime;
		}
		else
		{
			m_uiLastAnimIndex = m_uiCurAnimIndex;
			m_fCrossFadingTime = 0.0f;
			SKInputNode* pInputNode1 = GetInputNode(m_uiCurAnimIndex);
			if (pInputNode1->GetOutputLink())
			{
				SKAnimFunction *pAnimBaseFunction1 = (SKAnimFunction *)pInputNode1->GetOutputLink()->GetOwner();
				LineBlendTwo(this, pAnimBaseFunction1, NULL, 0.0f);
				LineBlendTwo(m_RootAtom, pAnimBaseFunction1, NULL, 0.0f);
			}
		}
	}
	else
	{
		m_fCrossFadingTime = 0.0f;
		SKInputNode* pInputNode1 = GetInputNode(m_uiCurAnimIndex);
		if (pInputNode1->GetOutputLink())
		{
			SKAnimFunction *pAnimBaseFunction1 = (SKAnimFunction *)pInputNode1->GetOutputLink()->GetOwner();
			LineBlendTwo(this, pAnimBaseFunction1, NULL, 0.0f);
			LineBlendTwo(m_RootAtom, pAnimBaseFunction1, NULL, 0.0f);
		}

	}

	return 1;
}