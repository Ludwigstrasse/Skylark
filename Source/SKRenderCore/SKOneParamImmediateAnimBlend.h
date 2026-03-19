#ifndef SKONEPARAMIMMEDIATEANIMBLEND_H
#define SKONEPARAMIMMEDIATEANIMBLEND_H
#include "SKOneParamAnimBlend.h"
namespace SKEngine2
{
	/*
	CrossFading IMMEDIATE|______|______|  [0,1) Ƕ0 [1,2) Ƕ1 Դ
	0		1		2


	*/
	class SKStream;
	class SKGRAPHIC_API SKOneParamImmediateAnimBlend : public SKOneParamAnimBlend
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL


	public:
		virtual ~SKOneParamImmediateAnimBlend();

		SKOneParamImmediateAnimBlend(const SKUsedName & ShowName, SKAnimTree * pAnimTree);
		virtual void AddInputNode();
		virtual void DeleteInputNode();
	public:
		
		SKOneParamImmediateAnimBlend();
		virtual bool Update(double dAppTime);
		virtual bool ComputeOutBoneMatrix(double dAppTime);
	protected:
		SKArray<SKREAL> m_NodeCrossFadingTime;
		SKREAL			m_fCrossFadingTime;
		unsigned int	m_uiLastAnimIndex;
		unsigned int	m_uiCurAnimIndex;
	public:
		FORCEINLINE void SetNodeCrossFadingTime(unsigned int uiIndex, SKREAL fTime)
		{
			if (uiIndex >= m_pInput.GetNum())
			{
				return;
			}
			m_NodeCrossFadingTime[uiIndex] = fTime;
		}

	};
	DECLARE_Ptr(SKOneParamImmediateAnimBlend);
	SKTYPE_MARCO(SKOneParamImmediateAnimBlend);
}
#endif
