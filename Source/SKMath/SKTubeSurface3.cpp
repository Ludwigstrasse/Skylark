#include "SKTubeSurface3.h"
using namespace SKEngine2;
SKTubeSurface3::SKTubeSurface3()
{
	m_pCurve = NULL;
}
/*----------------------------------------------------------------*/
SKTubeSurface3::~SKTubeSurface3()
{
	m_pCurve = NULL;

}
/*----------------------------------------------------------------*/
bool SKTubeSurface3::SetCurve(SKCurve3 * pCurve,SKREAL fN,SKREAL fB)
{
	if(!pCurve)
		return 0;
	m_pCurve = pCurve;
	m_fN = fN;
	m_fB = fB;
	return 1;
}
/*----------------------------------------------------------------*/
SKVector3 SKTubeSurface3::GetPoint(SKREAL U,SKREAL V)
{
	SKVector3 T,B,N;
	m_pCurve->GetFrenetFrameBNT(U,B,N,T);
	B.Normalize();
	N.Normalize();
	return m_pCurve->GetPoint(U) + N * COS(VS2PI * V) * m_fN  + B * SIN(VS2PI * V) * m_fB;
}
/*----------------------------------------------------------------*/
SKVector3 SKTubeSurface3::GetUPartialDerivative(SKREAL U,SKREAL V)
{
	return m_pCurve->GetFirstDerivative(U);
}
/*----------------------------------------------------------------*/
SKVector3 SKTubeSurface3::GetVPartialDerivative(SKREAL U,SKREAL V)
{
	SKVector3 T,B,N;
	m_pCurve->GetFrenetFrameBNT(U,B,N,T);
	B.Normalize();
	N.Normalize();
	return B * COS(VS2PI * V) * m_fB - N * SIN(VS2PI * V) * m_fN ;
}
/*----------------------------------------------------------------*/