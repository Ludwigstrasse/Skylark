#include "SKBezierSurface3.h"
#include "SKMemManager.h"
using namespace SKEngine2;
/*----------------------------------------------------------------*/
SKBezierSurface3::SKBezierSurface3()
{
	m_UC = NULL;
	m_VC = NULL;
	m_UPDValue = NULL;
	m_VPDValue = NULL;

}
/*----------------------------------------------------------------*/
SKBezierSurface3::~SKBezierSurface3()
{
	SKMAC_DELETEA(m_UC);
	SKMAC_DELETEA(m_VC);
	SKMAC_DELETEA(m_UPDValue);
	SKMAC_DELETEA(m_VPDValue);
}
/*----------------------------------------------------------------*/
bool SKBezierSurface3::Set(const SKVector3 * pControlPoint,unsigned int uiRow,unsigned int uiColumn)
{
	if(!pControlPoint || !uiRow || !uiColumn)
		return 0;
	m_uiColumn = uiColumn;
	m_uiRow = uiRow;
	m_uiControlPointNum = m_uiColumn * m_uiRow;
	
	SKMAC_DELETEA(m_UC);
	SKMAC_DELETEA(m_VC);
	SKMAC_DELETEA(m_UPDValue);
	SKMAC_DELETEA(m_VPDValue);
	SKMAC_DELETEA(m_pControlPoint);

	m_pControlPoint = SK_NEW SKVector3[m_uiControlPointNum];
	if(!m_pControlPoint)
		return 0;

	SKMemcpy(m_pControlPoint,pControlPoint,sizeof(SKVector3) * m_uiControlPointNum);

	m_UC = SK_NEW SKREAL [uiRow * uiRow];
	if(!m_UC)
		return 0;
	SKMemset(m_UC,0,sizeof(SKREAL) * uiRow * uiRow);
	SetUC(0,0,1.0f);
	SetUC(1,0,1.0f);
	SetUC(1,1,1.0f);
	for (unsigned int i = 2; i <  uiRow ; i++)
	{
		SetUC(i,0,1.0f);
		SetUC(i,i,1.0f);

		for (unsigned int j = 1; j < i; j++)
		{

			SetUC(i,j,GetUC(i - 1,j - 1) + GetUC(i - 1,j));
		}
	}

	m_VC = SK_NEW SKREAL [m_uiColumn * m_uiColumn];
	if(!m_VC)
		return 0;
	SKMemset(m_VC,0,sizeof(SKREAL) * m_uiColumn * m_uiColumn);
	SetVC(0,0,1.0f);
	SetVC(1,0,1.0f);
	SetVC(1,1,1.0f);
	for (unsigned int i = 2; i <  m_uiColumn; i++)
	{
		SetVC(i,0,1.0f);
		SetVC(i,i,1.0f);

		for (unsigned int j = 1; j < i; j++)
		{

			SetVC(i,j,GetVC(i - 1,j - 1) + GetVC(i - 1,j));
		}
	}

	m_UPDValue = SK_NEW SKVector3[(m_uiRow - 1) * m_uiColumn];
	if(!m_UPDValue)
		return 0;
	
	for (unsigned int i = 0; i < m_uiRow - 1; i++)
	{
		for(unsigned int j = 0 ; j < m_uiColumn ; j++)
		{
			SetUPD(i,j,GetControlPoint(i + 1,j) - GetControlPoint(i,j));
		}
	}

	m_VPDValue = SK_NEW SKVector3[m_uiRow * (m_uiColumn- 1)];
	if(!m_VPDValue)
		return 0;

	for (unsigned int i = 0; i < m_uiRow; i++)
	{
		for(unsigned int j = 0 ; j < m_uiColumn - 1 ; j++)
		{
			SetVPD(i,j,GetControlPoint(i,j+1) - GetControlPoint(i,j));
		}
	}
	return 1;
}
/*----------------------------------------------------------------*/
SKVector3 SKBezierSurface3::GetPoint(SKREAL U,SKREAL V)
{
	if(!m_UPDValue)
		return SKVector3(SKMAX_REAL,SKMAX_REAL,SKMAX_REAL);
	SKREAL fFU = (SKREAL)1.0 - U;

	SKREAL fU =(SKREAL)1.0;
	


	unsigned int uiDegreeU = m_uiRow - 1;
	unsigned int uiDegreeV = m_uiColumn - 1;
	SKVector3 kResult(0,0,0);

	for(unsigned int i = 0 ; i <= uiDegreeU ; i++)
	{
		SKREAL fFV = (SKREAL)1.0 - V;

		SKREAL fV =(SKREAL)1.0; 
		SKREAL PU = SKREAL(uiDegreeU - i);
		for(unsigned int j = 0; j <= uiDegreeV; j++)
		{
			SKREAL PV = SKREAL(uiDegreeV - j);
			SKVector3 fCoeff = GetControlPoint(i,j) * GetUC(uiDegreeU,i) * fU * POW(fFU,PU) * GetVC(uiDegreeV,j) * fV * POW(fFV,PV);
			kResult = kResult + fCoeff;
			fV *= V;
			fU *= U;

		}

	}

	return kResult;

}
/*----------------------------------------------------------------*/
SKVector3 SKBezierSurface3::GetUPartialDerivative(SKREAL U,SKREAL V)
{
	if(!m_UPDValue)
		return SKVector3(SKMAX_REAL,SKMAX_REAL,SKMAX_REAL);
	SKREAL fFU = (SKREAL)1.0 - U;
	SKREAL fU = (SKREAL)1.0;
	
	

	unsigned int uiDegreeU = m_uiRow - 2;
	unsigned int uiDegreeV = m_uiColumn - 1;

	SKVector3 kResult(0,0,0);
	
	for(unsigned int i = 0 ; i <= uiDegreeU ; i++)
	{
		SKREAL PU = SKREAL(uiDegreeU - i);
		SKREAL fFV = (SKREAL)1.0 - V;
		SKREAL fV = (SKREAL)1.0; 
		for(unsigned int j = 0; j <= uiDegreeV; j++)
		{
			SKREAL PV = SKREAL(uiDegreeV - j);
			SKVector3 fCoeff = GetUPD(i,j) * GetUC(uiDegreeU,i) * fU * POW(fFU,PU) * GetVC(uiDegreeV,j) * fV * POW(fFV,PV);
			kResult = kResult + fCoeff;
			fV *= V;
			fU *= U;
		}

	}
		
	kResult *= (SKREAL)uiDegreeU;	
	return kResult;

}
/*----------------------------------------------------------------*/
SKVector3 SKBezierSurface3::GetVPartialDerivative(SKREAL U,SKREAL V)
{
	if(!m_UPDValue)
		return SKVector3(SKMAX_REAL,SKMAX_REAL,SKMAX_REAL);
	SKREAL fFU = (SKREAL)1.0 - U;
	SKREAL fU = (SKREAL)1.0;
	


	unsigned int uiDegreeU = m_uiRow - 1;
	unsigned int uiDegreeV = m_uiColumn - 2;
	SKVector3 kResult(0,0,0);

	for(unsigned int i = 0 ; i <= uiDegreeU ; i++)
	{
		SKREAL PU = SKREAL(uiDegreeU - i);
		SKREAL fFV = (SKREAL)1.0 - V;
		SKREAL fV = (SKREAL)1.0; 
		for(unsigned int j = 0; j <= uiDegreeV; j++)
		{
			SKREAL PV = SKREAL(uiDegreeV - j);
			SKVector3 fCoeff = GetVPD(i,j) * GetUC(uiDegreeU,i) * fU * POW(fFU,PU) * GetVC(uiDegreeV,j) * fV * POW(fFV,PV);
			kResult = kResult + fCoeff;
			fV *= V;
			fU *= U;
		}

	}

	kResult *= (SKREAL)uiDegreeV;	
	return kResult;
}
/*----------------------------------------------------------------*/
bool SKBezierSurface3::Copy(const SKBezierSurface3 & BezierSurface3)
{
	if(!SKControlSurface3::Copy(BezierSurface3))
		return 0;
	if(!BezierSurface3.m_VC || !BezierSurface3.m_UC || !BezierSurface3.m_UPDValue || !BezierSurface3.m_VPDValue)
		return 0;

	SKMAC_DELETEA(m_UC);
	SKMAC_DELETEA(m_VC);
	SKMAC_DELETEA(m_UPDValue);
	SKMAC_DELETEA(m_VPDValue);
	
	m_UC = SK_NEW SKREAL [m_uiRow * m_uiRow];
	if(!m_UC)
		return 0;
	SKMemcpy(m_UC,BezierSurface3.m_UC,sizeof(SKREAL) * m_uiRow * m_uiRow);

	m_VC = SK_NEW SKREAL [m_uiColumn * m_uiColumn];
	if(!m_VC)
		return 0;
	SKMemcpy(m_VC,BezierSurface3.m_VC,sizeof(SKREAL) * m_uiColumn * m_uiColumn);

	m_UPDValue = SK_NEW SKVector3[(m_uiRow - 1) * m_uiColumn];
	if(!m_UPDValue)
		return 0;
	SKMemcpy(m_UPDValue,BezierSurface3.m_UPDValue,sizeof(SKVector3) * (m_uiRow - 1) * m_uiColumn);

	m_VPDValue = SK_NEW SKVector3[m_uiRow * (m_uiColumn- 1)];
	if(!m_VPDValue)
		return 0;
	SKMemcpy(m_VPDValue,BezierSurface3.m_VPDValue,sizeof(SKVector3) * m_uiRow * (m_uiColumn- 1));
	return 1;
}