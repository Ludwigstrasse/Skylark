#pragma once
#include "SKCore/SKCoreMinimal.h"
#include "SKRHI/SKRHI.h"

#include <functional>

namespace Skylark
{
	/**
	 * SKRenderGraph (UE5 RDG-aligned, minimal usable subset)
	 *
	 * V4: pass list.
	 * V5: RDG-Min:
	 *  - frame-local resources (Texture2D)
	 *  - pass read/write declarations
	 *  - dependency DAG + topological execution order
	 *  - simple debug dump (JSON)
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

		// Resource lookup
		virtual ISKRHITexture2D* GetTexture(FSKRGTextureHandle Handle) const = 0;

	protected:
		~FSKRGPassContext() = default;
	};

	using FSKRGPassExecute = std::function<void(const FSKRGPassContext&)>;

	class FSKRGPassBuilder
	{
	public:
		explicit FSKRGPassBuilder(struct FSKRGPass& InPass) : Pass(InPass) {}

		void ReadTexture(FSKRGTextureHandle Handle);
		void WriteTexture(FSKRGTextureHandle Handle);

	private:
		struct FSKRGPass& Pass;
	};

	using FSKRGPassSetup = std::function<void(FSKRGPassBuilder&)>;

	class FSKRenderGraphBuilder
	{
	public:
		void Reset();

		// Resource creation (frame-local)
		FSKRGTextureHandle CreateTexture(const FSKStringView DebugName, const FSKRHITextureDesc& Desc);

		// Named resource (like UE's RDG blackboard)
		FSKRGTextureHandle GetOrCreateTexture(const FSKStringView DebugName, const FSKRHITextureDesc& Desc);

		 		// External resource registration (non-owning)
		FSKRGTextureHandle RegisterExternalTexture(const FSKStringView DebugName, ISKRHITexture2D& Texture);

void AddPass(const FSKStringView Name, FSKRGPassSetup Setup, FSKRGPassExecute Execute);

		void Compile();
		void Execute(ISKRHIDevice& Device, ISKRHISwapChain& SwapChain);

		// Debug dump (compact JSON)
		FSKString DumpJson() const;

	private:
		struct FSKRGTexture
		{
			FSKString Name;
			FSKRHITextureDesc Desc{};
			TUniquePtr<ISKRHITexture2D> Runtime; // allocated at Execute (owned)
			ISKRHITexture2D* External = nullptr;          // non-owning
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

		FSKRGTexture* FindTextureByName(const FSKStringView Name);

	private:
		TArray<FSKRGTexture> Textures;  // Id = index+1
		TArray<FSKRGPass> Passes;

		TArray<uint32> ExecutionOrder; // pass indices
		bool bCompiled = false;
	};

} // namespace Skylark
