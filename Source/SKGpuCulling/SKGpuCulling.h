#pragma once
#include "SKCore/SKCoreMinimal.h"
#include "SKHZB/SKHZB.h"
#include "SKScene/SKSceneView.h"

namespace Skylark
{
	/**
	 * GPU-Driven Culling (V11)
	 * - UE5-style: build a visibility list on GPU (frustum + HZB occlusion), then generate indirect draw args.
	 * - V11 deliverable is the architecture surface + CPU fallback; D3D12 compute path can be filled incrementally.
	 */

	struct FSKGpuCullingConfig
	{
		bool bEnableFrustumCulling = true;
		bool bEnableOcclusionCulling = true;
		bool bEnableLodSelection = false;
	};

	struct FSKGpuCullingStats
	{
		uint32 TotalInstances = 0;
		uint32 VisibleInstances = 0;
		uint32 CulledByFrustum = 0;
		uint32 CulledByOcclusion = 0;
	};

	class ISKGpuCulling
	{
	public:
		virtual ~ISKGpuCulling() = default;

		virtual void BeginFrame() {}
		virtual void EndFrame() {}

		virtual void CullInstanceBatches(
			ISKRHIDevice& Device,
			ISKRHICommandList& Cmd,
			const FSKGpuCullingConfig& Config,
			const ISKHZB* Hzb,
			FSKSceneView& InOutView,
			FSKGpuCullingStats& OutStats) = 0;
	};

	/** CPU fallback implementation (keeps the same surface; does not use HZB). */
	class FSKCpuGpuCulling final : public ISKGpuCulling
	{
	public:
		void CullInstanceBatches(
			ISKRHIDevice& Device,
			ISKRHICommandList& Cmd,
			const FSKGpuCullingConfig& Config,
			const ISKHZB* Hzb,
			FSKSceneView& InOutView,
			FSKGpuCullingStats& OutStats) override
		{
			(void)Device;
			(void)Cmd;
			(void)Config;
			(void)Hzb;

			// V11: placeholder - keep all instances visible.
			OutStats = {};
			for (const auto& B : InOutView.VisibleInstances)
			{
				OutStats.TotalInstances += (uint32)B.InstanceLocalToWorld.size();
			}
			OutStats.VisibleInstances = OutStats.TotalInstances;
		}
	};

	TUniquePtr<ISKGpuCulling> SKCreateGpuCulling();
}
