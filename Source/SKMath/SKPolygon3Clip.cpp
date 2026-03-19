#include "SKPolygon3.h"
#include "SKAABB3.h"
#include "SKPlane3.h"
#include "SKSegment3.h"
using namespace SKEngine2;
void SKPolygon3::Clip(const SKPlane3 &Plane, 
					 SKPolygon3 &pFront,
					 SKPolygon3 &pBack)const
{


	int			nNumFront=0,     // ǰεĸ
				nNumBack=0,      // εĸ
				nLoop=0,
				nCurrent=0;

	SKVector3 *pvcFront = SK_NEW SKVector3[m_PointNum * 3];
	SKVector3 *pvcBack  = SK_NEW SKVector3[m_PointNum * 3];

	for(unsigned int i = 0 ; i < m_PointNum ; i++)
	{
		int j = i + 1;
		if(j == m_PointNum)
		{
			j = 0;
		}
		int iFlagi = Plane.RelationWith(m_pPoint[i]);
		int iFlagj = Plane.RelationWith(m_pPoint[j]);
		if(iFlagi == SKON)
		{
			pvcFront[nNumFront] = m_pPoint[i];
			nNumFront++;
			pvcBack[nNumBack] = m_pPoint[i];
			nNumBack++;
		}
		else
		{
			SKSegment3 Segment;
			Segment.Set(m_pPoint[i],m_pPoint[j]);
			SKREAL t;
			if(Segment.RelationWith(Plane,0,t) == SKINTERSECT && iFlagj != SKON)
			{
				SKVector3 Hit = Segment.GetParameterPoint(t);
				pvcFront[nNumFront] = Hit;
				nNumFront++;
				pvcBack[nNumBack] = Hit;
				nNumBack++;

				
			
			}
			
			if (iFlagi == SKFRONT) 
			{
				
					pvcFront[nNumFront++] = m_pPoint[i];
			} 
			else if (iFlagi == SKBACK) 
			{
				
					pvcBack[nNumBack++] = m_pPoint[i];
			} 
		
		}
	}


	
	pFront.Set(pvcFront,nNumFront);
	if (pFront.m_N.Dot(m_N) < 0.0f)
		pFront.SwapFaces();
	pBack.Set(pvcBack, nNumBack);
	if (pBack.m_N.Dot(m_N) < 0.0f)
		pBack.SwapFaces();

	SKMAC_DELETEA(pvcFront);
	SKMAC_DELETEA(pvcBack);



}

void SKPolygon3::Clip(const SKAABB3 &ABBB)
{
	SKPlane3 Plane[6];
	ABBB.GetPlane(Plane);
	SKPolygon3 Front,Back;
	for (int i = 0 ; i < 6 ; i++) 
	{
		if ( RelationWith(Plane[i]) == SKINTERSECT) 
		{ 
			Clip(Plane[i], Front, Back);
			CopyFrom(Back);
		}
	}
	
}