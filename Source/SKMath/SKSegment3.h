#ifndef SKSEGMENT3_H
#define SKSEGMENT3_H
#include "SKRay3.h"
#include "SKMatrix3X3W.h"
namespace SKEngine2
{
	class SKMATH_API SKSegment3:public SKRay3
	{
	private:
		SKVector3	m_End;
		SKREAL		m_fLen;
	public:
		SKSegment3();
		~SKSegment3();
		SKSegment3(const SKVector3 &Orig,const SKVector3 &End);
		SKSegment3(const SKVector3 &Orig,const SKVector3 &Dir,SKREAL fLen);
		bool GetParameter(const SKVector3 &Point,SKREAL &fSegmentParameter )const;
		//þ任
		void Transform(const SKSegment3 & Segment,const SKMatrix3X3W &Mat); 
		/*************************************FORCEINLINE************************************************/
		FORCEINLINE const SKVector3 & GetEnd()const;
		FORCEINLINE SKREAL GetLen()const;
		FORCEINLINE void Set(const SKVector3 &Orig,const SKVector3 &End);
		FORCEINLINE void Set(const SKVector3 &Orig,const SKVector3 &Dir,SKREAL fLen);
		FORCEINLINE SKVector3 GetParameterPoint(SKREAL fSegmentParameter)const;
	
		
		/********************************RelationWith******************************************/
		//߶λùϵ bCullΪǷΪ,Ƿǳ,tཻ
		//SKNOINTERSECT SKNTERSECT
		int RelationWith(const SKTriangle3 & Triangle, bool bCull,SKREAL &fSegmentParameter,
						SKREAL fTriangleParameter[3])const;
		//߶ƽλùϵ
		//SKNOINTERSECT SKNTERSECT SKON SKBACK SKFRONT
		int RelationWith(const SKPlane3 &Plane, bool bCull,SKREAL &fSegmentParameter)const;
		//߶λùϵ
		//SKNOINTERSECT SKNTERSECT
		int RelationWith(const SKRectangle3 &Rectangle,bool bCull,SKREAL &fSegmentParameter,
						SKREAL fRectangleParameter[2])const;



		//߶OBBλùϵ
		//SKNOINTERSECT SKNTERSECT SKIN
		int RelationWith(const SKOBB3 &OBB, unsigned int &Quantity,SKREAL &tNear,SKREAL &tFar)const;
		//߶AABBλùϵ
		//SKNOINTERSECT SKNTERSECT SKIN
		int RelationWith(const SKAABB3 &AABB, unsigned int &Quantity,SKREAL &tNear,SKREAL &tFar)const;
		//߶λùϵ
		//SKNOINTERSECT SKNTERSECT SKIN
		int RelationWith(const SKSphere3 &sphere, unsigned int &Quantity,SKREAL &tNear,SKREAL &tFar)const;
		//߶λùϵ
		//SKNOINTERSECT SKNTERSECT
		int RelationWith(const SKPolygon3 &Polygon,SKREAL &fSegmentParameter,
						bool bCull,int &iIndexTriangle,SKREAL fTriangleParameter[3])const;

		/*************************************************************************************/
		//߶ξ
		SKREAL SquaredDistance(const SKVector3 &Point,SKREAL &fSegmentParameter)const;
		//ֱߺ߶ξ
		SKREAL SquaredDistance(const SKLine3 &Line,SKREAL &fSegmentParameter,SKREAL &fLineParameter)const;
		//ߺ߶ξ
		SKREAL SquaredDistance(const SKRay3 &Ray,SKREAL &fSegmentParameter,SKREAL &fRayParameter)const;
		//߶κ߶ξ
		SKREAL SquaredDistance(const SKSegment3 & Segment,SKREAL &fSegment1Parameter,SKREAL &fSegment2Parameter)const;
		//߶κξ
		SKREAL SquaredDistance(const SKTriangle3& Triangle,SKREAL &fSegmentParameter,SKREAL fTriangleParameter[3])const;
		//߶κ;ξ
		SKREAL SquaredDistance(const SKRectangle3& Rectangle,SKREAL &fSegmentParameter,SKREAL fRectangleParameter[2])const;
		//߶κOBB
		SKREAL SquaredDistance(const SKOBB3 & OBB,SKREAL &fSegmentParameter,SKREAL fOBBParameter[3])const;

		//߶κľ
		SKREAL Distance(const SKSphere3 &Sphere,SKREAL &fSegmentParameter,SKVector3 & SpherePoint)const;

		//߶κƽ
		SKREAL Distance(const SKPlane3 & Plane,SKVector3 &SegmentPoint,SKVector3 &PlanePoint)const;

		//߶κAABB
		SKREAL SquaredDistance(const SKAABB3 &AABB,SKREAL &fSegmentParameter,SKREAL fAABBParameter[3])const;

		//߶κͶξ
		SKREAL SquaredDistance(const SKPolygon3 &Polygon,SKREAL &fSegmentParameter,
								int& IndexTriangle,
								SKREAL fTriangleParameter[3])const;
	};

#include "SKSegment3.inl"
};
#endif