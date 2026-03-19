#include "SKPlane3.h"
#include "SKSegment3.h"
#include "SKAABB3.h"
#include "SKPolygon3.h"
#include "SKCylinder3.h"
using namespace SKEngine2;
int SKPlane3::RelationWith(const SKVector3 &Point)const 
{
	SKREAL f = (Point.Dot(m_N)) + m_fD;

	if (f >  EPSILON_E4) return SKFRONT;
	if (f < -EPSILON_E4) return SKBACK;
	return SKON;
}
/*----------------------------------------------------------------*/
int SKPlane3::RelationWith(const SKLine3 &Line, bool bCull,SKREAL &fLineParameter)const
{
	
	return Line.RelationWith(*this,bCull,fLineParameter);
}
/*----------------------------------------------------------------*/
int SKPlane3::RelationWith(const SKRay3 &Ray, bool bCull,SKREAL &fRayParameter)const
{
	return Ray.RelationWith(*this,bCull,fRayParameter);

}
/*----------------------------------------------------------------*/
int SKPlane3::RelationWith(const SKSegment3 &Segment, bool bCull,SKREAL &fSegmentParameter)const
{
	return Segment.RelationWith(*this,bCull,fSegmentParameter);
}
/*----------------------------------------------------------------*/
int SKPlane3::RelationWith(const SKOBB3 &OBB)const
{
	return OBB.RelationWith(*this);

}
/*----------------------------------------------------------------*/
int SKPlane3::RelationWith(const SKAABB3 &AABB)const
{
	return AABB.RelationWith(*this);
}
/*----------------------------------------------------------------*/
int SKPlane3::RelationWith(const SKSphere3 &Sphere)const
{
	return Sphere.RelationWith(*this);
}
/*----------------------------------------------------------------*/
int SKPlane3::RelationWith(const SKTriangle3 &Triangle)const
{
	return Triangle.RelationWith(*this);
}
/*----------------------------------------------------------------*/
int SKPlane3::RelationWith(const SKPlane3 &Plane)const
{
	SKVector3 vcCross;
	SKREAL     fSqrLength;


	vcCross.Cross(m_N, Plane.m_N);
	fSqrLength = vcCross.GetSqrLength();
	
	
	if (fSqrLength < EPSILON_E4) 
	{
		//return Plane.m_Point.RelationWith(*this);
		return SKNOINTERSECT;	
	}
	return SKINTERSECT;
	/*SKREAL fN00 = m_N.GetSqrLength();
	SKREAL fN01 = m_N * Plane.m_N;
	SKREAL fN11 = Plane.m_N.GetSqrLength();
	SKREAL fDet = fN00*fN11 - fN01*fN01;

	if (ABS(fDet) < EPSILON_E4) 
		return Plane.m_Point.RelationWith(*this);

	SKREAL fInvDet = 1.0f/fDet;
	SKREAL fC0 = (fN11 * m_fD - fN01 * Plane.m_fD) * fInvDet;
	SKREAL fC1 = (fN00 * Plane.m_fD - fN01 * m_fD) * fInvDet;
	Line.Set(vcCross,m_N * fC0 + Plane.m_N * fC1);
	return SKINTERSECT;*/
}
/*----------------------------------------------------------------*/
int SKPlane3::RelationWith(const SKRectangle3 & Rectangle)const
{
	return Rectangle.RelationWith(*this);

}
/*----------------------------------------------------------------*/
int SKPlane3::RelationWith(const SKPlane3 &Plane,SKLine3 &Line)const
{
	SKVector3 vcCross;
	SKREAL     fSqrLength;


	vcCross.Cross(m_N, Plane.m_N);
	fSqrLength = vcCross.GetSqrLength();


	if (fSqrLength < EPSILON_E4) 
	{
		return SKNOINTERSECT;
		//return Plane.m_Point.RelationWith(*this);

	}
	SKREAL fN00 = m_N.GetSqrLength();
	SKREAL fN01 = m_N.Dot(Plane.m_N);
	SKREAL fN11 = Plane.m_N.GetSqrLength();
	SKREAL fDet = fN00*fN11 - fN01*fN01;

	if (ABS(fDet) < EPSILON_E4) 
		return SKNOINTERSECT;

	SKREAL fInvDet = 1.0f/fDet;
	SKREAL fC0 = (fN11 * m_fD - fN01 * Plane.m_fD) * fInvDet;
	SKREAL fC1 = (fN00 * Plane.m_fD - fN01 * m_fD) * fInvDet;
	Line.Set(m_N * fC0 + Plane.m_N * fC1,vcCross);
	return SKINTERSECT;/**/
}
/*----------------------------------------------------------------*/
int SKPlane3::RelationWith(const SKTriangle3 &Triangle ,SKSegment3 & Segment)const
{
	return Triangle.RelationWith(*this,Segment);

}
/*----------------------------------------------------------------*/
int SKPlane3::RelationWith(const SKRectangle3 &Rectangle,SKSegment3 &Segment)const
{
	return Rectangle.RelationWith(*this,Segment);

}
/*----------------------------------------------------------------*/
int SKPlane3::RelationWith(const SKPolygon3 &Polygon)const
{
	return Polygon.RelationWith(*this);
}
/*----------------------------------------------------------------*/
int SKPlane3::RelationWith(const SKPolygon3 &Polygon,SKSegment3 & Segment)const
{

	return Polygon.RelationWith(*this,Segment);
}
int SKPlane3::RelationWith(const SKCylinder3 &Cylinder3)const
{
	return SKINTERSECT;
}