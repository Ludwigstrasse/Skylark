#ifndef SKRAY3_H
#define SKRAY3_H
#include "SKLine3.h"
namespace SKEngine2
{
	//
	class  SKMATH_API SKRay3 :public SKLine3
	{
	public:

		SKRay3();
		SKRay3(const SKVector3 & Orig,const SKVector3 &Dir);
		~SKRay3();
		bool GetParameter(const SKVector3 &Point,SKREAL &fRayParameter )const;
		void Transform(const SKRay3 &Ray,const SKMatrix3X3W &Mat);
		/************************FORCEINLINE***************************************/
		FORCEINLINE SKVector3 GetParameterPoint(SKREAL fRayParameter)const;

		/********************************RelationWith******************************************/
		//Ƿλùϵ bCullΪǷΪ,Ƿǳ,tཻ
		//SKNOINTERSECT SKNTERSECT
		int RelationWith(const SKTriangle3 & Triangle, bool bCull,SKREAL &fRayParameter,
						SKREAL fTriangleParameter[3])const;
		//ƽλùϵ
		//SKNOINTERSECT SKNTERSECT SKON SKBACK SKFRONT
		int RelationWith(const SKPlane3 &Plane, bool bCull,SKREAL &fRayParameter)const;
		//λùϵ
		//SKNOINTERSECT SKNTERSECT
		int RelationWith(const SKRectangle3 &Rectangle,bool bCull,SKREAL &fRayParameter,
						SKREAL fRectangleParameter[2])const;

		//OBBλùϵ
		//SKNOINTERSECT SKNTERSECT
		int RelationWith(const SKOBB3 &OBB, unsigned int &Quantity,SKREAL &tNear,SKREAL &tFar)const;
		//ֱAABBλùϵ
		//SKNOINTERSECT SKNTERSECT
		int RelationWith(const SKAABB3 &AABB, unsigned int &Quantity,SKREAL &tNear,SKREAL &tFar)const;
		//λùϵ
		//SKNOINTERSECT SKNTERSECT
		int RelationWith(const SKSphere3 &sphere, unsigned int &Quantity,SKREAL &tNear,SKREAL &tFar)const;
		//λùϵ
		//SKNOINTERSECT SKNTERSECT
		int RelationWith(const SKPolygon3 &Polygon,SKREAL &fRayParameter ,
						bool bCull,int &iIndexTriangle,SKREAL fTriangleParameter[3])const;
		/*************************************************************************************/
		//߾
		SKREAL SquaredDistance(const SKVector3 &Point,SKREAL &fLineParameter)const;
		//ֱߺ߾
		SKREAL SquaredDistance(const SKLine3 &Line,SKREAL &fRayParameter,SKREAL &fLineParameter)const;
		//ߺ߾
		SKREAL SquaredDistance(const SKRay3 &Ray,SKREAL &fRay1Parameter,SKREAL &fRay2Parameter)const;
		//ߺ߶ξ
		SKREAL SquaredDistance(const SKSegment3 & Segment,SKREAL &fRayParameter,SKREAL &fSegmentParameter)const;
		//ߺξ
		SKREAL SquaredDistance(const SKTriangle3& Triangle,SKREAL &fRayParameter,SKREAL fTriangleParameter[3])const;
		//ߺ;ξ
		SKREAL SquaredDistance(const SKRectangle3& Rectangle,SKREAL &fRayParameter,SKREAL fRectangleParameter[2])const;
		//ߺOBB
		SKREAL SquaredDistance(const SKOBB3 & OBB,SKREAL &fRayParameter,SKREAL fOBBParameter[3])const;
		//ߺľ
		SKREAL Distance(const SKSphere3 &Sphere,SKREAL &fRayParameter,SKVector3 & SpherePoint)const;
		//ߺƽ
		SKREAL Distance(const SKPlane3 & Plane,SKVector3 &RayPoint,SKVector3 &PlanePoint)const;
		//ߺAABB
		SKREAL SquaredDistance(const SKAABB3 &AABB,SKREAL &fRayParameter, SKREAL fAABBParameter[3])const;
		//ߺͶξ
		SKREAL SquaredDistance(const SKPolygon3 &Polygon,SKREAL &fRayParameter,
								int& IndexTriangle,
								SKREAL fTriangleParameter[3])const;
	}; // class
	/*----------------------------------------------------------------*/
#include "SKRay3.inl"

}
#endif