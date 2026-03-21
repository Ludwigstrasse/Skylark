#pragma once

#include "SKCore/SKCoreMinimal.h"

#include <cstdint>

namespace Skylark
{
	/**
	 * Shader system core (UE5-aligned surface)
	 *
	 * Goals (V13):
	 * - Provide a backend-agnostic compilation request model.
	 * - Provide stable hashing/keying for caches (disk + memory).
	 * - Keep platform/compiler headers out of Public includes.
	 */

	// -----------------------------
	// Hashing (64-bit FNV-1a)
	// -----------------------------
	struct FSKHash64
	{
		uint64 Value = 1469598103934665603ull;

		void AddBytes(const void* Data, SIZE_T Size)
		{
			const uint8* P = static_cast<const uint8*>(Data);
			for (SIZE_T i = 0; i < Size; ++i)
			{
				Value ^= (uint64)P[i];
				Value *= 1099511628211ull;
			}
		}

		void AddString(FSKStringView S)
		{
			AddBytes(S.data(), (SIZE_T)S.size());
		}
	};

	// -----------------------------
	// Shader compile model
	// -----------------------------
	enum class ESKShaderLanguage : uint8
	{
		Unknown = 0,
		Hlsl,
		Glsl,
		Msl,
	};

	enum class ESKShaderStage : uint8
	{
		Vertex = 0,
		Pixel,
		Compute,
	};

	struct FSKShaderDefine
	{
		FSKString Name;
		FSKString Value;
	};

	struct FSKShaderSource
	{
		// If FilePath is non-empty, compiler should load from file.
		FSKString FilePath;

		// Otherwise, treat Code as the source.
		FSKString Code;
	};

	struct FSKShaderCompileRequest
	{
		ESKShaderLanguage Language = ESKShaderLanguage::Hlsl;
		ESKShaderStage Stage = ESKShaderStage::Vertex;

		FSKShaderSource Source;
		FSKString EntryPoint = "main";

		// Backend profile string (HLSL examples: vs_5_0, ps_5_0, cs_5_0).
		FSKString Profile;

		// Optional include search paths.
		TArray<FSKString> IncludeDirs;

		// Preprocessor defines.
		TArray<FSKShaderDefine> Defines;

		// Optional debug name.
		FSKString DebugName;
	};

	struct FSKShaderCompileOutput
	{
		bool bSucceeded = false;
		TArray<uint8> Bytecode;
		FSKString Errors;
		FSKHash64 Key{};
	};

	// A tiny interface to decouple compilers (UE: IShaderFormat / ShaderCompilerCommon).
	class ISKShaderCompiler
	{
	public:
		virtual ~ISKShaderCompiler() = default;
		virtual bool Compile(const FSKShaderCompileRequest& Request, FSKShaderCompileOutput& Out) = 0;
	};

	// Compute a stable cache key for a compile request.
	FSKHash64 SKComputeShaderKey(const FSKShaderCompileRequest& Request);
}
