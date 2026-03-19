#ifndef SKMATRIX3X3_H
#define SKMATRIX3X3_H
#include "SKVector3.h"
#include "SKQuat.h"
namespace SKEngine2
{
	//
	class  SKMATH_API SKMatrix3X3 
	{
	public:
		union
		{
			SKREAL m[9];
			struct
			{
				SKREAL _00, _01, _02;
				SKREAL _10, _11, _12;
				SKREAL _20, _21, _22;
			};
			SKREAL M[3][3];
		};

		SKMatrix3X3(void);
		SKMatrix3X3(SKREAL m00,SKREAL m01,SKREAL m02,
					SKREAL m10,SKREAL m11,SKREAL m12,
					SKREAL m20,SKREAL m21,SKREAL m22);
		

		void CreateFromDirection(SKVector3 & Direction , const SKVector3 &Up = SKVector3(0,1,0));
		void CreateRotX(SKREAL a);								// xת
		void CreateRotY(SKREAL a);								// yת
		void CreateRotZ(SKREAL a);								// zת
		void CreateEluer(SKREAL Roll,SKREAL Pitch, SKREAL Yaw);		// z x and yŷ  -pi <= y z <= pi  -pi/2 < x < pi/2
		void CreateAxisAngle(const SKVector3 &vAxis, SKREAL a);	//vAxisתa	
		//ת
		void CreateRot(const SKVector3 &U,const SKVector3 &V,const SKVector3 & N);
		//ž
		void CreateScale(SKREAL fX,SKREAL fY,SKREAL fZ);

		void CreateScale(const SKVector3 & Axis,SKREAL fScale);
		//õeluerǶ
		void GetEluer(SKREAL &Yaw,SKREAL &Pitch,SKREAL &Roll)const;
		//õתת
		void GetAxisAngle(SKVector3 & Axis,SKREAL & fAngle)const;
		//õԪ
		SKQuat GetQuat()const;
		//Բֵ
		void LineInterpolation(SKREAL t,const SKMatrix3X3 & M1, const SKMatrix3X3 &M2);
		//βֵ
		void Slerp(SKREAL t,const SKMatrix3X3 & M1, const SKMatrix3X3 &M2);
		SKMatrix3X3 operator *(SKREAL f)const;
		SKMatrix3X3 operator +(SKREAL f)const;
		SKMatrix3X3 operator -(SKREAL f)const;
		SKMatrix3X3 operator +(const SKMatrix3X3 &Matirx)const;
		SKMatrix3X3 operator -(const SKMatrix3X3 &Matirx)const;

		void operator *= (SKREAL f);
		void operator += (SKREAL f);
		void operator -= (SKREAL f);
		void operator += (const SKMatrix3X3 &Matirx);
		void operator -= (const SKMatrix3X3 &Matirx);

		bool operator ==(const SKMatrix3X3 &v)const;
		//л
		void GetRowVector(SKVector3 Row[3])const;
		//е
		void GetColumnVector(SKVector3 Column[3])const;

		void GetRowVector(SKVector3 &Row0,SKVector3 &Row1,SKVector3 &Row2)const;
		void GetColumnVector(SKVector3 &Column0,SKVector3 &Column1,SKVector3 &Column2)const;
		//UVN
		void GetUVN(SKVector3 UVN[3])const;
		void GetUVN(SKVector3 & U,SKVector3 &V,SKVector3 &N)const;
		//ֵ
		void GetEigenSystem(SKREAL EigenValue[3],SKVector3 Eigen[3])const;
		/*********************************** FORCEINLINE *************************************/
		FORCEINLINE void Identity(void);											//λ
		FORCEINLINE void TransposeOf(const SKMatrix3X3 &Matirx);					//ת
		FORCEINLINE void InverseOf(const SKMatrix3X3 &Matirx);					//
		FORCEINLINE SKREAL Det()const;											//бʽ
		FORCEINLINE SKMatrix3X3 operator * (const SKMatrix3X3 &Matirx)const;		// 
		FORCEINLINE SKVector3 operator * (const SKVector3 &vc)const;				// 

		//һ һ
		FORCEINLINE void CreateFromTwoVector(const SKVector3 & v1,const SKVector3 & v2);
		FORCEINLINE void SetZero();
		void GetScale(SKVector3 & Scale)const;
		void GetScaleAndRotated(SKVector3 & Scale);
		const static SKMatrix3X3 ms_CameraViewRight;
		const static SKMatrix3X3 ms_CameraViewLeft;
		const static SKMatrix3X3 ms_CameraViewUp;
		const static SKMatrix3X3 ms_CameraViewDown;
		const static SKMatrix3X3 ms_CameraViewFront;
		const static SKMatrix3X3 ms_CameraViewBack;
		const static SKMatrix3X3 ms_Identity;

	}; // class

#include "SKMatrix3X3.inl"
};
#endif