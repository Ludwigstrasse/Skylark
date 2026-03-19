#ifndef SKAABB3_H
#define SKAABB3_H
#include "SKVector3.h"
#include "SKPlane3.h"
#include "SKRectangle3.h"
/*
AABBԳƱ߽У3A1,A2,A3ԶƽڵǰᣬΪλᣬfA1,fA2,fA3Ϊ᳤ȣڲĵ㶼Աʾ
a * A1 + b * A2 + c * A3 a,b,cΪ|a|<=fA1,|b|<=fA2,|c|<=fA3
*/
namespace SKEngine2
{
	class SKMATH_API SKAABB3
	{
	private:
		static const SKVector3	m_A[3];			// 3				
		SKVector3	m_Center;					// ĵ
		SKREAL		m_fA[3];					// 3ĳ
		SKVector3	m_Max;						//
		SKVector3	m_Min;						//С
	public:
		
		SKAABB3();
		~SKAABB3();

		//ͨС㹹AABB
		SKAABB3(const SKVector3 & Max , const SKVector3 & Min);
		//ͨĵ3İ볤ȹAABB
		SKAABB3(const SKVector3 & Center,SKREAL fA0,SKREAL fA1,SKREAL fA2);
		SKAABB3(const SKVector3 & Center,SKREAL fA[3]);
		//ͨ㼯ϹAABB
		void CreateAABB(const SKVector3 * const pPointArray,unsigned int uiPointNum);
		/*********************************** FORCEINLINE *************************************/
		//ӦĲ
		FORCEINLINE void Set(const SKVector3 & Max , const SKVector3 & Min);
		FORCEINLINE void Set(const SKVector3 & Center,SKREAL fA0,SKREAL fA1,SKREAL fA2);
		FORCEINLINE void Set(const SKVector3 & Center,SKREAL fA[3]);
		//ȡӦĲ
		FORCEINLINE void GetfA(SKREAL fA[3])const;
		FORCEINLINE const SKVector3 & GetCenter()const;
		FORCEINLINE SKVector3 GetParameterPoint(SKREAL fAABBParameter[3])const;
		FORCEINLINE SKVector3 GetParameterPoint(SKREAL fAABBParameter0,SKREAL fAABBParameter1,SKREAL fAABBParameter2)const;
		FORCEINLINE const SKVector3 & GetMaxPoint()const;
		FORCEINLINE const SKVector3 & GetMinPoint()const;
		//õAABB6ƽ
		void GetPlane(SKPlane3 pPlanes[6])const;
		//õAABB8
		void GetPoint(SKVector3 Point[8])const;
		//õAABB6
		void GetRectangle(SKRectangle3 Rectangle[6])const;
		//AABBһ㷵AABBĲ
		bool GetParameter(const SKVector3 &Point,SKREAL fAABBParameter[3])const;
		//þ任AABB
		void Transform(const SKAABB3 &AABB, 
						const SKMatrix3X3W &m,bool bHasProject = false);
		//ϲ2AABB
		SKAABB3 MergAABB(const SKAABB3 &AABB)const;
		void GetQuadAABB(SKAABB3 AABB[4])const;
		void GetOctAABB(SKAABB3 AABB[8])const;
		SKAABB3 GetMin(const SKAABB3 & AABB) const;
		/*************************************************************************************/
		//AABB
		SKREAL SquaredDistance(const SKVector3 & Point,SKREAL fAABBParameter[3])const;
		//ֱߺAABB
		SKREAL SquaredDistance(const SKLine3 & Line, SKREAL fAABBParameter[3],SKREAL &fLineParameter)const;
		//ߺAABB
		SKREAL SquaredDistance(const SKRay3 &Ray,SKREAL fAABBParameter[3],SKREAL &fRayParameter)const;
		//߶κAABB
		SKREAL SquaredDistance(const SKSegment3 & Segment,SKREAL fAABBParameter[3],SKREAL &fSegmentParameter)const;

		//κAABB
		SKREAL SquaredDistance(const SKTriangle3 & Triangle , SKREAL AABBParameter[3],
			SKREAL TriangleParameter[3])const;

		//κAABB
		SKREAL SquaredDistance(const SKRectangle3& Rectangle , SKREAL AABBParameter[3],
			SKREAL RectangleParameter[2])const;
		//AABBAABB
		SKREAL SquaredDistance(const SKAABB3 &AABB , SKREAL AABB1Parameter[3],
								SKREAL AABB2Parameter[3])const;
		//AABBľ
		SKREAL Distance(const SKSphere3 & Sphere,SKREAL fAABBParameter[3],SKVector3 & SpherePoint)const;

		//AABBƽ
		SKREAL Distance(const SKPlane3 &Plane,SKVector3 &AABBPoint,SKVector3 &PlanePoint)const;

		//κAABB
		SKREAL SquaredDistance(const SKPolygon3 &Polygon,SKREAL AABBParameter[3],int& IndexTriangle,
								SKREAL TriangleParameter[3])const;
		/********************************RelationWith******************************************/
		//AABBλùϵ
		//SKIN SKOUT SKON
		int RelationWith(const SKVector3 &Point)const;
		//ֱAABBλùϵ
		//SKNOINTERSECT SKNTERSECT
		int RelationWith(const SKLine3 &Line, unsigned int &Quantity,SKREAL &tNear,SKREAL &tFar)const;
		//AABBλùϵ
		//SKNOINTERSECT SKNTERSECT
		int RelationWith(const SKRay3 &Ray, unsigned int &Quantity,SKREAL &tNear,SKREAL &tFar)const;
		//߶AABBλùϵ
		//SKNOINTERSECT SKNTERSECT SKIN
		int RelationWith(const SKSegment3 &Segment, unsigned int &Quantity,SKREAL &tNear,SKREAL &tFar)const;
		//ƽAABBλùϵ
		//SKFRONT SKBACK SKINTERSECT
		int RelationWith(const SKPlane3 &Plane)const;
		//κAABBλùϵ
		//SKNOINTERSECT SKINTERSECT SKIN
		int RelationWith(const SKTriangle3 &Triangle)const;
		//κAABBλùϵ
		//SKNOINTERSECT SKINTERSECT SKIN
		int RelationWith(const SKRectangle3 &Rectangle)const;

		//AABBAABBλùϵ
		//SKNOINTERSECT SKINTERSECT
		int RelationWith(const SKAABB3 &AABB)const;

		//AABBԲλùϵ
		//SKNOINTERSECT SKINTERSECT
		int RelationWith(const SKSphere3 &Sphere)const;

		bool GetIntersect(SKAABB3& AABB,SKAABB3 & OutAABB)const;
	private:
		/*void Face (int i0, int i1, int i2, SKVector3 Pnt,
			SKVector3 Dir, SKVector3 PmE,
			SKREAL& rfSqrDistance,SKREAL &fLParam)const;
		void CaseNoZeros (SKVector3& Pnt,
			const SKVector3& Dir, SKREAL& rfSqrDistance,SKREAL &fLParam)const;
		void Case0 (int i0, int i1, int i2, SKVector3& Pnt,
			const SKVector3& Dir, SKREAL& rfSqrDistance,SKREAL &fLParam)const;

		void Case00 (int i0, int i1, int i2,
			SKVector3& Pnt, const SKVector3& Dir,  SKREAL& rfSqrDistance,SKREAL &fLParam)const;
		void Case000 (SKVector3& Pnt, SKREAL& rfSqrDistance)const;*/
	};

#include "SKAABB3.inl"
};
#endif