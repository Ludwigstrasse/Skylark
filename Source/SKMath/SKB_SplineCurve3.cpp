#include "SKB_SplineCurve3.h"
#include "SKMemManager.h"
using namespace SKEngine2;
/*----------------------------------------------------------------*/
SKB_SplineCurve3::SKB_SplineCurve3()
{
	m_pControlPoint = NULL;
	m_pKnotVector = NULL;
	m_uiControlPointNum = 0;
	m_uiDegree = 0;
	m_uiKnotNum = 0;
	m_pBasis = 0;
	m_pBasisFD = 0;
	m_pBasisSD = 0;
	m_pBasisTD = 0;
	m_uiBasisNum = 0;
	m_CurveType = B_SPLINE;
}
/*----------------------------------------------------------------*/
SKB_SplineCurve3::~SKB_SplineCurve3()
{
	SKMAC_DELETEA(m_pKnotVector);
	SKMAC_DELETEA(m_pControlPoint);
	SKMAC_DELETEA(m_pBasis);
	SKMAC_DELETEA(m_pBasisFD);
	SKMAC_DELETEA(m_pBasisSD);
	SKMAC_DELETEA(m_pBasisTD);
}
/*----------------------------------------------------------------*/
bool SKB_SplineCurve3::Set(const SKVector3 * pControlPoint,unsigned int uiControlPointNum)
{
	if(!pControlPoint || !uiControlPointNum)
		return 0 ;
	SKMAC_DELETEA(m_pControlPoint);

	m_pControlPoint = SK_NEW SKVector3[uiControlPointNum];
	if(!m_pControlPoint)
		return 0;
	m_uiControlPointNum = uiControlPointNum;
	SKMemcpy(m_pControlPoint,pControlPoint,uiControlPointNum * sizeof(SKVector3));
	return 1;
	
}
/*----------------------------------------------------------------*/
bool SKB_SplineCurve3::ChangeToOpenClosePoints()
{
	if(!m_pControlPoint || !m_uiControlPointNum)
		return 0;
	m_uiControlPointNum = m_uiControlPointNum + 1;
	SKVector3 * m_pControlPointTemp = NULL;
	m_pControlPointTemp = SK_NEW SKVector3[m_uiControlPointNum];
	if(!m_pControlPointTemp)
		return 0;
	SKMemcpy(m_pControlPointTemp,m_pControlPoint,(m_uiControlPointNum - 1) * sizeof(SKVector3));
	m_pControlPointTemp[m_uiControlPointNum - 1] = m_pControlPointTemp[0];
	SKMAC_DELETEA(m_pControlPoint);
	m_pControlPoint = m_pControlPointTemp  ;
	return 1;
}
/*----------------------------------------------------------------*/
bool SKB_SplineCurve3::ChangeToPeriodicClosePoints(unsigned int uiDegree)
{
	if(!uiDegree || !m_pControlPoint || !m_uiControlPointNum)
		return 0;
	m_uiDegree = uiDegree;
	unsigned int k = m_uiControlPointNum ;
	m_uiControlPointNum = m_uiControlPointNum + m_uiDegree;
	SKVector3 * m_pControlPointTemp = NULL;
	m_pControlPointTemp	= SK_NEW SKVector3[m_uiControlPointNum];
	if(!m_pControlPointTemp)
		return 0;
	SKMemcpy(m_pControlPointTemp,m_pControlPoint,k * sizeof(SKVector3));
	for(unsigned int i = 0 ; i < m_uiDegree ; i++)
	{
		m_pControlPointTemp[k + i] = m_pControlPointTemp[i];
	}
	SKMAC_DELETEA(m_pControlPoint);
	m_pControlPoint = m_pControlPointTemp  ;
	return 1;

}
/*----------------------------------------------------------------*/
bool SKB_SplineCurve3::SetKnotVector(const SKREAL * pKnotVector,unsigned int uiDegree,unsigned int uiControlPointNum)
{
	if(!pKnotVector)
		return 0;
	for(unsigned int i = 0 ; i < uiDegree + uiControlPointNum; i++)
	{
		if(pKnotVector[i] > pKnotVector[i+1])
			return 0;
	}
	m_uiDegree = uiDegree;
	m_uiControlPointNum = uiControlPointNum;
	SKMAC_DELETEA(m_pKnotVector);
	m_uiKnotNum = uiControlPointNum + 1 + uiDegree;
	m_pKnotVector = SK_NEW SKREAL[m_uiKnotNum];
	if(!m_pKnotVector)
		return 0;
	SKMemcpy(m_pKnotVector,pKnotVector,sizeof(SKREAL) * (m_uiKnotNum));

	SKMAC_DELETEA(m_pBasis);
	m_uiBasisNum = (uiDegree + 1) * m_uiKnotNum;
	m_pBasis = SK_NEW SKREAL[m_uiBasisNum]; 
	if(!m_pBasis)
		return 0;


	SKMAC_DELETEA(m_pBasisFD);
	m_pBasisFD = SK_NEW SKREAL[m_uiBasisNum]; 
	if(!m_pBasisFD)
		return 0;


	SKMAC_DELETEA(m_pBasisSD);
	m_pBasisSD = SK_NEW SKREAL[m_uiBasisNum]; 
	if(!m_pBasisSD)
		return 0;


	SKMAC_DELETEA(m_pBasisTD);
	m_pBasisTD = SK_NEW SKREAL[m_uiBasisNum]; 
	if(!m_pBasisTD)
		return 0;

	return 1;

}
/*----------------------------------------------------------------*/
bool SKB_SplineCurve3::CreateOpenUniformKnotVector(unsigned int uiDegree,unsigned int uiControlPointNum)
{
	m_uiDegree = uiDegree;
	m_uiControlPointNum = uiControlPointNum;
	SKMAC_DELETEA(m_pKnotVector);
	m_uiKnotNum = uiControlPointNum + 1 + uiDegree;

	m_pKnotVector = SK_NEW SKREAL[m_uiKnotNum];
	if(!m_pKnotVector)
		return 0;
	for(unsigned int i  = 0; i < m_uiKnotNum ; i++)
	{
		if(i >= 0 && i <= m_uiDegree)
		{
			m_pKnotVector[i] = 0;

		}
		else if( i >= m_uiDegree + 1 && i <= m_uiControlPointNum)
		{
			m_pKnotVector[i] =((SKREAL)1.0 * (i - m_uiDegree)) / (m_uiControlPointNum + 1 - m_uiDegree);
		
		}
		else
		{
			m_pKnotVector[i] = 1;
		}
	}

	SKMAC_DELETEA(m_pBasis);
	m_uiBasisNum = (uiDegree + 1) * m_uiKnotNum;
	m_pBasis = SK_NEW SKREAL[m_uiBasisNum]; 
	if(!m_pBasis)
		return 0;


	SKMAC_DELETEA(m_pBasisFD);
	m_pBasisFD = SK_NEW SKREAL[m_uiBasisNum]; 
	if(!m_pBasisFD)
		return 0;


	SKMAC_DELETEA(m_pBasisSD);
	m_pBasisSD = SK_NEW SKREAL[m_uiBasisNum]; 
	if(!m_pBasisSD)
		return 0;


	SKMAC_DELETEA(m_pBasisTD);
	m_pBasisTD = SK_NEW SKREAL[m_uiBasisNum]; 
	if(!m_pBasisTD)
		return 0;
	return 1;
}
/*----------------------------------------------------------------*/
bool SKB_SplineCurve3::CreatePeriodicKnotVector(unsigned int uiDegree,unsigned int uiControlPointNum)
{
	m_uiDegree = uiDegree;
	m_uiControlPointNum = uiControlPointNum;
	SKMAC_DELETEA(m_pKnotVector);
	m_uiKnotNum = uiControlPointNum + 1 + uiDegree;
	m_pKnotVector = SK_NEW SKREAL[m_uiKnotNum];
	if(!m_pKnotVector)
		return 0;
	for(unsigned int i  = 0; i < m_uiKnotNum ; i++)
	{
		
		m_pKnotVector[i] =((SKREAL)1.0 * (i - m_uiDegree)) / (m_uiControlPointNum + 1 - m_uiDegree);
		
	}

	SKMAC_DELETEA(m_pBasis);
	m_uiBasisNum = (uiDegree + 1) * m_uiKnotNum;
	m_pBasis = SK_NEW SKREAL[m_uiBasisNum]; 
	if(!m_pBasis)
		return 0;
	

	SKMAC_DELETEA(m_pBasisFD);
	m_pBasisFD = SK_NEW SKREAL[m_uiBasisNum]; 
	if(!m_pBasisFD)
		return 0;
	
	SKMAC_DELETEA(m_pBasisSD);
	m_pBasisSD = SK_NEW SKREAL[m_uiBasisNum]; 
	if(!m_pBasisSD)
		return 0;
	

	SKMAC_DELETEA(m_pBasisTD);
	m_pBasisTD = SK_NEW SKREAL[m_uiBasisNum]; 
	if(!m_pBasisTD)
		return 0;
	
	return 1;
}
/*----------------------------------------------------------------*/
int SKB_SplineCurve3::GetKey(SKREAL t)const
{
	if( t > 1.0f || t < 0.0f)
		return SKMAX_INTEGER;
	unsigned int i;
	for(i = 0 ; i < m_uiKnotNum ; i++)
	{
		if(t < m_pKnotVector[i])
			break;
	
	}
	return (i - 1);

}
/*----------------------------------------------------------------*/
bool SKB_SplineCurve3::GetFuncValue(SKREAL t,int &i,unsigned int uiOrder)
{
	if(!m_pBasis)
		return 0;
	i = GetKey(t);
	if(i == SKMAX_INTEGER)
		return 0;
	SKMemset(m_pBasis,0,sizeof(SKREAL) * m_uiBasisNum);
	SKMemset(m_pBasisFD,0,sizeof(SKREAL) * m_uiBasisNum);
	SKMemset(m_pBasisSD,0,sizeof(SKREAL) * m_uiBasisNum);
	SKMemset(m_pBasisTD,0,sizeof(SKREAL) * m_uiBasisNum);
	SetBasisValue(0,i,1.0f);

	
	
	SKREAL fN0 = t - m_pKnotVector[i];
	SKREAL fN1 = m_pKnotVector[i + 1] - t;
	SKREAL fInvD0, fInvD1;
	unsigned int j;
	for (j = 1; j <= m_uiDegree; j++)
	{
		fInvD0 = ((SKREAL)1.0) / (m_pKnotVector[i + j] - m_pKnotVector[i]);
		fInvD1 = ((SKREAL)1.0) / (m_pKnotVector[i + 1] - m_pKnotVector[i - j + 1]);

		SetBasisValue(j,i,fN0 * GetBasisValue(j - 1,i) * fInvD0);
		SetBasisValue(j,i - j,fN1 * GetBasisValue(j - 1,i - j + 1) * fInvD1);

		if (uiOrder >= 1) 
		{
			SetBasisFDValue(j,i,(fN0 * GetBasisFDValue(j - 1,i) + GetBasisFDValue(j - 1,i)) * fInvD0);
			SetBasisFDValue(j,i - j, (fN1*GetBasisFDValue(j - 1,i - j + 1)-GetBasisFDValue(j - 1,i - j + 1)) * fInvD1);

			if (uiOrder >= 2)
			{
				SetBasisSDValue(j,i,(fN0 * GetBasisSDValue(j - 1,i) + ((SKREAL)2.0) * GetBasisSDValue(j - 1,i)) * fInvD0);
				SetBasisSDValue(j,i - j ,(fN1 * GetBasisSDValue(j - 1,i - j + 1) - ((SKREAL)2.0) * GetBasisSDValue(j - 1,i - j + 1)) * fInvD1);

				if (uiOrder >= 3)
				{
					SetBasisTDValue(j,i,(fN0 * GetBasisTDValue(j - 1,i) + ((SKREAL)3.0) * GetBasisTDValue(j - 1 , i)) * fInvD0);
					SetBasisTDValue(j,i - j,(fN1*GetBasisTDValue(j-1,i-j+1) -((SKREAL)3.0)*GetBasisTDValue(j-1,i-j+1))*fInvD1);
				}
			}
		}
	}

	for (j = 2; j <= m_uiDegree; j++)
	{
		for (int k = i - j + 1; k < i; k++)
		{
			fN0 = t - m_pKnotVector[k];
			fN1 = m_pKnotVector[k + j + 1]-t;
			fInvD0 = ((SKREAL)1.0) / (m_pKnotVector[k + j]  - m_pKnotVector[k]);
			fInvD1 = ((SKREAL)1.0) / (m_pKnotVector[k + j + 1] - m_pKnotVector[k +1 ]);

			SetBasisValue(j,k ,fN0 * GetBasisValue(j - 1,k) * fInvD0 + fN1 * GetBasisValue(j - 1,k + 1) * fInvD1);

			if (uiOrder >= 1)
			{
				SetBasisFDValue(j,k,(fN0 * GetBasisFDValue(j-1,k) + GetBasisFDValue(j-1,k) ) *
					fInvD0 + (fN1 * GetBasisFDValue(j-1,k+1) - GetBasisFDValue(j-1,k+1)) *
					fInvD1);

				if (uiOrder >= 2)
				{
					SetBasisSDValue(j,k,(fN0*GetBasisSDValue(j - 1,k) +
						((SKREAL)2.0)*GetBasisSDValue(j - 1,k))*fInvD0 +
						(fN1*GetBasisSDValue(j - 1,k + 1)- ((SKREAL)2.0)*
						GetBasisSDValue(j - 1,k + 1))*fInvD1);

					if (uiOrder >= 3)
					{
						SetBasisTDValue(j,k, (fN0*GetBasisTDValue(j-1,k) +
							((SKREAL)3.0)*GetBasisTDValue(j-1,k))*fInvD0 +
							(fN1*GetBasisTDValue(j-1,k+1) - ((SKREAL)3.0)*
							GetBasisTDValue(j-1,k+1))*fInvD1);
					}
				}
			}
		}
	}
	
	return 1;
}
/*----------------------------------------------------------------*/
SKVector3 SKB_SplineCurve3::GetPoint(SKREAL t)
{
	int i;
	if(GetFuncValue(t,i,0))
	{
		SKVector3 Point(0,0,0);
		for(int j = i - m_uiDegree; j <= i ; j++)
		{
			Point += m_pControlPoint[j] * GetBasisValue(m_uiDegree,j);
		
		}
		return Point;
	}
	else
	{
		return SKVector3(SKMAX_REAL,SKMAX_REAL,SKMAX_REAL);
	}

}
/*----------------------------------------------------------------*/
SKVector3 SKB_SplineCurve3::GetFirstDerivative(SKREAL t)
{
	int i;
	if(GetFuncValue(t,i,1))
	{
		SKVector3 Point(0,0,0);
		for(int j = i - m_uiDegree; j <= i ; j++)
		{
			Point += m_pControlPoint[j] * GetBasisFDValue(m_uiDegree,j);

		}
		return Point;
	}
	else
	{
		return SKVector3(SKMAX_REAL,SKMAX_REAL,SKMAX_REAL);
	}

}
/*----------------------------------------------------------------*/
SKVector3 SKB_SplineCurve3::GetSecondDerivative(SKREAL t)
{
	int i;
	if(GetFuncValue(t,i,2))
	{
		SKVector3 Point(0,0,0);
		for(int j = i - m_uiDegree; j <= i ; j++)
		{
			Point += m_pControlPoint[j] * GetBasisSDValue(m_uiDegree,j);

		}
		return Point;
	}
	else
	{
		return SKVector3(SKMAX_REAL,SKMAX_REAL,SKMAX_REAL);
	}
}
/*----------------------------------------------------------------*/
SKVector3 SKB_SplineCurve3::GetThirdDerivative(SKREAL t)
{
	int i;
	if(GetFuncValue(t,i,3))
	{
		SKVector3 Point(0,0,0);
		for(int j = i - m_uiDegree; j <= i ; j++)
		{
			Point += m_pControlPoint[j] * GetBasisTDValue(m_uiDegree,j);

		}
		return Point;
	}
	else
	{
		return SKVector3(SKMAX_REAL,SKMAX_REAL,SKMAX_REAL);
	}

}
/*----------------------------------------------------------------*/
bool SKB_SplineCurve3::Copy(const SKB_SplineCurve3 & B_SplineCurve3)
{
	if(!SKControlCurve3::Copy(B_SplineCurve3))
		return 0;

	if(!B_SplineCurve3.m_pBasis || !B_SplineCurve3.m_pBasisFD
		||!B_SplineCurve3.m_pBasisSD ||!B_SplineCurve3.m_pBasisTD||
		!B_SplineCurve3.m_pKnotVector ||!B_SplineCurve3.m_uiBasisNum
		||!B_SplineCurve3.m_uiDegree||!B_SplineCurve3.m_uiKnotNum)
		return 0;

	m_uiDegree = B_SplineCurve3.m_uiDegree;
	m_uiControlPointNum = B_SplineCurve3.m_uiControlPointNum;
	m_uiKnotNum = B_SplineCurve3.m_uiKnotNum;

	SKMAC_DELETEA(m_pKnotVector);
	SKMAC_DELETEA(m_pControlPoint);
	SKMAC_DELETEA(m_pBasis);
	SKMAC_DELETEA(m_pBasisFD);
	SKMAC_DELETEA(m_pBasisSD);
	SKMAC_DELETEA(m_pBasisTD);
	
	m_pKnotVector = SK_NEW SKREAL[m_uiKnotNum];
	if(!m_pKnotVector)
		return 0;
	SKMemcpy(m_pKnotVector,B_SplineCurve3.m_pKnotVector,sizeof(SKREAL) * (m_uiKnotNum));

	SKMAC_DELETEA(m_pBasis);
	m_uiBasisNum = B_SplineCurve3.m_uiBasisNum;
	m_pBasis = SK_NEW SKREAL[m_uiBasisNum]; 
	if(!m_pBasis)
		return 0;


	SKMAC_DELETEA(m_pBasisFD);
	m_pBasisFD = SK_NEW SKREAL[m_uiBasisNum]; 
	if(!m_pBasisFD)
		return 0;


	SKMAC_DELETEA(m_pBasisSD);
	m_pBasisSD = SK_NEW SKREAL[m_uiBasisNum]; 
	if(!m_pBasisSD)
		return 0;


	SKMAC_DELETEA(m_pBasisTD);
	m_pBasisTD = SK_NEW SKREAL[m_uiBasisNum]; 
	if(!m_pBasisTD)
		return 0;

	return 1;

}