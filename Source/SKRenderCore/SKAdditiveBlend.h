#ifndef SKADDITIVEBLEND_H
#define SKADDITIVEBLEND_H
#include "SKAnimBlendFunction.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKAdditiveBlend : public SKAnimBlendFunction
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL


	public:
		virtual ~SKAdditiveBlend();

		SKAdditiveBlend(const SKUsedName & ShowName, SKAnimTree * pAnimTree);

		virtual void AddInputNode();
		virtual void DeleteInputNode();

		SKAdditiveBlend();
		virtual bool ComputePara(double dAppTime){ return true; }
		virtual bool ComputeOutBoneMatrix(double dAppTime);
	};
	DECLARE_Ptr(SKAdditiveBlend);
	SKTYPE_MARCO(SKAdditiveBlend);
}
#endif
