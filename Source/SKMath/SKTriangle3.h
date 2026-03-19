#ifndef SKTRIANGLE3_H
#define SKTRIANGLE3_H
#include "SKVector3.h"
#include "SKPlane3.h"
/***************************************************************************************

				    fTriangleParameter[1]
		   m_V[0]	_________________________m_V[1]
				   \                       /
				    \                     /
				     \                   /
				      \                 /  
				       \               /
					    \             /
fTriangleParameter[2]  \           /  fTriangleParameter[0]
					      \         /
					       \       /
						     \    /
						      \  /
						       \/m_V[2]
***************************************************************************************/
namespace SKEngine2
{
	class SKOBB3;
	class SKMATH_API SKTriangle3:public SKPlane3
	{
	private:	
		SKVector3 m_V[3];
	public:
		
		SKTriangle3();
		~SKTriangle3();
		SKTriangle3(const SKVector3 & P0,const SKVector3 &P1,const SKVector3 &P2);
		SKTriangle3(const SKVector3 Point[3]);
		bool GetParameter(const SKVector3 &Point,SKREAL fTriangleParameter[3])const;
		void Transform(const SKTriangle3 & Triangle,const SKMatrix3X3W &Mat); 
		/*************************************FORCEINLINE************************************************/

		FORCEINLINE void Set(const SKVector3 & P1,const SKVector3 &P2,const SKVector3 &P3);
		FORCEINLINE void Set(const SKVector3 Point[3]);
		FORCEINLINE void GetPoint(SKVector3  Point[3])const;
		FORCEINLINE SKVector3 GetParameterPoint(SKREAL fTriangleParameter[3])const;
		FORCEINLINE SKVector3 GetParameterPoint(SKREAL fTriangleParameter0,
										SKREAL fTriangleParameter1,SKREAL fTriangleParameter2)const;
		/*************************************************************************************/
		//ξ
		SKREAL SquaredDistance(const SKVector3 & Point,SKREAL fTriangleParameter[3])const;
		//κξ
		SKREAL SquaredDistance(const SKTriangle3 &Triangle,
			SKREAL fTriangle1Parameter[3],
			SKREAL fTriangle2Parameter[3])const;
		//κ;ξ
		SKREAL SquaredDistance(const SKRectangle3 &Rectangle,
							SKREAL fTriangleParameter[3],
							SKREAL fRectangleParameter[2])const;
		
		//ֱߺξ
		SKREAL SquaredDistance(const SKLine3 &Line,SKREAL fTriangleParameter[3],SKREAL &fLineParameter)const;
		//ߺξ
		SKREAL SquaredDistance(const SKRay3 & Ray,SKREAL fTriangleParameter[3],SKREAL &fRayParameter)const;
		//߶κξ
		SKREAL SquaredDistance(const SKSegment3& Segment,SKREAL fTriangleParameter[3],SKREAL &fSegmentParameter)const;
		//κOBB
		SKREAL SquaredDistance(const SKOBB3 &OBB,SKREAL TriangleParameter[3], SKREAL OBBParameter[3])const;
		//κľ
		SKREAL Distance(const SKSphere3 &Sphere,SKREAL fTriangleParameter[3],SKVector3 & SpherePoint)const;

		//ƽξ
		SKREAL Distance(const SKPlane3 &Plane,SKVector3 &TrianglePoint,SKVector3 &PlanePoint)const;
		//κAABB
		SKREAL SquaredDistance(const SKAABB3 &AABB ,SKREAL TriangleParameter[3]
								, SKREAL AABBParameter[3])const;
		//κͶξ
		SKREAL SquaredDistance(const SKPolygon3& Polygon,
								SKREAL fTriangle1Parameter[3],
								int& IndexTriangle,
								SKREAL fTriangle2Parameter[3])const;
		/********************************RelationWith******************************************/
		//ֱλùϵ bCullΪǷΪ,Ƿǳ,tཻ
		//SKNOINTERSECT SKNTERSECT
		int RelationWith(const SKLine3 &Line, bool bCull,SKREAL fTriangleParameter[3],
						SKREAL &fLineParameter)const;
		//λùϵ
		//SKNOINTERSECT SKNTERSECT
		int RelationWith(const SKRay3 &Ray, bool bCull,SKREAL fTriangleParameter[3],
						SKREAL &fRayParameter)const;
		//߶λùϵ
		//SKNOINTERSECT SKNTERSECT
		int RelationWith(const SKSegment3 &Segment, bool bCull,SKREAL fTriangleParameter[3],
						SKREAL &fSegmentParameter)const;
		//ƽλùϵ
		//SKON SKFRONT SKBACK SKINTERSECT
		int RelationWith(const SKPlane3 & Plane)const;
		int RelationWith(const SKPlane3 & Plane,SKSegment3 & Segment)const;
		//κλùϵ
		//SKNOINTERSECT SKINTERSECT
		int RelationWith(const SKTriangle3 & Triangle)const;
		int RelationWith(const SKTriangle3 & Triangle,SKSegment3 & Segment)const;
		//κ;λùϵ
		//SKNOINTERSECT SKINTERSECT
		int RelationWith(const SKRectangle3 & Rectangle)const;
		int RelationWith(const SKRectangle3 & Rectangle,SKSegment3 & Segment)const;
		//κAABBλùϵ
		//SKNOINTERSECT SKINTERSECT SKIN
		int RelationWith(const SKAABB3 &AABB)const;

		//κOBBλùϵ
		//SKNOINTERSECT SKINTERSECT SKIN
		int RelationWith(const SKOBB3 &OBB)const;

		//κԲλùϵ
		//SKNOINTERSECT SKINTERSECT SKIN
		int RelationWith(const SKSphere3 &Sphere)const;
	};

#include "SKTriangle3.inl"

};
#endif