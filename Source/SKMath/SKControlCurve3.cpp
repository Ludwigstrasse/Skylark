#include "SKControlCurve3.h"
#include "SKMatrix3X3W.h"
#include "SKMemManager.h"
#include "SKCurveLengthFunction1.h"
using namespace SKEngine2;
/*----------------------------------------------------------------*/
SKControlCurve3::SKControlCurve3()
{
	m_pControlPoint = NULL;
	m_uiControlPointNum = 0;

}
/*----------------------------------------------------------------*/
SKControlCurve3::~SKControlCurve3()
{
	SKMAC_DELETEA(m_pControlPoint);
	m_uiControlPointNum = 0;

}
/*----------------------------------------------------------------*/
void	SKControlCurve3::GetFrenetFrameBNT(SKREAL t,SKVector3 & B,SKVector3 & N,SKVector3 &T)
{
	SKVector3 FD = GetFirstDerivative(t);
	FD.Normalize();
	SKVector3 SD = GetSecondDerivative(t);
	SD.Normalize();
	SKREAL FDDot = FD.Dot(FD);
	SKREAL SDDotFD = FD.Dot(SD);
	N = SD - FD * SDDotFD;
	N.Normalize();
	T = FD;
	B.Cross(N,T);

}
/*----------------------------------------------------------------*/
SKREAL SKControlCurve3::GetTotalLength(unsigned int uiIterations)
{
	return GetLength(0.0f,1.0f,uiIterations);

}
/*----------------------------------------------------------------*/
SKREAL SKControlCurve3::GetTime (SKREAL fLength, unsigned int uiIterations,SKREAL fTolerance)
{
	if (fLength <= 0.0f)
	{
		return 0.0f;
	}
	SKREAL TotalLength = GetTotalLength(uiIterations);
	if (fLength >= TotalLength )
	{
		return 1.0f;
	}

	SKREAL fTime = fLength / TotalLength;

	for (unsigned int i = 0; i < uiIterations; i++)
	{
		SKREAL fDifference = GetLength(0.0f,fTime,uiIterations) - fLength;
		if (ABS(fDifference) < fTolerance)
		{
			return fTime;
		}

		fTime -= fDifference/(GetFirstDerivative(fTime).GetLength());
	}

	return fTime;

}
/*----------------------------------------------------------------*/
SKREAL SKControlCurve3::GetCurvature(SKREAL fTime)
{
	SKVector3 FD = GetFirstDerivative(fTime);
	SKREAL fSpeedSqr = FD.GetSqrLength();

	if (fSpeedSqr >= EPSILON_E4)
	{
		SKVector3 SD = GetSecondDerivative(fTime);
		SKVector3 Cross;
		Cross.Cross(FD,SD);
		SKREAL fNumer = Cross.GetLength();
		SKREAL fDenom = POW(fSpeedSqr,(SKREAL)1.5);

		return fNumer / fDenom;
	}
	else
	{

		return 0.0f;
	}

}
/*----------------------------------------------------------------*/
SKREAL SKControlCurve3::GetTorsion(SKREAL fTime)
{
	SKVector3 FD = GetFirstDerivative(fTime);
	SKVector3 SD = GetSecondDerivative(fTime);
	SKVector3 Cross;
	Cross.Cross(FD,SD);
	SKREAL fDenom = Cross.GetSqrLength();

	if (fDenom >= EPSILON_E4)
	{
		SKVector3 TD= GetThirdDerivative(fTime);
		SKREAL fNumer = Cross.Dot(TD);
		return fNumer / fDenom;
	}
	else
	{
		return 0.0f;
	}

}
/*----------------------------------------------------------------*/
bool SKControlCurve3::Copy(const SKControlCurve3& ControlCurve3)
{
	if(!ControlCurve3.m_pControlPoint || !ControlCurve3.m_uiControlPointNum)
		return 0;
	SKMAC_DELETEA(m_pControlPoint);
	m_uiControlPointNum = ControlCurve3.m_uiControlPointNum;
	m_pControlPoint = SK_NEW SKVector3[m_uiControlPointNum];
	if(!m_pControlPoint)
		return 0;
	SKMemcpy(m_pControlPoint,ControlCurve3.m_pControlPoint,sizeof(SKVector3) * m_uiControlPointNum);
	return 1;
}
/*----------------------------------------------------------------*/
SKREAL	SKControlCurve3::GetLength(SKREAL t1 ,SKREAL t2,unsigned int uiIterations)
{
	SKCurveLengthFunction1 LengthF;
	LengthF.SetCurve(this);
	return LengthF.Integration(t1,t2,uiIterations);

}