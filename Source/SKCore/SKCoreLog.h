#pragma once
#include <cstdarg>
#include <cstdio>
#include "SKCore/SKCoreTypes.h"

namespace Skylark
{
	enum class ESKLogVerbosity : int
	{
		Fatal = 0,
		Error,
		Warning,
		Display,
		Log,
		Verbose,
		VeryVerbose,
	};

	struct FSKLogCategory
	{
		const char* Name = "LogSkylark";
		ESKLogVerbosity DefaultVerbosity = ESKLogVerbosity::Log;
	};

	SK_FORCEINLINE void SKVLog(const FSKLogCategory& Category, ESKLogVerbosity Verbosity, const char* Fmt, va_list Args)
	{
		(void)Verbosity;
		std::fprintf(stdout, "[%s] ", Category.Name);
		std::vfprintf(stdout, Fmt, Args);
		std::fprintf(stdout, "\n");
		std::fflush(stdout);
	}

	SK_FORCEINLINE void SKLog(const FSKLogCategory& Category, ESKLogVerbosity Verbosity, const char* Fmt, ...)
	{
		va_list Args;
		va_start(Args, Fmt);
		SKVLog(Category, Verbosity, Fmt, Args);
		va_end(Args);
	}

	inline FSKLogCategory GLogSkylark { "LogSkylark", ESKLogVerbosity::Log };
}

#define SK_LOG(Category, Verbosity, Fmt, ...) ::Skylark::SKLog((Category), (Verbosity), (Fmt), ##__VA_ARGS__)
