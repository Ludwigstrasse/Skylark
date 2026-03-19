#ifndef SKMORPHSEQUENCEFUNC_H
#define SKMORPHSEQUENCEFUNC_H
#include "SKMorphFunction.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKMorphSequenceFunc : public SKMorphFunction
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
		
	public:
		virtual ~SKMorphSequenceFunc();
		virtual void UpdateGeometryDate(unsigned int GeometryIndex);
		virtual void UpdateVertexDate(unsigned int uiVertexIndex);
		SKMorphSequenceFunc(const SKUsedName & ShowName,SKMorphTree * pMorphTree);
		void SetMorph(const SKUsedName & MorphName);
		virtual void ClearChangeFlag();
	protected:
		SKMorphSequenceFunc();
	protected:
		SKUsedName m_MorphName;
		SKVertexBuffer * m_pVertexBuffer;
		SKDataBuffer * pPosDate[MAX_NUM_POS3];
		SKDataBuffer * pNormalDate[MAX_NUM_NORMAL3];
		SKDataBuffer * pTangentDate;
		SKDataBuffer * pBinormalDate;
		SKDataBuffer * pColorDate[MAX_NUM_COLOR];
	};
	DECLARE_Ptr(SKMorphSequenceFunc);
	SKTYPE_MARCO(SKMorphSequenceFunc);
}
#endif