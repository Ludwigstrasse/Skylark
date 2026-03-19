#ifndef SKONEPARAMSMOOTHMORPHBLEND_H
#define SKONEPARAMSMOOTHMORPHBLEND_H
#include "SKOneParamMorphBlend.h"
namespace SKEngine2
{
	/*
	CrossFading IMMEDIATE|______|______|  [0,1) Ƕ0 [1,2) Ƕ1 Դ
	0		1		2


	*/
	class SKStream;
	class SKGRAPHIC_API SKOneParamSmoothMorphBlend : public SKOneParamMorphBlend
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL


	public:
		virtual ~SKOneParamSmoothMorphBlend();

		SKOneParamSmoothMorphBlend(const SKUsedName & ShowName, SKMorphTree * pMorphTree);
		virtual void UpdateVertexDate(unsigned int uiVertexIndex);
		virtual bool Update(double dAppTime);
	public:

		SKOneParamSmoothMorphBlend();
	};
	DECLARE_Ptr(SKOneParamSmoothMorphBlend);
	SKTYPE_MARCO(SKOneParamSmoothMorphBlend);
}
#endif