#ifndef SKTWOPARAMANIMBLEND_H
#define SKTWOPARAMANIMBLEND_H
#include "SKAnimBlendFunction.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKTwoParamAnimBlend : public SKAnimBlendFunction
	{
		//RTTI
		DECLARE_RTTI
		DECLARE_INITIAL
	public:
		virtual ~SKTwoParamAnimBlend();

		SKTwoParamAnimBlend(const SKUsedName & ShowName,SKAnimTree * pAnimTree);
		void CreateSlot(unsigned int uiWidth, unsigned int uiHeight);
		virtual bool SetObject(SKObject * pObject);
	protected:
		SKREAL m_fParam[2];
		SKREAL m_fParamMax[2];
		SKREAL m_fParamMin[2];
		SKAnimAtom m_BlendRootMatrix[2];
		unsigned int m_uiWidth;
		unsigned int m_uiHeight;
		SKArray<SKAnimAtom> m_BlendBoneMatrix[2];
		SKTwoParamAnimBlend();
		virtual bool ComputePara(double dAppTime);
		virtual bool ComputeOutBoneMatrix(double dAppTime);
		SKAnimFunction * GetAnimFuntion(unsigned int i, unsigned int j);
	};
	DECLARE_Ptr(SKTwoParamAnimBlend);
	SKTYPE_MARCO(SKTwoParamAnimBlend);
}
#endif