#ifndef SKCURVESET3_H
#define SKCURVESET3_H
#include "SKCurve3.h"
#include "SKSegment3.h"
/*
	߼


*/
namespace SKEngine2
{
	class SKMATH_API SKCurveSet3 : public SKMemObject
	{
	public:
		SKCurveSet3();
		~SKCurveSet3();
		void Clear();
		bool Set(SKCurve3 * pCurve);
		bool	GetFrenetFrameBNT(SKREAL t,SKVector3 & B,SKVector3 & N,SKVector3 &T);
		bool	GetFixedUpBNT(SKREAL t ,SKVector3 & B,SKVector3 & N,SKVector3& T);
		SKVector3	GetFirstDerivative(SKREAL t);
		SKVector3	GetSecondDerivative(SKREAL t);
		SKVector3	GetThirdDerivative(SKREAL t);
		SKREAL		GetLength(SKREAL t1 ,SKREAL t2,unsigned int uiIterations);
		SKVector3	GetPoint(SKREAL t);
		SKREAL		GetTotalLength(unsigned int uiIterations);
		SKREAL GetTime (SKREAL fLength, int iIterations = 32,
						SKREAL fTolerance = EPSILON_E4);
		unsigned int GetIndex(SKREAL &t);
		bool GetElement(unsigned int Index,SKCurve3 * &Element );
		bool GetElement(SKREAL &t,SKCurve3 * &Element);

		FORCEINLINE unsigned int GetCurveNum()const{return m_uiNum;}
	protected:
		typedef struct ELEMENT
		{
			SKCurve3 * Ptr;
			ELEMENT * next;
		};
		ELEMENT * m_pHead;
		ELEMENT * m_pNext;
		unsigned int m_uiNum;
	};


}
#endif