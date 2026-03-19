#ifndef SKMORPHBLENDFUNCTION_H
#define SKMORPHBLENDFUNCTION_H
#include "SKMorphFunction.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKMorphBlendFunction : public SKMorphFunction
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY

	public:
		SKMorphBlendFunction(const SKUsedName & ShowName, SKMorphTree * pMorphTree);
		virtual ~SKMorphBlendFunction() = 0;
		virtual bool Update(double dAppTime);
		static void LineBlendTwo(SKMorphFunction * pOut,
			SKMorphFunction * pMorphFunction1, SKMorphFunction * pMorphFunction2, SKREAL fWeight);
	protected:
		SKMorphBlendFunction();
		virtual bool ComputePara(double dAppTime) = 0;




	};
	DECLARE_Ptr(SKMorphBlendFunction);
	SKTYPE_MARCO(SKMorphBlendFunction);

}
#endif