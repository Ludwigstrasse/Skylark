#ifndef SKNURBSCURVE3_H
#define SKNURBSCURVE3_H
#include "SKB_SplineCurve3.h"
/*
NURB
*/
namespace SKEngine2
{
class SKMATH_API SKNURBSCurve3 : public SKB_SplineCurve3
{
public:
	SKNURBSCurve3();
	virtual ~SKNURBSCurve3();
	//Ȩ
	bool Set(const SKREAL * pWeight,unsigned int uiControlPointNum);
	//
	bool Copy(const SKNURBSCurve3 &NURBSCurve3);
	/***************************************FORCEINLINE************************************/
	//ıȨ
	FORCEINLINE bool ChangeWeight(unsigned int i,SKREAL Weight);
	//ȡȨָ
	FORCEINLINE const SKREAL * GetWeight()const;
	//õȨ
	FORCEINLINE SKREAL GetWeight(unsigned int i)const;
	//ȡõ
	virtual SKVector3	GetFirstDerivative(SKREAL t);
	virtual SKVector3	GetSecondDerivative(SKREAL t);
	virtual SKVector3	GetThirdDerivative(SKREAL t);
	//virtual SKREAL		GetLength(SKREAL t1 ,SKREAL t2);
	//õ
	virtual SKVector3	GetPoint(SKREAL t);
protected:
	bool Get(unsigned int Order,SKREAL t ,SKVector3 *D0,SKVector3 *D1,SKVector3 *D2,SKVector3 *D3);
private:
	SKREAL *m_pWeight;

};
#include "SKNURBSCurve3.inl"
}
#endif