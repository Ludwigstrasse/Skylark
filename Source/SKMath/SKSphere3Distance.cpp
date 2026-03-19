#include "SKSphere3.h"
#include "SKMath.h"
#include "SKSegment3.h"
#include "SKAABB3.h"
#include "SKPolygon3.h"
using namespace SKEngine2;
/*----------------------------------------------------------------*/
SKREAL SKSphere3::Distance(const SKVector3 & Point,SKVector3 & SpherePoint)const
{
	SKREAL sqrDist = Point.SquaredDistance(m_Center);

	sqrDist = SQRT(sqrDist );
	sqrDist = sqrDist - m_fRadius;

	SKLine3 Line(m_Center,Point);

	SpherePoint = Line.GetParameterPoint(m_fRadius);
	return sqrDist;
}
/*----------------------------------------------------------------*/
SKREAL SKSphere3::Distance(const SKLine3 & Line,SKVector3 & SpherePoint,SKREAL &fLineParameter)const
{

	SKREAL sqrDist = Line.SquaredDistance(m_Center,fLineParameter);
	sqrDist = SQRT(sqrDist );
	sqrDist = sqrDist - m_fRadius;

	SKLine3 LineTemp(m_Center,Line.GetParameterPoint(fLineParameter));

	SpherePoint = LineTemp.GetParameterPoint(m_fRadius);
	return sqrDist;
	
}
/*----------------------------------------------------------------*/
SKREAL SKSphere3::Distance(const SKRay3 & Ray,SKVector3 & SpherePoint,SKREAL &fRayParameter)const
{
	SKREAL sqrDist = Ray.SquaredDistance(m_Center,fRayParameter);

	sqrDist = SQRT(sqrDist );
	sqrDist = sqrDist - m_fRadius;

	SKLine3 LineTemp(m_Center,Ray.GetParameterPoint(fRayParameter));

	SpherePoint = LineTemp.GetParameterPoint(m_fRadius);
	return sqrDist;

}
/*----------------------------------------------------------------*/
SKREAL SKSphere3::Distance(const SKSegment3 & Segment,SKVector3 & SpherePoint,SKREAL &fSegmentParameter)const
{
	SKREAL sqrDist = Segment.SquaredDistance(m_Center,fSegmentParameter);

	sqrDist = SQRT(sqrDist );
	sqrDist = sqrDist - m_fRadius;

	SKLine3 LineTemp(m_Center,Segment.GetParameterPoint(fSegmentParameter));

	SpherePoint = LineTemp.GetParameterPoint(m_fRadius);
	return sqrDist;

}
/*----------------------------------------------------------------*/
SKREAL SKSphere3::Distance(const SKOBB3 &OBB,SKVector3 & SpherePoint,SKREAL fOBBParameter[3])const
{
	SKREAL sqrDist = OBB.SquaredDistance(m_Center,fOBBParameter);

	sqrDist = SQRT(sqrDist );
	sqrDist = sqrDist - m_fRadius;

	SKLine3 LineTemp(m_Center,OBB.GetParameterPoint(fOBBParameter));

	SpherePoint = LineTemp.GetParameterPoint(m_fRadius);
	return sqrDist;
}
/*----------------------------------------------------------------*/
SKREAL SKSphere3::Distance(const SKPlane3 & Plane,SKVector3 & SpherePoint)const
{
	SKVector3 PlanePoint;
	SKREAL sqrDist = Plane.Distance(m_Center,PlanePoint);


	sqrDist = sqrDist - m_fRadius;

	SKLine3 LineTemp(m_Center,PlanePoint);

	SpherePoint = LineTemp.GetParameterPoint(m_fRadius);
	return sqrDist;

}
/*----------------------------------------------------------------*/
SKREAL SKSphere3::Distance(const SKRectangle3 & Rectangle,SKVector3 & SpherePoint,SKREAL fRectangleParameter[2])const
{
	SKREAL sqrDist = Rectangle.SquaredDistance(m_Center,fRectangleParameter);

	sqrDist = SQRT(sqrDist );
	sqrDist = sqrDist - m_fRadius;

	SKLine3 LineTemp(m_Center,Rectangle.GetParameterPoint(fRectangleParameter));

	SpherePoint = LineTemp.GetParameterPoint(m_fRadius);
	return sqrDist;

}
/*----------------------------------------------------------------*/
SKREAL SKSphere3::Distance(const SKTriangle3 Triangle,SKVector3 & SpherePoint,SKREAL fTriangleParameter[3])const
{
	SKREAL sqrDist = Triangle.SquaredDistance(m_Center,fTriangleParameter);

	sqrDist = SQRT(sqrDist );
	sqrDist = sqrDist - m_fRadius;

	SKLine3 LineTemp(m_Center,Triangle.GetParameterPoint(fTriangleParameter));

	SpherePoint = LineTemp.GetParameterPoint(m_fRadius);
	return sqrDist;
}
/*----------------------------------------------------------------*/
SKREAL SKSphere3::Distance(const SKAABB3 &AABB,SKVector3 & SpherePoint,SKREAL fAABBParameter[3])const
{
	SKREAL sqrDist = AABB.SquaredDistance(m_Center,fAABBParameter);

	sqrDist = SQRT(sqrDist );
	sqrDist = sqrDist - m_fRadius;

	SKLine3 LineTemp(m_Center,AABB.GetParameterPoint(fAABBParameter));

	SpherePoint = LineTemp.GetParameterPoint(m_fRadius);
	return sqrDist;
}
/*----------------------------------------------------------------*/
SKREAL SKSphere3::Distance(const SKPolygon3 &Polygon,SKVector3 & SpherePoint,int& IndexTriangle,
							SKREAL fTriangleParameter[3])const
{

	return Polygon.Distance(*this,IndexTriangle,fTriangleParameter,SpherePoint);
}