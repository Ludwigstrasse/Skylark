#ifndef SKOBB3_H
#define SKOBB3_H
#include "SKVector3.h"
#include "SKTriangle3.h"
#include "SKPlane3.h"
#include "SKMatrix3X3W.h"
/*****************************************************************************************


                               p1  _______________________________ P0
                                  /|                            /|
                                /  |                          /  |
							  /    |m_A[1]  |        m_A[2] /    |
						  p5/______|________|______/______/ p4   |
							|      |        |    /        |      |
							|  p2  |_ _  _ _|  / _ _ _ _ _|_ _ __| p3
							|     /         |/____________|      /
							|	 /	             m_A[0]    |    /      
							|  /                          |  /
						p6	|/____________________________|/ p7



Plane0 right Plane1 left Plane2 front Plane3 back Plane4 up Plane5 down
****************************************************************************************/
namespace SKEngine2
{
class SKMatrix3X3;
class SKLine3;
class SKRay3;
class SKRectangle3;
class SKSegment3;
	class SKMATH_API SKOBB3
	{
	private:
		SKVector3	m_A[3];		// 3
		SKVector3	m_Center;	// ĵ
		SKREAL		m_fA[3];	// 3ĳ
	public:	
		
		
		SKOBB3();
		~SKOBB3();
		//AABB
		SKOBB3(const SKVector3 A[3],SKREAL fA[3],const SKVector3 & Center);
		SKOBB3(const SKVector3 &A0,const SKVector3 &A1,const SKVector3 &A2,
				SKREAL fA0,SKREAL fA1,SKREAL fA2,
				const SKVector3 & Center);
		//ͨ㼯ϹAABB
		void CreateOBB(const SKVector3 * const pPointArray,unsigned int uiPointNum);
		//ϲAABB
		SKOBB3 MergOBB(const SKOBB3 &OBB)const;
		//ȡ6ƽ
		void GetPlane(SKPlane3 pPlanes[6])const;
		//ȡ8
		void GetPoint(SKVector3 Point[8])const;
		//ȡ6
		void GetRectangle(SKRectangle3 Rectangle[6])const;
		//ñ任OBB±任OBBͬһϵ
		void GetTransform(SKMatrix3X3W &m)const;
		//õ
		bool GetParameter(const SKVector3 &Point,SKREAL fOBBParameter[3])const;
		//øOBBͱ任OBB
		void Transform(const SKOBB3 &OBB, 
			const SKMatrix3X3W &Mat);
		SKAABB3 GetAABB()const;
		/*********************************** FORCEINLINE *************************************/
		FORCEINLINE void Set(const SKVector3 A[3],SKREAL fA[3],const SKVector3 & Center);
		FORCEINLINE void Set(const SKVector3 &A0,const SKVector3 &A1,const SKVector3 &A2,
						SKREAL fA0,SKREAL fA1,SKREAL fA2,
						const SKVector3 & Center);
		
		FORCEINLINE void GetA(SKVector3 A[3])const;
		FORCEINLINE void GetfA(SKREAL fA[3])const;
		FORCEINLINE const SKVector3 & GetCenter()const;
		//ݲص
		FORCEINLINE SKVector3 GetParameterPoint(SKREAL fOBBParameter[3])const;
		FORCEINLINE SKVector3 GetParameterPoint(SKREAL fOBBParameter0,SKREAL fOBBParameter1,SKREAL fOBBParameter2)const;
		 
		
		/*************************************************************************************/
		//OBB
		SKREAL SquaredDistance(const SKVector3 & Point,SKREAL fOBBParameter[3])const;
		//ֱߺOBB
		SKREAL SquaredDistance(const SKLine3 & Line, SKREAL fOBBParameter[3],SKREAL &fLineParameter)const;
		//ߺOBB
		SKREAL SquaredDistance(const SKRay3 &Ray,SKREAL fOBBParameter[3],SKREAL &fRayParameter)const;
		//߶κOBB
		SKREAL SquaredDistance(const SKSegment3 & Segment,SKREAL fOBBParameter[3],SKREAL &fSegmentParameter)const;

		//κOBB
		SKREAL SquaredDistance(const SKTriangle3 & Triangle , SKREAL OBBParameter[3],
								SKREAL TriangleParameter[3])const;

		//κOBB
		SKREAL SquaredDistance(const SKRectangle3& Rectangle , SKREAL OBBParameter[3],
								SKREAL RectangleParameter[2])const;
		//OBBOBB
		SKREAL SquaredDistance(const SKOBB3 &OBB , SKREAL OBB1Parameter[3],
								SKREAL OBB2Parameter[3])const;

		//OBBľ
		SKREAL Distance(const SKSphere3 & Sphere,SKREAL fOBBParameter[3],SKVector3 & SpherePoint)const;

		//OBBƽ
		SKREAL Distance(const SKPlane3 &Plane,SKVector3 &OBBPoint,SKVector3 &PlanePoint)const;

		//κOBB
		SKREAL SquaredDistance(const SKPolygon3 &Polygon, 
							SKREAL OBBParameter[3],int& IndexTriangle,
							SKREAL TriangleParameter[3])const;
		/********************************RelationWith******************************************/
		//OBBλùϵ
		//SKIN SKOUT SKON
		int RelationWith(const SKVector3 &Point)const;
		//ֱOBBλùϵ
		//SKNOINTERSECT SKNTERSECT
		int RelationWith(const SKLine3 &Line, unsigned int &Quantity,SKREAL &tNear,SKREAL &tFar)const;
		//OBBλùϵ
		//SKNOINTERSECT SKNTERSECT
		int RelationWith(const SKRay3 &Ray, unsigned int &Quantity,SKREAL &tNear,SKREAL &tFar)const;
		//߶OBBλùϵ
		//SKNOINTERSECT SKNTERSECT SKIN
		int RelationWith(const SKSegment3 &Segment, unsigned int &Quantity,SKREAL &tNear,SKREAL &tFar)const;
		//ƽOBBλùϵ
		//SKFRONT SKBACK SKINTERSECT
		int RelationWith(const SKPlane3 &Plane)const;
		//OBBλùϵ
		//SKNOINTERSECT SKINTERSECT SKIN
		int RelationWith(const SKTriangle3 & Triangle)const;    

		//κOBBλùϵ
		//SKNOINTERSECT SKINTERSECT SKIN
		int RelationWith(const SKRectangle3 &Rectangle)const;

		//OBBOBBλùϵ
		//SKNOINTERSECT SKINTERSECT
		int RelationWith(const SKOBB3 &OBB)const;

		//OBBԲλùϵ
		//SKNOINTERSECT SKINTERSECT
		int RelationWith(const SKSphere3 &Sphere)const;
	private:
		/*void OBBProj(const SKOBB3 &OBB, const SKVector3 &vcV, 
			SKREAL *pfMin, SKREAL *pfMax)const;
		void TriProj(const SKVector3 &v0, const SKVector3 &v1, 
			const SKVector3 &v2, const SKVector3 &vcV, 
			SKREAL *pfMin, SKREAL *pfMax)const;

		void Face (int i0, int i1, int i2, SKVector3 Pnt,
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

#include "SKOBB3.inl"
};
#endif