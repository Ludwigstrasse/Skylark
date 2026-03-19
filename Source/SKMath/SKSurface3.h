#ifndef SKSURFACE3_H
#define SKSURFACE3_H
#include "SKVector3.h"
/*

*/
namespace SKEngine2
{
	class SKMATH_API SKSurface3 : public SKMemObject
	{
	public:
		SKSurface3();
		virtual ~SKSurface3() = 0;
		//
		virtual SKVector3 GetPoint(SKREAL U,SKREAL V) = 0;
		//ƫ
		virtual SKVector3 GetUPartialDerivative(SKREAL U,SKREAL V) = 0;
		virtual SKVector3 GetVPartialDerivative(SKREAL U,SKREAL V) = 0;
		//õ
		FORCEINLINE SKVector3 GetN(SKREAL U,SKREAL V);
	};

#include "SKSurface3.inl"
}
#endif