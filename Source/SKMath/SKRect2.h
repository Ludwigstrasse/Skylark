#ifndef SKRECT2_H
#define SKRECT2_H
#include "SKMath.h"
#include "SKVector2.h"
namespace SKEngine2
{
	class SKMATH_API SKRect2
	{
	public:
		SKVector2 m_Min;
		SKVector2 m_Max;
		SKRect2(void);
		SKRect2(const SKVector2& Min, const SKVector2 &Max);

	};


}
#endif