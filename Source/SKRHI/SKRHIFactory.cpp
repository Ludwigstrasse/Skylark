#include "SKRHI/SKRHIFactory.h"
#include "SKRHI/SKNullRHI/SKNullRHI.h"

#if defined(SK_WITH_D3D11_RHI) && SK_WITH_D3D11_RHI
	#include "SKD3D11RHI/SKD3D11RHI.h"
#endif

#if defined(SK_WITH_D3D12_RHI) && SK_WITH_D3D12_RHI
	#include "SKD3D12RHI/SKD3D12RHI.h"
#endif

#if defined(SK_WITH_OPENGL_RHI) && SK_WITH_OPENGL_RHI
	#include "SKOpenGLRHI/SKOpenGLRHI.h"
#endif

#if defined(SK_WITH_VULKAN_RHI) && SK_WITH_VULKAN_RHI
	#include "SKVulkanRHI/SKVulkanRHI.h"
#endif

#include <algorithm>
#include <cctype>
#include <mutex>

namespace Skylark
{
	namespace
	{
		struct FRegistry
		{
			std::mutex Mutex;
			TMap<int32, FSKRHIBackendDesc> ByApi;
			TMap<FSKString, int32> NameToApi;
			bool bBuiltinsRegistered = false;
		};

		FRegistry& SKGetRegistry()
		{
			static FRegistry G;
			return G;
		}

		FSKString SKNormalizeName(const char* Text)
		{
			FSKString Out;
			if (!Text)
			{
				return Out;
			}
			while (*Text)
			{
				Out.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(*Text))));
				++Text;
			}
			return Out;
		}

		void SKRegisterBuiltins_NoLock(FRegistry& Registry)
		{
			if (Registry.bBuiltinsRegistered)
			{
				return;
			}

			auto RegisterNoLock = [&](const FSKRHIBackendDesc& Desc)
			{
				Registry.ByApi[(int32)Desc.Api] = Desc;
				Registry.NameToApi[SKNormalizeName(Desc.Name)] = (int32)Desc.Api;
			};

			auto AlwaysSupported = []() -> bool
			{
				return true;
			};

			Registry.bBuiltinsRegistered = true;

			RegisterNoLock({ ESKRHIApi::Null, "null", []() -> TUniquePtr<ISKRHIDevice>
			{
				return std::make_unique<FSKNullRHIDevice>();
			}, AlwaysSupported });

#if defined(SK_WITH_D3D11_RHI) && SK_WITH_D3D11_RHI
			RegisterNoLock({ ESKRHIApi::D3D11, "d3d11", &SKCreateD3D11RHIDevice, AlwaysSupported });
#endif

#if defined(SK_WITH_D3D12_RHI) && SK_WITH_D3D12_RHI
			RegisterNoLock({ ESKRHIApi::D3D12, "d3d12", &SKCreateD3D12RHIDevice, AlwaysSupported });
#endif

#if defined(SK_WITH_OPENGL_RHI) && SK_WITH_OPENGL_RHI
			RegisterNoLock({ ESKRHIApi::OpenGL, "opengl", &SKCreateOpenGLRHIDevice, AlwaysSupported });
#endif
#if defined(SK_WITH_VULKAN_RHI) && SK_WITH_VULKAN_RHI
			RegisterNoLock({ ESKRHIApi::Vulkan, "vulkan", &SKCreateVulkanRHIDevice, &SKIsVulkanRHISupported });
#endif
		}

		void SKEnsureBuiltinsRegistered()
		{
			FRegistry& Registry = SKGetRegistry();
			std::lock_guard<std::mutex> Lock(Registry.Mutex);
			SKRegisterBuiltins_NoLock(Registry);
		}
	}

	void SKRegisterRHIBackend(const FSKRHIBackendDesc& Desc)
	{
		if (!Desc.Name || !Desc.CreateDevice)
		{
			return;
		}

		FRegistry& Registry = SKGetRegistry();
		std::lock_guard<std::mutex> Lock(Registry.Mutex);
		Registry.ByApi[(int32)Desc.Api] = Desc;
		Registry.NameToApi[SKNormalizeName(Desc.Name)] = (int32)Desc.Api;
	}

	const FSKRHIBackendDesc* SKFindRHIBackend(ESKRHIApi Api)
	{
		SKEnsureBuiltinsRegistered();
		FRegistry& Registry = SKGetRegistry();
		std::lock_guard<std::mutex> Lock(Registry.Mutex);
		auto It = Registry.ByApi.find((int32)Api);
		return (It != Registry.ByApi.end()) ? &It->second : nullptr;
	}

	const FSKRHIBackendDesc* SKFindRHIBackendByName(const char* Name)
	{
		SKEnsureBuiltinsRegistered();
		FRegistry& Registry = SKGetRegistry();
		std::lock_guard<std::mutex> Lock(Registry.Mutex);
		auto ItName = Registry.NameToApi.find(SKNormalizeName(Name));
		if (ItName == Registry.NameToApi.end())
		{
			return nullptr;
		}
		auto ItApi = Registry.ByApi.find(ItName->second);
		return (ItApi != Registry.ByApi.end()) ? &ItApi->second : nullptr;
	}

	bool SKIsRHIBackendSupported(ESKRHIApi Api)
	{
		if (const FSKRHIBackendDesc* Backend = SKFindRHIBackend(Api))
		{
			return Backend->IsSupported ? Backend->IsSupported() : true;
		}
		return false;
	}

	bool SKIsRHIBackendNameSupported(const char* Name)
	{
		if (const FSKRHIBackendDesc* Backend = SKFindRHIBackendByName(Name))
		{
			return Backend->IsSupported ? Backend->IsSupported() : true;
		}
		return false;
	}

	const char* SKRHIApiToString(ESKRHIApi Api)
	{
		switch (Api)
		{
		case ESKRHIApi::Null:   return "null";
		case ESKRHIApi::D3D11:  return "d3d11";
		case ESKRHIApi::D3D12:  return "d3d12";
		case ESKRHIApi::OpenGL: return "opengl";
		case ESKRHIApi::Vulkan: return "vulkan";
		case ESKRHIApi::Metal:  return "metal";
		default:                return "unknown";
		}
	}

	bool SKTryParseRHIApi(const char* Text, ESKRHIApi& OutApi)
	{
		const FSKString Name = SKNormalizeName(Text);
		if (Name == "null")   { OutApi = ESKRHIApi::Null; return true; }
		if (Name == "d3d11")  { OutApi = ESKRHIApi::D3D11; return true; }
		if (Name == "d3d12")  { OutApi = ESKRHIApi::D3D12; return true; }
		if (Name == "opengl" || Name == "gl") { OutApi = ESKRHIApi::OpenGL; return true; }
		if (Name == "vulkan" || Name == "vk") { OutApi = ESKRHIApi::Vulkan; return true; }
		if (Name == "metal")  { OutApi = ESKRHIApi::Metal; return true; }
		return false;
	}

	ESKRHIApi SKGetDefaultRHIApiForPlatform()
	{
#if defined(SK_PLATFORM_WINDOWS) && SK_PLATFORM_WINDOWS
		if (SKIsRHIBackendSupported(ESKRHIApi::D3D12)) return ESKRHIApi::D3D12;
		if (SKIsRHIBackendSupported(ESKRHIApi::D3D11)) return ESKRHIApi::D3D11;
		if (SKIsRHIBackendSupported(ESKRHIApi::OpenGL)) return ESKRHIApi::OpenGL;
		return ESKRHIApi::Null;
#elif defined(SK_PLATFORM_MAC) && SK_PLATFORM_MAC
		if (SKIsRHIBackendSupported(ESKRHIApi::Metal)) return ESKRHIApi::Metal;
		if (SKIsRHIBackendSupported(ESKRHIApi::OpenGL)) return ESKRHIApi::OpenGL;
		return ESKRHIApi::Null;
#else
		if (SKIsRHIBackendSupported(ESKRHIApi::Vulkan)) return ESKRHIApi::Vulkan;
		if (SKIsRHIBackendSupported(ESKRHIApi::OpenGL)) return ESKRHIApi::OpenGL;
		return ESKRHIApi::Null;
#endif
	}

	TUniquePtr<ISKRHIDevice> SKCreateRHIDevice(const FSKRHIDeviceDesc& Desc)
	{
		if (const FSKRHIBackendDesc* Backend = SKFindRHIBackend(Desc.Api))
		{
			if (Backend->CreateDevice)
			{
				return Backend->CreateDevice();
			}
		}
		return std::make_unique<FSKNullRHIDevice>();
	}
}
