#pragma once
#include "SKCore/SKCoreMinimal.h"

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <future>
#include <memory>

namespace Skylark
{
	/**
	 * RenderThread & RenderCommand (V11)
	 * - UE5-aligned concept: game/UI thread enqueues render commands; render thread executes them.
	 * - No global singleton; runtime owns the instance.
	 */

	struct FSKRenderCommand
	{
		FSKString Name;
		std::function<void()> Execute;
	};

	class FSKRenderCommandQueue
	{
	public:
		void Enqueue(FSKRenderCommand Cmd)
		{
			{
				std::lock_guard<std::mutex> Lock(Mutex);
				Queue.push(std::move(Cmd));
			}
			CV.notify_one();
		}

		bool TryDequeue(FSKRenderCommand& OutCmd)
		{
			std::lock_guard<std::mutex> Lock(Mutex);
			if (Queue.empty())
			{
				return false;
			}
			OutCmd = std::move(Queue.front());
			Queue.pop();
			return true;
		}

		void WaitForWork()
		{
			std::unique_lock<std::mutex> Lock(Mutex);
			CV.wait(Lock, [&]() { return bStop || !Queue.empty(); });
		}

		void RequestStop()
		{
			{
				std::lock_guard<std::mutex> Lock(Mutex);
				bStop = true;
			}
			CV.notify_all();
		}

		bool IsStopRequested() const
		{
			return bStop;
		}

	private:
		mutable std::mutex Mutex;
		std::condition_variable CV;
		std::queue<FSKRenderCommand> Queue;
		std::atomic<bool> bStop = false;
	};

	class FSKRenderThread
	{
	public:
		FSKRenderThread() = default;
		~FSKRenderThread() { Stop(); }

		void Start()
		{
			if (bRunning)
			{
				return;
			}
			bRunning = true;
			Thread = std::thread([this]() { Run(); });
		}

		void Stop()
		{
			if (!bRunning)
			{
				return;
			}
			CmdQueue.RequestStop();
			if (Thread.joinable())
			{
				Thread.join();
			}
			bRunning = false;
		}

		bool IsRunning() const { return bRunning; }

		void Enqueue(const char* Name, std::function<void()> Fn)
		{
			FSKRenderCommand Cmd;
			Cmd.Name = Name ? Name : "RenderCmd";
			Cmd.Execute = std::move(Fn);
			CmdQueue.Enqueue(std::move(Cmd));
		}

		/** Flush by enqueuing a fence command and waiting for completion (UE-style). */
		void EnqueueAndWait(const char* Name, std::function<void()> Fn)
		{
			// NOTE:
			// std::function requires copyable callables. A lambda capturing std::promise by move is NOT copyable.
			// Use a shared promise to keep the queued command copyable on GCC/Clang.
			auto P = std::make_shared<std::promise<void>>();
			auto F = P->get_future();

			Enqueue(Name, [Fn2 = std::move(Fn), P]() mutable
			{
				if (Fn2)
				{
					Fn2();
				}
				P->set_value();
			});

			F.wait();
		}

		void Flush()
		{
			EnqueueAndWait("Flush", []() {});
		}

	private:
		void Run()
		{
			while (!CmdQueue.IsStopRequested())
			{
				CmdQueue.WaitForWork();

				FSKRenderCommand Cmd;
				while (CmdQueue.TryDequeue(Cmd))
				{
					if (Cmd.Execute)
					{
						Cmd.Execute();
					}
				}
			}
		}

	private:
		std::atomic<bool> bRunning = false;
		FSKRenderCommandQueue CmdQueue;
		std::thread Thread;
	};
}
