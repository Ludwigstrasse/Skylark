#pragma once
#include "SKCore/SKCoreTypes.h"

namespace Skylark
{
	/**
	 * FSKGuid (UE: FGuid)
	 * - 128-bit identifier (non-cryptographic)
	 */
	struct FSKGuid
	{
		uint64 A = 0;
		uint64 B = 0;

		static FSKGuid Zero() { return FSKGuid{}; }
		bool IsValid() const { return (A | B) != 0; }

		bool operator==(const FSKGuid& Rhs) const { return A == Rhs.A && B == Rhs.B; }
		bool operator!=(const FSKGuid& Rhs) const { return !(*this == Rhs); }
	};

	inline uint64 SKHashGuid(const FSKGuid& G)
	{
		// 64-bit mix (splitmix64-like)
		uint64 x = G.A ^ (G.B + 0x9E3779B97F4A7C15ull);
		x ^= x >> 30; x *= 0xBF58476D1CE4E5B9ull;
		x ^= x >> 27; x *= 0x94D049BB133111EBull;
		x ^= x >> 31;
		return x;
	}
}
