#ifndef SKSEGMENTCURVE3_H
#define SKSEGMENTCURVE3_H
#include "SKCurve3.h"
#include "SKSegment3.h"
/*
߶

*/
namespace SKEngine2
{
	class SKMATH_API SKSegmentCurve3:public SKCurve3
	{
	public:
		SKSegmentCurve3();
		~SKSegmentCurve3();
		void Set(const SKSegment3 &Segment);
		SKSegment3 & GetSegment();
		virtual unsigned int GetCurveType(){return CT_SEGMENT;}
		virtual void GetFrenetFrameBNT(SKREAL t,SKVector3 & B,SKVector3 & N,SKVector3 &T);
		FORCEINLINE virtual SKVector3	GetFirstDerivative(SKREAL t);
		FORCEINLINE virtual SKVector3	GetSecondDerivative(SKREAL t);
		FORCEINLINE virtual SKVector3	GetThirdDerivative(SKREAL t);
		FORCEINLINE virtual SKREAL		GetLength(SKREAL t1 ,SKREAL t2,unsigned int uiIterations);
		FORCEINLINE virtual SKVector3	GetPoint(SKREAL t);
		FORCEINLINE virtual SKREAL		GetTotalLength(unsigned int uiIterations);
		FORCEINLINE virtual SKREAL GetTime (SKREAL fLength, unsigned int uiIterations = 32,
										SKREAL fTolerance = EPSILON_E4);
		FORCEINLINE virtual SKREAL GetCurvature(SKREAL fTime);
		FORCEINLINE virtual SKREAL GetTorsion(SKREAL fTime);

	private:
		SKSegment3 m_Segment;
	};

#include "SKSegmentCurve3.inl"
}
#endif