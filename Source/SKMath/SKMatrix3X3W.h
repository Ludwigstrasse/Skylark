#ifndef SKMATRIX3X3W_H
#define SKMATRIX3X3W_H
#include "SKVector3.h"
#include "SKVector3W.h"
#include "SKMatrix3X3.h"
#include "SKFastFunction.h"
//#include "SKPlane3.h"
namespace SKEngine2
{
	//
	class  SKMATH_API SKMatrix3X3W 
	{
	public:
		union
		{
			SKREAL m[16];
			struct
			{
				SKREAL _00, _01, _02, _03;
				SKREAL _10, _11, _12, _13;
				SKREAL _20, _21, _22, _23;
				SKREAL _30, _31, _32, _33;
			};
			SKREAL M[4][4];
		};

		SKMatrix3X3W(void);
		SKMatrix3X3W(SKREAL m00,SKREAL m01,SKREAL m02,SKREAL m03,
					SKREAL m10,SKREAL m11,SKREAL m12,SKREAL m13,
					SKREAL m20,SKREAL m21,SKREAL m22,SKREAL m23,
					SKREAL m30,SKREAL m31,SKREAL m32,SKREAL m33);

		
		//3*3󴴽
		void CreateFrom3X3(const SKMatrix3X3 & Mat);
		//ƽƾ
		void CreateTranslate(SKREAL dx, SKREAL dy, SKREAL dz);
		void CreateTranslate(const SKVector3 & V);
		//任
		void CreateInWorldObject(const SKVector3 &U,const SKVector3 &V,const SKVector3 & N,const SKVector3 &Point);
		
		//Ʊ任
		void CreateFormBillboard(const SKVector3 &vcPos,					//λ
								const SKMatrix3X3 &CameraRotMatrix,		//
								bool bAxisY);						//ǷֻYת
		//(Ӽ)
		bool CreateFromLookDir(const SKVector3 &vcPos,					//λ
					const SKVector3 &vcDir,								//۲췽
					const SKVector3 &vcWorldUp = SKVector3(0,1,0));
		//(Ŀλ)
		bool CreateFromLookAt(const SKVector3 &vcPos,									//λ
								const SKVector3 &vcLookAt,							//۲λ
								const SKVector3 &vcWorldUp = SKVector3(0,1,0));		//Ϸ
		//͸ͶӰ
		bool CreatePerspective(SKREAL fFov ,				//XŽ
								SKREAL fAspect,				//߱
								SKREAL fZN ,				//
								SKREAL fZF);				//Զ
		//ͶӰ
		bool CreateOrthogonal(SKREAL fW ,				//
							SKREAL fH,					//
							SKREAL fZN ,				//
							SKREAL fZF)	;				//Զ
		//ӿھ
		bool CreateViewPort(SKREAL fX,SKREAL fY,SKREAL fWidth,SKREAL fHeight,SKREAL fMinz,SKREAL fMaxz);
		//void CreatePointLightPlaneShadow(const SKVector3 & LightPoint,const SKPlane3 &Plane);
		//void CreateInfiniteLightPlaneShadow(const SKVector3 & LightDir,const SKPlane3 &Plane);
		SKMatrix3X3W operator *(SKREAL f)const;
		SKMatrix3X3W operator +(SKREAL f)const;
		SKMatrix3X3W operator -(SKREAL f)const;
		SKMatrix3X3W operator +(const SKMatrix3X3W &Matirx)const;
		SKMatrix3X3W operator -(const SKMatrix3X3W &Matirx)const;

		void operator *= (SKREAL f);
		void operator += (SKREAL f);
		void operator -= (SKREAL f);
		void operator += (const SKMatrix3X3W &Matirx);
		void operator -= (const SKMatrix3X3W &Matirx);

		bool operator ==(const SKMatrix3X3W &v)const;

		//3*3 ת
		void Add3X3(const SKMatrix3X3 & Mat);
		//ƽ
		void AddTranslate(const SKVector3 & V);
		void AddTranslate(SKREAL dx, SKREAL dy, SKREAL dz);
		/*********************************** FORCEINLINE *************************************/

		FORCEINLINE void Identity(void);											//λ
		FORCEINLINE void SetZero();
		FORCEINLINE void TransposeOf(const SKMatrix3X3W &Matrix);				//ת
		FORCEINLINE void InverseOf(const SKMatrix3X3W & Mat);					//

		FORCEINLINE SKMatrix3X3W GetTranspose()const;										//ת
		FORCEINLINE SKMatrix3X3W GetInverse()const;											//

		FORCEINLINE SKVector3 GetTranslation(void)const;							//õƽ
		FORCEINLINE void Get3X3(SKMatrix3X3 & Mat)const;							//õ3*3
		FORCEINLINE SKMatrix3X3W operator * (const SKMatrix3X3W &Matirx)const;    // 
		FORCEINLINE SKVector3 operator * (const SKVector3 &vc)const;				// 
		FORCEINLINE SKVector3W operator * (const SKVector3W &vc)const;			// 


		//Ӧ3X3
		FORCEINLINE SKVector3 Apply3X3(const SKVector3 &v)const;
		//Ӧƽ
		FORCEINLINE SKVector3 ApplyTranlaste(const SKVector3 &Point)const;

		//л
		void GetRowVector(SKVector3W Row[4])const;
		//е
		void GetColumnVector(SKVector3W Column[4])const;

		void GetRowVector(SKVector3W &Row0,SKVector3W &Row1,SKVector3W &Row2,SKVector3W &Row3)const;
		void GetColumnVector(SKVector3W &Column0,SKVector3W &Column1,SKVector3W &Column2,SKVector3W &Column3)const;
		static SKMatrix3X3W ms_Materix3X3WIdentity;
	}; // class

#include "SKMatrix3X3W.inl"
};
#endif