#include "SKAnimBlendFunction.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI_NoCreateFun(SKAnimBlendFunction,SKAnimFunction)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(SKAnimBlendFunction)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(SKAnimBlendFunction,SKAnimFunction)
END_ADD_PROPERTY
SKAnimBlendFunction::SKAnimBlendFunction()
{

}
SKAnimBlendFunction::SKAnimBlendFunction(const SKUsedName & ShowName,SKAnimTree * pAnimTree)
:SKAnimFunction(ShowName,pAnimTree)
{


	SKString OutputName = _T("Output");
	SKOutputNode * pOutputNode = NULL;
	pOutputNode = SK_NEW SKOutputNode(SKPutNode::AVT_ANIM,OutputName,this);
	SKMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);

	

}
SKAnimBlendFunction::~SKAnimBlendFunction()
{
	
}
bool SKAnimBlendFunction::Update(double dAppTime)
{
	if (!SKAnimFunction::Update(dAppTime))
	{
		return 0;

	}
	if(!ComputePara(dAppTime))
		return 0;
	if(!ComputeOutBoneMatrix(dAppTime))
		return 0;
	return 1;
}
void SKAnimBlendFunction::AdditiveBlend(SKAnimFunction * pOut, SKAnimFunction * pAnimFunction1, SKAnimFunction * pAnimFunction2)
{
	if (!pOut)
	{
		return;
	}
	unsigned int uiBoneNum = pOut->m_BoneOutPut.GetNum();
	if (!uiBoneNum)
	{
		return;
	}

	if (pAnimFunction1 && pAnimFunction2)
	{

		if (uiBoneNum != pAnimFunction1->m_BoneOutPut.GetNum())
		{
			return;
		}
		if (uiBoneNum != pAnimFunction2->m_BoneOutPut.GetNum())
		{
			return;
		}

		for (unsigned int i = 0; i < uiBoneNum; i++)
		{
			pOut->m_BoneOutPut[i].AddTwo(pAnimFunction1->m_BoneOutPut[i], pAnimFunction2->m_BoneOutPut[i]);
		}
	}
	else if (pAnimFunction1)
	{
		if (uiBoneNum != pAnimFunction1->m_BoneOutPut.GetNum())
		{
			return;
		}

		for (unsigned int i = 0; i < uiBoneNum; i++)
		{
			pOut->m_BoneOutPut[i] = pAnimFunction1->m_BoneOutPut[i];
		}
	}
	else if (pAnimFunction2)
	{
		if (uiBoneNum != pAnimFunction2->m_BoneOutPut.GetNum())
		{
			return;
		}


		for (unsigned int i = 0; i < uiBoneNum; i++)
		{
			pOut->m_BoneOutPut[i] = pAnimFunction2->m_BoneOutPut[i];
		}
	}
}
void SKAnimBlendFunction::LineBlendTwo(SKAnimFunction * pOut,
									   SKAnimFunction * pAnimFunction1,SKAnimFunction * pAnimFunction2,
									   SKREAL fWeight)
{
	if (!pOut || fWeight < 0.0f || fWeight > 1.0f)
	{
		return ;
	}
	unsigned int uiBoneNum = pOut->m_BoneOutPut.GetNum();
	if (!uiBoneNum)
	{
		return ;
	}
	
	if (pAnimFunction1 && pAnimFunction2)
	{

		if (uiBoneNum != pAnimFunction1->m_BoneOutPut.GetNum())
		{
			return ;
		}
		if (uiBoneNum != pAnimFunction2->m_BoneOutPut.GetNum())
		{
			return ;
		}

		for (unsigned int i = 0 ; i < uiBoneNum ;i++)
		{
			pOut->m_BoneOutPut[i].Interpolation(pAnimFunction1->m_BoneOutPut[i],pAnimFunction2->m_BoneOutPut[i],fWeight);
		}
	}
	else if (pAnimFunction1)
	{
		if (uiBoneNum != pAnimFunction1->m_BoneOutPut.GetNum())
		{
			return ;
		}

		for (unsigned int i = 0 ; i < uiBoneNum ;i++)
		{
			pOut->m_BoneOutPut[i] = pAnimFunction1->m_BoneOutPut[i];
		}
	}
	else if (pAnimFunction2)
	{
		if (uiBoneNum != pAnimFunction2->m_BoneOutPut.GetNum())
		{
			return ;
		}


		for (unsigned int i = 0 ; i < uiBoneNum ;i++)
		{
			pOut->m_BoneOutPut[i] = pAnimFunction2->m_BoneOutPut[i];
		}
	}
}

void SKAnimBlendFunction::LineBlendTwo(SKArray<SKAnimAtom> *pOut,
						 const SKArray<SKAnimAtom> * pIn1,const SKArray<SKAnimAtom> *pIn2,
						 SKREAL fWeight)
{

	if (pOut || fWeight < 0.0f || fWeight > 1.0f)
	{
		return ;
	}
	unsigned int uiBoneNum = pOut->GetNum();
	if (!uiBoneNum)
	{
		return ;
	}

	if (pIn1 && pIn2)
	{

		if (uiBoneNum != pIn1->GetNum())
		{
			return ;
		}
		if (uiBoneNum != pIn2->GetNum())
		{
			return ;
		}

		for (unsigned int i = 0 ; i < uiBoneNum ;i++)
		{
			(*pOut)[i].Interpolation((*pIn1)[i],(*pIn2)[i],fWeight); 
		}
	}
	else if (pIn1)
	{
		if (uiBoneNum != pIn1->GetNum())
		{
			return ;
		}

		for (unsigned int i = 0 ; i < uiBoneNum ;i++)
		{
			(*pOut)[i] = (*pIn1)[i];
		}
	}
	else if (pIn2)
	{
		if (uiBoneNum != pIn2->GetNum())
		{
			return ;
		}


		for (unsigned int i = 0 ; i < uiBoneNum ;i++)
		{
			(*pOut)[i] = (*pIn2)[i];
		}
	}
}

void SKAnimBlendFunction::LineBlendTwo(SKAnimAtom &pOut,
	SKAnimFunction * pAnimFunction1, SKAnimFunction * pAnimFunction2, SKREAL fWeight)
{
	if (fWeight < 0.0f || fWeight > 1.0f)
	{
		return;
	}


	if (pAnimFunction1 && pAnimFunction2)
	{

		pOut.Interpolation(pAnimFunction1->m_RootAtom, pAnimFunction2->m_RootAtom, fWeight);

	}
	else if (pAnimFunction1)
	{

		pOut = pAnimFunction1->m_RootAtom;
		
	}
	else if (pAnimFunction2)
	{


		pOut = pAnimFunction2->m_RootAtom;
	}
}
void SKAnimBlendFunction::LineBlendTwo(SKArray<SKAnimAtom> *pOut,
									   SKAnimFunction * pAnimFunction1,SKAnimFunction * pAnimFunction2,
									   SKREAL fWeight)
{
	if (!pOut || fWeight < 0.0f || fWeight > 1.0f)
	{
		return ;
	}
	unsigned int uiBoneNum = pOut->GetNum();
	if (!uiBoneNum)
	{
		return ;
	}

	if (pAnimFunction1 && pAnimFunction2)
	{

		if (uiBoneNum != pAnimFunction1->m_BoneOutPut.GetNum())
		{
			return ;
		}
		if (uiBoneNum != pAnimFunction2->m_BoneOutPut.GetNum())
		{
			return ;
		}

		for (unsigned int i = 0 ; i < uiBoneNum ;i++)
		{
			(*pOut)[i].Interpolation(pAnimFunction1->m_BoneOutPut[i],pAnimFunction2->m_BoneOutPut[i],fWeight);
		}
	}
	else if (pAnimFunction1)
	{
		if (uiBoneNum != pAnimFunction1->m_BoneOutPut.GetNum())
		{
			return ;
		}

		for (unsigned int i = 0 ; i < uiBoneNum ;i++)
		{
			(*pOut)[i] = pAnimFunction1->m_BoneOutPut[i];
		}
	}
	else if (pAnimFunction2)
	{
		if (uiBoneNum != pAnimFunction2->m_BoneOutPut.GetNum())
		{
			return ;
		}


		for (unsigned int i = 0 ; i < uiBoneNum ;i++)
		{
			(*pOut)[i] = pAnimFunction2->m_BoneOutPut[i];
		}
	}
}

