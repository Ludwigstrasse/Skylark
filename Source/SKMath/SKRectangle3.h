#ifndef SKRECTANGLE3_H
#define SKRECTANGLE3_H
#include "SKVector3.h"
#include "SKPlane3.h"
#include "SKMatrix3X3W.h"
/********************************************************************************************


__________________
|        |        |  
|    A[0]|        |      
|		  |        |      
|        |_A[1]___|
|                 |
|                 |
|_________________|


ֶ  Ϊ cross (A[0],A[1]),涨ɼ Ļ⣬ 
********************************************************************************************/
namespace SKEngine2
{
class SKTriangle3;
class SKMATH_API SKRectangle3:public SKPlane3
{
private:
	SKVector3 m_A[2];
	SKREAL m_fA[2];
	SKVector3 m_Center;	
public:	
	SKRectangle3();
	~SKRectangle3();
	SKRectangle3(const SKVector3 & Center,const SKVector3 &A0,const SKVector3 & A1,SKREAL fA0, SKREAL fA1);
	SKRectangle3(const SKVector3 & Center,const SKVector3 A[2],const SKREAL fA[2]);
	bool GetParameter(const SKVector3 &Point,SKREAL fRectangleParameter[2])const;
	void Transform(const SKRectangle3 &Rectangle, 
				const SKMatrix3X3W &Mat);
	/*************************************FORCEINLINE************************************************/
	FORCEINLINE void Set(const SKVector3 & Center,const SKVector3 &A0,const SKVector3 & A1,SKREAL fA0, SKREAL fA1);
	FORCEINLINE void Set(const SKVector3 & Center,const SKVector3 A[2],const SKREAL fA[2]);
	FORCEINLINE void GetA(SKVector3 A[2])const;
	FORCEINLINE void GetfA(SKREAL fA[2])const;
	FORCEINLINE const SKVector3 & GetCenter()const;
	FORCEINLINE void GetPoint(SKVector3 Point[4])const;
	FORCEINLINE SKVector3 GetParameterPoint(SKREAL fRectangleParameter[2])const;
	FORCEINLINE SKVector3 GetParameterPoint(SKREAL fRectangleParameter0,SKREAL fRectangleParameter1)const;
	/*************************************************************************************/
	//;ξ
	SKREAL SquaredDistance(const SKVector3 &Point,SKREAL fRectangleParameter[2])const;

	//κ;ξ
	SKREAL SquaredDistance(const SKRectangle3 &Rectangle,
							SKREAL fRectangle1Parameter[2],SKREAL fRectangle2Parameter[2])const;

	//κ;ξ
	SKREAL SquaredDistance(const SKTriangle3 &Triangle,SKREAL fRectangleParameter[2],SKREAL fTriangleParameter[3])const;

	//ֱߺ;ξ
	SKREAL SquaredDistance(const SKLine3& Line,SKREAL fRectangleParameter[2],SKREAL &fLineParameter)const;
	//ߺ;ξ
	SKREAL SquaredDistance(const SKRay3 &Ray,SKREAL fRectangleParameter[2],SKREAL &fRayParameter)const;
	//߶κ;ξ
	SKREAL SquaredDistance(const SKSegment3 & Segment,SKREAL fRectangleParameter[2],SKREAL &fSegmentParameter)const;
	//κOBB
	SKREAL SquaredDistance(const SKOBB3 &OBB,SKREAL RectangleParameter[2], SKREAL OBBParameter[3])const;
	//κľ
	SKREAL Distance(const SKSphere3 &Sphere,SKREAL fRectangleParameter[2],SKVector3 & SpherePoint)const;
	//κƽ
	SKREAL Distance(const SKPlane3 &Plane,SKVector3 &RectanglePoint,SKVector3 &PlanePoint)const;
	//κAABB
	SKREAL SquaredDistance(const SKAABB3 &AABB ,SKREAL RectangleParameter[2]
							, SKREAL AABBParameter[3])const;

	//κ;ξ
	SKREAL SquaredDistance(const SKPolygon3 &Polygon,
								SKREAL fRectangleParameter[2],
								int& IndexTriangle,
								SKREAL fTriangleParameter[3])const;
	/********************************RelationWith******************************************/
	//ֱλùϵ
	//SKNOINTERSECT SKNTERSECT
	int RelationWith(const SKLine3 &Line,bool bCull,SKREAL fRectangleParameter[2],SKREAL &fLineParameter)const;
	//λùϵ
	//SKNOINTERSECT SKNTERSECT
	int RelationWith(const SKRay3 &Ray,bool bCull,SKREAL fRectangleParameter[2],SKREAL &fRayParameter)const;
	//߶λùϵ
	//SKNOINTERSECT SKNTERSECT
	int RelationWith(const SKSegment3 &Segment,bool bCull,SKREAL fRectangleParameter[2],SKREAL &fSegmentParameter)const;
	//ƽ;λùϵ
	//SKON SKFRONT SKBACK SKINTERSECT
	int RelationWith(const SKPlane3 &Plane)const;
	int RelationWith(const SKPlane3 &Plane,SKSegment3 &Segment)const;
	//κ;λùϵ
	//SKNOINTERSECT SKINTERSECT
	int RelationWith(const SKTriangle3 &Triangle)const;
	int RelationWith(const SKTriangle3 &Triangle,SKSegment3 & Segment)const;
	//κ;λùϵ
	//SKNOINTERSECT SKINTERSECT
	int RelationWith(const SKRectangle3 &Rectangle)const;
	int RelationWith(const SKRectangle3 &Rectangle,SKSegment3 & Segment)const;
	//κAABBλùϵ
	//SKNOINTERSECT SKINTERSECT SKIN
	int RelationWith(const SKAABB3 &AABB)const;

	//κOBBλùϵ
	//SKNOINTERSECT SKINTERSECT SKIN
	int RelationWith(const SKOBB3 &OBB)const;

	//κԲλùϵ
	//SKNOINTERSECT SKINTERSECT SKIN
	int RelationWith(const SKSphere3 & Sphere)const;
};

#include "SKRectangle3.inl"
};
#endif