#pragma once
#include <cstdint>
#include <cstddef>

#if defined(_MSC_VER)
  #define SK_FORCEINLINE __forceinline
#else
  #define SK_FORCEINLINE inline __attribute__((always_inline))
#endif

namespace Skylark
{
	using int8  = std::int8_t;
	using int16 = std::int16_t;
	using int32 = std::int32_t;
	using int64 = std::int64_t;

	using uint8  = std::uint8_t;
	using uint16 = std::uint16_t;
	using uint32 = std::uint32_t;
	using uint64 = std::uint64_t;

	using SIZE_T = std::size_t;
}
