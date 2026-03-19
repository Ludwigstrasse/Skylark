#include "SKRay3.h"
#include "SKSegment3.h"
#include "SKAABB3.h"
#include "SKPolygon3.h"
using namespace SKEngine2;
/*----------------------------------------------------------------*/
int SKRay3::RelationWith(const SKTriangle3 & Triangle, bool bCull,SKREAL &fRayParameter,
				 SKREAL fTriangleParameter[3])const
{
	if(SKLine3::RelationWith(Triangle,bCull,fRayParameter,fTriangleParameter) == SKNOINTERSECT)
		return SKNOINTERSECT;
	if(fRayParameter < 0.0f)
		return SKNOINTERSECT;
	return SKINTERSECT;
}
/*----------------------------------------------------------------*/
int SKRay3::RelationWith(const SKPlane3 &Plane, bool bCull,SKREAL &fRayParameter)const
{
	int iFlag = SKLine3::RelationWith(Plane,bCull,fRayParameter);
	if( iFlag != SKINTERSECT)
		return iFlag;
	if(fRayParameter < 0.0f)
	{
		iFlag = m_Orig.RelationWith(Plane);
		return iFlag;
	}
	return SKINTERSECT;

}
/*----------------------------------------------------------------*/
int SKRay3::RelationWith(const SKRectangle3 &Rectangle,bool bCull,SKREAL &fRayParameter,
						SKREAL fRectangleParameter[2])const
{
	if(SKLine3::RelationWith(Rectangle,bCull,fRayParameter,fRectangleParameter) == SKNOINTERSECT)
		return SKNOINTERSECT;
	if(fRayParameter < 0.0f)
		return SKNOINTERSECT;

	return SKINTERSECT;
}
/*----------------------------------------------------------------*/
int SKRay3::RelationWith(const SKOBB3 &OBB, unsigned int &Quantity,SKREAL &tNear,SKREAL &tFar)const
{
	if(SKLine3::RelationWith(OBB,Quantity, tNear,tFar) == SKNOINTERSECT)
		return SKNOINTERSECT;
	if(tFar < 0.0f)
	{
		Quantity = 0;
		return SKNOINTERSECT;
	}
	if(tNear<0.0f)
	{
		Quantity = 1;
		tNear = tFar;
	}
	return SKINTERSECT;
}
/*----------------------------------------------------------------*/
int SKRay3::RelationWith(const SKSphere3 &sphere, unsigned int &Quantity,SKREAL &tNear,SKREAL &tFar)const
{
	if(SKLine3::RelationWith(sphere,Quantity, tNear,tFar) == SKNOINTERSECT)
		return SKNOINTERSECT;
	if(tFar < 0.0f)
	{
		Quantity = 0;
		return SKNOINTERSECT;
	}
	if(tNear<0.0f)
	{
		Quantity = 1;
		tNear = tFar;
	}
	return SKINTERSECT;

}
/*----------------------------------------------------------------*/
int SKRay3::RelationWith(const SKAABB3 &AABB, unsigned int &Quantity,SKREAL &tNear,SKREAL &tFar)const
{
	if(SKLine3::RelationWith(AABB,Quantity, tNear,tFar) == SKNOINTERSECT)
		return SKNOINTERSECT;
	if(tFar < 0.0f)
	{
		Quantity = 0;
		return SKNOINTERSECT;
	}
	if(tNear<0.0f)
	{
		Quantity = 1;
		tNear = tFar;
	}
	return SKINTERSECT;

}
/*----------------------------------------------------------------*/
int SKRay3::RelationWith(const SKPolygon3 &Polygon,SKREAL &fRayParameter ,
				 bool bCull,int &iIndexTriangle,SKREAL fTriangleParameter[3])const
{

	return Polygon.RelationWith(*this,bCull,iIndexTriangle,fTriangleParameter,fRayParameter);
}