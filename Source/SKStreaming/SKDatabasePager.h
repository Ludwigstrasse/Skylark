#pragma once
#include "SKCore/SKCoreMinimal.h"
#include "SKStreaming/SKStreaming.h"

#include <functional>
#include <mutex>

namespace Skylark
{
	class FSKNode;

	/**
	 * DatabasePager (V10)
	 * - OSG DatabasePager inspired: background load/build -> main thread merge.
	 * - Uses FSKStreamingQueue to run tasks.
	 */
	class FSKDatabasePager
	{
	public:
		explicit FSKDatabasePager(FSKStreamingQueue* InQueue) : Queue(InQueue) {}

		using FOnNodeReady = std::function<void(TSharedPtr<FSKNode> Loaded)>;

		void RequestNodeAsync(FSKString Uri, FOnNodeReady OnReady);
		void TickMainThread();

	private:
		FSKStreamingQueue* Queue = nullptr;

		struct FPendingReady
		{
			FOnNodeReady OnReady;
			TSharedPtr<FSKNode> Node;
		};

		std::mutex Mutex;
		TArray<FPendingReady> Completed;
	};
}
