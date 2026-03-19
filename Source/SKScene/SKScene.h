#pragma once
#include "SKCore/SKCoreMinimal.h"

namespace Skylark
{
	struct FSKTransform
	{
		// UE风格：Translation/Rotation/Scale；Rotation后续可换FQuat
		// 这里先最小化，避免强耦合
		float Translation[3] = {0,0,0};
		float Scale[3] = {1,1,1};
	};

	class ISKSceneNode
	{
	public:
		virtual ~ISKSceneNode() = default;
		virtual const char* GetName() const = 0;
		virtual void SetTransform(const FSKTransform& InTransform) = 0;
	};

	class ISKScene
	{
	public:
		virtual ~ISKScene() = default;
		virtual ISKSceneNode* GetRoot() = 0;
	};
}
