#ifndef SKANIMBLENDFUNCTION_H
#define SKANIMBLENDFUNCTION_H
#include "SKAnimFunction.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKAnimBlendFunction : public SKAnimFunction
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY

	public:
		
		virtual ~SKAnimBlendFunction() = 0;
		virtual bool Update(double dAppTime);

		SKAnimBlendFunction(const SKUsedName & ShowName,SKAnimTree * pAnimTree);
		static void LineBlendTwo(SKAnimFunction * pOut,
						SKAnimFunction * pAnimFunction1,SKAnimFunction * pAnimFunction2,SKREAL fWeight);


		static void LineBlendTwo(SKArray<SKAnimAtom> *pOut,
			const SKArray<SKAnimAtom> * pIn1,const SKArray<SKAnimAtom> *pIn2,SKREAL fWeight);


		static void LineBlendTwo(SKArray<SKAnimAtom> *pOut,
			SKAnimFunction * pAnimFunction1,SKAnimFunction * pAnimFunction2,SKREAL fWeight);


		static void LineBlendTwo(SKAnimAtom &pOut,
			SKAnimFunction * pAnimFunction1, SKAnimFunction * pAnimFunction2, SKREAL fWeight);
		static void AdditiveBlend(SKAnimFunction * pOut, SKAnimFunction * pAnimFunction1, SKAnimFunction * pAnimFunction2);
	protected:
		SKAnimBlendFunction();
		virtual bool ComputePara(double dAppTime) = 0;
		virtual bool ComputeOutBoneMatrix(double dAppTime) = 0;

		

	};
	DECLARE_Ptr(SKAnimBlendFunction);
	SKTYPE_MARCO(SKAnimBlendFunction);

}
#endif