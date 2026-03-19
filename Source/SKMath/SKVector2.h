#ifndef SKVECTOR2_H
#define SKVECTOR2_H
#include "SKMath.h"
#include "SKFastFunction.h"
namespace SKEngine2
{
	class SKMATH_API SKVector2
	{
	public:
		union
		{
			SKREAL m[2];
			struct  
			{
				SKREAL x, y;
			};
		};
		SKVector2(void);
		SKVector2(SKREAL _x, SKREAL _y);

		/*************************************FORCEINLINE************************************************/
		FORCEINLINE void  Set(SKREAL _x, SKREAL _y);
		
		void operator += (const SKVector2 &v);          
		void operator -= (const SKVector2 &v);          

		void operator *= (SKREAL f);                     
		void operator /= (SKREAL f);                     
		void operator += (SKREAL f);                     
		void operator -= (SKREAL f);                     

		SKREAL     operator * (const SKVector2 &v)const; //

		bool operator ==(const SKVector2 &v)const;


		SKVector2 operator * (SKREAL f)const;            
		SKVector2 operator / (SKREAL f)const;            
		SKVector2 operator + (SKREAL f)const;            
		SKVector2 operator - (SKREAL f)const;            



		SKVector2 operator + (const SKVector2 &v)const; 
		SKVector2 operator - (const SKVector2 &v)const; 

		void Normalize();
	};
	FORCEINLINE bool VS2DIntersect(SKVector2 & A1,SKVector2 & B1,SKVector2 & A2 ,SKVector2 & B2, SKVector2 & Out)
	{
		SKREAL denominator = (B1.y * (A2.x - B2.x) + A1.y * (B2.x - A2.x) + (A1.x - B1.x) * (A2.y - B2.y));

		if(ABS(denominator) < 0.0001)
			return false;

		Out = SKVector2((-B1.x * A2.y * B2.x + A1.y * B1.x * (B2.x - A2.x) + B1.x * A2.x * B2.y + A1.x * (B1.y * A2.x - B2.y * A2.x - B1.y * B2.x + A2.y * B2.x)) / denominator,
			( B1.y * (-A2.y * B2.x + A1.x * (A2.y - B2.y) + A2.x * B2.y) + A1.y * (A2.y * B2.x - A2.x * B2.y + B1.x * (B2.y - A2.y))) / denominator);

		return true;
	}
#include "SKVector2.inl"

}
#endif