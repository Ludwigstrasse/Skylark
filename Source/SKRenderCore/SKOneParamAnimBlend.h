#ifndef SKONEPARAMANIMBLEND_H
#define SKONEPARAMANIMBLEND_H
#include "SKAnimBlendFunction.h"
namespace SKEngine2
{
	/*
		CrossFading IMMEDIATE|______|______|  [0,1) Ƕ0 [1,2) Ƕ1 Դ
							 0		1		2


	*/
	class SKStream;
	class SKGRAPHIC_API SKOneParamAnimBlend : public SKAnimBlendFunction
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
		

	public:
		virtual ~SKOneParamAnimBlend();

		SKOneParamAnimBlend(const SKUsedName & ShowName,SKAnimTree * pAnimTree);

		virtual void AddInputNode();
		virtual void DeleteInputNode();
	public:
		SKREAL m_fParam;
		SKREAL m_fParamMax;
		SKREAL m_fParamMin;
		
		SKOneParamAnimBlend();
		virtual bool ComputePara(double dAppTime);
		virtual bool ComputeOutBoneMatrix(double dAppTime);
		virtual void SetPara(void * pPara)
		{
			m_fParam = *((SKREAL *)pPara);
		}
	};
	DECLARE_Ptr(SKOneParamAnimBlend);
	SKTYPE_MARCO(SKOneParamAnimBlend);
}
#endif
