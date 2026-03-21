#include "SKStreaming/SKStreaming.h"

namespace Skylark
{
	FSKStreamingQueue::FSKStreamingQueue() = default;

	FSKStreamingQueue::~FSKStreamingQueue()
	{
		Stop();
	}

	void FSKStreamingQueue::Start()
	{
		if (bRunning.exchange(true))
		{
			return;
		}
		Worker = std::thread([this]() { ThreadMain(); });
	}

	void FSKStreamingQueue::Stop()
	{
		if (!bRunning.exchange(false))
		{
			return;
		}
		Cv.notify_all();
		if (Worker.joinable())
		{
			Worker.join();
		}
		// drain
		std::lock_guard<std::mutex> Lock(Mutex);
		while (!Queue.empty())
		{
			Queue.pop();
		}
	}

	void FSKStreamingQueue::Enqueue(std::function<void()>&& Work)
	{
		if (!Work)
		{
			return;
		}
		{
			std::lock_guard<std::mutex> Lock(Mutex);
			Queue.push(std::move(Work));
		}
		Cv.notify_one();
	}

	void FSKStreamingQueue::ThreadMain()
	{
		while (bRunning.load())
		{
			std::function<void()> Job;
			{
				std::unique_lock<std::mutex> Lock(Mutex);
				Cv.wait(Lock, [this]() { return !bRunning.load() || !Queue.empty(); });
				if (!bRunning.load())
				{
					break;
				}
				if (!Queue.empty())
				{
					Job = std::move(Queue.front());
					Queue.pop();
				}
			}
			if (Job)
			{
				Job();
			}
		}
	}
}
