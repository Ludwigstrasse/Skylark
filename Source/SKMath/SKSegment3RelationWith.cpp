#include "SKSegment3.h"
#include "SKAABB3.h"
#include "SKPolygon3.h"
using namespace SKEngine2;
/*----------------------------------------------------------------*/
int SKSegment3::RelationWith(const SKTriangle3 & Triangle, bool bCull,SKREAL &fSegmentParameter,
							SKREAL fTriangleParameter[3])const
{
	if(SKRay3::RelationWith(Triangle,bCull,fSegmentParameter,fTriangleParameter) == SKNOINTERSECT)
		return SKNOINTERSECT;
	if(fSegmentParameter > m_fLen)
		return SKNOINTERSECT;
	return SKINTERSECT;
}
/*----------------------------------------------------------------*/
int SKSegment3::RelationWith(const SKPlane3 &Plane, bool bCull,SKREAL &fSegmentParameter)const
{
	int iFlag = SKRay3::RelationWith(Plane,bCull,fSegmentParameter);
	if(iFlag != SKINTERSECT)
		return iFlag;
	if(fSegmentParameter > m_fLen)
	{
		return m_Orig.RelationWith(Plane);
	}
	return SKINTERSECT;
}
/*----------------------------------------------------------------*/
int SKSegment3::RelationWith(const SKRectangle3 &Rectangle,bool bCull,SKREAL &fSegmentParameter,
				 SKREAL fRectangleParameter[2])const
{

	if(SKRay3::RelationWith(Rectangle,bCull,fSegmentParameter,fRectangleParameter) == SKNOINTERSECT)
		return SKNOINTERSECT;
	if(fSegmentParameter > m_fLen)
		return SKNOINTERSECT;
	return SKINTERSECT;
}
/*----------------------------------------------------------------*/
int SKSegment3::RelationWith(const SKOBB3 &OBB, unsigned int &Quantity,SKREAL &tNear,SKREAL &tFar)const
{
	if(SKRay3::RelationWith(OBB,Quantity,tNear,tFar) == SKNOINTERSECT)
		return SKNOINTERSECT;
	if(tNear > m_fLen)
	{
		Quantity = 0;
		if(m_Orig.RelationWith(OBB) != SKOUT)
			return SKIN;
		return SKNOINTERSECT;
	}

	if(tFar > m_fLen)
	{
		Quantity = 1;
		tFar = tNear;
	}
	return SKINTERSECT;
}
/*----------------------------------------------------------------*/
int SKSegment3::RelationWith(const SKSphere3 &sphere, unsigned int &Quantity,SKREAL &tNear,SKREAL &tFar)const
{
	if(SKRay3::RelationWith(sphere,Quantity,tNear,tFar) == SKNOINTERSECT)
		return SKNOINTERSECT;
	if(tNear > m_fLen)
	{
		Quantity = 0;
		if(m_Orig.RelationWith(sphere) != SKOUT)
			return SKIN;
		return SKNOINTERSECT;
	}

	if(tFar > m_fLen)
	{
		Quantity = 1;
		tFar = tNear;
	}
	return SKINTERSECT;

}
/*----------------------------------------------------------------*/
int SKSegment3::RelationWith(const SKAABB3 &AABB, unsigned int &Quantity,SKREAL &tNear,SKREAL &tFar)const
{

	if(SKRay3::RelationWith(AABB,Quantity,tNear,tFar) == SKNOINTERSECT)
		return SKNOINTERSECT;
	if(tNear > m_fLen)
	{
		Quantity = 0;
		if(m_Orig.RelationWith(AABB) != SKOUT)
			return SKIN;
		return SKNOINTERSECT;
	}

	if(tFar > m_fLen)
	{
		Quantity = 1;
		tFar = tNear;
	}
	return SKINTERSECT;

}
/*----------------------------------------------------------------*/
int SKSegment3::RelationWith(const SKPolygon3 &Polygon,SKREAL &fSegmentParameter,
				 bool bCull,int &iIndexTriangle,SKREAL fTriangleParameter[3])const
{

	return Polygon.RelationWith(*this,bCull,iIndexTriangle,fTriangleParameter,fSegmentParameter);
}