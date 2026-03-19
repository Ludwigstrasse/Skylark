#ifndef SKPARTIALANIMBLEND_H
#define SKPARTIALANIMBLEND_H
#include "SKAnimBlendFunction.h"
namespace SKEngine2
{
	/*
	CrossFading IMMEDIATE |______|______|  [0,1) Ƕ0 [1,2) Ƕ1 Դ
	0		1		2


	*/
#define  FAST_BLEND_PARIAL_ANIMITION
	class SKStream;
	class SKGRAPHIC_API SKPartialAnimBlend : public SKAnimBlendFunction
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
		

	public:
		virtual ~SKPartialAnimBlend();

		SKPartialAnimBlend(const SKUsedName & ShowName,SKAnimTree * pAnimTree);

		virtual void AddInputNode();

		virtual void DeleteInputNode();

		virtual void SetBoneWeight(unsigned int i, const SKUsedName & BoneName,SKREAL Weight);

		virtual bool ComputePara(double dAppTime);
		void AddControllBone(const SKUsedName & BoneName);
	protected:
#ifdef FAST_BLEND_PARIAL_ANIMITION
		SKArray<SKMatrix3X3W> m_FastTempMatrix;
		SKMatrix3X3W m_FastRootMatrix;
#endif
		SKArray<SKArray<SKREAL>> m_Weight;
		SKMap<SKUsedName, SKArray<SKREAL>> m_BoneWeight;
		void ComputeWeight();
	public:

		SKPartialAnimBlend();
		virtual bool ComputeOutBoneMatrix(double dAppTime);
		virtual bool SetObject(SKObject * pObject);

	};
	DECLARE_Ptr(SKPartialAnimBlend);
	SKTYPE_MARCO(SKPartialAnimBlend);
}
#endif