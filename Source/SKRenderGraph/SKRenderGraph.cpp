#include "SKRenderGraph/SKRenderGraph.h"
#include "SKCore/SKAssert.h"
#include "SKCore/SKCoreLog.h"

#include <unordered_map>
#include <queue>

namespace Skylark
{
	void FSKRGPassBuilder::ReadTexture(FSKRGTextureHandle Handle)
	{
		if (Handle.IsValid())
		{
			Pass.ReadTextures.push_back(Handle);
		}
	}

	void FSKRGPassBuilder::WriteTexture(FSKRGTextureHandle Handle)
	{
		if (Handle.IsValid())
		{
			Pass.WriteTextures.push_back(Handle);
		}
	}

	void FSKRenderGraphBuilder::Reset()
	{
		Textures.clear();
		Passes.clear();
		ExecutionOrder.clear();
		bCompiled = false;
	}

	FSKRenderGraphBuilder::FSKRGTexture* FSKRenderGraphBuilder::FindTextureByName(const FSKStringView Name)
	{
		for (auto& T : Textures)
		{
			if (T.Name == Name)
			{
				return &T;
			}
		}
		return nullptr;
	}

	FSKRGTextureHandle FSKRenderGraphBuilder::CreateTexture(const FSKStringView DebugName, const FSKRHITextureDesc& Desc)
	{
		FSKRGTexture Tex{};
		Tex.Name = FSKString(DebugName);
		Tex.Desc = Desc;
		Textures.push_back(std::move(Tex));
		return FSKRGTextureHandle{ static_cast<uint32>(Textures.size()) };
	}

	FSKRGTextureHandle FSKRenderGraphBuilder::GetOrCreateTexture(const FSKStringView DebugName, const FSKRHITextureDesc& Desc)
	{
		if (auto* Existing = FindTextureByName(DebugName))
		{
			// Keep first desc; assume stable per-frame.
			(void)Desc;
			const uint32 Id = static_cast<uint32>((Existing - &Textures[0]) + 1);
			return FSKRGTextureHandle{ Id };
		}
		return CreateTexture(DebugName, Desc);
	}

	FSKRGTextureHandle FSKRenderGraphBuilder::RegisterExternalTexture(const FSKStringView DebugName, ISKRHITexture2D& Texture)
	{
		// If name exists, overwrite external pointer (same handle)
		if (auto* Existing = FindTextureByName(DebugName))
		{
			Existing->External = &Texture;
			Existing->bExternal = true;
			Existing->Runtime.reset();
			Existing->Desc = Texture.GetDesc();
			const uint32 Id = static_cast<uint32>((Existing - &Textures[0]) + 1);
			return FSKRGTextureHandle{ Id };
		}

		FSKRGTexture Tex{};
		Tex.Name = FSKString(DebugName);
		Tex.Desc = Texture.GetDesc();
		Tex.External = &Texture;
		Tex.bExternal = true;
		Textures.push_back(std::move(Tex));
		return FSKRGTextureHandle{ static_cast<uint32>(Textures.size()) };
	}


	void FSKRenderGraphBuilder::AddPass(const FSKStringView Name, FSKRGPassSetup Setup, FSKRGPassExecute Execute)
	{
		FSKRGPass Pass{};
		Pass.Name = FSKString(Name);
		Pass.Execute = std::move(Execute);

		if (Setup)
		{
			FSKRGPassBuilder Builder(Pass);
			Setup(Builder);
		}

		Passes.push_back(std::move(Pass));
		bCompiled = false;
	}

	static void SKComputeDependencies(TArray<FSKRenderGraphBuilder::FSKRGPass>& Passes)
	{
		// Last writer per texture
		std::unordered_map<uint32, uint32> LastWriter; // TexId -> PassIndex

		for (uint32 PassIndex = 0; PassIndex < (uint32)Passes.size(); ++PassIndex)
		{
			auto& P = Passes[PassIndex];

			// Reads depend on last writer
			for (const auto& Tex : P.ReadTextures)
			{
				if (!Tex.IsValid()) continue;
				auto It = LastWriter.find(Tex.Id);
				if (It != LastWriter.end())
				{
					P.DependsOn.push_back(It->second);
				}
			}

			// Writes depend on last writer (write-after-write), then update last writer
			for (const auto& Tex : P.WriteTextures)
			{
				if (!Tex.IsValid()) continue;
				auto It = LastWriter.find(Tex.Id);
				if (It != LastWriter.end())
				{
					P.DependsOn.push_back(It->second);
				}
				LastWriter[Tex.Id] = PassIndex;
			}

			// Dedup dependencies (small sizes, O(n^2) acceptable)
			TArray<uint32> Unique;
			for (uint32 D : P.DependsOn)
			{
				bool bFound = false;
				for (uint32 U : Unique)
				{
					if (U == D) { bFound = true; break; }
				}
				if (!bFound && D != PassIndex)
				{
					Unique.push_back(D);
				}
			}
			P.DependsOn = std::move(Unique);
		}
	}

	static bool SKTopoSort(const TArray<FSKRenderGraphBuilder::FSKRGPass>& Passes, TArray<uint32>& OutOrder)
	{
		const uint32 N = (uint32)Passes.size();
		OutOrder.clear();
		OutOrder.reserve(N);

		TArray<uint32> InDegree;
		InDegree.resize(N, 0);

		TArray<TArray<uint32>> OutEdges;
		OutEdges.resize(N);

		for (uint32 i = 0; i < N; ++i)
		{
			for (uint32 dep : Passes[i].DependsOn)
			{
				if (dep >= N) continue;
				InDegree[i] += 1;
				OutEdges[dep].push_back(i);
			}
		}

		std::queue<uint32> Q;
		for (uint32 i = 0; i < N; ++i)
		{
			if (InDegree[i] == 0)
			{
				Q.push(i);
			}
		}

		while (!Q.empty())
		{
			uint32 v = Q.front();
			Q.pop();
			OutOrder.push_back(v);

			for (uint32 to : OutEdges[v])
			{
				SK_CHECK(to < N);
				if (InDegree[to] > 0)
				{
					InDegree[to] -= 1;
					if (InDegree[to] == 0)
					{
						Q.push(to);
					}
				}
			}
		}

		return OutOrder.size() == N;
	}

	void FSKRenderGraphBuilder::Compile()
	{
		SKComputeDependencies(Passes);

		if (!SKTopoSort(Passes, ExecutionOrder))
		{
			SK_LOG(GLogSkylark, ESKLogVerbosity::Error, "RenderGraph: dependency cycle detected. Falling back to insertion order.");
			ExecutionOrder.clear();
			for (uint32 i = 0; i < (uint32)Passes.size(); ++i)
			{
				ExecutionOrder.push_back(i);
			}
		}

		bCompiled = true;
	}

	struct FSKRGContextImpl final : public FSKRGPassContext
	{
		const TArray<FSKRenderGraphBuilder::FSKRGTexture>* Textures = nullptr;

		ISKRHITexture2D* GetTexture(FSKRGTextureHandle Handle) const override
		{
			if (!Textures || !Handle.IsValid())
			{
				return nullptr;
			}
			const uint32 Index = Handle.Id - 1;
			if (Index >= (uint32)Textures->size())
			{
				return nullptr;
			}
			auto& T = (*Textures)[Index];
			return T.bExternal ? T.External : T.Runtime.get();
		}
	};

	void FSKRenderGraphBuilder::Execute(ISKRHIDevice& Device, ISKRHISwapChain& SwapChain)
	{
		if (!bCompiled)
		{
			Compile();
		}

		// Allocate frame-local resources
		for (auto& T : Textures)
		{
			if (T.bExternal)
			{
				continue;
			}
			if (!T.Runtime)
			{
				T.Runtime = Device.CreateTexture2D(T.Desc);
				if (!T.Runtime)
				{
					SK_LOG(GLogSkylark, ESKLogVerbosity::Error, "RenderGraph: failed to create texture '%s'.", T.Name.c_str());
				}
			}
		}

		FSKRGContextImpl Ctx;
		Ctx.Device = &Device;
		Ctx.Cmd = &Device.GetImmediateCommandList();
		Ctx.SwapChain = &SwapChain;
		Ctx.Textures = &Textures;

		for (uint32 PassIndex : ExecutionOrder)
		{
			if (PassIndex >= (uint32)Passes.size())
			{
				continue;
			}

			auto& P = Passes[PassIndex];
			if (P.Execute)
			{
				P.Execute(Ctx);
			}
		}
	}

	FSKString FSKRenderGraphBuilder::DumpJson() const
	{
		// Minimal JSON for tooling/CI inspection
		FSKString Json;
		Json += "{\n  \"textures\": [\n";
		for (SIZE_T i = 0; i < Textures.size(); ++i)
		{
			const auto& T = Textures[i];
			Json += "    {\"id\": ";
			Json += std::to_string((uint32)(i + 1));
			Json += ", \"name\": \"";
			Json += T.Name;
			Json += "\", \"w\": ";
			Json += std::to_string(T.Desc.Width);
			Json += ", \"h\": ";
			Json += std::to_string(T.Desc.Height);
			Json += "}";
			if (i + 1 < Textures.size()) Json += ",";
			Json += "\n";
		}
		Json += "  ],\n  \"passes\": [\n";
		for (SIZE_T i = 0; i < Passes.size(); ++i)
		{
			const auto& P = Passes[i];
			Json += "    {\"id\": ";
			Json += std::to_string((uint32)i);
			Json += ", \"name\": \"";
			Json += P.Name;
			Json += "\", \"deps\": [";
			for (SIZE_T k = 0; k < P.DependsOn.size(); ++k)
			{
				Json += std::to_string(P.DependsOn[k]);
				if (k + 1 < P.DependsOn.size()) Json += ", ";
			}
			Json += "]}";
			if (i + 1 < Passes.size()) Json += ",";
			Json += "\n";
		}
		Json += "  ],\n  \"exec\": [";
		for (SIZE_T i = 0; i < ExecutionOrder.size(); ++i)
		{
			Json += std::to_string(ExecutionOrder[i]);
			if (i + 1 < ExecutionOrder.size()) Json += ", ";
		}
		Json += "]\n}\n";
		return Json;
	}

} // namespace Skylark
