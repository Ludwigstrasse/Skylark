#include "SKMeasurement/SKMeasurement.h"
#include <cmath>

namespace Skylark
{
	FSKMeasureResult FSKMeasurementSystem::MeasureDistance(const FSKVector3f& A, const FSKVector3f& B)
	{
		FSKMeasureResult R;
		R.Type = ESKMeasureType::Distance;
		R.PointA = A;
		R.PointB = B;
		const float dx = B.X - A.X;
		const float dy = B.Y - A.Y;
		const float dz = B.Z - A.Z;
		R.Value = std::sqrt(dx * dx + dy * dy + dz * dz);
		return R;
	}

	FSKMeasureResult FSKMeasurementSystem::MeasureAngleDeg(const FSKVector3f& V0, const FSKVector3f& V1)
	{
		FSKMeasureResult R;
		R.Type = ESKMeasureType::Angle;

		const float a = std::sqrt(V0.X * V0.X + V0.Y * V0.Y + V0.Z * V0.Z);
		const float b = std::sqrt(V1.X * V1.X + V1.Y * V1.Y + V1.Z * V1.Z);
		if (a <= 1e-6f || b <= 1e-6f)
		{
			R.Value = 0.0f;
			return R;
		}

		const float dot = (V0.X * V1.X + V0.Y * V1.Y + V0.Z * V1.Z) / (a * b);
		const float c = std::max(-1.0f, std::min(1.0f, dot));
		R.Value = std::acos(c) * 180.0f / 3.14159265358979323846f;
		return R;
	}
}
