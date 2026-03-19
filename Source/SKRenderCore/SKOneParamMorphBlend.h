#ifndef SKONEPARAMMORPHBLEND_H
#define SKONEPARAMMORPHBLEND_H
#include "SKMorphBlendFunction.h"
namespace SKEngine2
{
	/*
	CrossFading IMMEDIATE|______|______|  [0,1) Ƕ0 [1,2) Ƕ1 Դ
	0		1		2


	*/
	class SKStream;
	class SKGRAPHIC_API SKOneParamMorphBlend : public SKMorphBlendFunction
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL


	public:
		virtual ~SKOneParamMorphBlend();

		SKOneParamMorphBlend(const SKUsedName & ShowName, SKMorphTree * pMorphTree);

		virtual void AddInputNode();
		virtual void DeleteInputNode();
	public:
		SKREAL m_fParam;
		SKREAL m_fParamMax;
		SKREAL m_fParamMin;

		SKOneParamMorphBlend();
		virtual bool ComputePara(double dAppTime);
		virtual void SetPara(void * pPara)
		{
			m_fParam = *((SKREAL *)pPara);
		}
	protected:
		SKMorphFunction *m_pMorphBaseFunction1;
		SKMorphFunction *m_pMorphBaseFunction2;
		SKREAL m_fWeight;
	};
	DECLARE_Ptr(SKOneParamMorphBlend);
	SKTYPE_MARCO(SKOneParamMorphBlend);
}
#endif
