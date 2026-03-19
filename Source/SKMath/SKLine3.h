#ifndef SKLINE3_H
#define SKLINE3_H
#include "SKMath.h"
#include "SKPlane3.h"
#include "SKOBB3.h"
#include "SKSphere3.h"
#include "SKTriangle3.h"
#include "SKRectangle3.h"
/*
	ֱ


*/
namespace SKEngine2
{
	class SKRay3;
	class SKSegment3;
	class SKMATH_API SKLine3
	{
	protected:
		SKVector3	m_Orig;  // Դ
		SKVector3	m_Dir;   // 
	public:
		SKLine3();
		SKLine3(const SKVector3 & Orig,const SKVector3 &Dir);
		~SKLine3();
		void Transform(const SKLine3 &Line,const SKMatrix3X3W &Mat);
		bool GetParameter(const SKVector3 &Point,SKREAL &fLineParameter )const;
		/************************FORCEINLINE***************************************/
		FORCEINLINE void Set(const SKVector3 & Orig,const SKVector3 &Dir);
		FORCEINLINE const SKVector3 & GetOrig()const;
		FORCEINLINE const SKVector3 & GetDir()const;

		FORCEINLINE SKVector3 GetParameterPoint(SKREAL fLineParameter)const;	
		
		
		/********************************RelationWith******************************************/
		//ֱλùϵ bCullΪǷΪ,Ƿǳ,tཻ
		//SKNOINTERSECT SKNTERSECT
		int RelationWith(const SKTriangle3 & Triangle, bool bCull,SKREAL &fLineParameter,
						SKREAL fTriangleParameter[3])const;
		//ֱƽλùϵ
		//SKNOINTERSECT SKNTERSECT SKON SKBACK SKFRONT
		int RelationWith(const SKPlane3 &Plane, bool bCull,SKREAL &fLineParameter)const;
		//ֱλùϵ
		//SKNOINTERSECT SKNTERSECT
		int RelationWith(const SKRectangle3 &Rectangle,bool bCull,SKREAL &fLineParameter,
						SKREAL fRectangleParameter[2])const;
		//ֱλùϵ
		//SKNOINTERSECT SKNTERSECT
		int RelationWith(const SKSphere3 &sphere, unsigned int &Quantity,SKREAL &tNear,SKREAL &tFar)const;
		//ֱOBBλùϵ
		//SKNOINTERSECT SKNTERSECT
		int RelationWith(const SKOBB3 &OBB, unsigned int &Quantity,SKREAL &tNear,SKREAL &tFar)const;
		

		//ֱAABBλùϵ
		//SKNOINTERSECT SKNTERSECT
		int RelationWith(const SKAABB3 &AABB, unsigned int &Quantity,SKREAL &tNear,SKREAL &tFar)const;

		//ֱλùϵ bCullΪǷΪ,Ƿǳ,tཻ
		//SKNOINTERSECT SKNTERSECT
		int RelationWith(const SKPolygon3 &Polygon,SKREAL &fLineParameter, 
						bool bCull,int &iIndexTriangle,SKREAL fTriangleParameter[3])const;
		/*************************************************************************************/

		//㵽ֱ߾
		SKREAL SquaredDistance(const SKVector3 &Point,SKREAL &fLineParameter)const;
		//ֱߺֱߵľ
		SKREAL SquaredDistance(const SKLine3 &Line,SKREAL &fLine1Parameter,SKREAL &fLine2Parameter)const;
		//ֱߺ߾
		SKREAL SquaredDistance(const SKRay3 &Ray,SKREAL &fLineParameter,SKREAL &fRayParameter)const;
		//ֱߺ߶ξ
		SKREAL SquaredDistance(const SKSegment3 & Segment,SKREAL &fLineParameter,SKREAL &fSegmentParameter)const;
		//ֱߺξ
		SKREAL SquaredDistance(const SKTriangle3& Triangle,SKREAL &fLineParameter,SKREAL fTriangleParameter[3])const;
		//ֱߺ;ξ
		SKREAL SquaredDistance(const SKRectangle3& Rectangle,SKREAL &fLineParameter,SKREAL fRectangleParameter[2])const;
		//ֱߺOBB
		SKREAL SquaredDistance(const SKOBB3 & OBB,SKREAL &fLineParameter,SKREAL fOBBParameter[3])const;
		//ֱߺľ
		SKREAL Distance(const SKSphere3 &Sphere,SKREAL &fLineParameter,SKVector3 & SpherePoint)const;
		//ֱߺƽ
		SKREAL Distance(const SKPlane3 &Plane,SKVector3 &LinePoint,SKVector3 &PlanePoint)const;
		//ֱߺAABB
		SKREAL SquaredDistance(const SKAABB3 &AABB,SKREAL &fLineParameter, SKREAL fAABBParameter[3])const;

		//ֱߺͶξ
		SKREAL SquaredDistance(const SKPolygon3 & Polygon,SKREAL &fLineParameter,
								int& IndexTriangle,
								SKREAL fTriangleParameter[3])const;
	};

#include "SKLine3.inl"
};
#endif