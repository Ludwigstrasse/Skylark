#include "SKPolygon3.h"
#include "SKSegment3.h"
#include "SKAABB3.h"
using namespace SKEngine2;
SKREAL SKPolygon3::SquaredDistance(const SKVector3 & Point,int& IndexTriangle,SKREAL fTriangleParameter[3])const
{
	int iTriangleNum = m_IndexNum / 3;
	SKTriangle3 TriangleTemp;
	SKREAL distTemp = SKMAX_REAL;
	
	for(int i = 0 ; i < iTriangleNum ; i++)
	{
		TriangleTemp.Set(m_pPoint[m_pIndex[i]],m_pPoint[m_pIndex[i+1]],m_pPoint[m_pIndex[i+2]]);
		SKREAL fParaTemp[3];
		SKREAL dist = TriangleTemp.SquaredDistance(Point,fParaTemp);
		if(distTemp < dist)
		{
			distTemp = dist;
			for( int j = 0 ; j < 3 ; j++)
				fTriangleParameter[j] = fParaTemp[j];
			IndexTriangle = i;
		}
	
	}
	return distTemp;
}
SKREAL SKPolygon3::SquaredDistance(const SKTriangle3 &Triangle,
									int& IndexTriangle,
									SKREAL fTriangle1Parameter[3],
									SKREAL fTriangle2Parameter[3])const
{

	int iTriangleNum = m_IndexNum / 3;
	SKTriangle3 TriangleTemp;
	SKREAL distTemp = SKMAX_REAL;

	for(int i = 0 ; i < iTriangleNum ; i++)
	{
		TriangleTemp.Set(m_pPoint[m_pIndex[i]],m_pPoint[m_pIndex[i+1]],m_pPoint[m_pIndex[i+2]]);
		SKREAL fPara1Temp[3];
		SKREAL fPara2Temp[3];
		SKREAL dist = TriangleTemp.SquaredDistance(Triangle,fPara1Temp,fPara2Temp);
		if(distTemp < dist)
		{
			distTemp = dist;
			for( int j = 0 ; j < 3 ; j++)
			{
				fTriangle1Parameter[j] = fPara1Temp[j];
				fTriangle2Parameter[j] = fPara2Temp[j];
			}
			IndexTriangle = i;
		}

	}
	return distTemp;
}
//κ;ξ
SKREAL SKPolygon3::SquaredDistance(const SKRectangle3 &Rectangle,
									int& IndexTriangle,
									SKREAL fTriangleParameter[3],
									SKREAL fRectangleParameter[2])const
{

	int iTriangleNum = m_IndexNum / 3;
	SKTriangle3 TriangleTemp;
	SKREAL distTemp = SKMAX_REAL;

	for(int i = 0 ; i < iTriangleNum ; i++)
	{
		TriangleTemp.Set(m_pPoint[m_pIndex[i]],m_pPoint[m_pIndex[i+1]],m_pPoint[m_pIndex[i+2]]);
		SKREAL fPara1Temp[3];
		SKREAL fPara2Temp[2];
		SKREAL dist = TriangleTemp.SquaredDistance(Rectangle,fPara1Temp,fPara2Temp);
		if(distTemp < dist)
		{
			distTemp = dist;
			for( int j = 0 ; j < 3 ; j++)
			{
				fTriangleParameter[j] = fPara1Temp[j];
				
			}
			for( int k = 0 ; k < 2 ; k++)
			{
				fRectangleParameter[k] = fPara2Temp[k];
			}
			IndexTriangle = i;
		}

	}
	return distTemp;

}
SKREAL SKPolygon3::SquaredDistance(const SKLine3 &Line,int& IndexTriangle,
									SKREAL fTriangleParameter[3],SKREAL &fLineParameter)const
{
	int iTriangleNum = m_IndexNum / 3;
	SKTriangle3 TriangleTemp;
	SKREAL distTemp = SKMAX_REAL;

	for(int i = 0 ; i < iTriangleNum ; i++)
	{
		TriangleTemp.Set(m_pPoint[m_pIndex[i]],m_pPoint[m_pIndex[i+1]],m_pPoint[m_pIndex[i+2]]);
		SKREAL fPara1Temp[3];
		SKREAL fPara2;
		SKREAL dist = TriangleTemp.SquaredDistance(Line,fPara1Temp,fPara2);
		if(distTemp < dist)
		{
			distTemp = dist;
			for( int j = 0 ; j < 3 ; j++)
			{
				fTriangleParameter[j] = fPara1Temp[j];
			}
			fLineParameter = fPara2;
			IndexTriangle = i;
		}

	}
	return distTemp;			

}
SKREAL SKPolygon3::SquaredDistance(const SKRay3 & Ray,int& IndexTriangle,
					   SKREAL fTriangleParameter[3],SKREAL &fRayParameter)const
{
	int iTriangleNum = m_IndexNum / 3;
	SKTriangle3 TriangleTemp;
	SKREAL distTemp = SKMAX_REAL;

	for(int i = 0 ; i < iTriangleNum ; i++)
	{
		TriangleTemp.Set(m_pPoint[m_pIndex[i]],m_pPoint[m_pIndex[i+1]],m_pPoint[m_pIndex[i+2]]);
		SKREAL fPara1Temp[3];
		SKREAL fPara2;
		SKREAL dist = TriangleTemp.SquaredDistance(Ray,fPara1Temp,fPara2);
		if(distTemp < dist)
		{
			distTemp = dist;
			for( int j = 0 ; j < 3 ; j++)
			{
				fTriangleParameter[j] = fPara1Temp[j];
			}
			fRayParameter = fPara2;
			IndexTriangle = i;
		}

	}
	return distTemp;	


}
SKREAL SKPolygon3::SquaredDistance(const SKSegment3& Segment,int& IndexTriangle,
					   SKREAL fTriangleParameter[3],SKREAL &fSegmentParameter)const
{

	int iTriangleNum = m_IndexNum / 3;
	SKTriangle3 TriangleTemp;
	SKREAL distTemp = SKMAX_REAL;

	for(int i = 0 ; i < iTriangleNum ; i++)
	{
		TriangleTemp.Set(m_pPoint[m_pIndex[i]],m_pPoint[m_pIndex[i+1]],m_pPoint[m_pIndex[i+2]]);
		SKREAL fPara1Temp[3];
		SKREAL fPara2;
		SKREAL dist = TriangleTemp.SquaredDistance(Segment,fPara1Temp,fPara2);
		if(distTemp < dist)
		{
			distTemp = dist;
			for( int j = 0 ; j < 3 ; j++)
			{
				fTriangleParameter[j] = fPara1Temp[j];
			}
			fSegmentParameter = fPara2;
			IndexTriangle = i;
		}

	}
	return distTemp;
}
SKREAL SKPolygon3::SquaredDistance(const SKOBB3 & OBB,int& IndexTriangle,
					   SKREAL TriangleParameter[3], SKREAL OBBParameter[3])const
{
	int iTriangleNum = m_IndexNum / 3;
	SKTriangle3 TriangleTemp;
	SKREAL distTemp = SKMAX_REAL;

	for(int i = 0 ; i < iTriangleNum ; i++)
	{
		TriangleTemp.Set(m_pPoint[m_pIndex[i]],m_pPoint[m_pIndex[i+1]],m_pPoint[m_pIndex[i+2]]);
		SKREAL fPara1Temp[3];
		SKREAL fPara2Temp[3];
		SKREAL dist = TriangleTemp.SquaredDistance(OBB,fPara1Temp,fPara2Temp);
		if(distTemp < dist)
		{
			distTemp = dist;
			for( int j = 0 ; j < 3 ; j++)
			{
				TriangleParameter[j] = fPara1Temp[j];
				OBBParameter[j] = fPara2Temp[j];
			}
			IndexTriangle = i;
		}

	}
	return distTemp;

}

SKREAL SKPolygon3::Distance(const SKSphere3 &Sphere,int& IndexTriangle,
				SKREAL fTriangleParameter[3],SKVector3 & SpherePoint)const
{
	int iTriangleNum = m_IndexNum / 3;
	SKTriangle3 TriangleTemp;
	SKREAL distTemp = SKMAX_REAL;

	for(int i = 0 ; i < iTriangleNum ; i++)
	{
		TriangleTemp.Set(m_pPoint[m_pIndex[i]],m_pPoint[m_pIndex[i+1]],m_pPoint[m_pIndex[i+2]]);
		SKREAL fPara1Temp[3];
		SKVector3 Point;
		SKREAL dist = TriangleTemp.Distance(Sphere,fPara1Temp,Point);
		if(distTemp < dist)
		{
			distTemp = dist;
			for( int j = 0 ; j < 3 ; j++)
			{
				fTriangleParameter[j] = fPara1Temp[j];
	
			}
			SpherePoint = Point;
			IndexTriangle = i;
		}

	}
	return distTemp;

}


SKREAL SKPolygon3::Distance(const SKPlane3 &Plane,int& IndexTriangle,
				SKVector3 &TrianglePoint,SKVector3 &PlanePoint)const
{
	int iTriangleNum = m_IndexNum / 3;
	SKTriangle3 TriangleTemp;
	SKREAL distTemp = SKMAX_REAL;

	for(int i = 0 ; i < iTriangleNum ; i++)
	{
		TriangleTemp.Set(m_pPoint[m_pIndex[i]],m_pPoint[m_pIndex[i+1]],m_pPoint[m_pIndex[i+2]]);
		SKVector3 Point1,Point2;
		SKREAL dist = TriangleTemp.Distance(Plane,Point1,Point2);
		if(distTemp < dist)
		{
			distTemp = dist;
		
			TrianglePoint = Point1;
			PlanePoint = Point2;
			IndexTriangle = i;
		}

	}
	return distTemp;

}

SKREAL SKPolygon3::SquaredDistance(const SKAABB3 &AABB ,int& IndexTriangle,
					   SKREAL TriangleParameter[3]
						, SKREAL AABBParameter[3])const
{
	int iTriangleNum = m_IndexNum / 3;
	SKTriangle3 TriangleTemp;
	SKREAL distTemp = SKMAX_REAL;

	for(int i = 0 ; i < iTriangleNum ; i++)
	{
		TriangleTemp.Set(m_pPoint[m_pIndex[i]],m_pPoint[m_pIndex[i+1]],m_pPoint[m_pIndex[i+2]]);
		SKREAL fPara1Temp[3];
		SKREAL fPara2Temp[3];
		SKREAL dist = TriangleTemp.SquaredDistance(AABB,fPara1Temp,fPara2Temp);
		if(distTemp < dist)
		{
			distTemp = dist;
			for( int j = 0 ; j < 3 ; j++)
			{
				TriangleParameter[j] = fPara1Temp[j];
				AABBParameter[j] = fPara2Temp[j];
			}
			IndexTriangle = i;
		}

	}
	return distTemp;					
}
SKREAL SKPolygon3::SquaredDistance(const SKPolygon3 & Polygon ,int& Index1Triangle,
									SKREAL Triangle1Parameter[3],int& Index2Triangle,
									SKREAL Triangle2Parameter[3])const
{
	int iTriangleNum = m_IndexNum / 3;
	SKTriangle3 TriangleTemp;
	SKREAL distTemp = SKMAX_REAL;

	for(int i = 0 ; i < iTriangleNum ; i++)
	{
		TriangleTemp.Set(m_pPoint[m_pIndex[i]],m_pPoint[m_pIndex[i+1]],m_pPoint[m_pIndex[i+2]]);
		SKREAL fPara1Temp[3];
		SKREAL fPara2Temp[3];
		int k;
		SKREAL dist = TriangleTemp.SquaredDistance(Polygon,fPara1Temp,k,fPara2Temp);
		if(distTemp < dist)
		{
			distTemp = dist;
			for( int j = 0 ; j < 3 ; j++)
			{
				Triangle1Parameter[j] = fPara1Temp[j];
				Triangle2Parameter[j] = fPara2Temp[j];
			}
			Index1Triangle = i;
			Index2Triangle = k;
		}

	}
	return distTemp;		
}
