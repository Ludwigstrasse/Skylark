#pragma once
#include "SKCore/SKCoreTypes.h"
#include "SKCore/SKMath.h"

namespace Skylark
{
	/**
	 * Large World Coordinates (V11)
	 * - CPU side uses double precision (CAD/BIM/CAE).
	 * - GPU side typically uses float precision with camera-relative origin.
	 *
	 * Alignment:
	 * - UE5: LWC + camera-relative rendering.
	 * - OCCT: double precision geometry & tolerant modeling.
	 */

	struct FSKVector3d
	{
		double X = 0.0;
		double Y = 0.0;
		double Z = 0.0;

		FSKVector3d() = default;
		FSKVector3d(double InX, double InY, double InZ) : X(InX), Y(InY), Z(InZ) {}
	};

	struct FSKMatrix4d
	{
		double M[4][4]{};

		static FSKMatrix4d Identity()
		{
			FSKMatrix4d R{};
			R.M[0][0] = 1.0; R.M[1][1] = 1.0; R.M[2][2] = 1.0; R.M[3][3] = 1.0;
			return R;
		}

		static FSKMatrix4d Translation(double Tx, double Ty, double Tz)
		{
			FSKMatrix4d R = Identity();
			R.M[0][3] = Tx;
			R.M[1][3] = Ty;
			R.M[2][3] = Tz;
			return R;
		}
	};

	SK_FORCEINLINE FSKMatrix4d SKMatrixMultiplyD(const FSKMatrix4d& A, const FSKMatrix4d& B)
	{
		FSKMatrix4d R{};
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

	/**
	 * Convert a double-precision affine matrix into a float matrix, using camera-relative translation.
	 * This is the core of LWC rendering: keep rotation/scale in float, but translate relative to ViewOrigin.
	 */
	SK_FORCEINLINE FSKMatrix4f SKMakeCameraRelativeMatrix(const FSKMatrix4d& LocalToWorldD, const FSKVector3d& ViewOrigin)
	{
		FSKMatrix4f R = FSKMatrix4f::Identity();

		for (int r = 0; r < 4; ++r)
		{
			for (int c = 0; c < 4; ++c)
			{
				R.M[r][c] = (float)LocalToWorldD.M[r][c];
			}
		}

		// camera-relative translation (assumes translation stored in last column)
		R.M[0][3] = (float)(LocalToWorldD.M[0][3] - ViewOrigin.X);
		R.M[1][3] = (float)(LocalToWorldD.M[1][3] - ViewOrigin.Y);
		R.M[2][3] = (float)(LocalToWorldD.M[2][3] - ViewOrigin.Z);

		return R;
	}

	SK_FORCEINLINE FSKMatrix4d SKCastToDouble(const FSKMatrix4f& M)
	{
		FSKMatrix4d R{};
		for (int r = 0; r < 4; ++r)
		{
			for (int c = 0; c < 4; ++c)
			{
				R.M[r][c] = (double)M.M[r][c];
			}
		}
		return R;
	}
}
