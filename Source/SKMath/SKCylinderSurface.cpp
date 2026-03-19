#include "SKCylinderSurface3.h"
using namespace SKEngine2;
/*----------------------------------------------------------------*/
SKCylinderSurface3::SKCylinderSurface3()
{
	m_pCurve1 = NULL;
	m_pCurve2 = NULL;
}
/*----------------------------------------------------------------*/
SKCylinderSurface3::~SKCylinderSurface3()
{
	m_pCurve1 = NULL;
	m_pCurve2 = NULL;
}
/*----------------------------------------------------------------*/
bool SKCylinderSurface3::SetCurve(SKCurve3 * pCurve1,SKSegmentCurve3 *pCurve2)
{
	if(!pCurve1 || !pCurve2)
		return 0;
	m_pCurve1 = pCurve1;
	m_pCurve2 = pCurve2;
	return 1;

}
/*----------------------------------------------------------------*/
SKVector3 SKCylinderSurface3::GetPoint(SKREAL U,SKREAL V)
{
	return m_pCurve1->GetPoint(U) + m_pCurve2->GetSegment().GetDir() * (V *  m_pCurve2->GetSegment().GetLen());

}
/*----------------------------------------------------------------*/
SKVector3 SKCylinderSurface3::GetUPartialDerivative(SKREAL U,SKREAL V)
{

	return m_pCurve1->GetFirstDerivative(U);
}
/*----------------------------------------------------------------*/
SKVector3 SKCylinderSurface3::GetVPartialDerivative(SKREAL U,SKREAL V)
{

	return m_pCurve2->GetSegment().GetDir();
}
/*----------------------------------------------------------------*/