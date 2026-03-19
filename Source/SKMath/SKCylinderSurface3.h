#ifndef SKCYLINDERSURFACE3_H
#define SKCYLINDERSURFACE3_H
#include "SKSurface3.h"
#include "SKSegmentCurve3.h"
/*
	

*/
namespace SKEngine2
{
class SKMATH_API SKCylinderSurface3:public SKSurface3
{
public:

	SKCylinderSurface3();
	~SKCylinderSurface3();
	bool SetCurve(SKCurve3 * pCurve1,SKSegmentCurve3 *pCurve2);
	virtual SKVector3 GetPoint(SKREAL U,SKREAL V);
	virtual SKVector3 GetUPartialDerivative(SKREAL U,SKREAL V);
	virtual SKVector3 GetVPartialDerivative(SKREAL U,SKREAL V);
private:
	SKCurve3 *m_pCurve1;
	SKSegmentCurve3 *m_pCurve2;



};


}
#endif