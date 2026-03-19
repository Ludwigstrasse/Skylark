#include "SKSphere3.h"
#include "SKMath.h"
#include "SKSegment3.h"
#include "SKAABB3.h"
using namespace SKEngine2;
int SKSphere3::RelationWith(const SKVector3 &Point)const
{

	return Point.RelationWith(*this);
}
/*----------------------------------------------------------------*/
int SKSphere3::RelationWith(const SKLine3 &Line, unsigned int &Quantity,SKREAL &tNear,SKREAL &tFar)const
{
	return Line.RelationWith(*this,Quantity,tNear,tFar);

}
/*----------------------------------------------------------------*/
int SKSphere3::RelationWith(const SKRay3 &Ray, unsigned int &Quantity,SKREAL &tNear,SKREAL &tFar)const
{
	return Ray.RelationWith(*this,Quantity,tNear,tFar);

}
/*----------------------------------------------------------------*/
int SKSphere3::RelationWith(const SKSegment3 &Segment, unsigned int &Quantity,SKREAL &tNear,SKREAL &tFar)const
{
	return Segment.RelationWith(*this,Quantity,tNear,tFar);
}
/*----------------------------------------------------------------*/
int SKSphere3::RelationWith(const SKPlane3 &Plane)const
{
	SKVector3 N = Plane.GetN();
	SKREAL fD = Plane.GetfD();

	SKREAL test = m_Center.Dot(N) + fD;
	if(test > m_fRadius)
		return SKFRONT;
	else if(test < -m_fRadius)
		return SKBACK;
	else
		return SKINTERSECT;

}
/*----------------------------------------------------------------*/
int SKSphere3::RelationWith(const SKTriangle3 Triangle)const
{
	return Triangle.RelationWith(*this);

}
/*----------------------------------------------------------------*/
int SKSphere3::RelationWith(const SKRectangle3 &Rectangle)const
{
	return Rectangle.RelationWith(*this);
}
/*----------------------------------------------------------------*/
int SKSphere3::RelationWith(const SKOBB3 &OBB)const
{
	return OBB.RelationWith(*this);
}
/*----------------------------------------------------------------*/
int SKSphere3::RelationWith(const SKSphere3 &Sphere)const
{
	SKREAL Sum = m_fRadius + Sphere.m_fRadius;

	Sum *=Sum;

	SKVector3 Sub = m_Center - Sphere.m_Center;

	if(Sub.GetSqrLength() > Sum)
		return SKNOINTERSECT;

	return SKINTERSECT;

}