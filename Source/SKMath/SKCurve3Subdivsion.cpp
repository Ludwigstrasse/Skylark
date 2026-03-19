#include "SKCurve3Subdivision.h"
#include "SKMemManager.h"
#include "SKB_SplineCurve3.h"
using namespace SKEngine2;
/*----------------------------------------------------------------*/
SKCurve3Subdivision::SKCurve3Subdivision()
{
	m_pPoint = NULL;
	m_uiPointNum = 0;
	m_pCurve = NULL;
	m_pT = NULL;
}
/*----------------------------------------------------------------*/
SKCurve3Subdivision::~SKCurve3Subdivision()
{
	SKMAC_DELETEA(m_pPoint);
	SKMAC_DELETEA(m_pT);
	m_uiPointNum = 0;


}
/*----------------------------------------------------------------*/
bool SKCurve3Subdivision::SetSubCurve(SKControlCurve3 * pCurve)
{
	if(!pCurve)
		return 0;
	m_pCurve = pCurve;
	return 1;
}
/*----------------------------------------------------------------*/
bool SKCurve3Subdivision::ControlPointChange(unsigned int i)
{
	if(!m_pCurve || !m_pPoint || !m_pT || !m_uiPointNum)
		return 0;

	if(i >= m_pCurve->GetControlPointNum())
		return 0;
	if(m_pCurve->GetType() == SKControlCurve3::BEZIER)
	{
		for(unsigned int i = 0; i < m_uiPointNum ; i++)
			m_pPoint[i] = m_pCurve->GetPoint(m_pT[i]);
		return 1;
	}
	else if(m_pCurve->GetType() == SKControlCurve3::B_SPLINE || m_pCurve->GetType() == SKControlCurve3::NURBS)
	{
		SKB_SplineCurve3 *pCurve =(SKB_SplineCurve3 *)m_pCurve;
		SKREAL t1 =pCurve->GetKnotVectorValue(i);
		SKREAL t2 =pCurve->GetKnotVectorValue(pCurve->GetDegree() + i); 
		for(unsigned int i = 0; i < m_uiPointNum ; i++)
			if(m_pT[i] >= t1 && m_pT[i] <= t2)
				m_pPoint[i] = m_pCurve->GetPoint(m_pT[i]);
		return 1;
	}
	else
		return 0;
}
/*----------------------------------------------------------------*/