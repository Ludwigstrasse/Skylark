#include "SKCurveLengthFunction1.h"
using namespace SKEngine2;
/*----------------------------------------------------------------*/
SKCurveLengthFunction1::SKCurveLengthFunction1()
{
	m_pCurve = NULL;
}
/*----------------------------------------------------------------*/
SKCurveLengthFunction1::~SKCurveLengthFunction1()
{

	m_pCurve = NULL;
}
/*----------------------------------------------------------------*/
SKREAL SKCurveLengthFunction1::GetValue(SKREAL fParameter)
{
	if(!m_pCurve)
		return SKMAX_REAL;
	SKVector3 p = m_pCurve->GetFirstDerivative(fParameter);
	return SQRT(p.x * p.x + p.y * p.y + p.z * p.z);

}
/*----------------------------------------------------------------*/
SKREAL SKCurveLengthFunction1::GetFirstDerivative(SKREAL fParameter)
{
	return 0.0;
}
/*----------------------------------------------------------------*/
SKREAL SKCurveLengthFunction1::GetSecondDerivative(SKREAL fParameter)
{
	return 0.0;
}
/*----------------------------------------------------------------*/
SKREAL SKCurveLengthFunction1::GetThirdDerivative(SKREAL fParameter)
{
	return 0.0;
}
/*----------------------------------------------------------------*/