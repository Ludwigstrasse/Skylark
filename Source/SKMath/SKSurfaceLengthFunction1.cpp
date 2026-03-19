#include "SKSurfaceLengthFunction1.h"
using namespace SKEngine2;
/*----------------------------------------------------------------*/
SKSurfaceLengthFunction1::SKSurfaceLengthFunction1(unsigned int uiLengthType,SKREAL fTypePara)
{
	m_pSurface = NULL;
	m_fTypePara = fTypePara;
	m_uiLengthType = uiLengthType;
}
/*----------------------------------------------------------------*/
SKSurfaceLengthFunction1::~SKSurfaceLengthFunction1()
{

	m_pSurface = NULL;
}
/*----------------------------------------------------------------*/
SKREAL SKSurfaceLengthFunction1::GetValue(SKREAL fParameter)
{
	if(!m_pSurface)
		return SKMAX_REAL;
	if (m_uiLengthType == LT_U_LEN)
	{
		SKVector3 p = m_pSurface->GetUPartialDerivative(fParameter,m_fTypePara);
		return SQRT(p.x * p.x + p.y * p.y + p.z * p.z);
	}
	else
	{
		SKVector3 p = m_pSurface->GetVPartialDerivative(m_fTypePara,fParameter);
		return SQRT(p.x * p.x + p.y * p.y + p.z * p.z);
	}

}
/*----------------------------------------------------------------*/
SKREAL SKSurfaceLengthFunction1::GetFirstDerivative(SKREAL fParameter)
{
	return 0.0;
}
/*----------------------------------------------------------------*/
SKREAL SKSurfaceLengthFunction1::GetSecondDerivative(SKREAL fParameter)
{
	return 0.0;
}
/*----------------------------------------------------------------*/
SKREAL SKSurfaceLengthFunction1::GetThirdDerivative(SKREAL fParameter)
{
	return 0.0;
}
/*----------------------------------------------------------------*/