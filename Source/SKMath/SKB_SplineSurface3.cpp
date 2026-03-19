#include "SKB_SplineSurface3.h"
#include "SKMemManager.h"
using namespace SKEngine2;
/*----------------------------------------------------------------*/
SKB_SplineSurface3::SKB_SplineSurface3()
{
	m_pUKnotVector = NULL;
	m_uiUDegree = 0;
	m_uiUKnotNum = 0;
	m_pUBasis = NULL;
	m_pUBasisFD = NULL;
	m_uiUBasisNum = 0;


	m_pVKnotVector = NULL;
	m_uiVDegree = 0;
	m_uiVKnotNum = 0;
	m_pVBasis = NULL;
	m_pVBasisFD = NULL;
	m_uiVBasisNum = 0;
	
}
/*----------------------------------------------------------------*/
SKB_SplineSurface3::~SKB_SplineSurface3()
{
	SKMAC_DELETEA(m_pUKnotVector);
	m_uiUDegree = 0;
	m_uiUKnotNum = 0;
	SKMAC_DELETEA(m_pUBasis);
	SKMAC_DELETEA(m_pUBasisFD);
	m_uiUBasisNum = 0;


	SKMAC_DELETEA(m_pVKnotVector);
	m_uiVDegree = 0;
	m_uiVKnotNum = 0;
	SKMAC_DELETEA(m_pVBasis);
	SKMAC_DELETEA(m_pVBasisFD);
	m_uiVBasisNum = 0;

}
/*----------------------------------------------------------------*/
bool SKB_SplineSurface3::Set(const SKVector3 * pControlPoint,unsigned int uiRow,unsigned int uiColumn)
{
	if(!pControlPoint || !uiRow || !uiColumn)
		return 0 ;
	SKMAC_DELETEA(m_pControlPoint);
	m_uiControlPointNum = uiRow * uiColumn;
	m_uiRow = uiRow;
	m_uiColumn = uiColumn;
	m_pControlPoint = SK_NEW SKVector3[m_uiControlPointNum];
	if(!m_pControlPoint)
		return 0;
	SKMemcpy(m_pControlPoint,pControlPoint,m_uiControlPointNum * sizeof(SKVector3));
	return 1;

}
/*----------------------------------------------------------------*/
bool SKB_SplineSurface3::SetUKnotVector(const SKREAL * pUKnotVector,unsigned int uiUDegree,unsigned int uiRow)
{
	if(!pUKnotVector)
		return 0;
	for(unsigned int i = 0 ; i < uiUDegree + uiRow; i++)
	{
		if(pUKnotVector[i] > pUKnotVector[i+1])
			return 0;
	}
	m_uiUDegree = uiUDegree;
	m_uiRow = uiRow;
	SKMAC_DELETEA(m_pUKnotVector);
	m_uiUKnotNum = uiRow + 1 + uiUDegree;
	m_pUKnotVector = SK_NEW SKREAL[m_uiUKnotNum];
	if(!m_pUKnotVector)
		return 0;
	SKMemcpy(m_pUKnotVector,pUKnotVector,sizeof(SKREAL) * m_uiUKnotNum);

	SKMAC_DELETEA(m_pUBasis);
	m_uiUBasisNum = (uiUDegree + 1) * m_uiUKnotNum;
	m_pUBasis = SK_NEW SKREAL[m_uiUBasisNum]; 
	if(!m_pUBasis)
		return 0;


	SKMAC_DELETEA(m_pUBasisFD);
	m_pUBasisFD = SK_NEW SKREAL[m_uiUBasisNum]; 
	if(!m_pUBasisFD)
		return 0;



	return 1;

}
/*----------------------------------------------------------------*/
bool SKB_SplineSurface3::SetVKnotVector(const SKREAL * pVKnotVector,unsigned int uiVDegree,unsigned int uiColumn)
{
	if(!pVKnotVector)
		return 0;
	for(unsigned int i = 0 ; i < uiVDegree + uiColumn; i++)
	{
		if(pVKnotVector[i] > pVKnotVector[i+1])
			return 0;
	}
	m_uiVDegree = uiVDegree;
	m_uiColumn= uiColumn;
	SKMAC_DELETEA(m_pVKnotVector);
	m_uiVKnotNum = uiColumn + 1 + uiVDegree;
	m_pVKnotVector = SK_NEW SKREAL[m_uiVKnotNum];
	SKMemcpy(m_pVKnotVector,pVKnotVector,sizeof(SKREAL) * (m_uiVKnotNum));

	SKMAC_DELETEA(m_pVBasis);
	m_uiVBasisNum = (uiVDegree + 1) * m_uiVKnotNum;
	m_pVBasis = SK_NEW SKREAL[m_uiVBasisNum]; 
	if(!m_pVBasis)
		return 0;


	SKMAC_DELETEA(m_pVBasisFD);
	m_pVBasisFD = SK_NEW SKREAL[m_uiVBasisNum]; 
	if(!m_pVBasisFD)
		return 0;



	return 1;
}
/*----------------------------------------------------------------*/
bool SKB_SplineSurface3::CreateOpenUniformUKnotVector(unsigned int uiDegree,unsigned int uiRow)
{
	m_uiUDegree = uiDegree;
	m_uiRow = uiRow;
	SKMAC_DELETEA(m_pUKnotVector);
	m_uiUKnotNum = uiRow + 1 + uiDegree;

	m_pUKnotVector = SK_NEW SKREAL[m_uiUKnotNum];
	if(!m_pUKnotVector)
		return 0;
	for(unsigned int i = 0; i < m_uiUKnotNum ; i++)
	{
		if(i >= 0 && i <= m_uiUDegree)
		{
			m_pUKnotVector[i] = 0;

		}
		else if( i >= m_uiUDegree + 1 && i <= m_uiRow)
		{
			m_pUKnotVector[i] = ((SKREAL)1.0 * (i - m_uiUDegree)) / (m_uiRow + 1 - m_uiUDegree);

		}
		else
		{
			m_pUKnotVector[i] = 1;
		}
	}

	SKMAC_DELETEA(m_pUBasis);
	m_uiUBasisNum = (uiDegree + 1) * m_uiUKnotNum;
	m_pUBasis = SK_NEW SKREAL[m_uiUBasisNum]; 
	if(!m_pUBasis)
		return 0;


	SKMAC_DELETEA(m_pUBasisFD);
	m_pUBasisFD = SK_NEW SKREAL[m_uiUBasisNum]; 
	if(!m_pUBasisFD)
		return 0;


	return 1;
}
/*----------------------------------------------------------------*/
bool SKB_SplineSurface3::CreatePeriodicUKnotVector(unsigned int uiDegree,unsigned int uiRow)
{
	m_uiUDegree = uiDegree;
	m_uiRow = uiRow;
	SKMAC_DELETEA(m_pUKnotVector);
	m_uiUKnotNum = uiRow + 1 + uiDegree;
	m_pUKnotVector = SK_NEW SKREAL[m_uiUKnotNum];
	if(!m_pUKnotVector)
		return 0;
	for(unsigned int i = 0 ; i < m_uiUKnotNum ; i++)
	{

		m_pUKnotVector[i] = ((SKREAL)1.0 * (i - m_uiUDegree)) / (m_uiRow + 1 - m_uiUDegree);

	}

	SKMAC_DELETEA(m_pUBasis);
	m_uiUBasisNum = (uiDegree + 1) * m_uiUKnotNum;
	m_pUBasis = SK_NEW SKREAL[m_uiUBasisNum]; 
	if(!m_pUBasis)
		return 0;


	SKMAC_DELETEA(m_pUBasisFD);
	m_pUBasisFD = SK_NEW SKREAL[m_uiUBasisNum]; 
	if(!m_pUBasisFD)
		return 0;

	return 1;
}
/*----------------------------------------------------------------*/
bool SKB_SplineSurface3::CreateOpenUniformVKnotVector(unsigned int uiDegree,unsigned int uiColumn)
{
	m_uiVDegree = uiDegree;
	m_uiColumn = uiColumn;
	SKMAC_DELETEA(m_pVKnotVector);
	m_uiVKnotNum = uiColumn + 1 + uiDegree;

	m_pVKnotVector = SK_NEW SKREAL[m_uiVKnotNum];
	if(!m_pVKnotVector)
		return 0;
	for(unsigned int i  = 0 ; i < m_uiVKnotNum ; i++)
	{
		if(i >= 0 && i <= m_uiVDegree)
		{
			m_pVKnotVector[i] = 0;

		}
		else if( i >= m_uiVDegree + 1 && i <= m_uiColumn)
		{
			m_pVKnotVector[i] = ((SKREAL)1.0 * (i - m_uiVDegree)) / (m_uiColumn + 1 - m_uiVDegree);

		}
		else
		{
			m_pVKnotVector[i] = 1;
		}
	}

	SKMAC_DELETEA(m_pVBasis);
	m_uiVBasisNum = (uiDegree + 1) * m_uiVKnotNum;
	m_pVBasis = SK_NEW SKREAL[m_uiVBasisNum]; 
	if(!m_pVBasis)
		return 0;


	SKMAC_DELETEA(m_pVBasisFD);
	m_pVBasisFD = SK_NEW SKREAL[m_uiVBasisNum]; 
	if(!m_pVBasisFD)
		return 0;

	return 1;
}
/*----------------------------------------------------------------*/
bool SKB_SplineSurface3::CreatePeriodicVKnotVector(unsigned int uiDegree,unsigned int uiColumn)
{
	m_uiVDegree = uiDegree;
	m_uiColumn = uiColumn;
	SKMAC_DELETEA(m_pVKnotVector);
	m_uiVKnotNum = uiColumn + 1 + uiDegree;
	m_pVKnotVector = SK_NEW SKREAL[m_uiVKnotNum];
	if(!m_pVKnotVector)
		return 0;
	for(unsigned int i = 0; i < m_uiVKnotNum ; i++)
	{

		m_pVKnotVector[i] = ((SKREAL)1.0 * (i - m_uiVDegree)) / (m_uiColumn + 1 - m_uiVDegree);

	}

	SKMAC_DELETEA(m_pVBasis);
	m_uiVBasisNum = (uiDegree + 1) * m_uiVKnotNum;
	m_pVBasis = SK_NEW SKREAL[m_uiVBasisNum]; 
	if(!m_pVBasis)
		return 0;


	SKMAC_DELETEA(m_pVBasisFD);
	m_pVBasisFD = SK_NEW SKREAL[m_uiVBasisNum]; 
	if(!m_pVBasisFD)
		return 0;


	return 1;

}
/*----------------------------------------------------------------*/
int SKB_SplineSurface3::GetUKey(SKREAL t)const
{
	if( t > 1.0f || t < 0.0f)
		return SKMAX_INTEGER;
	unsigned int i;
	for(i = 0 ; i < m_uiUKnotNum ; i++)
	{
		if(t < m_pUKnotVector[i])
			break;

	}
	return (i - 1);
}
/*----------------------------------------------------------------*/
int SKB_SplineSurface3::GetVKey(SKREAL t)const
{
	if( t > 1.0f || t < 0.0f)
		return SKMAX_INTEGER;
	unsigned int i;
	for(i = 0 ; i < m_uiVKnotNum ; i++)
	{
		if(t < m_pVKnotVector[i])
			break;

	}
	return (i - 1);
}
/*----------------------------------------------------------------*/
bool SKB_SplineSurface3::GetUFuncValue(SKREAL t,int &i,unsigned int uiOrder)
{
	if(!m_pUBasis)
		return 0;
	i = GetUKey(t);
	if(i == SKMAX_INTEGER)
		return 0;
	SKMemset(m_pUBasis,0,sizeof(SKREAL) * m_uiUBasisNum);
	SKMemset(m_pUBasisFD,0,sizeof(SKREAL) * m_uiUBasisNum);

	SetUBasisValue(0,i,1.0f);



	SKREAL fN0 = t - m_pUKnotVector[i];
	SKREAL fN1 = m_pUKnotVector[i + 1] - t;
	SKREAL fInvD0, fInvD1;
	unsigned int j;
	for (j = 1; j <= m_uiUDegree; j++)
	{
		fInvD0 = ((SKREAL)1.0) / (m_pUKnotVector[i + j] - m_pUKnotVector[i]);
		fInvD1 = ((SKREAL)1.0) / (m_pUKnotVector[i + 1] - m_pUKnotVector[i - j + 1]);

		SetUBasisValue(j,i,fN0 * GetUBasisValue(j - 1,i) * fInvD0);
		SetUBasisValue(j,i - j,fN1 * GetUBasisValue(j - 1,i - j + 1) * fInvD1);

		if (uiOrder >= 1) 
		{
			SetUBasisFDValue(j,i,(fN0 * GetUBasisFDValue(j - 1,i) + GetUBasisFDValue(j - 1,i)) * fInvD0);
			SetUBasisFDValue(j,i - j, (fN1*GetUBasisFDValue(j - 1,i - j + 1)-GetUBasisFDValue(j - 1,i - j + 1)) * fInvD1);

			
		}
	}

	for (j = 2; j <= m_uiUDegree; j++)
	{
		for (int k = i - j + 1; k < i; k++)
		{
			fN0 = t - m_pUKnotVector[k];
			fN1 = m_pUKnotVector[k + j + 1]-t;
			fInvD0 = ((SKREAL)1.0) / (m_pUKnotVector[k + j]  - m_pUKnotVector[k]);
			fInvD1 = ((SKREAL)1.0) / (m_pUKnotVector[k + j + 1] - m_pUKnotVector[k +1 ]);

			SetUBasisValue(j,k ,fN0 * GetUBasisValue(j - 1,k) * fInvD0 + fN1 * GetUBasisValue(j - 1,k + 1) * fInvD1);

			if (uiOrder >= 1)
			{
				SetUBasisFDValue(j,k,(fN0 * GetUBasisFDValue(j-1,k) + GetUBasisFDValue(j-1,k) ) *
					fInvD0 + (fN1 * GetUBasisFDValue(j-1,k+1) - GetUBasisFDValue(j-1,k+1)) *
					fInvD1);

				
			}
		}
	}

	return 1;
}
/*----------------------------------------------------------------*/
bool SKB_SplineSurface3::GetVFuncValue(SKREAL t,int &i,unsigned int uiOrder)
{

	if(!m_pVBasis)
		return 0;
	i = GetVKey(t);
	if(i == SKMAX_INTEGER)
		return 0;
	SKMemset(m_pVBasis,0,sizeof(SKREAL) * m_uiVBasisNum);
	SKMemset(m_pVBasisFD,0,sizeof(SKREAL) * m_uiVBasisNum);

	SetVBasisValue(0,i,1.0f);



	SKREAL fN0 = t - m_pVKnotVector[i];
	SKREAL fN1 = m_pVKnotVector[i + 1] - t;
	SKREAL fInvD0, fInvD1;
	unsigned int j;
	for (j = 1; j <= m_uiVDegree; j++)
	{
		fInvD0 = ((SKREAL)1.0) / (m_pVKnotVector[i + j] - m_pVKnotVector[i]);
		fInvD1 = ((SKREAL)1.0) / (m_pVKnotVector[i + 1] - m_pVKnotVector[i - j + 1]);

		SetVBasisValue(j,i,fN0 * GetVBasisValue(j - 1,i) * fInvD0);
		SetVBasisValue(j,i - j,fN1 * GetVBasisValue(j - 1,i - j + 1) * fInvD1);

		if (uiOrder >= 1) 
		{
			SetVBasisFDValue(j,i,(fN0 * GetVBasisFDValue(j - 1,i) + GetVBasisFDValue(j - 1,i)) * fInvD0);
			SetVBasisFDValue(j,i - j, (fN1*GetVBasisFDValue(j - 1,i - j + 1)-GetVBasisFDValue(j - 1,i - j + 1)) * fInvD1);


		}
	}

	for (j = 2; j <= m_uiVDegree; j++)
	{
		for (int k = i - j + 1; k < i; k++)
		{
			fN0 = t - m_pVKnotVector[k];
			fN1 = m_pVKnotVector[k + j + 1]-t;
			fInvD0 = ((SKREAL)1.0) / (m_pVKnotVector[k + j]  - m_pVKnotVector[k]);
			fInvD1 = ((SKREAL)1.0) / (m_pVKnotVector[k + j + 1] - m_pVKnotVector[k +1 ]);

			SetVBasisValue(j,k ,fN0 * GetVBasisValue(j - 1,k) * fInvD0 + fN1 * GetVBasisValue(j - 1,k + 1) * fInvD1);

			if (uiOrder >= 1)
			{
				SetVBasisFDValue(j,k,(fN0 * GetVBasisFDValue(j-1,k) + GetVBasisFDValue(j-1,k) ) *
					fInvD0 + (fN1 * GetVBasisFDValue(j-1,k+1) - GetVBasisFDValue(j-1,k+1)) *
					fInvD1);


			}
		}
	}

	return 1;
}
/*----------------------------------------------------------------*/
SKVector3 SKB_SplineSurface3::GetPoint(SKREAL U,SKREAL V)
{
	int i0,i1;
	if(GetUFuncValue(U,i0,0) && GetUFuncValue(V,i1,0))
	{
		SKVector3 Point(0,0,0);
		for(int j0 = i0 - m_uiUDegree; j0 <= i0 ; j0++)
		for(int j1 = i1 - m_uiVDegree; j1 <= i1 ; j1++)
		{
			Point += GetControlPoint(j0,j1) * GetUBasisValue(m_uiUDegree,j0) * GetUBasisValue(m_uiVDegree,j1);

		}
		return Point;
	}
	else
	{
		return SKVector3(SKMAX_REAL,SKMAX_REAL,SKMAX_REAL);
	}
}
/*----------------------------------------------------------------*/
SKVector3 SKB_SplineSurface3::GetUPartialDerivative(SKREAL U,SKREAL V)
{
	int i0,i1;
	if(GetUFuncValue(U,i0,1) && GetUFuncValue(V,i1,0))
	{
		SKVector3 Point(0,0,0);
		for(int j0 = i0 - m_uiUDegree; j0 <= i0 ; j0++)
			for(int j1 = i1 - m_uiVDegree; j1 <= i1 ; j1++)
			{
				Point += GetControlPoint(j0,j1) * GetUBasisFDValue(m_uiUDegree,j0) * GetUBasisValue(m_uiVDegree,j1);

			}
			return Point;
	}
	else
	{
		return SKVector3(SKMAX_REAL,SKMAX_REAL,SKMAX_REAL);
	}
}
/*----------------------------------------------------------------*/
SKVector3 SKB_SplineSurface3::GetVPartialDerivative(SKREAL U,SKREAL V)
{
	int i0,i1;
	if(GetUFuncValue(U,i0,0) && GetUFuncValue(V,i1,1))
	{
		SKVector3 Point(0,0,0);
		for(int j0 = i0 - m_uiUDegree; j0 <= i0 ; j0++)
			for(int j1 = i1 - m_uiVDegree; j1 <= i1 ; j1++)
			{
				Point += GetControlPoint(j0,j1) * GetUBasisValue(m_uiUDegree,j0) * GetUBasisFDValue(m_uiVDegree,j1);

			}
			return Point;
	}
	else
	{
		return SKVector3(SKMAX_REAL,SKMAX_REAL,SKMAX_REAL);
	}
}
/*----------------------------------------------------------------*/
bool SKB_SplineSurface3::Copy(const SKB_SplineSurface3 & B_SplineSurface3)
{
	if(!SKControlSurface3::Copy(B_SplineSurface3))
		return 0;

	if(!B_SplineSurface3.m_pUBasis || !B_SplineSurface3.m_pUKnotVector || !B_SplineSurface3.m_pUBasisFD ||
		!B_SplineSurface3.m_uiUDegree || !B_SplineSurface3.m_uiUBasisNum || !B_SplineSurface3.m_uiUKnotNum)
		return 0;
	if(!B_SplineSurface3.m_pVBasis || !B_SplineSurface3.m_pVKnotVector || !B_SplineSurface3.m_pVBasisFD ||
		!B_SplineSurface3.m_uiVDegree || !B_SplineSurface3.m_uiVBasisNum || !B_SplineSurface3.m_uiVKnotNum)
		return 0;

	m_uiUDegree = B_SplineSurface3.m_uiUDegree;
	SKMAC_DELETEA(m_pUKnotVector);
	m_uiUKnotNum = B_SplineSurface3.m_uiUKnotNum;
	m_pUKnotVector = SK_NEW SKREAL[m_uiUKnotNum];
	if(!m_pUKnotVector)
		return 0;
		SKMemcpy(m_pUKnotVector,B_SplineSurface3.m_pUKnotVector,sizeof(SKREAL) * (m_uiUKnotNum));

	SKMAC_DELETEA(m_pUBasis);
	m_uiUBasisNum = B_SplineSurface3.m_uiUBasisNum;
	m_pUBasis = SK_NEW SKREAL[m_uiUBasisNum]; 
	if(!m_pUBasis)
		return 0;


	SKMAC_DELETEA(m_pUBasisFD);
	m_pUBasisFD = SK_NEW SKREAL[m_uiUBasisNum]; 
	if(!m_pUBasisFD)
		return 0;
	


	m_uiVDegree = B_SplineSurface3.m_uiVDegree;
	SKMAC_DELETEA(m_pVKnotVector);
	m_uiVKnotNum = B_SplineSurface3.m_uiVKnotNum;
	m_pVKnotVector = SK_NEW SKREAL[m_uiVKnotNum];
	if(!m_pVKnotVector)
		return 0;
		SKMemcpy(m_pVKnotVector,B_SplineSurface3.m_pVKnotVector,sizeof(SKREAL) * (m_uiVKnotNum));

	SKMAC_DELETEA(m_pVBasis);
	m_uiVBasisNum = B_SplineSurface3.m_uiVBasisNum;
	m_pVBasis = SK_NEW SKREAL[m_uiVBasisNum]; 
	if(!m_pVBasis)
		return 0;


	SKMAC_DELETEA(m_pVBasisFD);
	m_pVBasisFD = SK_NEW SKREAL[m_uiVBasisNum]; 
	if(!m_pVBasisFD)
		return 0;
	return 1;
}