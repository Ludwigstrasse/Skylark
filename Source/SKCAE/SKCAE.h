#pragma once
#include "SKCore/SKCoreMinimal.h"

namespace Skylark
{
	/**
	 * CAE 可视化：标量云图、等值面、矢量箭头、变形对比、截面/探针等
	 * 对标 ParaView/VTK + 商业 CAE 后处理器。
	 */
	enum class ESKFieldType : uint8
	{
		Scalar = 0,
		Vector,
		Tensor
	};

	struct FSKFieldMeta
	{
		FSKString Name;
		ESKFieldType Type = ESKFieldType::Scalar;
		float MinValue = 0.0f;
		float MaxValue = 1.0f;
	};

	class ISKFieldVisualizer
	{
	public:
		virtual ~ISKFieldVisualizer() = default;
		virtual void SetActiveField(const FSKStringView FieldName) = 0;
		virtual void SetDeformScale(float Scale) = 0;
	};
}
