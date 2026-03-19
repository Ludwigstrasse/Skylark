#ifndef SKSPHERE3_H
#define SKSPHERE3_H
#include "SKVector3.h"
namespace SKEngine2
{
	class SKMATH_API SKSphere3
	{
	public:
		SKVector3 m_Center;		//
		SKREAL m_fRadius;		//뾶
		SKSphere3();
		~SKSphere3();
		SKSphere3(const SKVector3 & Center, SKREAL fRadius);
		//Χ
		void CreateSphere(const SKVector3 *pPointArray,unsigned int uiPointNum);
		//ϰΧ
		SKSphere3 MergSpheres(const SKSphere3 &Sphere)const;
		//任
		void Transform(const SKSphere3 & Sphere,const SKMatrix3X3W &Mat); 
		SKAABB3 GetAABB()const;
		/*************************************FORCEINLINE************************************************/
		FORCEINLINE void Set(const SKVector3 & Center, SKREAL fRadius);
		
		/*************************************************************************************/
		//ľ
		SKREAL Distance(const SKVector3 & Point,SKVector3 & SpherePoint)const;
		//ߺľ
		SKREAL Distance(const SKLine3 & Line,SKVector3 & SpherePoint,SKREAL &fLineParameter)const;
		//ߺľ
		SKREAL Distance(const SKRay3 & Ray,SKVector3 & SpherePoint,SKREAL &fRayParameter)const;
		//߶κľ
		SKREAL Distance(const SKSegment3 & Segment,SKVector3 & SpherePoint,SKREAL &fSegmentParameter)const;
		//OBBľ
		SKREAL Distance(const SKOBB3 &OBB,SKVector3 & SpherePoint,SKREAL fOBBParameter[3])const;
		//ƽľ
		SKREAL Distance(const SKPlane3 & Plane,SKVector3 & SpherePoint)const;
		//κľ
		SKREAL Distance(const SKRectangle3 & Rectangle,SKVector3 & SpherePoint,SKREAL fRectangleParameter[2])const;
		//κľ
		SKREAL Distance(const SKTriangle3 Triangle,SKVector3 & SpherePoint,SKREAL fTriangleParameter[3])const;
		//AABBľ
		SKREAL Distance(const SKAABB3 &AABB,SKVector3 & SpherePoint,SKREAL fAABBParameter[3])const;

		//κľ
		SKREAL Distance(const SKPolygon3 &Polygon,SKVector3 & SpherePoint,int& IndexTriangle,
						SKREAL fTriangleParameter[3])const;
		/********************************RelationWith******************************************/
		//λùϵ
		//SKIN SKOUT SKON	
		int RelationWith(const SKVector3 &Point)const;
		//ֱλùϵ
		//SKNOINTERSECT SKNTERSECT
		int RelationWith(const SKLine3 &Line, unsigned int &Quantity,SKREAL &tNear,SKREAL &tFar)const;
		//λùϵ
		//SKNOINTERSECT SKNTERSECT
		int RelationWith(const SKRay3 &Ray, unsigned int &Quantity,SKREAL &tNear,SKREAL &tFar)const;
		//߶λùϵ
		//SKNOINTERSECT SKNTERSECT SKIN
		int RelationWith(const SKSegment3 &Segment, unsigned int &Quantity,SKREAL &tNear,SKREAL &tFar)const;
		//ƽλùϵ
		//SKFRONT SKBACK SKINTERSECT
		int RelationWith(const SKPlane3 &Plane)const;
		//κԲλùϵ
		//SKNOINTERSECT SKINTERSECT SKIN
		int RelationWith(const SKTriangle3 Triangle)const;
		//κԲλùϵ
		//SKNOINTERSECT SKINTERSECT SKIN
		int RelationWith(const SKRectangle3 &Rectangle)const;

		//OBBԲλùϵ
		//SKNOINTERSECT SKINTERSECT
		int RelationWith(const SKOBB3 &OBB)const;
		//԰Բλùϵ
		//SKNOINTERSECT SKINTERSECT
		int RelationWith(const SKSphere3 &Sphere)const;
	};

#include "SKSphere3.inl"
};
#endif