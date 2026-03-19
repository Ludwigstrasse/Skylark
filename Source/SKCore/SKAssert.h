#pragma once
#include <cstdlib>
#include <cstdio>

namespace Skylark
{
	SK_FORCEINLINE void SKOnAssertFailed(const char* Expr, const char* File, int Line)
	{
		std::fprintf(stderr, "SK_CHECK failed: %s (%s:%d)\n", Expr, File, Line);
		std::fflush(stderr);
		std::abort();
	}
}

#define SK_CHECK(Expr) do { if(!(Expr)) { ::Skylark::SKOnAssertFailed(#Expr, __FILE__, __LINE__); } } while(0)
