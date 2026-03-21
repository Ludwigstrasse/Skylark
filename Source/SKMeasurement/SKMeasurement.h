#pragma once
#include "SKCore/SKCoreMinimal.h"
#include "SKCore/SKObjectId.h"

namespace Skylark
{
	/**
	 * SKMeasurement (V9)
	 * - Industrial measurement helpers (distance/angle/radius placeholders).
	 * - Precise CAD queries (radius, min distance to curve/surface) should be provided by a CAD kernel backend:
	 *   ISKCadQueryProvider (OCCT).
	 */

	enum class ESKMeasureType : uint8
	{
		None = 0,
		Distance,
		Angle,
		Radius,
		Diameter,
	};

	struct FSKMeasureResult
	{
		ESKMeasureType Type = ESKMeasureType::None;
		float Value = 0.0f;

		FSKObjectId A{};
		FSKObjectId B{};

		FSKVector3f PointA{};
		FSKVector3f PointB{};
	};

	class FSKMeasurementSystem final
	{
	public:
		static FSKMeasureResult MeasureDistance(const FSKVector3f& A, const FSKVector3f& B);
		static FSKMeasureResult MeasureAngleDeg(const FSKVector3f& V0, const FSKVector3f& V1);

		// Future: precise queries via OCCT
	};
}
