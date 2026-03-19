#ifndef SKOTHERMATH_H
#define SKOTHERMATH_H
#include "SKVector3.h"
#include "SKMatrix3X3.h"
#include "SKQuat.h"
#include "SKVector3W.h"
namespace SKEngine2
{
	bool SKMATH_API IsUniformScale(SKREAL fScale);
	bool SKMATH_API IsZeroTranslate(const SKVector3 & Translate);
	bool SKMATH_API IsIdentityRotate(const SKMatrix3X3 & Rotate);
	bool SKMATH_API IsIdentityRotate(const SKQuat & Rotate);

// 	template< typename T>
// 	T LineInterpolation (T t1, T t2, SKREAL t)
// 	{
// 		return t1 + (t2 - t1) * t;
// 	}
// 	SKQuat LineInterpolation (SKQuat t1, SKQuat t2, SKREAL t)
// 	{
// 		SKREAL fCos = t2.Dot(t1);
// 		if(fCos < 0.0f)
// 		{
// 			t1 *= -1.0f;
// 		}
// 		return t1 + (t2 - t1) * t;
// 	}
	SKREAL SKMATH_API LineInterpolation (SKREAL t1, SKREAL t2, SKREAL t);

	SKVector3 SKMATH_API LineInterpolation (const SKVector3& t1, const SKVector3& t2, SKREAL t);

	SKQuat SKMATH_API LineInterpolation(const SKQuat& t1, const SKQuat& t2, SKREAL t);

	SKVector3W SKMATH_API LineInterpolation(const SKVector3W& t1, const SKVector3W& t2, SKREAL t);
}
#endif
