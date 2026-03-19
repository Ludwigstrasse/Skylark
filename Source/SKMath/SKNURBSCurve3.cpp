#include "SKNURBSCurve3.h"
#include "SKMemManager.h"
using namespace SKEngine2;
/*----------------------------------------------------------------*/
SKNURBSCurve3::SKNURBSCurve3()
{
	m_pWeight = NULL;
	m_CurveType = NURBS;
}
/*----------------------------------------------------------------*/
SKNURBSCurve3::~SKNURBSCurve3()
{
	SKMAC_DELETEA(m_pWeight);
}
/*----------------------------------------------------------------*/
bool SKNURBSCurve3::Set(const SKREAL * pWeight,unsigned int uiControlPointNum)
{
	if(!pWeight || !uiControlPointNum)
		return 0;
	m_pWeight = SK_NEW SKREAL[uiControlPointNum];
	if(!m_pWeight)
		return 0;
	SKMemcpy(m_pWeight,pWeight,sizeof(SKREAL) * uiControlPointNum);
	return 1;

}
/*----------------------------------------------------------------*/
bool SKNURBSCurve3::Get(unsigned int Order,SKREAL t ,SKVector3 *D0,SKVector3 *D1,SKVector3 *D2,SKVector3 *D3)
{
	int i;
	if(GetFuncValue(t,i,Order))
	{
		SKVector3 Point(0,0,0);
		SKREAL totalW = 0.0f;
		for(int j = i - m_uiDegree; j <= i ; j++)
		{
			SKREAL tmp =  GetBasisValue(m_uiDegree,j) * m_pWeight[j];

			Point += m_pControlPoint[j] * tmp;
			totalW +=tmp;

		}
		SKREAL K0 = 1.0f / totalW;
		SKVector3 Point0 = Point * K0 ;
		if(D0)
			*D0 = Point0;
		if(Order == 0)
			return 1;



		SKVector3 kXDer1(0,0,0);
		SKREAL fWDer1 = 0.0f;
		for(int j = i - m_uiDegree; j <= i ; j++)
		{
			SKREAL fTmp = GetBasisFDValue(m_uiDegree,i) * m_pWeight[i];
			kXDer1 += m_pControlPoint[i] * fTmp;
			fWDer1 += fTmp;


		}
		SKREAL K1 = fWDer1;
		SKVector3 Point1 = (kXDer1 - Point0 * fWDer1) * K0 ;
		if(D1)
			*D1 = Point1;
		if(Order == 1)
			return 1;

		SKVector3 kXDer2(0,0,0);
		SKREAL fWDer2 = 0.0f;
		for(int j = i - m_uiDegree; j <= i ; j++)
		{
			SKREAL fTmp = GetBasisSDValue(m_uiDegree,i) * m_pWeight[i];
			kXDer2 += m_pControlPoint[i] * fTmp;
			fWDer2 += fTmp;

		}
		SKREAL K2 = fWDer2;
		SKVector3 Point2 = (kXDer2 - Point1 * (2.0f) * K1  - Point0  * fWDer2) * K0;
		if(D2)
			*D2 = Point2;
		if(Order == 2)
			return 1;

		SKVector3 kXDer3(0,0,0);
		SKREAL fWDer3 = 0.0f;
		for(int j = i - m_uiDegree; j <= i ; j++)
		{
			SKREAL fTmp = GetBasisTDValue(m_uiDegree,i) * m_pWeight[i];
			kXDer3 += m_pControlPoint[i] * fTmp;
			fWDer3 += fTmp;

		}
		SKVector3 Point3 = (kXDer3 -  Point2 * ( 3.0 )* K1  - Point1 * ( 3.0 ) * K2   -  Point0 * fWDer3) * K0;
		if(D3)
			*D3 = Point3;
		if(Order ==  3)
			return 1;

		//not control all path
		return 1;
	}
	else
		return 0;

}
/*----------------------------------------------------------------*/
SKVector3	SKNURBSCurve3::GetPoint(SKREAL t)
{


	SKVector3 Point;
	if(Get(0,t,&Point,NULL,NULL,NULL))
	{
		return Point;
	}
	else
	{
		return SKVector3(SKMAX_REAL,SKMAX_REAL,SKMAX_REAL);
	}

}
/*----------------------------------------------------------------*/
SKVector3	SKNURBSCurve3::GetFirstDerivative(SKREAL t)
{

	SKVector3 Point;
	if(Get(1,t,NULL,&Point,NULL,NULL))
	{
		return Point;
	}
	else
	{
		return SKVector3(SKMAX_REAL,SKMAX_REAL,SKMAX_REAL);
	}

}
/*----------------------------------------------------------------*/
SKVector3	SKNURBSCurve3::GetSecondDerivative(SKREAL t)
{

	SKVector3 Point;
	if(Get(2,t,NULL,NULL,&Point,NULL))
	{
		return Point;
	}
	else
	{
		return SKVector3(SKMAX_REAL,SKMAX_REAL,SKMAX_REAL);
	}

}
/*----------------------------------------------------------------*/
SKVector3	SKNURBSCurve3::GetThirdDerivative(SKREAL t)
{

	SKVector3 Point;
	if(Get(3,t,NULL,NULL,NULL,&Point))
	{
		return Point;
	}
	else
	{
		return SKVector3(SKMAX_REAL,SKMAX_REAL,SKMAX_REAL);
	}

}
/*----------------------------------------------------------------*/
bool SKNURBSCurve3::Copy(const SKNURBSCurve3 &NURBSCurve3)
{
	if(!SKB_SplineCurve3::Copy(NURBSCurve3))
		return 0;
	if(!NURBSCurve3.m_pWeight)
		return 0;
	m_pWeight = SK_NEW SKREAL[m_uiControlPointNum];
	if(!m_pWeight)
		return 0;
	SKMemcpy(m_pWeight,NURBSCurve3.m_pWeight,sizeof(SKREAL) * m_uiControlPointNum);
	return 1;

}