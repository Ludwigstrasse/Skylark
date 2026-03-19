#include "SKOtherMath.h"
namespace SKEngine2
{
	bool IsUniformScale(SKREAL fScale)
	{
		if ( ABS(fScale - 1.0f) < EPSILON_E4)
		{
			return true;
		}
		return false;
	}
	bool IsZeroTranslate(const SKVector3 & Translate)
	{
		if(Translate.GetSqrLength() < EPSILON_E4)
		{
			return true;
		}
		return false;
	}
	bool IsIdentityRotate(const SKMatrix3X3 & Rotate)
	{
		for (unsigned int i = 0 ; i < 3 ; i++)
		{
			for (unsigned int j = 0 ; j < 3 ; j++)
			{
				if (i != j)
				{
					if (ABS(Rotate.M[i][j]) > EPSILON_E4)
					{
						return false;
					}
				}
				else
				{
					if (ABS(Rotate.M[i][j] - 1.0f) > EPSILON_E4)
					{
						return false;
					}
				}

			}
		}
		return true;
	}
	bool IsIdentityRotate(const SKQuat & Rotate)
	{
		SKVector3 R(Rotate.x,Rotate.y,Rotate.z);
		if (R.GetSqrLength() < EPSILON_E4)
		{
			return true;
		}
		return false;
	}
	SKREAL LineInterpolation (SKREAL t1, SKREAL t2, SKREAL t)
	{
		return t1 + (t2 - t1) * t;
	}
	SKVector3 LineInterpolation(const SKVector3& t1, const SKVector3& t2, SKREAL t)
	{
		return t1 + (t2 - t1) * t;
	}
	SKQuat LineInterpolation(const SKQuat& t1, const SKQuat& t2, SKREAL t)
	{
		SKREAL fCos = t2.Dot(t1);
		SKQuat Temp = t1;
		if(fCos < 0.0f)
		{
			Temp = t1 * (-1.0f);
		}
		return Temp + (t2 - Temp) * t;
	}
	SKVector3W LineInterpolation(const SKVector3W& t1, const SKVector3W& t2, SKREAL t)
	{
		return t1 + (t2 - t1) * t;
	}
}
