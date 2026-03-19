#ifndef SKPOLYGON3_H
#define SKPOLYGON3_H
#include"SKPlane3.h"
#include "SKMemManager.h"
namespace SKEngine2
{
	class SKMATH_API SKPolygon3 : public SKPlane3 , public SKMemObject
	{
	private:

		unsigned int	m_PointNum;		//
		unsigned int	m_IndexNum;		//
		bool			m_Flag;			// ־λ
		SKVector3		*m_pPoint;		// б
		SKUSHORT_INDEX	*m_pIndex;		// б
	public:
		SKPolygon3();
		~SKPolygon3();
	
		SKPolygon3(const SKVector3 * pPoint,unsigned int PointNum);
		//
		bool CopyFrom(const SKPolygon3 & Polygon);
		//ת
		void SwapFaces();
		/*********************************** FORCEINLINE *************************************/
		FORCEINLINE void Set(const SKVector3 * pPoint,unsigned int PointNum);
		FORCEINLINE unsigned int GetPointNum()const;
		FORCEINLINE unsigned int GetIndexNum()const;
		FORCEINLINE const SKVector3 * const GetpPoint()const;
		FORCEINLINE const SKUSHORT_INDEX * const GetpIndex()const;
		FORCEINLINE unsigned int GetFlag()const;
		/*************************************************************************************/
		//Ͷξ
		SKREAL SquaredDistance(const SKVector3 & Point,int& IndexTriangle,SKREAL fTriangleParameter[3])const;
		//κͶξ
		SKREAL SquaredDistance(const SKTriangle3 &Triangle,
								int& IndexTriangle,
								SKREAL fTriangle1Parameter[3],
								SKREAL fTriangle2Parameter[3])const;
		//κ;ξ
		SKREAL SquaredDistance(const SKRectangle3 &Rectangle,
								int& IndexTriangle,
								SKREAL fTriangleParameter[3],
								SKREAL fRectangleParameter[2])const;

		//ֱߺͶξ
		SKREAL SquaredDistance(const SKLine3 &Line,int& IndexTriangle,
								SKREAL fTriangleParameter[3],SKREAL &fLineParameter)const;
		//ߺͶξ
		SKREAL SquaredDistance(const SKRay3 & Ray,int& IndexTriangle,
								SKREAL fTriangleParameter[3],SKREAL &fRayParameter)const;
		//߶κͶξ
		SKREAL SquaredDistance(const SKSegment3& Segment,int& IndexTriangle,
								SKREAL fTriangleParameter[3],SKREAL &fSegmentParameter)const;
		//κOBB
		SKREAL SquaredDistance(const SKOBB3 &OBB,int& IndexTriangle,
								SKREAL TriangleParameter[3], SKREAL OBBParameter[3])const;
		//κľ
		SKREAL Distance(const SKSphere3 &Sphere,int& IndexTriangle,
						SKREAL fTriangleParameter[3],SKVector3 & SpherePoint)const;

		//ƽͶξ
		SKREAL Distance(const SKPlane3 &Plane,int& IndexTriangle,
						SKVector3 &TrianglePoint,SKVector3 &PlanePoint)const;
		//κAABB
		SKREAL SquaredDistance(const SKAABB3 &AABB ,int& IndexTriangle,
								SKREAL TriangleParameter[3]
								, SKREAL AABBParameter[3])const;

		//κͶξ
		SKREAL SquaredDistance(const SKPolygon3 & Polygon ,int& Index1Triangle,
								SKREAL Triangle1Parameter[3],int& Index2Triangle,
								SKREAL Triangle2Parameter[3])const;
	/********************************RelationWith******************************************/
		//ֱλùϵ bCullΪǷΪ,Ƿǳ,tཻ
		//SKNOINTERSECT SKNTERSECT
		int RelationWith(const SKLine3 &Line, bool bCull,int &iIndexTriangle,SKREAL fTriangleParameter[3],
						SKREAL &fLineParameter)const;
		//λùϵ
		//SKNOINTERSECT SKNTERSECT
		int RelationWith(const SKRay3 &Ray, bool bCull,int &iIndexTriangle,SKREAL fTriangleParameter[3],
						SKREAL &fRayParameter)const;
		//߶λùϵ
		//SKNOINTERSECT SKNTERSECT
		int RelationWith(const SKSegment3 &Segment, bool bCull,int &iIndexTriangle,SKREAL fTriangleParameter[3],
						SKREAL &fSegmentParameter)const;
		//ƽͶλùϵ
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
		/*************************************** Clip *********************************/
		//ƽж
		void	Clip(const SKPlane3 &Plane, 
							SKPolygon3 &pFront,
							SKPolygon3 &pBack)const;
		//AABBжʺAABB
		void	Clip(const SKAABB3 &ABBB);
	};
	#include "SKPolygon3.inl"
}
#endif