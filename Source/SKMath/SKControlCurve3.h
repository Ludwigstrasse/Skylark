#ifndef SKCONTROLCURVE3_H
#define SKCONTROLCURVE3_H
#include "SKCurve3.h"
/*
Ƶ࣬еĿƵ߶ûʵ

*/
namespace SKEngine2
{
class SKMATH_API SKControlCurve3:public SKCurve3
{
public:
	enum {B_SPLINE,NURBS,BEZIER};
	SKControlCurve3();
	virtual ~SKControlCurve3() = 0;
	virtual unsigned int GetCurveType(){return CT_CONTROL;}
	//
	bool Copy(const SKControlCurve3 & ControlCurve3);
	/***************************************FORCEINLINE************************************/
	//õƵָ
	FORCEINLINE const SKVector3 * GetpControlPoint()const;
	//ĳƵ
	FORCEINLINE SKVector3 GetControlPoint(unsigned int i)const;
	//ıĳƵ
	FORCEINLINE bool ChangeControlPoint(unsigned int i,const SKVector3 & ControlPoint);
	//õƵ
	FORCEINLINE unsigned int GetControlPointNum()const;
	//õ
	FORCEINLINE unsigned int GetType()const;
	//FRENET3
	virtual void	GetFrenetFrameBNT(SKREAL t,SKVector3 & B,SKVector3 & N,SKVector3 &T);
	//
	virtual SKVector3	GetFirstDerivative(SKREAL t)= 0 ;
	virtual SKVector3	GetSecondDerivative(SKREAL t)= 0;
	virtual SKVector3	GetThirdDerivative(SKREAL t)= 0;
	//󳤶
	virtual SKREAL		GetLength(SKREAL t1 ,SKREAL t2,unsigned int uiIterations = 32);
	//õ
	virtual SKVector3	GetPoint(SKREAL t) = 0;
	//õܳ
	virtual SKREAL		GetTotalLength(unsigned int uiIterations);
	//ݳʱ
	virtual SKREAL GetTime (SKREAL fLength, unsigned int uiIterations = 32,
						SKREAL fTolerance = EPSILON_E4);
	//õ
	virtual SKREAL GetCurvature(SKREAL fTime);
	//õת
	virtual SKREAL GetTorsion(SKREAL fTime);
protected:
	unsigned int m_CurveType;
	SKVector3 * m_pControlPoint;
	unsigned int m_uiControlPointNum;
};
#include "SKControlCurve3.inl"
}
#endif