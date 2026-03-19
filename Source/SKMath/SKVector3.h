#ifndef SKVECTOR3_H
#define SKVECTOR3_H
#include "SKMath.h"
#include "SKFastFunction.h"
namespace SKEngine2
{
	class SKMatrix3X3;
	class SKQuat;
	class SKRay3;
	class SKLine3;
	class SKPlane3;
	class SKRay3;
	class SKSegment3;
	class SKTriangle3;
	class SKRectangle3;
	class SKOBB3;
	class SKSphere3;
	class SKAABB3;
	class SKMatrix3X3W;
	class SKPolygon3;
	//
	class  SKMATH_API SKVector3 
	{
	public:
		union
		{
			SKREAL m[3];
			struct  
			{
				SKREAL x, y, z;
			};
		};


		SKVector3(void);
		SKVector3(SKREAL _x, SKREAL _y, SKREAL _z);

		/*************************************FORCEINLINE************************************************/
		FORCEINLINE void  Set(SKREAL _x, SKREAL _y, SKREAL _z);
		FORCEINLINE SKREAL GetLength(void)const;                   // 
		FORCEINLINE SKREAL GetSqrLength(void) const;          // ƽ
		FORCEINLINE void  Negate(void);                      // -1
		FORCEINLINE void  Normalize(void);                   // λ
		FORCEINLINE void Cross(const SKVector3 &v1,          //
						const SKVector3 &v2); 

		SKREAL AngleWith( SKVector3 &v);          // ļн()
		void  Create(const SKVector3 &v1,			// ӵv1v2
							const SKVector3 &v2);


		/*            N     _
				 \	  /|\   /|
			Dir	  \	   |   /  Reflect		 
		          _\|  |  /
		--------------------------
		*/
		SKVector3 ReflectDir(const SKVector3 &N)const;

		SKVector3 & operator += (const SKVector3 &v);
		SKVector3 & operator -= (const SKVector3 &v);

		SKVector3 & operator *= (SKREAL f);
		SKVector3 & operator /= (SKREAL f);
		SKVector3 & operator += (SKREAL f);
		SKVector3 & operator -= (SKREAL f);

		
		SKREAL Dot(const SKVector3 &v)const;//
		bool operator ==(const SKVector3 &v)const;


		SKVector3 operator * (SKREAL f)const;            
		SKVector3 operator / (SKREAL f)const;            
		SKVector3 operator + (SKREAL f)const;            
		SKVector3 operator - (SKREAL f)const;            

		SKQuat   operator * (const SKQuat   &q)const; 
		SKVector3 operator * (const SKMatrix3X3 &m)const; 
		SKVector3 operator * (const SKMatrix3X3W &m)const;
		SKVector3 operator + (const SKVector3 &v)const; 
		SKVector3 operator - (const SKVector3 &v)const; 
		SKVector3 operator / (const SKVector3 &v)const;
		SKVector3 operator * (const SKVector3 &v)const;

		SKVector3 & operator /= (const SKVector3 &v);
		SKVector3 & operator *= (const SKVector3 &v);
		bool IsParallel(const SKVector3 & Vector)const;
		/**************************************************************************/
		//͵
		SKREAL SquaredDistance(const SKVector3 &Point)const;
		//㵽ֱ߾
		SKREAL SquaredDistance(const SKLine3 & Line,SKREAL &fLineParameter)const;
		//߾
		SKREAL SquaredDistance(const SKRay3 & Ray,SKREAL &fRayParameter)const;
		//߶ξ
		SKREAL SquaredDistance(const SKSegment3 & Segment,SKREAL &fSegmentParameter)const;
		//ξ
		SKREAL SquaredDistance(const SKTriangle3 &Triangle,SKREAL fTriangleParameter[3])const;
		//;ξ
		SKREAL SquaredDistance(const SKRectangle3 &Rectangle,SKREAL fRectangleParameter[2])const;
		//OBB
		SKREAL SquaredDistance(const SKOBB3 &OBB,SKREAL fOBBParameter[3])const;
		//ľ
		SKREAL Distance(const SKSphere3& Sphere,SKVector3 & SpherePoint)const;
		//ƽ
		SKREAL Distance(const SKPlane3 & Plane,SKVector3 &PlanePoint)const;
		//AABB
		SKREAL SquaredDistance(const SKAABB3 &AABB,SKREAL fAABBParameter[3])const;
		//Ͷξ
		SKREAL SquaredDistance(const SKPolygon3 & Polygon , int& IndexTriangle,SKREAL fTriangleParameter[3])const;
		/********************************RelationWith******************************************/
		//λùϵ
		//SKIN SKOUT SKON	
		int RelationWith(const SKSphere3 & Sphere)const;
		//ƽλùϵ
		//SKFRONT SKBACK SKON
		int RelationWith(const SKPlane3 &Plane)const;
		//OBBλùϵ
		//SKIN SKOUT SKON
		int RelationWith(const SKOBB3 &OBB)const; 
		//AABBλùϵ
		//SKIN SKOUT SKON
		int RelationWith(const SKAABB3 &AABB)const; 
		const static SKVector3 ms_Up;
		const static SKVector3 ms_Right;
		const static SKVector3 ms_Front;
		const static SKVector3 ms_Zero;
		const static SKVector3 ms_One;
	};  // class
	#include "SKVector3.inl"
}
#endif