#pragma once
#include "SKCore/SKCoreMinimal.h"
#include "SKProfiler/SKProfiler.h"
#include "SKRHI/SKRHI.h"

#include <functional>

namespace Skylark
{
	/**
	 * SKRenderGraph (UE5 RDG-aligned, minimal usable subset)
	 *
	 * V12 fixes:
	 * - Make internal types namespace-scoped (portable C++), not private nested types.
	 * - Keep public API stable: CreateTexture/RegisterExternalTexture/AddPass/Compile/Execute/DumpJson.
	 */

	struct FSKRGTextureHandle
	{
		uint32 Id = 0;
		bool IsValid() const { return Id != 0; }
	};

	struct FSKRGPassContext
	{
		ISKRHIDevice* Device = nullptr;
		ISKRHICommandList* Cmd = nullptr;
		ISKRHISwapChain* SwapChain = nullptr;

		virtual ISKRHITexture2D* GetTexture(FSKRGTextureHandle Handle) const = 0;

	protected:
		~FSKRGPassContext() = default;
	};

	using FSKRGPassExecute = std::function<void(const FSKRGPassContext&)>;

	struct FSKRGTexture
	{
		FSKString Name;
		FSKRHITextureDesc Desc{};
		TUniquePtr<ISKRHITexture2D> Runtime; // allocated at Execute (owned)
		ISKRHITexture2D* External = nullptr; // non-owning
		bool bExternal = false;
	};

		struct FSKRGPass
		{
			FSKString Name;
			TArray<FSKRGTextureHandle> ReadTextures;
			TArray<FSKRGTextureHandle> WriteTextures;
			TArray<uint32> DependsOn; // pass indices
			FSKRGPassExecute Execute;
		};

	class FSKRGPassBuilder
	{
	public:
		explicit FSKRGPassBuilder(FSKRGPass& InPass) : Pass(&InPass) {}

		void ReadTexture(FSKRGTextureHandle Handle);
		void WriteTexture(FSKRGTextureHandle Handle);

	private:
		FSKRGPass* Pass = nullptr;
	};

	using FSKRGPassSetup = std::function<void(FSKRGPassBuilder&)>;

	class FSKRenderGraphBuilder
	{
	public:
		void SetProfiler(FSKFrameProfiler* InProfiler) { Profiler = InProfiler; }

		void Reset();

		// Resource creation (frame-local)
		FSKRGTextureHandle CreateTexture(FSKStringView DebugName, const FSKRHITextureDesc& Desc);
		FSKRGTextureHandle GetOrCreateTexture(FSKStringView DebugName, const FSKRHITextureDesc& Desc);

		// External resource registration (non-owning)
		FSKRGTextureHandle RegisterExternalTexture(FSKStringView DebugName, ISKRHITexture2D& Texture);

		void AddPass(FSKStringView Name, FSKRGPassSetup Setup, FSKRGPassExecute Execute);

		void Compile();
		void Execute(ISKRHIDevice& Device, ISKRHISwapChain& SwapChain);

		FSKString DumpJson() const;

	private:
		FSKRGTexture* FindTextureByName(FSKStringView Name);

	private:
		TArray<FSKRGTexture> Textures;  // Id = index+1
		TArray<FSKRGPass> Passes;
		TArray<uint32> ExecutionOrder;
		FSKFrameProfiler* Profiler = nullptr;
		bool bCompiled = false;
	};

} // namespace Skylark
