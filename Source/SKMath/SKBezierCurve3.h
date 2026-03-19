#ifndef SKBEZIERCURVE3_H
#define SKBEZIERCURVE3_H
#include "SKControlCurve3.h"
/*
Bezierߣt0-1֮ɿƵͶʽ

*/
namespace SKEngine2
{
class SKMATH_API SKBezierCurve3 : public  SKControlCurve3
{
public:
	SKBezierCurve3();
	virtual ~SKBezierCurve3();
	//ÿƵ
	bool Set(const SKVector3 * pControlPoint,unsigned int uiControlPointNum);
	//
	bool Copy(const SKBezierCurve3 & BezierCurve3);
	//
	virtual SKVector3	GetFirstDerivative(SKREAL t);
	virtual SKVector3	GetSecondDerivative(SKREAL t);
	virtual SKVector3	GetThirdDerivative(SKREAL t);
	//virtual SKREAL		GetLength(SKREAL t1 ,SKREAL t2);
	//
	virtual SKVector3	GetPoint(SKREAL t);
private:
	FORCEINLINE SKREAL GetC(int i,int j)const;
	FORCEINLINE SKREAL SetC(int i,int j,SKREAL Value);

	SKREAL * m_C;
	SKVector3 * m_FDValue;
	SKVector3 * m_SDValue;
	SKVector3 * m_TDValue;

};
#include "SKBezierCurve3.inl"

}
#endif