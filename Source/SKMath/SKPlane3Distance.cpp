#include "SKPlane3.h"
#include "SKSegment3.h"
#include "SKAABB3.h"
#include "SKPolygon3.h"
using namespace SKEngine2;
/*----------------------------------------------------------------*/
SKREAL SKPlane3::Distance(const SKVector3 &Point,SKVector3 &PlanePoint)const
{

	SKREAL Dist = ABS((m_N.Dot(Point)) - m_fD);

	SKLine3 Line(Point,m_N * (-1));
	PlanePoint = Line.GetParameterPoint(Dist);

	return Dist;
}
/*----------------------------------------------------------------*/
SKREAL SKPlane3::Distance(const SKSphere3 &Sphere,SKVector3 & SpherePoint)const
{
	return Sphere.Distance(*this,SpherePoint);
	
}
/*----------------------------------------------------------------*/
SKREAL SKPlane3::Distance(const SKLine3 &Line,SKVector3 &PlanePoint,SKVector3 &LinePoint)const
{
	SKREAL fDot = Line.GetDir().Dot(m_N);
	if(ABS(fDot) < EPSILON_E4)
	{
		LinePoint = Line.GetOrig();
		return LinePoint.Distance(*this,PlanePoint);
	}
	else
		return 0;
}
/*----------------------------------------------------------------*/
SKREAL SKPlane3::Distance(const SKRay3 & Ray,SKVector3 &PlanePoint,SKVector3 &RayPoint)const
{
	SKREAL fDot = Ray.GetDir().Dot(m_N);
	RayPoint = Ray.GetOrig();
	
	SKREAL f = RayPoint.Dot(m_N) + m_fD;
	if(fDot * f > 0)
		return RayPoint.Distance(*this,PlanePoint);
	else
		return 0;
}
/*----------------------------------------------------------------*/
SKREAL SKPlane3::Distance(const SKSegment3 & Segment,SKVector3 &PlanePoint,SKVector3 &SegmentPoint)const
{

	SKVector3 SegmentOrig = Segment.GetOrig();
	SKVector3 SegmentEnd = Segment.GetEnd();
	SKREAL f0 = SegmentOrig.Dot(m_N) + m_fD;
	SKREAL f1 = SegmentEnd.Dot(m_N) + m_fD;
		
	if(f0 * f1 > 0)
	{
		SKVector3 PPoint;
		SKREAL dist = SegmentOrig.Distance(*this, PPoint);
		SegmentPoint = SegmentOrig;
		PlanePoint = PPoint;
		SKREAL distTemp = SegmentEnd.Distance(*this, PPoint);
		if(dist > distTemp)
		{
			dist = distTemp;
			SegmentPoint = SegmentEnd;
			PlanePoint = PPoint;
		}
		return dist;
	}
	else
		return 0;

}
/*----------------------------------------------------------------*/
SKREAL SKPlane3::Distance(const SKPlane3 &Plane,SKVector3 &Plane1Point,SKVector3 &Plane2Point)const
{
	if(m_N.IsParallel(Plane.m_N))
	{
		Plane1Point = GetPoint();
		return Plane1Point.Distance(Plane,Plane2Point);
	}
	else
		return 0;
	
}
/*----------------------------------------------------------------*/
SKREAL SKPlane3::Distance(const SKTriangle3 &Triangle,SKVector3 &PlanePoint,SKVector3 &TrianglePoint)const
{
	/*int i[3];
	SKVector3 Point[3];
	Triangle.GetPoint(Point);
	i[0] = RelationWith(Point[0]);
	i[1] = RelationWith(Point[1]);
	i[2] = RelationWith(Point[2]);

	if((i[0] == i[1]) && (i[1] == i[2]))
	{
		SKREAL dist[3],distTemp;
		SKVector3 PlanePointTemp[3];
		dist[0] = Point[0].Distance(*this,PlanePointTemp[0]);
		dist[1] = Point[1].Distance(*this,PlanePointTemp[1]);
		dist[2] = Point[2].Distance(*this,PlanePointTemp[2]);
		distTemp = dist[0];
		PlanePoint = PlanePointTemp[0];
		TrianglePoint = Point[0];
		if(distTemp > dist[1])
		{
			distTemp = dist[1];
			PlanePoint = PlanePointTemp[1];
			TrianglePoint = Point[1];
		}
		if(distTemp > dist[2])
		{
			distTemp = dist[2];
			PlanePoint = PlanePointTemp[2];
			TrianglePoint = Point[2];
		}
		return distTemp;
	}
	return 0;*/
	SKVector3 Point[3];
	Triangle.GetPoint(Point);

	SKREAL dist[3],distTemp;
	SKVector3 PlanePointTemp[3];
	dist[0] = Point[0].Distance(*this,PlanePointTemp[0]);
	dist[1] = Point[1].Distance(*this,PlanePointTemp[1]);
	dist[2] = Point[2].Distance(*this,PlanePointTemp[2]);
	distTemp = dist[0];
	PlanePoint = PlanePointTemp[0];
	TrianglePoint = Point[0];
	if(distTemp > dist[1])
	{
		distTemp = dist[1];
		PlanePoint = PlanePointTemp[1];
		TrianglePoint = Point[1];
	}
	if(distTemp > dist[2])
	{
		distTemp = dist[2];
		PlanePoint = PlanePointTemp[2];
		TrianglePoint = Point[2];
	}
	return distTemp;


}
/*----------------------------------------------------------------*/
SKREAL SKPlane3::Distance(const SKRectangle3 &Rectangle,SKVector3 &PlanePoint,SKVector3 &RectanglePoint)const
{
	/*int f[4];
	SKVector3 Point[4];
	Rectangle.GetPoint(Point);
	for(int i = 0 ; i < 4 ; i++)
	{
		f[i] = RelationWith(Point[i]);
	}

	if((f[0] == f[1]) && (f[1] == f[2]) && (f[2] == f[3]))
	{
		SKREAL dist[4],distTemp;
		SKVector3 PlanePointTemp[4];
		for(int i = 0 ; i < 4 ; i++)
			dist[i] = Point[i].Distance(*this,PlanePointTemp[i]);

		distTemp = dist[0];
		PlanePoint = PlanePointTemp[0];
		RectanglePoint = Point[0];
		for(int i = 1 ; i < 4 ; i++)
		{
			if(distTemp > dist[i])
			{
				distTemp = dist[i];
				PlanePoint = PlanePointTemp[i];
				RectanglePoint = Point[i];
			}
		}
		return distTemp;
	}
	return 0;*/

	SKVector3 Point[4];
	Rectangle.GetPoint(Point);



	SKREAL dist[4],distTemp;
	SKVector3 PlanePointTemp[4];
	for(int i = 0 ; i < 4 ; i++)
		dist[i] = Point[i].Distance(*this,PlanePointTemp[i]);

	distTemp = dist[0];
	PlanePoint = PlanePointTemp[0];
	RectanglePoint = Point[0];
	for(int i = 1 ; i < 4 ; i++)
	{
		if(distTemp > dist[i])
		{
			distTemp = dist[i];
			PlanePoint = PlanePointTemp[i];
			RectanglePoint = Point[i];
		}
	}
	return distTemp;



}
/*----------------------------------------------------------------*/
SKREAL SKPlane3::Distance(const SKOBB3& OBB,SKVector3 &PlanePoint,SKVector3 &OBBPoint)const
{
	/*int f[8];
	SKVector3 Point[8];
	OBB.GetPoint(Point);
	for(int i = 0 ; i < 8 ; i++)
	{
		f[i] = RelationWith(Point[i]);
	}

	if((f[0] == f[1]) && (f[1] == f[2]) && (f[2] == f[3]) && (f[3] == f[4]) 
			&& (f[4] == f[6]) && (f[5] == f[6]) && (f[6] == f[7]))
	{
		SKREAL dist[4],distTemp;
		SKVector3 PlanePointTemp[8];
		for(int i = 0 ; i < 8 ; i++)
			dist[i] = Point[i].Distance(*this,PlanePointTemp[i]);

		distTemp = dist[0];
		PlanePoint = PlanePointTemp[0];
		OBBPoint = Point[0];
		for(int i = 1 ; i < 8 ; i++)
		{
			if(distTemp > dist[i])
			{
				distTemp = dist[i];
				PlanePoint = PlanePointTemp[i];
				OBBPoint = Point[i];
			}
		}
		return distTemp;
	}
	return 0;*/

	SKVector3 Point[8];
	OBB.GetPoint(Point);

	
	SKREAL dist[4],distTemp;
	SKVector3 PlanePointTemp[8];
	for(int i = 0 ; i < 8 ; i++)
		dist[i] = Point[i].Distance(*this,PlanePointTemp[i]);

	distTemp = dist[0];
	PlanePoint = PlanePointTemp[0];
	OBBPoint = Point[0];
	for(int i = 1 ; i < 8 ; i++)
	{
		if(distTemp > dist[i])
		{
			distTemp = dist[i];
			PlanePoint = PlanePointTemp[i];
			OBBPoint = Point[i];
		}
	}
	return distTemp;


}
/*----------------------------------------------------------------*/
SKREAL SKPlane3::Distance(const SKAABB3 &AABB,SKVector3 &PlanePoint,SKVector3 &AABBPoint)const
{
	/*int f[8];
	SKVector3 Point[8];
	AABB.GetPoint(Point);
	for(int i = 0 ; i < 8 ; i++)
	{
		f[i] = RelationWith(Point[i]);
	}

	if((f[0] == f[1]) && (f[1] == f[2]) && (f[2] == f[3]) && (f[3] == f[4]) 
		&& (f[4] == f[6]) && (f[5] == f[6]) && (f[6] == f[7]))
	{
		SKREAL dist[4],distTemp;
		SKVector3 PlanePointTemp[8];
		for(int i = 0 ; i < 8 ; i++)
			dist[i] = Point[i].Distance(*this,PlanePointTemp[i]);

		distTemp = dist[0];
		PlanePoint = PlanePointTemp[0];
		AABBPoint = Point[0];
		for(int i = 1 ; i < 8 ; i++)
		{
			if(distTemp > dist[i])
			{
				distTemp = dist[i];
				PlanePoint = PlanePointTemp[i];
				AABBPoint = Point[i];
			}
		}
		return distTemp;
	}
	return 0;*/

	SKVector3 Point[8];
	AABB.GetPoint(Point);
	


	SKREAL dist[4],distTemp;
	SKVector3 PlanePointTemp[8];
	for(int i = 0 ; i < 8 ; i++)
		dist[i] = Point[i].Distance(*this,PlanePointTemp[i]);

	distTemp = dist[0];
	PlanePoint = PlanePointTemp[0];
	AABBPoint = Point[0];
	for(int i = 1 ; i < 8 ; i++)
	{
		if(distTemp > dist[i])
		{
			distTemp = dist[i];
			PlanePoint = PlanePointTemp[i];
			AABBPoint = Point[i];
		}
	}
	return distTemp;
	
}
/*----------------------------------------------------------------*/
SKREAL SKPlane3::Distance(const SKPolygon3 &Polygon,SKVector3 &PlanePoint,int& IndexTriangle,
						 SKVector3 &TrianglePoint)const
{

	return Polygon.Distance(*this,IndexTriangle,TrianglePoint,PlanePoint);
}