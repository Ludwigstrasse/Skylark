#ifndef SKPLANE3_H
#define SKPLANE3_H
#include "SKVector3.h"
#include "SKMatrix3X3.h"

namespace SKEngine2
{
	class SKLine3;
	class SKPlane3;
	class SKSphere3;
	class SKCylinder3;
	class  SKMATH_API SKPlane3 
	{
	protected:
		SKVector3	m_N;       //ƽ淨
		SKREAL		m_fD;       // ԭľ(ax+by+cz+d=0)
	public:
		SKPlane3();
		SKPlane3(const SKVector3 &N, const SKVector3 &P);
		SKPlane3(const SKVector3 &P0,  const SKVector3 &P1, const SKVector3 &P2);
		SKPlane3(const SKVector3 Point[3]);
		SKPlane3(const SKVector3 &N , SKREAL fD);
		void Transform(const SKPlane3 &Plane,const SKMatrix3X3W &Mat);
		/*********************************** FORCEINLINE *************************************/
		FORCEINLINE const SKVector3 & GetN()const;
		FORCEINLINE SKVector3 GetPoint()const;
		FORCEINLINE SKREAL GetfD()const;
		FORCEINLINE void  Set(const SKVector3 &N, const SKVector3 &P);
		FORCEINLINE void  Set(const SKVector3 &N , SKREAL fD);
		FORCEINLINE void  Set(const SKVector3 &P0,  const SKVector3 &P1, const SKVector3 &P2);
		FORCEINLINE void  Set(const SKVector3 Point[3]);
		FORCEINLINE SKPlane3 GetPlane()const;
		SKVector3 ReflectDir(const SKVector3 & Dir)const;
		/*************************************************************************************/
		//㵽ƽ
		SKREAL Distance(const SKVector3 &Point,SKVector3 &PlanePoint)const;
		//ƽľ
		SKREAL Distance(const SKSphere3 &Sphere,SKVector3 & SpherePoint)const;
		//ֱߺƽ
		SKREAL Distance(const SKLine3 &Line,SKVector3 &PlanePoint,SKVector3 &LinePoint)const;
		//ߺƽ
		SKREAL Distance(const SKRay3 & Ray,SKVector3 &PlanePoint,SKVector3 &RayPoint)const;
		//߶κƽ
		SKREAL Distance(const SKSegment3 & Segment,SKVector3 &PlanePoint,SKVector3 &SegmentPoint)const;

		//뺯ûпཻ¼ģΪཻ¾Ϊ0RelationWithжλùϵ
		//ƽƽ
		SKREAL Distance(const SKPlane3 &Plane,SKVector3 &Plane1Point,SKVector3 &Plane2Point)const;
		//ƽξ
		SKREAL Distance(const SKTriangle3 &Triangle,SKVector3 &PlanePoint,SKVector3 &TrianglePoint)const;
		//κƽ
		SKREAL Distance(const SKRectangle3 &Rectangle,SKVector3 &PlanePoint,SKVector3 &RectanglePoint)const;
		//OBBƽ
		SKREAL Distance(const SKOBB3& OBB,SKVector3 &PlanePoint,SKVector3 &OBBPoint)const;
		//AABBƽ
		SKREAL Distance(const SKAABB3 &AABB,SKVector3 &PlanePoint,SKVector3 &AABBPoint)const;
		//ƽͶξ
		SKREAL Distance(const SKPolygon3 &Polygon,SKVector3 &PlanePoint,int& IndexTriangle,
						SKVector3 &TrianglePoint)const;
		/********************************RelationWith******************************************/
		//ƽλùϵ(SKFRONT SKBACK SKPLANAR)
		int RelationWith(const SKVector3 &Point)const;
		//ֱƽλùϵ
		//SKNOINTERSECT SKNTERSECT SKON SKBACK SKFRONT
		int RelationWith(const SKLine3 &Line, bool bCull,SKREAL &fLineParameter)const;
		//ƽλùϵ
		//SKNOINTERSECT SKNTERSECT SKON SKBACK SKFRONT
		int RelationWith(const SKRay3 &Ray, bool bCull,SKREAL &fRayParameter)const;
		//߶ƽλùϵ
		//SKNOINTERSECT SKNTERSECT SKON SKBACK SKFRONT
		int RelationWith(const SKSegment3 &Segment, bool bCull,SKREAL &fSegmentParameter)const;
		//ƽOBBλùϵ
		//SKFRONT SKBACK SKINTERSECT
		int RelationWith(const SKOBB3 &OBB)const;
		//ƽAABBλùϵ
		//SKFRONT SKBACK SKINTERSECT
		int RelationWith(const SKAABB3 &AABB)const;
		//ƽλùϵ
		//SKFRONT SKBACK SKINTERSECT
		int RelationWith(const SKSphere3 &Sphere)const;
		//ƽλùϵ
		//SKON SKFRONT SKBACK SKINTERSECT
		int RelationWith(const SKTriangle3 &Triangle)const;
		int RelationWith(const SKTriangle3 &Triangle ,SKSegment3 & Segment)const;
		//Ϊƽƽλùϵ
		//SKNOINTERSECT SKINTERSECT 
		int RelationWith(const SKPlane3 &Plane)const;
		int RelationWith(const SKPlane3 &Plane,SKLine3 &Line)const;
		//ƽ;λùϵ
		//SKON SKFRONT SKBACK SKINTERSECT
		int RelationWith(const SKRectangle3 & Rectangle)const;		
		int RelationWith(const SKRectangle3 &Rectangle,SKSegment3 &Segment)const;

		//ƽͶλùϵ
		//SKON SKFRONT SKBACK SKINTERSECT
		int RelationWith(const SKPolygon3 &Polygon)const;
		int RelationWith(const SKPolygon3 &Polygon,SKSegment3 & Segment)const;

		//ƽԲλùϵ
		int RelationWith(const SKCylinder3 &Cylinder3)const;

	}; // class
	/*----------------------------------------------------------------*/
#include "SKPlane3.inl"

};
#endif