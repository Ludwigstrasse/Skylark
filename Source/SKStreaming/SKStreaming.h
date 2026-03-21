#pragma once
#include "SKCore/SKCoreMinimal.h"

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

namespace Skylark
{
	/**
	 * SKStreaming (V8)
	 * - Minimal async task queue for background loading/processing.
	 * - UE alignment: AsyncTask/TaskGraph-like entry point (but simplified).
	 *
	 * Notes:
	 * - The engine is still single-threaded on the render side.
	 * - This queue is intended for IO/parse/build CPU mesh, then handoff to main thread.
	 */
	class FSKStreamingQueue final
	{
	public:
		FSKStreamingQueue();
		~FSKStreamingQueue();

		void Start();
		void Stop();

		// Enqueue work to run on background thread.
		void Enqueue(std::function<void()>&& Work);

	private:
		void ThreadMain();

	private:
		std::atomic<bool> bRunning{ false };
		std::thread Worker;

		std::mutex Mutex;
		std::condition_variable Cv;
		std::queue<std::function<void()>> Queue;
	};
}
