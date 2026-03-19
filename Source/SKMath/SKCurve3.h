#ifndef SKCURVE3_H
#define SKCURVE3_H
#include "SKVector3.h"
#include "SKMatrix3X3.h"
/*


*/
namespace SKEngine2
{
	class SKMATH_API SKCurve3 : public SKMemObject
	{
	public:
		enum //CURVE TYPE
		{
			CT_SEGMENT,
			CT_CONTROL,
			CT_MAX
		};
		SKCurve3();
		virtual ~SKCurve3() = 0;

		virtual unsigned int GetCurveType() = 0;
		//FrenetFrame3
		virtual void GetFrenetFrameBNT(SKREAL t,SKVector3 & B,SKVector3 & N,SKVector3 &T) = 0;
		//FixedUp3
		void	GetFixedUpBNT(SKREAL t ,SKVector3 & B,SKVector3 & N,SKVector3& T);
		//
		virtual SKVector3	GetFirstDerivative(SKREAL t)= 0 ;
		virtual SKVector3	GetSecondDerivative(SKREAL t)= 0;
		virtual SKVector3	GetThirdDerivative(SKREAL t)= 0;
		//󳤶
		virtual SKREAL		GetLength(SKREAL t1 ,SKREAL t2,unsigned int uiIterations)= 0;
		//
		virtual SKVector3	GetPoint(SKREAL t) = 0;
		//ܳ
		virtual SKREAL		GetTotalLength(unsigned int uiIterations) = 0;
		//ݳȵõʱ
		virtual SKREAL GetTime (SKREAL fLength, unsigned int iIterations = 32,
				SKREAL fTolerance = EPSILON_E4) = 0;
		//
		virtual SKREAL GetCurvature(SKREAL fTime) = 0;
		//ת
		virtual SKREAL GetTorsion(SKREAL fTime) = 0;
	
	};
}
#endif