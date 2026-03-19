#ifndef  SKCURVESUBDIVISIONBYUNIFORM_H
#define  SKCURVESUBDIVISIONBYUNIFORM_H
#include "SKCurve3Subdivision.h"
/*
Ȼַ



*/
namespace SKEngine2
{
class SKMATH_API SKCurve3SubdivisionByUniform :public SKCurve3Subdivision
{
public:
	SKCurve3SubdivisionByUniform(unsigned int PointNum,SKREAL t1 = 0.0f,SKREAL t2 = 1.0f);
	virtual ~SKCurve3SubdivisionByUniform();
	//ĸ t1,t2֮
	virtual bool Subdivision();
	void SetPointNum(unsigned int uiPointNum);
	virtual unsigned int GetSubdivisionMethod(){ return SM_UNIFORM;}
};


}
#endif