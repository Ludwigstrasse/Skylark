#pragma once
#include "SKCore/SKCoreTypes.h"
#include <cmath>


namespace Skylark
{
	struct FSKVector3f
	{
		float X = 0.0f;
		float Y = 0.0f;
		float Z = 0.0f;

		FSKVector3f() = default;
		FSKVector3f(float InX, float InY, float InZ) : X(InX), Y(InY), Z(InZ) {}

		float Size() const { return std::sqrt(X * X + Y * Y + Z * Z); }
	};



	struct FSKMatrix4f
	{
		float M[4][4]{};

		static FSKMatrix4f Identity()
		{
			FSKMatrix4f R;
			R.M[0][0] = 1.0f; R.M[1][1] = 1.0f; R.M[2][2] = 1.0f; R.M[3][3] = 1.0f;
			return R;
		}
	};

	struct FSKColor
	{
		// Packed RGBA8
		uint32 RGBA = 0xFFFFFFFFu;
	};
}
