#include "SKPipelineCache/SKPipelineCache.h"

#include <cstdio>
#include <filesystem>
#include <fstream>

namespace Skylark
{
	FSKDiskBytecodeCache::FSKDiskBytecodeCache(FSKString InRootDir)
		: RootDir(std::move(InRootDir))
	{
		if (RootDir.empty())
		{
			RootDir = ".sk_cache";
		}
		std::filesystem::create_directories(RootDir);
	}

	FSKString FSKDiskBytecodeCache::MakePath(const FSKHash64& Key) const
	{
		char Buf[32];
		std::snprintf(Buf, sizeof(Buf), "%016llx", (unsigned long long)Key.Value);
		return (std::filesystem::path(RootDir) / (FSKString(Buf) + ".bin")).string();
	}

	bool FSKDiskBytecodeCache::Load(const FSKHash64& Key, TArray<uint8>& OutBytes)
	{
		std::lock_guard<std::mutex> Lock(Mutex);
		const FSKString Path = MakePath(Key);
		std::ifstream F(Path, std::ios::binary);
		if (!F)
		{
			return false;
		}
		F.seekg(0, std::ios::end);
		const std::streamsize Size = F.tellg();
		F.seekg(0, std::ios::beg);
		if (Size <= 0)
		{
			return false;
		}
		OutBytes.resize((SIZE_T)Size);
		F.read((char*)OutBytes.data(), Size);
		return true;
	}

	bool FSKDiskBytecodeCache::Store(const FSKHash64& Key, const TArray<uint8>& Bytes)
	{
		std::lock_guard<std::mutex> Lock(Mutex);
		const FSKString Path = MakePath(Key);
		std::ofstream F(Path, std::ios::binary | std::ios::trunc);
		if (!F)
		{
			return false;
		}
		F.write((const char*)Bytes.data(), (std::streamsize)Bytes.size());
		return true;
	}

	FSKShaderBytecodeCache::FSKShaderBytecodeCache(TUniquePtr<FSKDiskBytecodeCache> InDisk)
		: Disk(std::move(InDisk))
	{
		if (!Disk)
		{
			Disk = std::make_unique<FSKDiskBytecodeCache>(".sk_cache");
		}
	}

	bool FSKShaderBytecodeCache::Get(const FSKHash64& Key, TArray<uint8>& Out)
	{
		{
			std::lock_guard<std::mutex> Lock(Mutex);
			auto It = Memory.find(Key.Value);
			if (It != Memory.end())
			{
				Out = It->second;
				Stats.Hits++;
				return true;
			}
		}

		if (Disk && Disk->Load(Key, Out))
		{
			std::lock_guard<std::mutex> Lock(Mutex);
			Memory[Key.Value] = Out;
			Stats.Hits++;
			return true;
		}

		Stats.Misses++;
		return false;
	}

	void FSKShaderBytecodeCache::Put(const FSKHash64& Key, const TArray<uint8>& Bytes)
	{
		{
			std::lock_guard<std::mutex> Lock(Mutex);
			Memory[Key.Value] = Bytes;
		}
		if (Disk)
		{
			Disk->Store(Key, Bytes);
		}
	}
}
