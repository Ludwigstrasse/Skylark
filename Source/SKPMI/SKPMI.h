#pragma once
#include "SKCore/SKCoreMinimal.h"
#include "SKCore/SKObjectId.h"

namespace Skylark
{
	enum class ESKPMIType : uint8
	{
		Dimension = 0,
		Tolerance,
		SurfaceFinish,
		WeldSymbol,
		Datum,
		Note,
	};

	struct FSKPMIEntity
	{
		ESKPMIType Type = ESKPMIType::Note;
		FSKObjectId OwnerId{};
		FSKString Text;
		FSKMatrix4f WorldTransform = FSKMatrix4f::Identity();
	};

	class ISKPMIProvider
	{
	public:
		virtual ~ISKPMIProvider() = default;
		virtual void EnumeratePMI(TArray<FSKPMIEntity>& OutPMI) = 0;
	};
}
