#include "SKShaderCompiler/SKShaderCompiler.h"
#include "SKCore/SKCoreLog.h"

#include <fstream>
#include <sstream>
#include <filesystem>
#include <unordered_map>
#include <cstring>

#if SK_PLATFORM_WINDOWS
	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif
	#include <windows.h>
	#include <d3dcompiler.h>
	#pragma comment(lib, "d3dcompiler.lib")
#endif

namespace Skylark
{
	static bool SKLoadFileToString(const FSKString& Path, FSKString& Out)
	{
		std::ifstream F(Path, std::ios::binary);
		if (!F)
		{
			return false;
		}
		std::ostringstream SS;
		SS << F.rdbuf();
		Out = SS.str();
		return true;
	}

#if SK_PLATFORM_WINDOWS
	class FSKD3DInclude final : public ID3DInclude
	{
	public:
		explicit FSKD3DInclude(TArray<FSKString> InIncludeDirs)
			: IncludeDirs(std::move(InIncludeDirs))
		{
		}

		HRESULT Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR FileName, LPCVOID ParentData, LPCVOID* Data, UINT* Bytes) override
		{
			(void)IncludeType;
			(void)ParentData;
			if (!FileName || !Data || !Bytes)
			{
				return E_FAIL;
			}

			FSKString Candidate;
			// 1) Try direct
			Candidate = FileName;
			FSKString Content;
			if (SKLoadFileToString(Candidate, Content))
			{
				return StoreBuffer(std::move(Content), Data, Bytes);
			}

			// 2) Search include dirs
			for (const auto& Dir : IncludeDirs)
			{
				std::filesystem::path P = std::filesystem::path(Dir) / FileName;
				Candidate = P.string();
				if (SKLoadFileToString(Candidate, Content))
				{
					return StoreBuffer(std::move(Content), Data, Bytes);
				}
			}

			return E_FAIL;
		}

		HRESULT Close(LPCVOID Data) override
		{
			auto It = Buffers.find(Data);
			if (It != Buffers.end())
			{
				delete[] It->second;
				Buffers.erase(It);
			}
			return S_OK;
		}

	private:
		HRESULT StoreBuffer(FSKString&& Text, LPCVOID* OutData, UINT* OutBytes)
		{
			const SIZE_T N = Text.size();
			char* Mem = new char[N + 1];
			std::memcpy(Mem, Text.data(), N);
			Mem[N] = '\0';
			*OutData = Mem;
			*OutBytes = (UINT)N;
			Buffers.emplace(*OutData, Mem);
			return S_OK;
		}

	private:
		TArray<FSKString> IncludeDirs;
		std::unordered_map<LPCVOID, char*> Buffers;
	};
#endif

	class FSKDefaultShaderCompiler final : public ISKShaderCompiler
	{
	public:
		bool Compile(const FSKShaderCompileRequest& Request, FSKShaderCompileOutput& Out) override
		{
			Out = {};
			Out.Key = SKComputeShaderKey(Request);

			FSKString Code;
			if (!Request.Source.FilePath.empty())
			{
				if (!SKLoadFileToString(Request.Source.FilePath, Code))
				{
					Out.Errors = "Failed to load shader file: " + Request.Source.FilePath;
					return false;
				}
			}
			else
			{
				Code = Request.Source.Code;
			}

#if SK_PLATFORM_WINDOWS
			TArray<D3D_SHADER_MACRO> Macros;
			Macros.reserve(Request.Defines.size() + 1);
			for (const auto& D : Request.Defines)
			{
				D3D_SHADER_MACRO M;
				M.Name = D.Name.c_str();
				M.Definition = D.Value.c_str();
				Macros.push_back(M);
			}
			Macros.push_back({ nullptr, nullptr });

			UINT Flags = D3DCOMPILE_ENABLE_STRICTNESS;
			#ifdef _DEBUG
				Flags |= D3DCOMPILE_DEBUG;
				Flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
			#else
				Flags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
			#endif

			FSKD3DInclude Include(Request.IncludeDirs);

			ID3DBlob* ShaderBlob = nullptr;
			ID3DBlob* ErrorBlob = nullptr;

			const HRESULT Hr = D3DCompile(
				Code.data(),
				Code.size(),
				Request.DebugName.empty() ? nullptr : Request.DebugName.c_str(),
				Macros.data(),
				&Include,
				Request.EntryPoint.c_str(),
				Request.Profile.c_str(),
				Flags,
				0,
				&ShaderBlob,
				&ErrorBlob);

			if (FAILED(Hr) || !ShaderBlob)
			{
				if (ErrorBlob)
				{
					Out.Errors.assign((const char*)ErrorBlob->GetBufferPointer(), ErrorBlob->GetBufferSize());
					ErrorBlob->Release();
				}
				Out.bSucceeded = false;
				return false;
			}

			const uint8* Bytes = (const uint8*)ShaderBlob->GetBufferPointer();
			const SIZE_T Size = (SIZE_T)ShaderBlob->GetBufferSize();
			Out.Bytecode.assign(Bytes, Bytes + Size);
			Out.bSucceeded = true;
			ShaderBlob->Release();
			if (ErrorBlob) ErrorBlob->Release();
			return true;
#else
			(void)Request;
			Out.Errors = "Shader compilation is not available on this platform/toolchain. Build with NullRHI or provide a platform compiler.";
			Out.bSucceeded = false;
			return false;
#endif
		}
	};

	TUniquePtr<ISKShaderCompiler> SKCreateDefaultShaderCompiler()
	{
		return std::make_unique<FSKDefaultShaderCompiler>();
	}
}
