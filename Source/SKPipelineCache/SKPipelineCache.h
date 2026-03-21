#pragma once

#include "SKCore/SKCoreMinimal.h"
#include "SKShaderCore/SKShaderCore.h"

#include <mutex>

namespace Skylark
{
	/**
	 * Pipeline/Shader cache (UE5-aligned intentions)
	 *
	 * V13 scope:
	 * - Disk + memory cache for compiled shader bytecode.
	 * - Stable keying (FSKHash64 from SKShaderCore).
	 * - Pipeline cache surface is introduced, but backend-specific PSO serialization is optional.
	 */

	struct FSKCacheStats
	{
		uint32 Hits = 0;
		uint32 Misses = 0;
	};

	class FSKDiskBytecodeCache
	{
	public:
		explicit FSKDiskBytecodeCache(FSKString InRootDir);

		bool Load(const FSKHash64& Key, TArray<uint8>& OutBytes);
		bool Store(const FSKHash64& Key, const TArray<uint8>& Bytes);

		const FSKString& GetRootDir() const { return RootDir; }

	private:
		FSKString MakePath(const FSKHash64& Key) const;

	private:
		FSKString RootDir;
		std::mutex Mutex;
	};

	class FSKShaderBytecodeCache
	{
	public:
		explicit FSKShaderBytecodeCache(TUniquePtr<FSKDiskBytecodeCache> InDisk);

		// Try get bytecode; returns true on hit.
		bool Get(const FSKHash64& Key, TArray<uint8>& Out);
		void Put(const FSKHash64& Key, const TArray<uint8>& Bytes);

		FSKCacheStats GetStats() const { return Stats; }

	private:
		TUniquePtr<FSKDiskBytecodeCache> Disk;
		mutable std::mutex Mutex;
		TMap<uint64, TArray<uint8>> Memory; // Key.Value -> bytes
		FSKCacheStats Stats;
	};
}
