#include "SKSegment3.h"
#include "SKAABB3.h"
#include "SKPolygon3.h"
using namespace SKEngine2;
/*----------------------------------------------------------------*/
SKREAL SKSegment3::SquaredDistance(const SKVector3 &Point,SKREAL &fSegmentParameter)const
{
	SKREAL SqDis = SKRay3::SquaredDistance(Point,fSegmentParameter);
	if(fSegmentParameter > m_fLen)
	{
		fSegmentParameter = m_fLen;
		SKVector3 Diff;
		Diff = Point - m_End;
		return Diff.GetSqrLength();
	}
	else
	{
		return SqDis;
	}
}
/*----------------------------------------------------------------*/
SKREAL SKSegment3::SquaredDistance(const SKLine3 &Line,SKREAL &fSegmentParameter,SKREAL &fLineParameter)const
{
	return Line.SquaredDistance(*this,fLineParameter,fSegmentParameter);
}
/*----------------------------------------------------------------*/
SKREAL SKSegment3::SquaredDistance(const SKRay3 &Ray,SKREAL &fSegmentParameter,SKREAL &fRayParameter)const
{
	return Ray.SquaredDistance(*this,fRayParameter,fSegmentParameter);
}
/*----------------------------------------------------------------*/
SKREAL SKSegment3::SquaredDistance(const SKSegment3 & Segment,SKREAL &fSegment1Parameter,SKREAL &fSegment2Parameter)const
{
	const SKRay3 & Ray = Segment;
	SKREAL sqrDist = SquaredDistance(Ray,fSegment1Parameter,fSegment2Parameter);
	SKREAL fLen = Segment.GetLen();
	if(fSegment2Parameter > fLen)
	{
		fSegment2Parameter = fLen;
		SKVector3 End = Segment.GetEnd();
		sqrDist = SquaredDistance(End,fSegment1Parameter);
	}
	return sqrDist;
}
/*----------------------------------------------------------------*/
SKREAL SKSegment3::SquaredDistance(const SKTriangle3& Triangle,SKREAL &fSegmentParameter,SKREAL fTriangleParameter[3])const
{
	SKREAL sqrDist = SKRay3::SquaredDistance(Triangle,fSegmentParameter,fTriangleParameter);

	if(fSegmentParameter > m_fLen)
	{
		fSegmentParameter = m_fLen;
		sqrDist = m_End.SquaredDistance(Triangle,fTriangleParameter);

	}


	return sqrDist;

}
/*----------------------------------------------------------------*/
SKREAL SKSegment3::SquaredDistance(const SKRectangle3& Rectangle,SKREAL &fSegmentParameter,SKREAL fRectangleParameter[2])const
{
	SKREAL sqrDist = SKRay3::SquaredDistance(Rectangle,fSegmentParameter,fRectangleParameter);

	if(fSegmentParameter > m_fLen)
	{
		fSegmentParameter = m_fLen;
		sqrDist = m_End.SquaredDistance(Rectangle,fRectangleParameter);

	}


	return sqrDist;


}
/*----------------------------------------------------------------*/
SKREAL SKSegment3::SquaredDistance(const SKOBB3 & OBB,SKREAL &fSegmentParameter,SKREAL fOBBParameter[3])const
{
	SKREAL sqrDist = SKRay3::SquaredDistance(OBB,fSegmentParameter,fOBBParameter);
	if(fSegmentParameter > m_fLen)
	{
		fSegmentParameter = m_fLen;
		sqrDist = OBB.SquaredDistance(m_End,fOBBParameter);

	}


	return sqrDist;

}
/*----------------------------------------------------------------*/
SKREAL SKSegment3::Distance(const SKSphere3 &Sphere,SKREAL &fSegmentParameter,SKVector3 & SpherePoint)const
{

	return Sphere.Distance(*this,SpherePoint,fSegmentParameter);
}
/*----------------------------------------------------------------*/
SKREAL SKSegment3::Distance(const SKPlane3 & Plane,SKVector3 &SegmentPoint,SKVector3 &PlanePoint)const
{
	return Plane.Distance(*this,PlanePoint,SegmentPoint);
}
/*----------------------------------------------------------------*/
SKREAL SKSegment3::SquaredDistance(const SKAABB3 &AABB,SKREAL &fSegmentParameter,SKREAL fAABBParameter[3])const
{
	SKREAL sqrDist = SKRay3::SquaredDistance(AABB,fSegmentParameter,fAABBParameter);
	if(fSegmentParameter > m_fLen)
	{
		fSegmentParameter = m_fLen;
		sqrDist = AABB.SquaredDistance(m_End,fAABBParameter);

	}


	return sqrDist;

}
/*----------------------------------------------------------------*/
SKREAL SKSegment3::SquaredDistance(const SKPolygon3 &Polygon,SKREAL &fSegmentParameter,
									int& IndexTriangle,
									SKREAL fTriangleParameter[3])const
{

	return Polygon.SquaredDistance(*this,IndexTriangle,fTriangleParameter,fSegmentParameter);
}