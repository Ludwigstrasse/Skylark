#pragma once
#include "SKCore/SKCoreMinimal.h"

#include <future>
#include <functional>

namespace Skylark
{
	/**
	 * TaskGraph (V11)
	 * - Minimal UE-style async execution surface for Skylark.
	 * - Not a full UE TaskGraph clone; provides a stable abstraction for:
	 *   * Streaming background tasks (IO/parse/build)
	 *   * Render-thread command execution helpers
	 */
	class FSKTaskHandle
	{
	public:
		FSKTaskHandle() = default;
		explicit FSKTaskHandle(std::future<void>&& InFuture) : Future(std::move(InFuture)) {}

		void Wait()
		{
			if (Future.valid())
			{
				Future.wait();
			}
		}

		bool IsValid() const { return Future.valid(); }

	private:
		std::future<void> Future;
	};

	class FSKTaskGraph
	{
	public:
		FSKTaskGraph() = default;

		FSKTaskHandle DispatchAsync(std::function<void()> Fn)
		{
			return FSKTaskHandle(std::async(std::launch::async, [F = std::move(Fn)]() { if (F) { F(); } }));
		}
	};
}
