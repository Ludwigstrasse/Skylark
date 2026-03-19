#ifndef SKONEPARAMSMOOTHANIMBLENDSEQUENCE_H
#define SKONEPARAMSMOOTHANIMBLENDSEQUENCE_H
#include "SKOneParamSmoothAnimBlend.h"
#include "SKAnimSequenceFunc.h"
namespace SKEngine2
{
	/*
	CrossFading IMMEDIATE|______|______|  [0,1) Ƕ0 [1,2) Ƕ1 Դ
	0		1		2


	*/
	class SKStream;
	class SKGRAPHIC_API SKOneParamSmoothAnimBlendSequence : public SKOneParamSmoothAnimBlend
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL


	public:
		virtual ~SKOneParamSmoothAnimBlendSequence();

		SKOneParamSmoothAnimBlendSequence(const SKUsedName & ShowName, SKAnimTree * pAnimTree);

	public:

		SKOneParamSmoothAnimBlendSequence();
		virtual bool ComputeOutBoneMatrix(double dAppTime);
		void CreateSlot(unsigned int uiWidth);
		void SetAnim(unsigned int i,const SKUsedName &AnimName);

		void SetAnimFrequency(unsigned int Index,double Frequency);
		void SetAnimPhase(unsigned int Index,double Phase);
		virtual bool SetObject(SKObject * pObject);
		virtual bool IsLeafNode(){ return true; }
	protected:
		SKArray<SKAnimSequenceFuncPtr> m_AnimSequenceFuncArray;
	};
	DECLARE_Ptr(SKOneParamSmoothAnimBlendSequence);
	SKTYPE_MARCO(SKOneParamSmoothAnimBlendSequence);
}
#endif