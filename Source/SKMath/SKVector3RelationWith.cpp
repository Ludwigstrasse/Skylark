#include "SKVector3.h"
#include "SKMath.h"
#include "SKMatrix3X3.h"
#include "SKQuat.h"
#include "SKLine3.h"
#include "SKRay3.h"
#include "SKSegment3.h"
#include "SKTriangle3.h"
#include "SKRectangle3.h"
#include "SKOBB3.h"
#include "SKSphere3.h"
#include "SKAABB3.h"
using namespace SKEngine2;
/*----------------------------------------------------------------*/
int SKVector3::RelationWith(const SKSphere3 & Sphere)const
{
	SKREAL sqrDist = SquaredDistance(Sphere.m_Center);
	SKREAL diff = sqrDist - Sphere.m_fRadius * Sphere.m_fRadius;
	if(diff > EPSILON_E4)
		return SKOUT;
	else if(diff < -EPSILON_E4)
		return SKIN;
	else 
		return SKON;

}
/*----------------------------------------------------------------*/
int SKVector3::RelationWith(const SKPlane3 &Plane)const
{
	return Plane.RelationWith(*this);
}
/*----------------------------------------------------------------*/
int SKVector3::RelationWith(const SKOBB3 &OBB)const
{
	SKMatrix3X3W m;
	OBB.GetTransform(m);
	SKVector3 PointTemp = *this * m;
	SKREAL fA[3];
	OBB.GetfA(fA);
	SKREAL fDiffX = ABS(PointTemp.x) - fA[0];
	SKREAL fDiffY = ABS(PointTemp.y) - fA[1];
	SKREAL fDiffZ = ABS(PointTemp.z) - fA[2];

	if(fDiffX > EPSILON_E4 || fDiffY > EPSILON_E4 || fDiffZ > EPSILON_E4)
		return SKOUT;
	
	if(fDiffX < -EPSILON_E4 || fDiffY < -EPSILON_E4 || fDiffZ < -EPSILON_E4)
		return SKIN;

	return SKON;

}
/*----------------------------------------------------------------*/
//SKIN SKOUT SKON
int SKVector3::RelationWith(const SKAABB3 &AABB)const
{

	return AABB.RelationWith(*this);
}