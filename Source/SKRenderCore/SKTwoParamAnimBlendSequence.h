#ifndef SKTWOPARAMANIMBLENDSEQUENCE_H
#define SKTWOPARAMANIMBLENDSEQUENCE_H
#include "SKTwoParamAnimBlend.h"
#include "SKAnimSequenceFunc.h"
namespace SKEngine2
{
	/*

		ParaMin[0]			width					ParaMax[0]

		j0				j1				j3				j4
	i0	-------------------------------------------------	ParaMin[1]
		|				|				|				|
		|				|				|				|
		|				|				|				|
		|				|				|				|
		|				|				|				|
	i1	-------------------------------------------------	 height
		|				|				|				|
		|				|				|				|
		|				|				|				|
		|				|				|				|
		|				|				|				|
	i2	-------------------------------------------------	ParaMax[1]


	*/
	class SKStream;
	class SKGRAPHIC_API SKTwoParamAnimBlendSequence : public SKTwoParamAnimBlend
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL


	public:
		virtual ~SKTwoParamAnimBlendSequence();

		SKTwoParamAnimBlendSequence(const SKUsedName & ShowName, SKAnimTree * pAnimTree);

	public:

		SKTwoParamAnimBlendSequence();
		virtual bool ComputeOutBoneMatrix(double dAppTime);
		void CreateSlot(unsigned int uiWidth, unsigned int uiHeight);
		void SetAnim(unsigned int i,unsigned int j ,const SKUsedName &AnimName);


		void SetAnimFrequency(unsigned int i, unsigned int j, double Frequency);
		void SetAnimPhase(unsigned int i, unsigned int j, double Phase);
		virtual bool IsLeafNode(){ return true; }
	protected:
		SKArray<SKAnimSequenceFuncPtr> m_AnimSequenceFuncArray;
		
		
		SKAnimSequenceFunc * GetAnimSequenceFuntion(unsigned int i, unsigned int j);
	};
	DECLARE_Ptr(SKTwoParamAnimBlendSequence);
	SKTYPE_MARCO(SKTwoParamAnimBlendSequence);
}
#endif