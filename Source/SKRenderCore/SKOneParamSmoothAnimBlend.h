#ifndef SKONEPARAMSMOOTHANIMBLEND_H
#define SKONEPARAMSMOOTHANIMBLEND_H
#include "SKOneParamAnimBlend.h"
namespace SKEngine2
{
	/*
	CrossFading IMMEDIATE|______|______|  [0,1) Ƕ0 [1,2) Ƕ1 Դ
	0		1		2


	*/
	class SKStream;
	class SKGRAPHIC_API SKOneParamSmoothAnimBlend : public SKOneParamAnimBlend
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL


	public:
		virtual ~SKOneParamSmoothAnimBlend();

		SKOneParamSmoothAnimBlend(const SKUsedName & ShowName, SKAnimTree * pAnimTree);

	public:

		SKOneParamSmoothAnimBlend();
		virtual bool ComputeOutBoneMatrix(double dAppTime);
	};
	DECLARE_Ptr(SKOneParamSmoothAnimBlend);
	SKTYPE_MARCO(SKOneParamSmoothAnimBlend);
}
#endif