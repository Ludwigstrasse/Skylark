#ifndef SKFASTFUNCTION_H
#define SKFASTFUNCTION_H
#include "SKMath.h"
namespace SKEngine2
{
	class SKMatrix3X3W;
	//sse fast function	
	SKMATH_API void SKFastMul(const SKMatrix3X3W & InM1,const SKMatrix3X3W & InM2,SKMatrix3X3W & OutM);
	SKMATH_API void SKFastAdd(const SKMatrix3X3W & InM1,const SKMatrix3X3W & InM2,SKMatrix3X3W & OutM);
	SKMATH_API void SKFastSub(const SKMatrix3X3W & InM1,const SKMatrix3X3W & InM2,SKMatrix3X3W & OutM);
	SKMATH_API void SKFastMul(const SKMatrix3X3W & InM1,SKREAL f,SKMatrix3X3W & OutM);
	SKMATH_API void SKFastLerp(const SKMatrix3X3W & InM1,const SKMatrix3X3W & InM2,SKREAL f,SKMatrix3X3W & OutM);

	class SKVector2;
	class SKVector3;
	class SKVector3W;


	SKMATH_API SKREAL SKFastLength(const SKVector2 &vec);
	SKMATH_API SKREAL SKFastLength(const SKVector3 &vec);
	SKMATH_API SKREAL SKFastLength(const SKVector3W &vec);


	SKMATH_API void SKFastCross(const SKVector3 &InV1, const SKVector3 &InV2,SKVector3 &OutV);

	SKMATH_API void SKFastNormalize(const SKVector2 &InV,SKVector2 &OutV);
	SKMATH_API void SKFastNormalize(const SKVector3 &InV,SKVector3 &OutV);
	SKMATH_API void SKFastNormalize(const SKVector3W &InV,SKVector3W &OutV);
}
#endif

