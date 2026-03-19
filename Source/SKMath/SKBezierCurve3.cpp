#include "SKBezierCurve3.h"
#include "SKMemManager.h"
using namespace SKEngine2;
/*----------------------------------------------------------------*/
SKBezierCurve3::SKBezierCurve3()
{
	m_pControlPoint = NULL;
	m_uiControlPointNum = 0;
	m_C = NULL;
	m_FDValue = NULL;
	m_SDValue = NULL;
	m_TDValue = NULL;
	m_CurveType = BEZIER;
}
/*----------------------------------------------------------------*/
SKBezierCurve3::~SKBezierCurve3()
{
	SKMAC_DELETEA(m_pControlPoint);
	m_uiControlPointNum = 0;
	SKMAC_DELETEA(m_C);
	SKMAC_DELETEA(m_FDValue);
	SKMAC_DELETEA(m_SDValue);
	SKMAC_DELETEA(m_TDValue);

}
/*----------------------------------------------------------------*/
bool SKBezierCurve3::Set(const SKVector3 * pControlPoint,unsigned int uiControlPointNum)
{
	if(!pControlPoint || !uiControlPointNum)
		return 0;
	SKMAC_DELETEA(m_pControlPoint);
	m_uiControlPointNum = uiControlPointNum;
	SKMAC_DELETEA(m_C);
	SKMAC_DELETEA(m_FDValue);
	SKMAC_DELETEA(m_SDValue);
	SKMAC_DELETEA(m_TDValue);

	m_pControlPoint = SK_NEW SKVector3[uiControlPointNum];
	if(!m_pControlPoint)
		return 0;
	SKMemcpy(m_pControlPoint,pControlPoint,sizeof(SKVector3) * m_uiControlPointNum);


	m_C = SK_NEW SKREAL [uiControlPointNum * uiControlPointNum];
	if(!m_C)
		return 0;
	SKMemset(m_C,0,sizeof(SKREAL) * uiControlPointNum * uiControlPointNum);
	SetC(0,0,1.0f);
	SetC(1,0,1.0f);
	SetC(1,1,1.0f);
	for (unsigned int i = 2; i <  uiControlPointNum ; i++)
	{
		SetC(i,0,1.0f);
		SetC(i,i,1.0f);
		
		for (unsigned int j = 1; j < i; j++)
		{

			SetC(i,j,GetC(i - 1,j - 1) + GetC(i - 1,j));
		}
	}
	
	m_FDValue = SK_NEW SKVector3[m_uiControlPointNum-1];
	if(!m_FDValue)
		return 0;
	for (unsigned int i = 0; i < m_uiControlPointNum - 1; i++)
	{
		m_FDValue[i] = m_pControlPoint[i + 1] - m_pControlPoint[i];
	}

	
	m_SDValue = SK_NEW SKVector3[m_uiControlPointNum - 2];
	if(!m_SDValue)
		return 0;
	for (unsigned int i = 0; i < m_uiControlPointNum - 2; i++)
	{
		m_SDValue[i] = m_FDValue[i + 1] - m_FDValue[i];
	}

	
	if (m_uiControlPointNum >= 4)
	{
		m_TDValue = SK_NEW SKVector3[m_uiControlPointNum - 3];
		if(!m_TDValue)
			return 0;
		for (unsigned int i = 0; i < m_uiControlPointNum - 3; i++)
		{
			m_TDValue[i] = m_SDValue[i + 1] - m_SDValue[i];
		}
	}
	return 1;
}
/*----------------------------------------------------------------*/
SKVector3	SKBezierCurve3::GetFirstDerivative(SKREAL t)
{
	if(!m_FDValue)
		return SKVector3(SKMAX_REAL,SKMAX_REAL,SKMAX_REAL);
	SKREAL fOmTime = (SKREAL)1.0 - t;
	SKREAL fPowTime = (SKREAL)1.0;
	
	unsigned int uiDegree = m_uiControlPointNum - 2;

	SKVector3 kResult(0,0,0);
	for (unsigned int i = 0; i <= uiDegree; i++)
	{
		SKREAL fCoeff = GetC(uiDegree,i) * fPowTime;
		SKREAL P =  SKREAL(uiDegree - i);
		kResult = kResult + m_FDValue[i] * fCoeff  * POW(fOmTime,P);
		fPowTime *= t;
	}
	kResult *= SKREAL(m_uiControlPointNum - 1);
	return kResult;

}
/*----------------------------------------------------------------*/
SKVector3	SKBezierCurve3::GetSecondDerivative(SKREAL t)
{
	if(!m_SDValue)
		return SKVector3(SKMAX_REAL,SKMAX_REAL,SKMAX_REAL);
	SKREAL fOmTime = (SKREAL)1.0 - t;
	SKREAL fPowTime = (SKREAL)1.0;

	unsigned int uiDegree = m_uiControlPointNum - 3;

	SKVector3 kResult(0,0,0);
	for (unsigned int i = 0; i <= uiDegree; i++)
	{
		SKREAL fCoeff = GetC(uiDegree,i) * fPowTime;
		SKREAL P =  SKREAL(uiDegree - i);
		kResult = kResult + m_SDValue[i] * fCoeff  * POW(fOmTime,P);
		fPowTime *= t;
	}
	kResult *= SKREAL((m_uiControlPointNum - 1) * (m_uiControlPointNum - 2));
	return kResult;

}
/*----------------------------------------------------------------*/
SKVector3	SKBezierCurve3::GetThirdDerivative(SKREAL t)
{
	if(!m_TDValue)
		return SKVector3(SKMAX_REAL,SKMAX_REAL,SKMAX_REAL);
	SKREAL fOmTime = (SKREAL)1.0 - t;
	SKREAL fPowTime = (SKREAL)1.0;
	
	unsigned int uiDegree = m_uiControlPointNum - 4;

	SKVector3 kResult(0,0,0);
	for (unsigned int i = 0; i <= uiDegree; i++)
	{
		SKREAL fCoeff = GetC(uiDegree,i) * fPowTime;
		SKREAL P =  SKREAL(uiDegree - i);
		kResult = kResult + m_TDValue[i] * fCoeff  * POW(fOmTime,P);
		fPowTime *= t;
	}
	kResult *= SKREAL((m_uiControlPointNum - 3) * (m_uiControlPointNum - 1) * (m_uiControlPointNum - 2));
	return kResult;

}
/*----------------------------------------------------------------*/
SKVector3	SKBezierCurve3::GetPoint(SKREAL t)
{
	SKREAL fOmTime = (SKREAL)1.0 - t;
	SKREAL fPowTime = (SKREAL)1.0;
	unsigned int uiDegree = m_uiControlPointNum - 1;

	SKVector3 kResult(0.0f,0.0f,0.0f);

	for (unsigned int i = 0; i <= uiDegree; i++)
	{
		SKREAL fCoeff = GetC(uiDegree,i) * fPowTime;
		SKREAL P =  SKREAL(uiDegree - i);
		kResult = kResult + m_pControlPoint[i] * fCoeff  * POW(fOmTime,P);
		fPowTime *= t;
	}


	return kResult;

}
/*----------------------------------------------------------------*/
bool SKBezierCurve3::Copy(const SKBezierCurve3 & BezierCurve3)
{
	
	if(!SKControlCurve3::Copy(BezierCurve3))
		return 0;
	if(BezierCurve3.m_C || !BezierCurve3.m_FDValue ||! BezierCurve3.m_SDValue ||!BezierCurve3.m_TDValue)
		return 0;
	SKMAC_DELETEA(m_C);
	SKMAC_DELETEA(m_FDValue);
	SKMAC_DELETEA(m_SDValue);
	SKMAC_DELETEA(m_TDValue);
	m_C = SK_NEW SKREAL [m_uiControlPointNum * m_uiControlPointNum];
	if(!m_C)
		return 0;
	SKMemcpy(m_C,BezierCurve3.m_C,sizeof(SKREAL) * m_uiControlPointNum * m_uiControlPointNum);

	m_FDValue = SK_NEW SKVector3[m_uiControlPointNum-1];
	if(!m_FDValue)
		return 0;
	SKMemcpy(m_FDValue,BezierCurve3.m_FDValue,sizeof(SKVector3) * (m_uiControlPointNum - 1));

	m_SDValue = SK_NEW SKVector3[m_uiControlPointNum - 2];
	if(!m_SDValue)
		return 0;
	SKMemcpy(m_SDValue,BezierCurve3.m_SDValue,sizeof(SKVector3) * (m_uiControlPointNum - 2));
	if (m_uiControlPointNum >= 4)
	{
		m_TDValue = SK_NEW SKVector3[m_uiControlPointNum - 3];
		if(!m_TDValue)
			return 0;
		SKMemcpy(m_TDValue,BezierCurve3.m_TDValue,sizeof(SKVector3) * (m_uiControlPointNum - 1));
	}
	return 1;
}
/*----------------------------------------------------------------*/
