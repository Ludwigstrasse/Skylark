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

	struct FSKVector4f
	{
		float X = 0.0f;
		float Y = 0.0f;
		float Z = 0.0f;
		float W = 1.0f;

		FSKVector4f() = default;
		FSKVector4f(float InX, float InY, float InZ, float InW) : X(InX), Y(InY), Z(InZ), W(InW) {}
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

		static FSKMatrix4f Translation(float Tx, float Ty, float Tz)
		{
			FSKMatrix4f R = Identity();
			R.M[0][3] = Tx;
			R.M[1][3] = Ty;
			R.M[2][3] = Tz;
			return R;
		}
	};

	// ------------------------------------------------------------
	// Minimal matrix helpers (UE-like surface, row-major assumed).
	// NOTE: This is intentionally small. A full math library can replace it.
	// ------------------------------------------------------------
	SK_FORCEINLINE FSKMatrix4f SKMatrixMultiply(const FSKMatrix4f& A, const FSKMatrix4f& B)
	{
		FSKMatrix4f R{};
		for (int r = 0; r < 4; ++r)
		{
			for (int c = 0; c < 4; ++c)
			{
				R.M[r][c] = A.M[r][0] * B.M[0][c]
					+ A.M[r][1] * B.M[1][c]
					+ A.M[r][2] * B.M[2][c]
					+ A.M[r][3] * B.M[3][c];
			}
		}
		return R;
	}

	SK_FORCEINLINE FSKVector4f SKTransformVector4(const FSKMatrix4f& M, const FSKVector4f& V)
	{
		FSKVector4f R;
		R.X = M.M[0][0] * V.X + M.M[0][1] * V.Y + M.M[0][2] * V.Z + M.M[0][3] * V.W;
		R.Y = M.M[1][0] * V.X + M.M[1][1] * V.Y + M.M[1][2] * V.Z + M.M[1][3] * V.W;
		R.Z = M.M[2][0] * V.X + M.M[2][1] * V.Y + M.M[2][2] * V.Z + M.M[2][3] * V.W;
		R.W = M.M[3][0] * V.X + M.M[3][1] * V.Y + M.M[3][2] * V.Z + M.M[3][3] * V.W;
		return R;
	}

	SK_FORCEINLINE FSKVector3f SKTransformPoint(const FSKMatrix4f& M, const FSKVector3f& P)
	{
		const FSKVector4f V(P.X, P.Y, P.Z, 1.0f);
		const FSKVector4f R = SKTransformVector4(M, V);
		FSKVector3f Out;
		Out.X = R.X;
		Out.Y = R.Y;
		Out.Z = R.Z;
		return Out;
	}

	struct FSKColor
	{
		// Packed RGBA8
		uint32 RGBA = 0xFFFFFFFFu;
	};
}
