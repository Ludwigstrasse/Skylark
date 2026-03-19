#include "SKControlSurface3.h"
using namespace SKEngine2;
#include "SKMemManager.h"
#include "SKSurfaceLengthFunction1.h"
/*----------------------------------------------------------------*/
SKControlSurface3::SKControlSurface3()
{
	m_pControlPoint = NULL;
	m_uiControlPointNum = 0;
	m_uiRow = 0;
	m_uiColumn = 0;

}
/*----------------------------------------------------------------*/
SKControlSurface3::~SKControlSurface3()
{
	SKMAC_DELETEA(m_pControlPoint);
	m_uiControlPointNum = 0;
	m_uiRow = 0;
	m_uiColumn = 0;

}

/*----------------------------------------------------------------*/
bool SKControlSurface3::Copy(const SKControlSurface3& ControlSurface3)
{
	if(!ControlSurface3.m_pControlPoint || !ControlSurface3.m_uiColumn || 
		!ControlSurface3.m_uiControlPointNum || !ControlSurface3.m_uiRow)
		return 0;
	SKMAC_DELETEA(m_pControlPoint);
	
	m_uiControlPointNum = ControlSurface3.m_uiControlPointNum;
	m_uiRow = ControlSurface3.m_uiRow;
	m_uiColumn = ControlSurface3.m_uiColumn;
	m_pControlPoint = SK_NEW SKVector3[m_uiControlPointNum];
	if(!m_pControlPoint)
		return 0;

	SKMemcpy(m_pControlPoint,ControlSurface3.m_pControlPoint,sizeof(SKVector3) * m_uiControlPointNum); 
	return 1;
}
SKREAL SKControlSurface3::GetULength(SKREAL V,SKREAL U1, SKREAL U2,unsigned int uiIterations)
{
	SKSurfaceLengthFunction1 LengthF(SKSurfaceLengthFunction1::LT_U_LEN,V);
	LengthF.SetSurface(this);
	return LengthF.Integration(U1,U2,uiIterations);
}
SKREAL SKControlSurface3::GetVLength(SKREAL U,SKREAL V1, SKREAL V2,unsigned int uiIterations)
{
	SKSurfaceLengthFunction1 LengthF(SKSurfaceLengthFunction1::LT_V_LEN,U);
	LengthF.SetSurface(this);
	return LengthF.Integration(V1,V2,uiIterations);
}
/*----------------------------------------------------------------*/
SKREAL SKControlSurface3::GetUTime (SKREAL V,SKREAL fLength, unsigned int uiIterations,SKREAL fTolerance)
{
	if (fLength <= 0.0f)
	{
		return 0.0f;
	}
	SKREAL TotalLength = GetULength(V,0.0f,1.0f,uiIterations);
	if (fLength >= TotalLength )
	{
		return 1.0f;
	}

	SKREAL fTime = fLength / TotalLength;

	for (unsigned int i = 0; i < uiIterations; i++)
	{
		SKREAL fDifference = GetULength(V,0.0f,fTime,uiIterations) - fLength;
		if (ABS(fDifference) < fTolerance)
		{
			return fTime;
		}

		fTime -= fDifference/(GetUPartialDerivative(fTime,V).GetLength());
	}

	return fTime;

}
/*----------------------------------------------------------------*/
SKREAL SKControlSurface3::GetVTime (SKREAL U,SKREAL fLength, unsigned int uiIterations,SKREAL fTolerance)
{
	if (fLength <= 0.0f)
	{
		return 0.0f;
	}
	SKREAL TotalLength = GetVLength(U,0.0f,1.0f,uiIterations);
	if (fLength >= TotalLength )
	{
		return 1.0f;
	}

	SKREAL fTime = fLength / TotalLength;

	for (unsigned int i = 0; i < uiIterations; i++)
	{
		SKREAL fDifference = GetVLength(U,0.0f,fTime,uiIterations) - fLength;
		if (ABS(fDifference) < fTolerance)
		{
			return fTime;
		}

		fTime -= fDifference/(GetVPartialDerivative(fTime,U).GetLength());
	}

	return fTime;

}