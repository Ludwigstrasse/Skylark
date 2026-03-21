#pragma once
#include "SKCore/SKCoreMinimal.h"
#include <chrono>

namespace Skylark
{
	struct FSKPassTiming
	{
		FSKString Name;
		double CpuMs = 0.0;
	};

	class FSKFrameProfiler
	{
	public:
		void BeginFrame() { Timings.clear(); }
		void EndFrame() {}

		void AddTiming(FSKString Name, double CpuMs)
		{
			FSKPassTiming T;
			T.Name = std::move(Name);
			T.CpuMs = CpuMs;
			Timings.push_back(std::move(T));
		}

		const TArray<FSKPassTiming>& GetTimings() const { return Timings; }

	private:
		TArray<FSKPassTiming> Timings;
	};

	class FSKScopedCpuTimer
	{
	public:
		explicit FSKScopedCpuTimer(FSKFrameProfiler* InProfiler, FSKString InName)
			: Profiler(InProfiler)
			, Name(std::move(InName))
			, Start(std::chrono::high_resolution_clock::now())
		{
		}

		~FSKScopedCpuTimer()
		{
			if (Profiler)
			{
				const auto End = std::chrono::high_resolution_clock::now();
				const double Ms = std::chrono::duration<double, std::milli>(End - Start).count();
				Profiler->AddTiming(std::move(Name), Ms);
			}
		}

	private:
		FSKFrameProfiler* Profiler = nullptr;
		FSKString Name;
		std::chrono::high_resolution_clock::time_point Start;
	};
}
