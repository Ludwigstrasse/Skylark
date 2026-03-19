#include "SKRectangle3.h"
#include "SKSegment3.h"
#include "SKAABB3.h"
#include "SKPolygon3.h"
using namespace SKEngine2;
/*----------------------------------------------------------------*/
void SKRectangle3::GetPoint(SKVector3 Point[4])const
{

	SKVector3 temp0 = m_Center + m_A[0] * m_fA[0];
	SKVector3 temp1 = m_Center + m_A[0] * (-m_fA[0]);
	Point[0] = temp0 + m_A[1] * m_fA[1] ;
	Point[1] = temp1 + m_A[1] * m_fA[1] ;
	Point[2] = temp1 + m_A[1] * (-m_fA[1]);
	Point[3] = temp0 + m_A[1] * (-m_fA[1]);


}
/*----------------------------------------------------------------*/
SKREAL SKRectangle3::SquaredDistance(const SKRectangle3 &Rectangle,
								   SKREAL fRectangle1Parameter[2],SKREAL fRectangle2Parameter[2])const
{
	SKREAL t,temp[2];
	SKSegment3 Edge;
	SKREAL fSqrDist = SKMAX_REAL;
	SKREAL fSqrDistTmp;
	SKVector3 RectanglePoint[4];
	Rectangle.GetPoint(RectanglePoint);
	Edge.Set(RectanglePoint[1],RectanglePoint[0]);
	fSqrDistTmp = Edge.SquaredDistance(*this,t,temp);
	SKREAL fS0,fT0,fS1,fT1;
	if (fSqrDistTmp < fSqrDist)
	{
		fSqrDist = fSqrDistTmp;
		fS1 = t - Rectangle.m_fA[0];
		fT1 = Rectangle.m_fA[1];
		fS0 = temp[0];
		fT0 = temp[1];
	}

	Edge.Set(RectanglePoint[3],RectanglePoint[0]);
	fSqrDistTmp = Edge.SquaredDistance(*this,t,temp);
	if (fSqrDistTmp < fSqrDist)
	{
		fSqrDist = fSqrDistTmp;
		fS1 = Rectangle.m_fA[0];
		fT1 = t - Rectangle.m_fA[1];
		fS0 = temp[0];
		fT0 = temp[1];
	}

	Edge.Set(RectanglePoint[2],RectanglePoint[3]);
	fSqrDistTmp = Edge.SquaredDistance(*this,t,temp);
	if (fSqrDistTmp < fSqrDist)
	{
		fSqrDist = fSqrDistTmp;
		fS1 = t -Rectangle.m_fA[0];
		fT1 = - Rectangle.m_fA[1];
		fS0 = temp[0];
		fT0 = temp[1];
	}

	Edge.Set(RectanglePoint[2],RectanglePoint[1]);
	fSqrDistTmp = Edge.SquaredDistance(*this,t,temp);
	if (fSqrDistTmp < fSqrDist)
	{
		fSqrDist = fSqrDistTmp;
		fS1 = -Rectangle.m_fA[0];
		fT1 = t - Rectangle.m_fA[1];
		fS0 = temp[0];
		fT0 = temp[1];
	}





	GetPoint(RectanglePoint);
	Edge.Set(RectanglePoint[1],RectanglePoint[0]);
	fSqrDistTmp = Edge.SquaredDistance(Rectangle,t,temp);
	if (fSqrDistTmp < fSqrDist)
	{
		fSqrDist = fSqrDistTmp;
		fS0 = t - Rectangle.m_fA[0];
		fT0 = Rectangle.m_fA[1];
		fS1 = temp[0];
		fT1 = temp[1];
	}

	Edge.Set(RectanglePoint[3],RectanglePoint[0]);
	fSqrDistTmp = Edge.SquaredDistance(Rectangle,t,temp);
	if (fSqrDistTmp < fSqrDist)
	{
		fSqrDist = fSqrDistTmp;
		fS0 = Rectangle.m_fA[0];
		fT0 = t - Rectangle.m_fA[1];
		fS1 = temp[0];
		fT1 = temp[1];
	}

	Edge.Set(RectanglePoint[2],RectanglePoint[3]);
	fSqrDistTmp = Edge.SquaredDistance(Rectangle,t,temp);
	if (fSqrDistTmp < fSqrDist)
	{
		fSqrDist = fSqrDistTmp;
		fS0 = t - Rectangle.m_fA[0];
		fT0 = - Rectangle.m_fA[1];
		fS1 = temp[0];
		fT1 = temp[1];
	}

	Edge.Set(RectanglePoint[2],RectanglePoint[1]);
	fSqrDistTmp = Edge.SquaredDistance(Rectangle,t,temp);
	if (fSqrDistTmp < fSqrDist)
	{
		fSqrDist = fSqrDistTmp;
		fS0 = -Rectangle.m_fA[0];
		fT0 = t - Rectangle.m_fA[1];
		fS1 = temp[0];
		fT1 = temp[1];
	}
	fRectangle1Parameter[0] = fS0;
	fRectangle1Parameter[1] = fT0;
	fRectangle2Parameter[0] = fS1;
	fRectangle2Parameter[1] = fT1;
	return fSqrDist;
}
/*----------------------------------------------------------------*/
//ֱߺ;ξ
SKREAL SKRectangle3::SquaredDistance(const SKLine3& Line,SKREAL fRectangleParameter[2],SKREAL &fLineParameter)const
{
	return Line.SquaredDistance(*this,fLineParameter,fRectangleParameter);


}
/*----------------------------------------------------------------*/
//κOBB
SKREAL SKRectangle3::SquaredDistance(const SKOBB3 &OBB,SKREAL RectangleParameter[2], SKREAL OBBParameter[3])const
{

	return OBB.SquaredDistance(*this,OBBParameter,RectangleParameter);

}
/*----------------------------------------------------------------*/
SKREAL SKRectangle3::SquaredDistance(const SKRay3 &Ray,SKREAL fRectangleParameter[2],SKREAL &fRayParameter)const
{
	return Ray.SquaredDistance(*this,fRayParameter,fRectangleParameter);

}
/*----------------------------------------------------------------*/
//߶ε
SKREAL SKRectangle3::SquaredDistance(const SKSegment3 & Segment,SKREAL fRectangleParameter[2],SKREAL &fSegmentParameter)const
{
	return Segment.SquaredDistance(*this,fSegmentParameter,fRectangleParameter);

}
/*----------------------------------------------------------------*/
//κ;ξ
SKREAL SKRectangle3::SquaredDistance(const SKTriangle3 &Triangle,SKREAL fRectangleParameter[2],SKREAL fTriangleParameter[3])const
{

	return Triangle.SquaredDistance(*this,fTriangleParameter,fRectangleParameter);

}
/*----------------------------------------------------------------*/
//;ξ
SKREAL SKRectangle3::SquaredDistance(const SKVector3 &Point,SKREAL fRectangleParameter[2])const
{
	return Point.SquaredDistance(*this,fRectangleParameter);

}
/*----------------------------------------------------------------*/
SKREAL SKRectangle3::Distance(const SKSphere3 &Sphere,SKREAL fRectangleParameter[2],SKVector3 & SpherePoint)const
{
	return Sphere.Distance(*this,SpherePoint,fRectangleParameter);
}
/*----------------------------------------------------------------*/
SKREAL SKRectangle3::Distance(const SKPlane3 &Plane,SKVector3 &RectanglePoint,SKVector3 &PlanePoint)const
{
	return Plane.Distance(*this,PlanePoint,RectanglePoint);

}
/*----------------------------------------------------------------*/
SKREAL SKRectangle3::SquaredDistance(const SKAABB3 &AABB,SKREAL RectangleParameter[2]
						, SKREAL AABBParameter[3])const
{
	return 	AABB.SquaredDistance(*this,RectangleParameter,AABBParameter);				
						
}
/*----------------------------------------------------------------*/
SKREAL SKRectangle3::SquaredDistance(const SKPolygon3 &Polygon,
						SKREAL fRectangleParameter[2],
						int& IndexTriangle,
						SKREAL fTriangleParameter[3])const
{

	return Polygon.SquaredDistance(*this,IndexTriangle,fTriangleParameter,fRectangleParameter);
}