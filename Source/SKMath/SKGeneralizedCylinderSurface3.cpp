#include "SKGeneralizedCylinderSurface3.h"
using namespace SKEngine2;
/*----------------------------------------------------------------*/
SKGeneralizedCylinderSurface3::SKGeneralizedCylinderSurface3()
{
	m_pCurve1 = NULL;
	m_pCurve2 = NULL;
}
/*----------------------------------------------------------------*/
SKGeneralizedCylinderSurface3::~SKGeneralizedCylinderSurface3()
{
	m_pCurve1 = NULL;
	m_pCurve2 = NULL;
}
/*----------------------------------------------------------------*/
bool SKGeneralizedCylinderSurface3::SetCurve(SKCurve3 * pCurve1,SKCurve3 *pCurve2)
{
	if(!pCurve1 || !pCurve2)
		return 0;
	m_pCurve1 = pCurve1;
	m_pCurve2 = pCurve2;
	return 1;
}
/*----------------------------------------------------------------*/
SKVector3 SKGeneralizedCylinderSurface3::GetPoint(SKREAL U,SKREAL V)
{
	return m_pCurve1->GetPoint(U) * (1 - V) + m_pCurve2->GetPoint(U) * V;
}
/*----------------------------------------------------------------*/
SKVector3 SKGeneralizedCylinderSurface3::GetUPartialDerivative(SKREAL U,SKREAL V)
{
	SKVector3 Temp = m_pCurve1->GetFirstDerivative(U) * (1 - V) + m_pCurve2->GetFirstDerivative(U) * V;
	//Temp.Normalize();
	return Temp;
}
/*----------------------------------------------------------------*/
SKVector3 SKGeneralizedCylinderSurface3::GetVPartialDerivative(SKREAL U,SKREAL V)
{
	SKVector3 Temp = m_pCurve2->GetPoint(U) - m_pCurve1->GetPoint(U);
	//Temp.Normalize();
	return Temp;
}
/*----------------------------------------------------------------*/