#include "SKPartialAnimBlend.h"
#include "SKSkelectonMeshNode.h"
#include "SKGraphicInclude.h"
#include "SKBoneNode.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI(SKPartialAnimBlend,SKAnimBlendFunction)
BEGIN_ADD_PROPERTY(SKPartialAnimBlend,SKAnimBlendFunction)
REGISTER_PROPERTY(m_BoneWeight, BoneWeight, SKProperty::F_SAVE_LOAD_CLONE)
END_ADD_PROPERTY
IMPLEMENT_INITIAL_BEGIN(SKPartialAnimBlend)
IMPLEMENT_INITIAL_END
SKPartialAnimBlend::~SKPartialAnimBlend()
{
	
	m_BoneWeight.Clear();
}
SKPartialAnimBlend::SKPartialAnimBlend()
{
	m_BoneWeight.Clear();
}
SKPartialAnimBlend::SKPartialAnimBlend(const SKUsedName & ShowName,SKAnimTree * pAnimTree)
:SKAnimBlendFunction(ShowName,pAnimTree)
{
	
	SKString InputName0 = _T("Child0");
	SKInputNode * pInputNode = NULL;
	pInputNode = SK_NEW SKInputNode(SKPutNode::AVT_ANIM,InputName0,this);
	SKMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);



	SKString InputName1 = _T("Child1");
	pInputNode = NULL;
	pInputNode = SK_NEW SKInputNode(SKPutNode::AVT_ANIM,InputName1,this);
	SKMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);


}
void SKPartialAnimBlend::AddInputNode()
{
	SKString InputName = _T("Child");
	SKString ID = IntToString(m_pInput.GetNum()); 
	InputName += ID;
	SKInputNode * pInputNode = NULL;
	pInputNode = SK_NEW SKInputNode(SKPutNode::AVT_ANIM,InputName,this);
	SKMAC_ASSERT(pInputNode);
	m_pInput.AddElement(pInputNode);
	for (unsigned int i = 0; i < m_BoneWeight.GetNum(); i++)
	{
		m_BoneWeight[i].Value.AddElement(0.0f);
	}

}
void SKPartialAnimBlend::DeleteInputNode()
{
	if (m_pInput.GetNum() > 2)
	{
		SKInputNode * pInputNode = m_pInput[m_pInput.GetNum() - 1];

		m_pInput.Erase(m_pInput.GetNum() - 1);
		if (!pInputNode)
		{
			return ;
		}
		SKMAC_DELETE(pInputNode);

		for (unsigned int i = 0; i < m_BoneWeight.GetNum(); i++)
		{
			m_BoneWeight[i].Value.Erase(m_BoneWeight[i].Value.GetNum() - 1);
		}
	}
}
void SKPartialAnimBlend::AddControllBone(const SKUsedName & BoneName)
{
	SKArray<SKREAL> Temp;
	Temp.SetBufferNum(m_pInput.GetNum());
	for (unsigned int i = 0; i < Temp.GetNum(); i++)
	{
		if (i == 0)
		{
			Temp[i] = 1.0f;
		}
		else
		{
			Temp[i] = 0.0f;
		}
	}
	m_BoneWeight.AddElement(BoneName, Temp);

}
void SKPartialAnimBlend::SetBoneWeight(unsigned int i, const SKUsedName & BoneName, SKREAL Weight)
{
	unsigned int uiIndex = m_BoneWeight.Find(BoneName);
	if (uiIndex != m_BoneWeight.GetNum())
	{
		if (i >= m_BoneWeight[uiIndex].Value.GetNum())
		{
			return;
		}
		m_BoneWeight[uiIndex].Value[i] = Weight;
	}
	
}
bool SKPartialAnimBlend::SetObject(SKObject * pObject)
{
	if (SKAnimFunction::SetObject(pObject))
	{
		ComputeWeight();
#ifdef FAST_BLEND_PARIAL_ANIMITION
		m_FastTempMatrix.SetBufferNum(m_BoneOutPut.GetNum());
#endif
		return true;
	}

	return false;
}
bool SKPartialAnimBlend::ComputeOutBoneMatrix(double dAppTime)
{

	if (m_Weight.GetNum() == 0)
	{
		SKMAC_ASSERT(0);
	}
	
	for ( unsigned int i = 0 ; i < m_pInput.GetNum() ; i++)
	{
		SKInputNode* pInputNode = GetInputNode(i);
		SKAnimFunction *pAnimBaseFunction = (SKAnimFunction *)pInputNode->GetOutputLink()->GetOwner();
		if (!pAnimBaseFunction)
		{
			return false;
		}
#ifdef FAST_BLEND_PARIAL_ANIMITION
		if (i == 0)
		{
			pAnimBaseFunction->m_RootAtom.GetMatrix(m_FastRootMatrix);
			m_FastRootMatrix *= m_Weight[i][0];
		}
		else
		{
			SKMatrix3X3W Temp;
			pAnimBaseFunction->m_RootAtom.GetMatrix(Temp);
			m_FastRootMatrix += Temp * m_Weight[i][0];
		}
#else
		if (i == 0)
		{
			m_RootAtom = pAnimBaseFunction->m_RootAtom * m_Weight[i][0];
		}
		else
		{
			m_RootAtom.BlendWith(pAnimBaseFunction->m_RootAtom * m_Weight[i][0]);
		}
#endif
		for (unsigned int j = 0; j < m_BoneOutPut.GetNum(); j++)
		{

#ifdef FAST_BLEND_PARIAL_ANIMITION
			if (i == 0)
			{
				pAnimBaseFunction->m_BoneOutPut[j].GetMatrix(m_FastTempMatrix[j]);
				m_FastTempMatrix[j] *= m_Weight[i][j];

				
			}
			else
			{
				SKMatrix3X3W Temp;
				pAnimBaseFunction->m_BoneOutPut[j].GetMatrix(Temp);
				m_FastTempMatrix[j] += Temp * m_Weight[i][j];

			}
#else
			if (i == 0)
			{
				m_BoneOutPut[j] = pAnimBaseFunction->m_BoneOutPut[j] * m_Weight[i][j];
			}
			else
			{
				m_BoneOutPut[j].BlendWith(pAnimBaseFunction->m_BoneOutPut[j] * m_Weight[i][j]);
			}
#endif
		}
	}
#ifdef FAST_BLEND_PARIAL_ANIMITION
	for (unsigned int j = 0; j < m_BoneOutPut.GetNum(); j++)
	{
		m_BoneOutPut[j].FromMatrix(m_FastTempMatrix[j]);
	}
	m_RootAtom.FromMatrix(m_FastRootMatrix);
#endif
	return 1;
}

void SKPartialAnimBlend::ComputeWeight()
{

	const SKSkelectonMeshNode * pMesh = GetSkelectonMeshNode();
	SKMAC_ASSERT(pMesh);
	if (!pMesh)
	{
		return ;
	}

	SKSkelecton * pSkelecton = pMesh->GetSkelecton();
	SKMAC_ASSERT(pSkelecton);
	if (!pSkelecton)
	{
		return ;
	}
	m_Weight.SetBufferNum(m_pInput.GetNum());
	for (unsigned int i = 0; i < m_Weight.GetNum(); i++)
	{
		m_Weight[i].SetBufferNum(pSkelecton->GetBoneNum());
		if (i == 0)
		{
			for (unsigned int j = 0; j < pSkelecton->GetBoneNum(); j++)
			{
				m_Weight[i][j] = 1.0f;
			}
		}
		else
		{
			for (unsigned int j = 0; j < pSkelecton->GetBoneNum(); j++)
			{
				m_Weight[i][j] = 0.0f;
			}
		}
	}


	for (unsigned int i = 0; i < m_BoneWeight.GetNum(); i++)
	{
		SKREAL Sum = 0.0f;
		for (unsigned int j = 0; j < m_BoneWeight[i].Value.GetNum(); j++)
		{
			Sum += m_BoneWeight[i].Value[j];
		}

		for (unsigned int j = 0; j < m_BoneWeight[i].Value.GetNum(); j++)
		{
			m_BoneWeight[i].Value[j] /= Sum;
		}
	}


	for (unsigned int i = 0; i < m_BoneWeight.GetNum(); i++)
	{
			
		SKBoneNode * pBone = pSkelecton->GetBoneNode(m_BoneWeight[i].Key);

		SKArray<SKUsedName> AllBoneName;

		SKArray<SKBoneNode *> AllBoneArray;
		pBone->GetAllBoneArray(AllBoneArray);

		for (unsigned int m = 0; m < AllBoneArray.GetNum(); m++)
		{
			AllBoneName.AddElement(AllBoneArray[m]->m_cName);
		}

		SKArray<unsigned int> BoneIndex;
		BoneIndex.SetBufferNum(AllBoneName.GetNum());
		for (unsigned int s = 0; s < AllBoneName.GetNum(); s++)
		{
			for (unsigned int j = 0; j < pSkelecton->GetBoneNum(); j++)
			{
				SKBoneNode * pBone = pSkelecton->GetBoneNode(j);

				if (pBone->m_cName == AllBoneName[s])
				{
					BoneIndex[s] = j;
				}
			}
		}	

		for (unsigned int s = 0; s < m_Weight.GetNum(); s++)
		{
			for (unsigned int j = 0; j < BoneIndex.GetNum(); j++)
			{

				m_Weight[s][BoneIndex[j]] = m_BoneWeight[i].Value[s];
			}
		}
	}		
}
bool SKPartialAnimBlend::ComputePara(double dAppTime)
{
	return true;
}