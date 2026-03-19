#ifndef SKENGINE_MATH_H
#define SKENGINE_MATH_H
#include "SKSystem.h"
#include <windows.h> 
#include <math.h>  
#include <xmmintrin.h>
#include <emmintrin.h>
#include "SKMemManager.h"
#ifdef SKMATH_EXPORTS
	#define  SKMATH_API __declspec(dllexport) 
#else
	#define  SKMATH_API __declspec(dllimport)
#endif
namespace SKEngine2
{
#define  SKFLOAT 
#ifdef SKFLOAT 
#define SKREAL 		float
#define SIN(x)		sinf(x)
#define COS(x)		cosf(x)
#define TAN(x)		tanf(x)
#define ASIN(x)		asinf(x)
#define ACOS(x)		acosf(x)
#define ATAN(x)		atanf(x)
#define ATAN2(y,x)	atan2f(y,x)
#define SQRT(x)		sqrtf(x)
#define POW(x,y)	powf(x,y)
#define CEIL(x)		ceilf(x);
#define EXP(x)		expf(x);
#define FMOD(x,y)	fmodf(x,y)
#else ifdef SKDOUBLE
#define SKREAL 		double
#define SIN(x)		sin(x)
#define COS(x)		cos(x)
#define TAN(x)		tan(x)
#define ASIN(x)		asin(x)
#define ACOS(x)		acos(x)
#define ATAN(x) 	atan(x)
#define ATAN2(y,x) 	atan2(y,x)
#define SQRT(x)		sqrt(x)
#define POW(x,y)	pow(x,y)
#define CEIL(x)		ceil(x);
#define EXP(x)		exp(x);
#define FMOD(x,y)	fmod(x,y)
#endif

#define EPSILON_E3 (SKREAL)(1E-3) 
#define EPSILON_E4 (SKREAL)(1E-4) 
#define EPSILON_E5 (SKREAL)(1E-5)
#define EPSILON_E6 (SKREAL)(1E-6)


#define SKFRONT			0
#define SKBACK			1
#define SKON			2

#define SKCLIPPED		3
#define SKCULLED		4
#define SKVISIBLE		5

#define SKINTERSECT		3
#define SKOUT			4
#define SKIN			5

#define SKNOINTERSECT	6

#define SKMIN_CHAR		(-128)      
#define SKMAX_CHAR		127       
#define SKMIN_INT		(-2147483647 - 1) 
#define SKMAX_INT       2147483647    
#define SKMAX_REAL		3.402823466e+38F
#define SKMIN_REAL		1.175494351e-38F
#define SKMAX_INTEGER	0xffffffff
#define SKREAL_1		0.99999f		
#define SKPI			3.14159265f
#define SKPI2			1.5707963f
#define VS2PI			6.2831853f

#define SKSHADOWON		0
#define SKSHADOWBACK	1
#define SKNOSHADOW		2


#define SKUSHORT_INDEX	unsigned short

#define SKUINT_INDEX	unsigned int
FORCEINLINE unsigned int CalcAlign(unsigned int n, unsigned int align)
{
	return ((n + align - 1) & (~(align - 1)));
}
FORCEINLINE SKREAL RadianToAngle(SKREAL Radian)
{
	return ( Radian * 180.0f ) / SKPI ;
}
FORCEINLINE SKREAL AngleToRadian(SKREAL Angle)
{
	return ( Angle * SKPI ) /  180.0f;
}
FORCEINLINE bool IsTwoPower(unsigned int uiN)
{
	return !(uiN & (uiN - 1));
}
FORCEINLINE unsigned short FloatToHalf
(
 SKREAL Value
 )
{
	unsigned int Result;

	unsigned int IValue = ((unsigned int *)(&Value))[0];
	unsigned int Sign = (IValue & 0x80000000U) >> 16U;
	IValue = IValue & 0x7FFFFFFFU;      // Hack off the sign

	if (IValue > 0x47FFEFFFU)
	{
		// The number is too large to be represented as a half.  Saturate to infinity.
		Result = 0x7FFFU;
	}
	else
	{
		if (IValue < 0x38800000U)
		{
			// The number is too small to be represented as a normalized half.
			// Convert it to a denormalized value.
			unsigned int Shift = 113U - (IValue >> 23U);
			IValue = (0x800000U | (IValue & 0x7FFFFFU)) >> Shift;
		}
		else
		{
			// Rebias the exponent to represent the value as a normalized half.
			IValue += 0xC8000000U;
		}

		Result = ((IValue + 0x0FFFU + ((IValue >> 13U) & 1U)) >> 13U)&0x7FFFU; 
	}
	return (unsigned short)(Result|Sign);
}
FORCEINLINE SKREAL HalfToFloat
(
 unsigned short Value
 )
{


	unsigned int Mantissa;
	unsigned int Exponent;
	unsigned int Result;

	Mantissa = (unsigned int)(Value & 0x03FF);

	if ((Value & 0x7C00) != 0)  // The value is normalized
	{
		Exponent = (unsigned int)((Value >> 10) & 0x1F);
	}
	else if (Mantissa != 0)     // The value is denormalized
	{
		// Normalize the value in the resulting SKREAL
		Exponent = 1;

		do
		{
			Exponent--;
			Mantissa <<= 1;
		} while ((Mantissa & 0x0400) == 0);

		Mantissa &= 0x03FF;
	}
	else                        // The value is zero
	{
		Exponent = (unsigned int)-112;
	}

	Result = ((Value & 0x8000) << 16) | // Sign
		((Exponent + 112) << 23) | // Exponent
		(Mantissa << 13);          // Mantissa

	return *(SKREAL*)&Result;
}
//f must [0,1]
FORCEINLINE unsigned int CompressUnitFloat(SKREAL f, unsigned int Bit = 16)
{
	unsigned int nIntervals = 1 << Bit;
	SKREAL scaled = f * (nIntervals - 1);
	unsigned int rounded = (unsigned int)(scaled + 0.5f);
	if (rounded > nIntervals - 1)
	{
		rounded = nIntervals - 1;
	}
	return rounded;
	
}
FORCEINLINE unsigned int CompressFloat(SKREAL f, SKREAL Max , SKREAL Min ,unsigned int Bit = 16)
{
	SKREAL Unitf = (f - Min)/(Max - Min);
	return CompressUnitFloat(Unitf,Bit);
}
FORCEINLINE SKREAL DecompressUnitFloat(unsigned int quantized,unsigned int Bit = 16)
{
	unsigned int nIntervals = 1 << Bit;
	SKREAL IntervalSize = 1.0f / (nIntervals - 1);
	
	return quantized * IntervalSize;
}
FORCEINLINE SKREAL DecompressFloat(unsigned int quantized,SKREAL Max , SKREAL Min ,unsigned int Bit = 16)
{
	SKREAL Unitf = DecompressUnitFloat(quantized,Bit);
	return (Min + Unitf * (Max - Min));
}
FORCEINLINE  SKREAL ClampAxis(SKREAL Angle)
{
	// returns Angle in the range (-360,360)
	Angle = FMOD(Angle, 360.f);

	if (Angle < 0.f)
	{
		// shift to [0,360) range
		Angle += 360.f;
	}

	return Angle;
}


FORCEINLINE  SKREAL NormalizeAxis(SKREAL Angle)
{
	// returns Angle in the range [0,360)
	Angle = ClampAxis(Angle);

	if (Angle > 180.f)
	{
		// shift to (-180,180]
		Angle -= 360.f;
	}

	return Angle;
}

//-360.....360 can use
FORCEINLINE SKREAL ClampAngle(SKREAL AngleDegrees, SKREAL MinAngleDegrees, SKREAL MaxAngleDegrees)
{
	SKREAL MaxDelta = ClampAxis(MaxAngleDegrees - MinAngleDegrees) * 0.5f;			// 0..180
	SKREAL RangeCenter = ClampAxis(MinAngleDegrees + MaxDelta);						// 0..360
	SKREAL DeltaFromCenter = NormalizeAxis(AngleDegrees - RangeCenter);				// -180..180

	// maybe clamp to nearest edge
	if (DeltaFromCenter > MaxDelta)
	{
		return NormalizeAxis(RangeCenter + MaxDelta);
	}
	else if (DeltaFromCenter < -MaxDelta)
	{
		return NormalizeAxis(RangeCenter - MaxDelta);
	}

	// already in range, just return it
	return NormalizeAxis(AngleDegrees);
}

FORCEINLINE int SKRand() { return rand(); }
FORCEINLINE void SKRandInit(int Seed) { srand(Seed); }
FORCEINLINE float SKFRand() { return SKRand() / (float)RAND_MAX; }
//ʼCPUϢ
bool SKMATH_API SKInitCPU(void);
bool SKMATH_API SKInitMath();
void SKMATH_API SKInitCRCTable();
FORCEINLINE SKREAL SKMATH_API GetFastSin(unsigned int i);
FORCEINLINE SKREAL SKMATH_API GetFastCos(unsigned int i);
unsigned int SKMATH_API CRC32Compute( const void *pData, unsigned int uiDateSize );
FORCEINLINE SKMATH_API unsigned int Rounding(SKREAL fNum);
extern bool g_bSSE;
extern SKREAL FastSin[361];
extern SKREAL FastCos[361];
#define SK_SSE
}
#endif