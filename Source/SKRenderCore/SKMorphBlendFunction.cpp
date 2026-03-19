#include "SKMorphBlendFunction.h"
#include "SKGraphicInclude.h"
#include "SKStream.h"
using namespace SKEngine2;
IMPLEMENT_RTTI_NoCreateFun(SKMorphBlendFunction, SKMorphFunction)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_BEGIN(SKMorphBlendFunction)
IMPLEMENT_INITIAL_NO_CLASS_FACTORY_END
BEGIN_ADD_PROPERTY(SKMorphBlendFunction, SKMorphFunction)
END_ADD_PROPERTY
SKMorphBlendFunction::SKMorphBlendFunction()
{

}
SKMorphBlendFunction::SKMorphBlendFunction(const SKUsedName & ShowName, SKMorphTree * pMorphTree)
:SKMorphFunction(ShowName, pMorphTree)
{


	SKString OutputName = _T("Output");
	SKOutputNode * pOutputNode = NULL;
	pOutputNode = SK_NEW SKOutputNode(SKPutNode::AVT_MORPH, OutputName, this);
	SKMAC_ASSERT(pOutputNode);
	m_pOutput.AddElement(pOutputNode);



}
SKMorphBlendFunction::~SKMorphBlendFunction()
{

}
bool SKMorphBlendFunction::Update(double dAppTime)
{
	if (!SKMorphFunction::Update(dAppTime))
	{
		return 0;

	}
	if (!ComputePara(dAppTime))
		return 0;

	return 1;
}
void SKMorphBlendFunction::LineBlendTwo(SKMorphFunction * pOut,
	SKMorphFunction * pMorphFunction1, SKMorphFunction * pMorphFunction2, SKREAL fWeight)
{
	if (!pOut || fWeight < 0.0f || fWeight > 1.0f)
	{
		return;
	}
	if (pMorphFunction1 && pMorphFunction2)
	{
		for (unsigned int uiLevel = 0; uiLevel < SKMorphFunction::MAX_NUM_POS3; uiLevel++)
		{
			SKVector3 * pVector1 = pMorphFunction1->GetPos(uiLevel);
			SKVector3 * pVector2 = pMorphFunction2->GetPos(uiLevel);
			if (pVector2 && pVector1)
			{
				SKVector3 Vec = LineInterpolation(*pVector1, *pVector2, fWeight);
				pOut->SetPos(Vec, uiLevel);
			}
		}

		for (unsigned int uiLevel = 0; uiLevel < SKMorphFunction::MAX_NUM_NORMAL3; uiLevel++)
		{
			SKVector3 * pVector1 = pMorphFunction1->GetNormal(uiLevel);
			SKVector3 * pVector2 = pMorphFunction2->GetNormal(uiLevel);
			if (pVector2 && pVector1)
			{
				SKVector3 Vec = LineInterpolation(*pVector1, *pVector2, fWeight);
				pOut->SetNormal(Vec, uiLevel);
			}
		}

		{
			SKVector3 * pVector1 = pMorphFunction1->GetBinormal();
			SKVector3 * pVector2 = pMorphFunction2->GetBinormal();
			if (pVector2 && pVector1)
			{
				SKVector3 Vec = LineInterpolation(*pVector1, *pVector2, fWeight);
				pOut->SetBinormal(Vec);
			}
		}

		{
			SKVector3W * pVector1 = pMorphFunction1->GetTangent();
			SKVector3W * pVector2 = pMorphFunction2->GetTangent();
			if (pVector2 && pVector1)
			{
				SKVector3W Vec = LineInterpolation(*pVector1, *pVector2, fWeight);
				pOut->SetTangent(Vec);
			}
		}
		for (unsigned int uiLevel = 0; uiLevel < SKMorphFunction::MAX_NUM_COLOR; uiLevel++)
		{
			DWORD * pVector1 = pMorphFunction1->GetColor(uiLevel);
			DWORD * pVector2 = pMorphFunction2->GetColor(uiLevel);
			if (pVector2 && pVector1)
			{
				SKColorRGBA Color1, Color2;
				Color1.CreateFromARGB(*pVector1);
				Color2.CreateFromARGB(*pVector2);
				SKColorRGBA Result = LineInterpolation(Color1, Color2, fWeight);
				pOut->SetColor(Result.GetDWARGB(), uiLevel);
			}
		}
	}
	else if (pMorphFunction1)
	{
		for (unsigned int uiLevel = 0; uiLevel < SKMorphFunction::MAX_NUM_POS3; uiLevel++)
		{
			SKVector3 * pVector1 = pMorphFunction1->GetPos(uiLevel);
			if (pVector1)
			{
				pOut->SetPos(*pVector1, uiLevel);
			}
		}

		for (unsigned int uiLevel = 0; uiLevel < SKMorphFunction::MAX_NUM_NORMAL3; uiLevel++)
		{
			SKVector3 * pVector1 = pMorphFunction1->GetNormal(uiLevel);

			if (pVector1)
			{
				pOut->SetNormal(*pVector1, uiLevel);
			}
		}

		{
			SKVector3 * pVector1 = pMorphFunction1->GetBinormal();
			if (pVector1)
			{
				pOut->SetBinormal(*pVector1);
			}
		}

		{
			SKVector3W * pVector1 = pMorphFunction1->GetTangent();

			if (pVector1)
			{
				pOut->SetTangent(*pVector1);
			}
		}
		for (unsigned int uiLevel = 0; uiLevel < SKMorphFunction::MAX_NUM_COLOR; uiLevel++)
		{
			DWORD * pVector1 = pMorphFunction1->GetColor(uiLevel);
			if (pVector1)
			{
				pOut->SetColor(*pVector1, uiLevel);
			}
		}
	}
	else if (pMorphFunction2)
	{
		for (unsigned int uiLevel = 0; uiLevel < SKMorphFunction::MAX_NUM_POS3; uiLevel++)
		{
			SKVector3 * pVector1 = pMorphFunction2->GetPos(uiLevel);
			if (pVector1)
			{
				pOut->SetPos(*pVector1, uiLevel);
			}
		}

		for (unsigned int uiLevel = 0; uiLevel < SKMorphFunction::MAX_NUM_NORMAL3; uiLevel++)
		{
			SKVector3 * pVector1 = pMorphFunction2->GetNormal(uiLevel);

			if (pVector1)
			{
				pOut->SetNormal(*pVector1, uiLevel);
			}
		}

		{
			SKVector3 * pVector1 = pMorphFunction2->GetBinormal();
			if (pVector1)
			{
				pOut->SetBinormal(*pVector1);
			}
		}

		{
			SKVector3W * pVector1 = pMorphFunction2->GetTangent();

			if (pVector1)
			{
				pOut->SetTangent(*pVector1);
			}
		}
		for (unsigned int uiLevel = 0; uiLevel < SKMorphFunction::MAX_NUM_COLOR; uiLevel++)
		{
			DWORD * pVector1 = pMorphFunction2->GetColor(uiLevel);
			if (pVector1)
			{
				pOut->SetColor(*pVector1, uiLevel);
			}
		}
	}
}
