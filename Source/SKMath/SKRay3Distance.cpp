#include "SKRay3.h"
#include "SKSegment3.h"
#include "SKAABB3.h"
#include "SKPolygon3.h"
using namespace SKEngine2;
/*----------------------------------------------------------------*/
SKREAL SKRay3::SquaredDistance(const SKVector3 &Point,SKREAL &fLineParameter)const
{
	SKREAL fSqDis = SKLine3::SquaredDistance(Point,fLineParameter);
	if(fLineParameter >= 0)
		return fSqDis;
	else
	{
		fLineParameter = 0;
		SKVector3 Diff;
		Diff = Point - m_Orig;
		return Diff.GetSqrLength();
	}
}
/*----------------------------------------------------------------*/
SKREAL SKRay3::SquaredDistance(const SKLine3 &Line,SKREAL &fRayParameter,SKREAL &fLineParameter)const
{
	return Line.SquaredDistance(*this,fLineParameter,fRayParameter);

}
/*----------------------------------------------------------------*/
SKREAL SKRay3::SquaredDistance(const SKRay3 &Ray,SKREAL &fRay1Parameter,SKREAL &fRay2Parameter)const
{
	const SKLine3 Line = Ray;
	SKREAL sqrDist = SquaredDistance(Line,fRay1Parameter,fRay2Parameter);
	if(fRay2Parameter < 0)
	{
		fRay2Parameter = 0;
		sqrDist = SquaredDistance(Ray.m_Orig,fRay1Parameter);
	}

	return sqrDist;
}
/*----------------------------------------------------------------*/
SKREAL SKRay3::SquaredDistance(const SKSegment3 & Segment,SKREAL &fRayParameter,SKREAL &fSegmentParameter)const
{

	const SKRay3 Ray = Segment;
	SKREAL sqrDist = SquaredDistance(Ray,fRayParameter,fSegmentParameter);
	SKREAL fLen = Segment.GetLen();
	if(fSegmentParameter > fLen)
	{
		fSegmentParameter = fLen;
		SKVector3 End = Segment.GetEnd();
		sqrDist = SquaredDistance(End,fRayParameter);

	}
	return sqrDist;
}
/*----------------------------------------------------------------*/
SKREAL SKRay3::SquaredDistance(const SKTriangle3& Triangle,SKREAL &fRayParameter,SKREAL fTriangleParameter[3])const
{
	SKREAL sqrDist = SKLine3::SquaredDistance(Triangle,fRayParameter,fTriangleParameter);
	if(fRayParameter < 0)
	{
		fRayParameter = 0;
		sqrDist = m_Orig.SquaredDistance(Triangle,fTriangleParameter);

	}


	return sqrDist;


}
/*----------------------------------------------------------------*/
SKREAL SKRay3::SquaredDistance(const SKRectangle3& Rectangle,SKREAL &fRayParameter,SKREAL fRectangleParameter[2])const
{
	SKREAL sqrDist = SKLine3::SquaredDistance(Rectangle,fRayParameter,fRectangleParameter);
	if(fRayParameter < 0)
	{
		fRayParameter = 0;
		sqrDist = m_Orig.SquaredDistance(Rectangle,fRectangleParameter);

	}


	return sqrDist;

}
/*----------------------------------------------------------------*/
SKREAL SKRay3::SquaredDistance(const SKOBB3 & OBB,SKREAL &fRayParameter,SKREAL fOBBParameter[3])const
{
	SKREAL sqrDist = SKLine3::SquaredDistance(OBB,fRayParameter,fOBBParameter);
	if(fRayParameter < 0)
	{
		fRayParameter = 0;
		sqrDist = OBB.SquaredDistance(m_Orig,fOBBParameter);

	}


	return sqrDist;

}
/*----------------------------------------------------------------*/
SKREAL SKRay3::Distance(const SKSphere3 &Sphere,SKREAL &fRayParameter,SKVector3 & SpherePoint)const
{
	return Sphere.Distance(*this,SpherePoint,fRayParameter);

}
/*----------------------------------------------------------------*/
SKREAL SKRay3::Distance(const SKPlane3 & Plane,SKVector3 &RayPoint,SKVector3 &PlanePoint)const
{
	return Plane.Distance(*this,PlanePoint,RayPoint);
}
/*----------------------------------------------------------------*/
SKREAL SKRay3::SquaredDistance(const SKAABB3 &AABB,SKREAL &fRayParameter, SKREAL fAABBParameter[3])const
{
	SKREAL sqrDist = SKLine3::SquaredDistance(AABB,fRayParameter,fAABBParameter);
	if(fRayParameter < 0)
	{
		fRayParameter = 0;
		sqrDist = AABB.SquaredDistance(m_Orig,fAABBParameter);

	}


	return sqrDist;

}
/*----------------------------------------------------------------*/
SKREAL SKRay3::SquaredDistance(const SKPolygon3 &Polygon,SKREAL &fRayParameter,
					   int& IndexTriangle,
					   SKREAL fTriangleParameter[3])const
{


	return Polygon.SquaredDistance(*this,IndexTriangle,fTriangleParameter,fRayParameter);
}