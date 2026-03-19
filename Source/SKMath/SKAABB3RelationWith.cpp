#include "SKAABB3.h"
#include "SKMath.h"
#include "SKMatrix3X3.h"
#include "SKLine3.h"
#include "SKRay3.h"
#include "SKRectangle3.h"
#include "SKSegment3.h"
#include "..\SKContainers\SKArray.h"
using namespace SKEngine2;
/*----------------------------------------------------------------*/
int SKAABB3::RelationWith(const SKVector3 &Point)const
{
	SKVector3 PointTemp = Point - m_Center;
	SKREAL fDiffX = ABS(PointTemp.x) - m_fA[0];
	SKREAL fDiffY = ABS(PointTemp.y) - m_fA[1];
	SKREAL fDiffZ = ABS(PointTemp.z) - m_fA[2];

	if(fDiffX > EPSILON_E4 || fDiffY > EPSILON_E4 || fDiffZ > EPSILON_E4)
		return SKOUT;

	if(fDiffX < -EPSILON_E4 || fDiffY < -EPSILON_E4 || fDiffZ < -EPSILON_E4)
		return SKIN;

	return SKON;

}
/*----------------------------------------------------------------*/
int SKAABB3::RelationWith(const SKLine3 &Line, unsigned int &Quantity,SKREAL &tNear,SKREAL &tFar)const
{
	return Line.RelationWith(*this,Quantity,tNear,tFar);

}
/*----------------------------------------------------------------*/
int SKAABB3::RelationWith(const SKRay3 &Ray, unsigned int &Quantity,SKREAL &tNear,SKREAL &tFar)const
{

	return Ray.RelationWith(*this,Quantity,tNear,tFar);
}
/*----------------------------------------------------------------*/
int SKAABB3::RelationWith(const SKSegment3 &Segment, unsigned int &Quantity,SKREAL &tNear,SKREAL &tFar)const
{
	return Segment.RelationWith(*this,Quantity,tNear,tFar);

}
/*----------------------------------------------------------------*/
int SKAABB3::RelationWith(const SKPlane3 &Plane)const
{
	SKVector3 N = Plane.GetN();
	SKREAL fD = Plane.GetfD();
	SKVector3 MinTemp,MaxTemp;
	// x 
	if (N.x >= 0.0f) 
	{
		MinTemp.x = m_Min.x;
		MaxTemp.x = m_Max.x;
	}
	else 
	{
		MinTemp.x = m_Max.x;
		MaxTemp.x = m_Min.x;
	}
	// y 
	if (N.y >= 0.0f) 
	{
		MinTemp.y = m_Min.y;
		MaxTemp.y = m_Max.y;
	}
	else 
	{
		MinTemp.y = m_Max.y;
		MaxTemp.y = m_Min.y;
	}
	// z 
	if (N.z >= 0.0f) 
	{
		MinTemp.z = m_Min.z;
		MaxTemp.z = m_Max.z;
	}
	else 
	{
		MinTemp.z = m_Max.z;
		MaxTemp.z = m_Min.z;
	}

	if ((N.Dot(MinTemp) + fD) > 0.0f)
		return SKFRONT;
	else if ((N.Dot(MaxTemp) + fD) < 0.0f)
		return SKBACK;
	else 
		return SKINTERSECT;
}
/*----------------------------------------------------------------*/
int SKAABB3::RelationWith(const SKTriangle3 &Triangle)const
{
	return Triangle.RelationWith(*this);


}
/*----------------------------------------------------------------*/
int SKAABB3::RelationWith(const SKRectangle3 &Rectangle)const
{
	return Rectangle.RelationWith(*this);
}
/*----------------------------------------------------------------*/
int SKAABB3::RelationWith(const SKAABB3 &AABB)const
{
	if ((m_Min.x > AABB.m_Max.x) || (AABB.m_Min.x > m_Max.x))
		return SKNOINTERSECT;
	if ((m_Min.y > AABB.m_Max.y) || (AABB.m_Min.y > m_Max.y))
		return SKNOINTERSECT;
	if ((m_Min.z > AABB.m_Max.z) || (AABB.m_Min.z > m_Max.z))
		return SKNOINTERSECT;
	return SKINTERSECT;
}
/*----------------------------------------------------------------*/
int SKAABB3::RelationWith(const SKSphere3 &Sphere)const
{
	SKREAL fAABBParameter[3];
	SKREAL Dist = Sphere.m_Center.SquaredDistance(*this,fAABBParameter);
	Dist = SQRT(Dist);
	if(Dist - Sphere.m_fRadius < 0.0f)
		return SKINTERSECT;

	return SKNOINTERSECT;
}
bool SKAABB3::GetIntersect(SKAABB3& AABB,SKAABB3 & OutAABB)const
{
	if (RelationWith(AABB) == SKINTERSECT)
	{
		
		SKArrayOrder<SKREAL> XArray;
		XArray.AddElement(GetMinPoint().x);
		XArray.AddElement(GetMaxPoint().x);
		XArray.AddElement(AABB.GetMinPoint().x);
		XArray.AddElement(AABB.GetMaxPoint().x);

		SKArrayOrder<SKREAL> YArray;
		YArray.AddElement(GetMinPoint().y);
		YArray.AddElement(GetMaxPoint().y);
		YArray.AddElement(AABB.GetMinPoint().y);
		YArray.AddElement(AABB.GetMaxPoint().y);

		SKArrayOrder<SKREAL> ZArray;
		ZArray.AddElement(GetMinPoint().z);
		ZArray.AddElement(GetMaxPoint().z);
		ZArray.AddElement(AABB.GetMinPoint().z);
		ZArray.AddElement(AABB.GetMaxPoint().z);

		OutAABB.Set(SKVector3(XArray[2],YArray[2],ZArray[2]),SKVector3(XArray[1],YArray[1],ZArray[1]));
		return true;


	}

	return false;
}