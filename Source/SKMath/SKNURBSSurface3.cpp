#include "SKNURBSSurface3.h"
#include "SKMemManager.h"
using namespace SKEngine2;
/*----------------------------------------------------------------*/
SKNURBSSurface3::SKNURBSSurface3()
{
	m_pWeight = NULL;
}
/*----------------------------------------------------------------*/
SKNURBSSurface3::~SKNURBSSurface3()
{
	SKMAC_DELETEA(m_pWeight);
}
/*----------------------------------------------------------------*/
bool SKNURBSSurface3::Set(const SKREAL * pWeight,unsigned int uiRow,unsigned int uiColumn)
{
	if(!pWeight || !uiRow || !uiColumn)
		return 0;
	unsigned int uiControlPointNum = uiRow * uiColumn;
	m_pWeight = SK_NEW SKREAL[uiControlPointNum];
	if(!m_pWeight)
		return 0;
	SKMemcpy(m_pWeight,pWeight,sizeof(SKREAL) * uiControlPointNum);
	return 1;

}
/*----------------------------------------------------------------*/
bool SKNURBSSurface3::Get(unsigned int Order,SKREAL U,SKREAL V ,SKVector3 *D0,SKVector3 *DU,SKVector3 *DV)
{
	int i0,i1;
	if(GetUFuncValue(U,i0,Order) && GetVFuncValue(V,i1,Order))
	{
		SKVector3 Point(0,0,0);
		SKREAL totalW = 0.0f;
		for(int j0 = i0 - m_uiUDegree; j0 <= i0 ; j0++)
		for(int j1 = i1 - m_uiVDegree; j1 <= i1 ; j1++)
		{
			SKREAL tmp =  GetUBasisValue(m_uiUDegree,j0) * GetVBasisValue(m_uiVDegree,j1) *GetWeight(j0,j1);

			Point += GetControlPoint(j0,j1) * tmp;
			totalW +=tmp;

		}
		SKREAL K0 = 1.0f / totalW;
		SKVector3 Point0 = Point * K0 ;
		if(D0)
			*D0 = Point0;
		if(Order == 0)
			return 1;



		SKVector3 kXDer1(0,0,0);
		SKVector3 kXDer2(0,0,0);
		SKREAL fWDer1 = 0.0f;
		SKREAL fWDer2 = 0.0f;
		for(int j0 = i0 - m_uiUDegree; j0 <= i0 ; j0++)
		for(int j1 = i1 - m_uiVDegree; j1 <= i1 ; j1++)
		{
			SKREAL fTmp1 = GetUBasisFDValue(m_uiUDegree,j0) * GetVBasisValue(m_uiVDegree,j1) *GetWeight(j0,j1);
			SKREAL fTmp2 = GetUBasisValue(m_uiUDegree,j0) * GetVBasisFDValue(m_uiVDegree,j1) *GetWeight(j0,j1); 
			kXDer1 += GetControlPoint(j0,j1) * fTmp1;
			kXDer2 += GetControlPoint(j0,j1) * fTmp2;
			fWDer1 += fTmp1;
			fWDer2 += fTmp2;

		}

		SKVector3 Point1 = (kXDer1 - Point0 * fWDer1) * K0 ;
		SKVector3 Point2 = (kXDer2 - Point0 * fWDer2) * K0 ;
		if(DU)
			*DU = Point1;
		if(DV)
			*DV = Point2;
		if(Order == 1)
			return 1;
		// not control all path
		return 1;
	}
	else
		return 0;

}
/*----------------------------------------------------------------*/
SKVector3 SKNURBSSurface3::GetPoint(SKREAL U,SKREAL V)
{
	SKVector3 Point;
	if(Get(0,U,V,&Point,NULL,NULL))
	{
		return Point;
	}
	else
	{
		return SKVector3(SKMAX_REAL,SKMAX_REAL,SKMAX_REAL);
	}

}
SKVector3 SKNURBSSurface3::GetUPartialDerivative(SKREAL U,SKREAL V)
{
	SKVector3 Point;
	if(Get(1,U,V,NULL,&Point,NULL))
	{
		return Point;
	}
	else
	{
		return SKVector3(SKMAX_REAL,SKMAX_REAL,SKMAX_REAL);
	}
}
/*----------------------------------------------------------------*/
SKVector3 SKNURBSSurface3::GetVPartialDerivative(SKREAL U,SKREAL V)
{
	SKVector3 Point;
	if(Get(1,U,V,NULL,NULL,&Point))
	{
		return Point;
	}
	else
	{
		return SKVector3(SKMAX_REAL,SKMAX_REAL,SKMAX_REAL);
	}


}
/*----------------------------------------------------------------*/
bool SKNURBSSurface3::Copy(const SKNURBSSurface3 & NURBSSurface3)
{
	if(!SKB_SplineSurface3::Copy(NURBSSurface3))
		return 0;
	if(!NURBSSurface3.m_pWeight)
		return 0;

	m_pWeight = SK_NEW SKREAL[m_uiControlPointNum];
	if(!m_pWeight)
		return 0;
	SKMemcpy(m_pWeight,NURBSSurface3.m_pWeight,sizeof(SKREAL) * m_uiControlPointNum);
	return 1;


}