#pragma once
#include "SKCore/SKCoreMinimal.h"

namespace Skylark
{
	/**
	 * BIM 层：对标 Revit/IFC 的语义对象与分类渲染
	 * - Category/Layer/Phase/Discipline 可用于渲染过滤、配色、LOD、透明度策略
	 */
	struct FSKBimCategory
	{
		FSKString Name;
		uint32 ColorRGBA = 0xFFFFFFFFu;
		bool bVisible = true;
	};

	class ISKBimScene
	{
	public:
		virtual ~ISKBimScene() = default;
		virtual void SetCategoryVisible(const FSKStringView CategoryName, bool bVisible) = 0;
	};
}
