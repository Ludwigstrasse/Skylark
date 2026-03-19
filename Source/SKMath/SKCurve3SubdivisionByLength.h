#ifndef SKCURVESUBDIVISIONBYLENGTH_H
#define SKCURVESUBDIVISIONBYLENGTH_H
#include "SKCurve3Subdivision.h"
/*
Ȼַ



*/
namespace SKEngine2
{
class SKMATH_API SKCurve3SubdivisionByLength :public SKCurve3Subdivision
{
public:
	SKCurve3SubdivisionByLength(unsigned int PointNum,SKREAL t1,SKREAL t2,unsigned int uiIterations = 32);
	virtual ~SKCurve3SubdivisionByLength();
	//ֵ͵߰l1,l2֮
	virtual bool Subdivision();
	void SetPointNum(unsigned int uiPointNum);
	virtual unsigned int GetSubdivisionMethod(){ return SM_LENGTH;}
protected:
	unsigned int m_uiIterations;
};


}

#endif