#include "SKVulkanRHI/SKVulkanRHI.h"
#include "SKCore/SKCoreLog.h"

#include <algorithm>
#include <cstring>
#include <vector>

#if defined(_WIN32)
	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif
	#include <windows.h>
#else
	#include <dlfcn.h>
#endif

namespace Skylark
{
	namespace VulkanMini
	{
		using VkFlags = uint32;
		using VkBool32 = uint32;
		using VkDeviceSize = uint64;
		using VkResult = int32;
		using VkStructureType = int32;
		using VkQueueFlags = VkFlags;
		using VkImageUsageFlags = VkFlags;
		using VkInstanceCreateFlags = VkFlags;
		using VkDeviceCreateFlags = VkFlags;
		using VkDeviceQueueCreateFlags = VkFlags;
		using VkSwapchainCreateFlagsKHR = VkFlags;
		using VkSemaphoreCreateFlags = VkFlags;
		using VkCommandPoolCreateFlags = VkFlags;
		using VkCommandBufferUsageFlags = VkFlags;
		using VkSurfaceTransformFlagsKHR = VkFlags;
		using VkCompositeAlphaFlagsKHR = VkFlags;
		using VkPresentModeKHR = int32;
		using VkFormat = int32;
		using VkColorSpaceKHR = int32;
		using VkSharingMode = int32;

		struct VkInstance_T;
		struct VkPhysicalDevice_T;
		struct VkDevice_T;
		struct VkQueue_T;
		struct VkImage_T;
		struct VkSurfaceKHR_T;
		struct VkSwapchainKHR_T;
		struct VkSemaphore_T;
		struct VkFence_T;
		struct VkCommandPool_T;
		struct VkCommandBuffer_T;

		using VkInstance = VkInstance_T*;
		using VkPhysicalDevice = VkPhysicalDevice_T*;
		using VkDevice = VkDevice_T*;
		using VkQueue = VkQueue_T*;
		using VkImage = VkImage_T*;
		using VkSurfaceKHR = VkSurfaceKHR_T*;
		using VkSwapchainKHR = VkSwapchainKHR_T*;
		using VkSemaphore = VkSemaphore_T*;
		using VkFence = VkFence_T*;
		using VkCommandPool = VkCommandPool_T*;
		using VkCommandBuffer = VkCommandBuffer_T*;

		constexpr VkResult VK_SUCCESS = 0;
		constexpr VkResult VK_SUBOPTIMAL_KHR = 1000001003;
		constexpr VkResult VK_ERROR_INITIALIZATION_FAILED = -3;
		constexpr VkResult VK_ERROR_LAYER_NOT_PRESENT = -6;
		constexpr VkResult VK_ERROR_EXTENSION_NOT_PRESENT = -7;
		constexpr VkResult VK_ERROR_INCOMPATIBLE_DRIVER = -9;
		constexpr VkResult VK_ERROR_OUT_OF_DATE_KHR = -1000001004;

		constexpr VkBool32 VK_FALSE = 0;
		constexpr VkBool32 VK_TRUE = 1;

		constexpr VkQueueFlags VK_QUEUE_GRAPHICS_BIT = 0x00000001u;
		constexpr VkImageUsageFlags VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT = 0x00000010u;
		constexpr VkCommandPoolCreateFlags VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT = 0x00000002u;
		constexpr VkCommandBufferUsageFlags VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT = 0x00000001u;
		constexpr VkSurfaceTransformFlagsKHR VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR = 0x00000001u;
		constexpr VkCompositeAlphaFlagsKHR VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR = 0x00000001u;
		constexpr VkSharingMode VK_SHARING_MODE_EXCLUSIVE = 0;
		constexpr VkPresentModeKHR VK_PRESENT_MODE_MAILBOX_KHR = 1;
		constexpr VkPresentModeKHR VK_PRESENT_MODE_FIFO_KHR = 2;
		constexpr VkFormat VK_FORMAT_B8G8R8A8_UNORM = 44;
		constexpr VkColorSpaceKHR VK_COLOR_SPACE_SRGB_NONLINEAR_KHR = 0;
		constexpr uint32 VK_SURFACE_UNDEFINED_EXTENT = 0xFFFFFFFFu;

		enum : VkStructureType
		{
			VK_STRUCTURE_TYPE_APPLICATION_INFO = 0,
			VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO = 1,
			VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO = 2,
			VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO = 3,
			VK_STRUCTURE_TYPE_SUBMIT_INFO = 4,
			VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO = 9,
			VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO = 38,
			VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO = 39,
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO = 40,
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO = 42,
			VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR = 1000001000,
			VK_STRUCTURE_TYPE_PRESENT_INFO_KHR = 1000001001,
			VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR = 1000009000,
		};

		enum : int32
		{
			VK_COMMAND_BUFFER_LEVEL_PRIMARY = 0,
		};

		struct VkApplicationInfo
		{
			VkStructureType sType;
			const void* pNext;
			const char* pApplicationName;
			uint32 applicationVersion;
			const char* pEngineName;
			uint32 engineVersion;
			uint32 apiVersion;
		};

		struct VkInstanceCreateInfo
		{
			VkStructureType sType;
			const void* pNext;
			VkInstanceCreateFlags flags;
			const VkApplicationInfo* pApplicationInfo;
			uint32 enabledLayerCount;
			const char* const* ppEnabledLayerNames;
			uint32 enabledExtensionCount;
			const char* const* ppEnabledExtensionNames;
		};

		struct VkDeviceQueueCreateInfo
		{
			VkStructureType sType;
			const void* pNext;
			VkDeviceQueueCreateFlags flags;
			uint32 queueFamilyIndex;
			uint32 queueCount;
			const float* pQueuePriorities;
		};

		struct VkDeviceCreateInfo
		{
			VkStructureType sType;
			const void* pNext;
			VkDeviceCreateFlags flags;
			uint32 queueCreateInfoCount;
			const VkDeviceQueueCreateInfo* pQueueCreateInfos;
			uint32 enabledLayerCount;
			const char* const* ppEnabledLayerNames;
			uint32 enabledExtensionCount;
			const char* const* ppEnabledExtensionNames;
			const void* pEnabledFeatures;
		};

		struct VkQueueFamilyProperties
		{
			VkQueueFlags queueFlags;
			uint32 queueCount;
			uint32 timestampValidBits;
			struct
			{
				uint32 width;
				uint32 height;
				uint32 depth;
			} minImageTransferGranularity;
		};

		struct VkPhysicalDeviceLimits { uint8 Reserved[512]; };
		struct VkPhysicalDeviceSparseProperties { VkBool32 Reserved[5]; };

		struct VkPhysicalDeviceProperties
		{
			uint32 apiVersion;
			uint32 driverVersion;
			uint32 vendorID;
			uint32 deviceID;
			uint32 deviceType;
			char deviceName[256];
			uint8 pipelineCacheUUID[16];
			VkPhysicalDeviceLimits limits;
			VkPhysicalDeviceSparseProperties sparseProperties;
		};

		struct VkExtensionProperties
		{
			char extensionName[256];
			uint32 specVersion;
		};

		struct VkExtent2D
		{
			uint32 width;
			uint32 height;
		};

		struct VkSurfaceCapabilitiesKHR
		{
			uint32 minImageCount;
			uint32 maxImageCount;
			VkExtent2D currentExtent;
			VkExtent2D minImageExtent;
			VkExtent2D maxImageExtent;
			uint32 maxImageArrayLayers;
			VkSurfaceTransformFlagsKHR supportedTransforms;
			VkSurfaceTransformFlagsKHR currentTransform;
			VkCompositeAlphaFlagsKHR supportedCompositeAlpha;
			VkImageUsageFlags supportedUsageFlags;
		};

		struct VkSurfaceFormatKHR
		{
			VkFormat format;
			VkColorSpaceKHR colorSpace;
		};

		struct VkSwapchainCreateInfoKHR
		{
			VkStructureType sType;
			const void* pNext;
			VkSwapchainCreateFlagsKHR flags;
			VkSurfaceKHR surface;
			uint32 minImageCount;
			VkFormat imageFormat;
			VkColorSpaceKHR imageColorSpace;
			VkExtent2D imageExtent;
			uint32 imageArrayLayers;
			VkImageUsageFlags imageUsage;
			VkSharingMode imageSharingMode;
			uint32 queueFamilyIndexCount;
			const uint32* pQueueFamilyIndices;
			VkSurfaceTransformFlagsKHR preTransform;
			VkCompositeAlphaFlagsKHR compositeAlpha;
			VkPresentModeKHR presentMode;
			VkBool32 clipped;
			VkSwapchainKHR oldSwapchain;
		};

		struct VkCommandPoolCreateInfo
		{
			VkStructureType sType;
			const void* pNext;
			VkCommandPoolCreateFlags flags;
			uint32 queueFamilyIndex;
		};

		struct VkCommandBufferAllocateInfo
		{
			VkStructureType sType;
			const void* pNext;
			VkCommandPool commandPool;
			int32 level;
			uint32 commandBufferCount;
		};

		struct VkCommandBufferBeginInfo
		{
			VkStructureType sType;
			const void* pNext;
			VkCommandBufferUsageFlags flags;
			const void* pInheritanceInfo;
		};

		struct VkSubmitInfo
		{
			VkStructureType sType;
			const void* pNext;
			uint32 waitSemaphoreCount;
			const VkSemaphore* pWaitSemaphores;
			const VkFlags* pWaitDstStageMask;
			uint32 commandBufferCount;
			const VkCommandBuffer* pCommandBuffers;
			uint32 signalSemaphoreCount;
			const VkSemaphore* pSignalSemaphores;
		};

		struct VkPresentInfoKHR
		{
			VkStructureType sType;
			const void* pNext;
			uint32 waitSemaphoreCount;
			const VkSemaphore* pWaitSemaphores;
			uint32 swapchainCount;
			const VkSwapchainKHR* pSwapchains;
			const uint32* pImageIndices;
			VkResult* pResults;
		};

		struct VkSemaphoreCreateInfo
		{
			VkStructureType sType;
			const void* pNext;
			VkSemaphoreCreateFlags flags;
		};

#if defined(_WIN32)
		struct VkWin32SurfaceCreateInfoKHR
		{
			VkStructureType sType;
			const void* pNext;
			VkFlags flags;
			HINSTANCE hinstance;
			HWND hwnd;
		};
#endif

		using PFN_vkVoidFunction = void(*)();
		using PFN_vkGetInstanceProcAddr = PFN_vkVoidFunction(*)(VkInstance, const char*);
		using PFN_vkCreateInstance = VkResult(*)(const VkInstanceCreateInfo*, const void*, VkInstance*);
		using PFN_vkEnumerateInstanceExtensionProperties = VkResult(*)(const char*, uint32*, VkExtensionProperties*);
		using PFN_vkDestroyInstance = void(*)(VkInstance, const void*);
		using PFN_vkEnumeratePhysicalDevices = VkResult(*)(VkInstance, uint32*, VkPhysicalDevice*);
		using PFN_vkGetPhysicalDeviceProperties = void(*)(VkPhysicalDevice, VkPhysicalDeviceProperties*);
		using PFN_vkGetPhysicalDeviceQueueFamilyProperties = void(*)(VkPhysicalDevice, uint32*, VkQueueFamilyProperties*);
		using PFN_vkEnumerateDeviceExtensionProperties = VkResult(*)(VkPhysicalDevice, const char*, uint32*, VkExtensionProperties*);
		using PFN_vkCreateDevice = VkResult(*)(VkPhysicalDevice, const VkDeviceCreateInfo*, const void*, VkDevice*);
		using PFN_vkGetDeviceProcAddr = PFN_vkVoidFunction(*)(VkDevice, const char*);
		using PFN_vkDestroyDevice = void(*)(VkDevice, const void*);
		using PFN_vkGetDeviceQueue = void(*)(VkDevice, uint32, uint32, VkQueue*);
		using PFN_vkGetPhysicalDeviceSurfaceSupportKHR = VkResult(*)(VkPhysicalDevice, uint32, VkSurfaceKHR, VkBool32*);
		using PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR = VkResult(*)(VkPhysicalDevice, VkSurfaceKHR, VkSurfaceCapabilitiesKHR*);
		using PFN_vkGetPhysicalDeviceSurfaceFormatsKHR = VkResult(*)(VkPhysicalDevice, VkSurfaceKHR, uint32*, VkSurfaceFormatKHR*);
		using PFN_vkGetPhysicalDeviceSurfacePresentModesKHR = VkResult(*)(VkPhysicalDevice, VkSurfaceKHR, uint32*, VkPresentModeKHR*);
		using PFN_vkDestroySurfaceKHR = void(*)(VkInstance, VkSurfaceKHR, const void*);
#if defined(_WIN32)
		using PFN_vkCreateWin32SurfaceKHR = VkResult(*)(VkInstance, const VkWin32SurfaceCreateInfoKHR*, const void*, VkSurfaceKHR*);
#endif
		using PFN_vkCreateSwapchainKHR = VkResult(*)(VkDevice, const VkSwapchainCreateInfoKHR*, const void*, VkSwapchainKHR*);
		using PFN_vkDestroySwapchainKHR = void(*)(VkDevice, VkSwapchainKHR, const void*);
		using PFN_vkGetSwapchainImagesKHR = VkResult(*)(VkDevice, VkSwapchainKHR, uint32*, VkImage*);
		using PFN_vkAcquireNextImageKHR = VkResult(*)(VkDevice, VkSwapchainKHR, uint64, VkSemaphore, VkFence, uint32*);
		using PFN_vkQueuePresentKHR = VkResult(*)(VkQueue, const VkPresentInfoKHR*);
		using PFN_vkCreateSemaphore = VkResult(*)(VkDevice, const VkSemaphoreCreateInfo*, const void*, VkSemaphore*);
		using PFN_vkDestroySemaphore = void(*)(VkDevice, VkSemaphore, const void*);
		using PFN_vkDeviceWaitIdle = VkResult(*)(VkDevice);
		using PFN_vkCreateCommandPool = VkResult(*)(VkDevice, const VkCommandPoolCreateInfo*, const void*, VkCommandPool*);
		using PFN_vkDestroyCommandPool = void(*)(VkDevice, VkCommandPool, const void*);
		using PFN_vkAllocateCommandBuffers = VkResult(*)(VkDevice, const VkCommandBufferAllocateInfo*, VkCommandBuffer*);
		using PFN_vkBeginCommandBuffer = VkResult(*)(VkCommandBuffer, const VkCommandBufferBeginInfo*);
		using PFN_vkEndCommandBuffer = VkResult(*)(VkCommandBuffer);
		using PFN_vkQueueSubmit = VkResult(*)(VkQueue, uint32, const VkSubmitInfo*, VkFence);

		struct FLoader
		{
#if defined(_WIN32)
			HMODULE Module = nullptr;
#else
			void* Module = nullptr;
#endif
			PFN_vkGetInstanceProcAddr GetInstanceProcAddr = nullptr;
			PFN_vkCreateInstance CreateInstance = nullptr;
			PFN_vkEnumerateInstanceExtensionProperties EnumerateInstanceExtensionProperties = nullptr;
			PFN_vkGetDeviceProcAddr GetDeviceProcAddr = nullptr;
		};

		static FLoader& GetLoader()
		{
			static FLoader G;
			return G;
		}

		static void* LoadSymbol(FLoader& Loader, const char* Name)
		{
#if defined(_WIN32)
			return Loader.Module ? reinterpret_cast<void*>(::GetProcAddress(Loader.Module, Name)) : nullptr;
#else
			return Loader.Module ? dlsym(Loader.Module, Name) : nullptr;
#endif
		}

		static bool EnsureLoader()
		{
			FLoader& Loader = GetLoader();
			if (Loader.GetInstanceProcAddr && Loader.CreateInstance)
			{
				return true;
			}

			if (!Loader.Module)
			{
#if defined(_WIN32)
				Loader.Module = ::LoadLibraryA("vulkan-1.dll");
#else
				Loader.Module = dlopen("libvulkan.so.1", RTLD_NOW | RTLD_LOCAL);
				if (!Loader.Module)
				{
					Loader.Module = dlopen("libvulkan.so", RTLD_NOW | RTLD_LOCAL);
				}
#endif
			}

			if (!Loader.Module)
			{
				return false;
			}

			Loader.GetInstanceProcAddr = reinterpret_cast<PFN_vkGetInstanceProcAddr>(LoadSymbol(Loader, "vkGetInstanceProcAddr"));
			if (!Loader.GetInstanceProcAddr)
			{
				return false;
			}

			Loader.CreateInstance = reinterpret_cast<PFN_vkCreateInstance>(Loader.GetInstanceProcAddr(nullptr, "vkCreateInstance"));
			Loader.EnumerateInstanceExtensionProperties = reinterpret_cast<PFN_vkEnumerateInstanceExtensionProperties>(Loader.GetInstanceProcAddr(nullptr, "vkEnumerateInstanceExtensionProperties"));
			Loader.GetDeviceProcAddr = reinterpret_cast<PFN_vkGetDeviceProcAddr>(Loader.GetInstanceProcAddr(nullptr, "vkGetDeviceProcAddr"));
			return Loader.CreateInstance != nullptr;
		}

		static PFN_vkVoidFunction GetInstanceProc(VkInstance Instance, const char* Name)
		{
			FLoader& Loader = GetLoader();
			if (!EnsureLoader() || !Loader.GetInstanceProcAddr)
			{
				return nullptr;
			}
			return Loader.GetInstanceProcAddr(Instance, Name);
		}

		static bool HasExtensionNamed(const std::vector<VkExtensionProperties>& Extensions, const char* Name)
		{
			for (const auto& Ext : Extensions)
			{
				if (std::strncmp(Ext.extensionName, Name, sizeof(Ext.extensionName)) == 0)
				{
					return true;
				}
			}
			return false;
		}
	}

	class FSKVulkanRHIDevice;
	class FSKVulkanGraphicsPipelineState;

	class FSKVulkanTexture2D final : public ISKRHITexture2D
	{
	public:
		explicit FSKVulkanTexture2D(const FSKRHITextureDesc& InDesc)
			: Desc(InDesc)
		{
			const uint32 W = std::max(1u, Desc.Width);
			const uint32 H = std::max(1u, Desc.Height);
			CpuPixels.resize(static_cast<SIZE_T>(W) * static_cast<SIZE_T>(H) * 4u, 0u);
		}

		const FSKRHITextureDesc& GetDesc() const override { return Desc; }

		uint8* GetPixelPtr(uint32 X, uint32 Y)
		{
			const uint32 W = std::max(1u, Desc.Width);
			const uint32 H = std::max(1u, Desc.Height);
			X = std::min(X, W - 1u);
			Y = std::min(Y, H - 1u);
			return CpuPixels.data() + ((static_cast<SIZE_T>(Y) * static_cast<SIZE_T>(W) + static_cast<SIZE_T>(X)) * 4u);
		}

	private:
		FSKRHITextureDesc Desc{};
		std::vector<uint8> CpuPixels;

		friend class FSKVulkanCommandList;
		friend class FSKVulkanRHIDevice;
		friend class FSKVulkanCommandList;
	};

	class FSKVulkanSwapChain final : public ISKRHISwapChain
	{
	public:
		FSKVulkanSwapChain(FSKVulkanRHIDevice* InOwner, const FSKRHISwapChainDesc& InDesc)
			: Owner(InOwner)
			, Desc(InDesc)
		{
		}

		~FSKVulkanSwapChain() override;

		const FSKRHISwapChainDesc& GetDesc() const override { return Desc; }

		void Resize(uint32 NewWidth, uint32 NewHeight) override
		{
			Desc.Width = (NewWidth == 0) ? 1u : NewWidth;
			Desc.Height = (NewHeight == 0) ? 1u : NewHeight;
			bResizeRequested = true;
		}

		void Present() override;

	private:
		FSKVulkanRHIDevice* Owner = nullptr;
		FSKRHISwapChainDesc Desc{};
		bool bHeadless = true;
		bool bResizeRequested = false;
		bool bInitialized = false;
		FSKRHIClearColor PendingClear{};
		uint32 AcquiredImageIndex = 0;
		uint32 ImageCount = 0;
		std::vector<VulkanMini::VkImage> Images;
		VulkanMini::VkSurfaceKHR Surface = nullptr;
		VulkanMini::VkSwapchainKHR Swapchain = nullptr;
		VulkanMini::VkSemaphore AcquireSemaphore = nullptr;

		friend class FSKVulkanRHIDevice;
		friend class FSKVulkanCommandList;
	};

	class FSKVulkanCommandList final : public ISKRHICommandList
	{
	public:
		explicit FSKVulkanCommandList(FSKVulkanRHIDevice* InOwner = nullptr)
			: Owner(InOwner)
		{
		}

		void SetOwner(FSKVulkanRHIDevice* InOwner) { Owner = InOwner; }

		void SetSwapChainRenderTarget(ISKRHISwapChain& SwapChain) override
		{
			BoundSwapChain = dynamic_cast<FSKVulkanSwapChain*>(&SwapChain);
			BoundTexture = nullptr;
		}

		void SetRenderTargetTexture(ISKRHITexture2D& ColorTarget) override
		{
			BoundTexture = dynamic_cast<FSKVulkanTexture2D*>(&ColorTarget);
			BoundSwapChain = nullptr;
		}

		void ClearRenderTarget(const FSKRHIClearColor& Color) override
		{
			if (BoundTexture)
			{
				const uint8 R = static_cast<uint8>(std::clamp(Color.R, 0.0f, 1.0f) * 255.0f);
				const uint8 G = static_cast<uint8>(std::clamp(Color.G, 0.0f, 1.0f) * 255.0f);
				const uint8 B = static_cast<uint8>(std::clamp(Color.B, 0.0f, 1.0f) * 255.0f);
				const uint8 A = static_cast<uint8>(std::clamp(Color.A, 0.0f, 1.0f) * 255.0f);

				const uint32 W = std::max(1u, BoundTexture->Desc.Width);
				const uint32 H = std::max(1u, BoundTexture->Desc.Height);
				for (uint32 Y = 0; Y < H; ++Y)
				{
					for (uint32 X = 0; X < W; ++X)
					{
						uint8* P = BoundTexture->GetPixelPtr(X, Y);
						P[0] = R; P[1] = G; P[2] = B; P[3] = A;
					}
				}
				return;
			}

			if (BoundSwapChain)
			{
				BoundSwapChain->PendingClear = Color;
			}
		}

		void BeginRenderPass(ISKRHISwapChain& SwapChain, const FSKRHIClearColor* ClearColor = nullptr) override
		{
			SetSwapChainRenderTarget(SwapChain);
			bInsideRenderPass = true;
			if (ClearColor)
			{
				ClearRenderTarget(*ClearColor);
			}
		}

		void EndRenderPass() override
		{
			bInsideRenderPass = false;
		}

		void BindGraphicsPipeline(ISKRHIGraphicsPipelineState& Pipeline) override
		{
			BoundPipeline = &Pipeline;
		}

		void DrawPrimitive(uint32 VertexCount, uint32 FirstVertex = 0u) override
		{
			RecordedVertexCount = VertexCount;
			RecordedFirstVertex = FirstVertex;
		}

		void DrawLineList(const FSKRHILineVertex* Vertices, uint32 VertexCount, const FSKRHILineDrawParams& Params) override
		{
			(void)Vertices;
			(void)VertexCount;
			(void)Params;
			RecordedVertexCount = VertexCount;
		}

		void Flush() override;

	private:
		FSKVulkanRHIDevice* Owner = nullptr;
		FSKVulkanSwapChain* BoundSwapChain = nullptr;
		FSKVulkanTexture2D* BoundTexture = nullptr;
		ISKRHIGraphicsPipelineState* BoundPipeline = nullptr;
		bool bInsideRenderPass = false;
		uint32 RecordedVertexCount = 0;
		uint32 RecordedFirstVertex = 0;
	};

	class FSKVulkanShaderModule final : public ISKRHIShaderModule
	{
	public:
		explicit FSKVulkanShaderModule(const FSKRHIShaderModuleDesc& InDesc)
			: Desc(InDesc)
		{
			const uint8* Bytes = reinterpret_cast<const uint8*>(Desc.Bytecode);
			if (Bytes && Desc.BytecodeSize > 0)
			{
				Bytecode.assign(Bytes, Bytes + Desc.BytecodeSize);
			}
		}

		const FSKRHIShaderModuleDesc& GetDesc() const override { return Desc; }

	private:
		FSKRHIShaderModuleDesc Desc{};
		std::vector<uint8> Bytecode;
	};

	class FSKVulkanGraphicsPipelineState final : public ISKRHIGraphicsPipelineState
	{
	public:
		explicit FSKVulkanGraphicsPipelineState(const FSKRHIGraphicsPipelineDesc& InDesc)
			: Desc(InDesc)
		{
		}

		const FSKRHIGraphicsPipelineDesc& GetDesc() const override { return Desc; }

	private:
		FSKRHIGraphicsPipelineDesc Desc{};
	};

	struct FSKVulkanShaderStageScaffold
	{
		FSKString DebugName;
		TArray<uint32> SpirvWords;
	};

	struct FSKVulkanPipelineScaffold
	{
		FSKString DebugName;
		bool bGraphics = true;
		bool bPresentPath = false;
		TArray<FSKVulkanShaderStageScaffold> Stages;
	};

	class FSKVulkanRHIDevice final : public ISKRHIDevice
	{
	public:
		FSKVulkanRHIDevice()
			: Immediate(this)
		{
		}

		~FSKVulkanRHIDevice() override
		{
			Shutdown();
		}

		bool Init(const FSKRHIDeviceDesc& InDesc) override
		{
			using namespace VulkanMini;

			Desc = InDesc;
			if (!EnsureLoader())
			{
				SK_LOG(GLogSkylark, ESKLogVerbosity::Warning, "SKVulkanRHI: Vulkan loader not available.");
				return false;
			}

			std::vector<const char*> InstanceExtensions;
			AvailableInstanceExtensions = QueryAvailableInstanceExtensions();
#if defined(_WIN32)
			if (HasExtensionNamed(AvailableInstanceExtensions, "VK_KHR_surface")
				&& HasExtensionNamed(AvailableInstanceExtensions, "VK_KHR_win32_surface"))
			{
				InstanceExtensions.push_back("VK_KHR_surface");
				InstanceExtensions.push_back("VK_KHR_win32_surface");
				bSurfaceInstanceExtensionsEnabled = true;
			}
			else
			{
				bSurfaceInstanceExtensionsEnabled = false;
			}
#else
			bSurfaceInstanceExtensionsEnabled = false;
#endif

			VkApplicationInfo AppInfo{};
			AppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			AppInfo.pApplicationName = "Skylark";
			AppInfo.applicationVersion = 17u;
			AppInfo.pEngineName = "SkylarkEngine";
			AppInfo.engineVersion = 17u;
			AppInfo.apiVersion = 0;

			VkInstanceCreateInfo InstanceInfo{};
			InstanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			InstanceInfo.pApplicationInfo = &AppInfo;
			InstanceInfo.enabledExtensionCount = static_cast<uint32>(InstanceExtensions.size());
			InstanceInfo.ppEnabledExtensionNames = InstanceExtensions.empty() ? nullptr : InstanceExtensions.data();

			const VkResult CreateInstanceResult = GetLoader().CreateInstance(&InstanceInfo, nullptr, &Instance);
			if (CreateInstanceResult != VK_SUCCESS || !Instance)
			{
				SK_LOG(GLogSkylark, ESKLogVerbosity::Warning, "SKVulkanRHI: vkCreateInstance failed (%d).", (int32)CreateInstanceResult);
				return false;
			}

			LoadInstanceFunctions();
			if (!EnumeratePhysicalDevices || !GetPhysicalDeviceQueueFamilyProperties || !CreateDeviceFn || !DestroyInstanceFn)
			{
				SK_LOG(GLogSkylark, ESKLogVerbosity::Warning, "SKVulkanRHI: instance function bootstrap incomplete.");
				Shutdown();
				return false;
			}

			uint32 PhysicalDeviceCount = 0;
			if (EnumeratePhysicalDevices(Instance, &PhysicalDeviceCount, nullptr) != VK_SUCCESS || PhysicalDeviceCount == 0)
			{
				SK_LOG(GLogSkylark, ESKLogVerbosity::Warning, "SKVulkanRHI: no physical devices exposed.");
				Shutdown();
				return false;
			}

			std::vector<VkPhysicalDevice> PhysicalDevices(PhysicalDeviceCount);
			if (EnumeratePhysicalDevices(Instance, &PhysicalDeviceCount, PhysicalDevices.data()) != VK_SUCCESS)
			{
				SK_LOG(GLogSkylark, ESKLogVerbosity::Warning, "SKVulkanRHI: physical device enumeration failed.");
				Shutdown();
				return false;
			}

			const uint32 RequestedIndex = (Desc.AdapterIndex >= 0 && static_cast<uint32>(Desc.AdapterIndex) < PhysicalDeviceCount)
				? static_cast<uint32>(Desc.AdapterIndex)
				: 0u;
			PhysicalDevice = PhysicalDevices[RequestedIndex];
			if (!PhysicalDevice)
			{
				Shutdown();
				return false;
			}

			if (GetPhysicalDeviceProperties)
			{
				GetPhysicalDeviceProperties(PhysicalDevice, &PhysicalDeviceProperties);
			}

			uint32 QueueFamilyCount = 0;
			GetPhysicalDeviceQueueFamilyProperties(PhysicalDevice, &QueueFamilyCount, nullptr);
			if (QueueFamilyCount == 0)
			{
				SK_LOG(GLogSkylark, ESKLogVerbosity::Warning, "SKVulkanRHI: queue family count is zero.");
				Shutdown();
				return false;
			}

			std::vector<VkQueueFamilyProperties> QueueFamilies(QueueFamilyCount);
			GetPhysicalDeviceQueueFamilyProperties(PhysicalDevice, &QueueFamilyCount, QueueFamilies.data());

			GraphicsQueueFamilyIndex = UINT32_MAX;
			for (uint32 Index = 0; Index < QueueFamilyCount; ++Index)
			{
				if ((QueueFamilies[Index].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0u && QueueFamilies[Index].queueCount > 0u)
				{
					GraphicsQueueFamilyIndex = Index;
					break;
				}
			}

			if (GraphicsQueueFamilyIndex == UINT32_MAX)
			{
				SK_LOG(GLogSkylark, ESKLogVerbosity::Warning, "SKVulkanRHI: no graphics queue family found.");
				Shutdown();
				return false;
			}

			AvailableDeviceExtensions = QueryAvailableDeviceExtensions();
			std::vector<const char*> DeviceExtensions;
			if (bSurfaceInstanceExtensionsEnabled && HasExtensionNamed(AvailableDeviceExtensions, "VK_KHR_swapchain"))
			{
				DeviceExtensions.push_back("VK_KHR_swapchain");
				bSwapchainDeviceExtensionEnabled = true;
			}
			else
			{
				bSwapchainDeviceExtensionEnabled = false;
			}

			const float GraphicsPriority = 1.0f;
			VkDeviceQueueCreateInfo QueueInfo{};
			QueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			QueueInfo.queueFamilyIndex = GraphicsQueueFamilyIndex;
			QueueInfo.queueCount = 1;
			QueueInfo.pQueuePriorities = &GraphicsPriority;

			VkDeviceCreateInfo DeviceInfo{};
			DeviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			DeviceInfo.queueCreateInfoCount = 1;
			DeviceInfo.pQueueCreateInfos = &QueueInfo;
			DeviceInfo.enabledExtensionCount = static_cast<uint32>(DeviceExtensions.size());
			DeviceInfo.ppEnabledExtensionNames = DeviceExtensions.empty() ? nullptr : DeviceExtensions.data();

			const VkResult CreateDeviceResult = CreateDeviceFn(PhysicalDevice, &DeviceInfo, nullptr, &Device);
			if (CreateDeviceResult != VK_SUCCESS || !Device)
			{
				SK_LOG(GLogSkylark, ESKLogVerbosity::Warning, "SKVulkanRHI: vkCreateDevice failed (%d).", (int32)CreateDeviceResult);
				Shutdown();
				return false;
			}

			LoadDeviceFunctions();
			if (!DestroyDeviceFn || !GetDeviceQueueFn)
			{
				SK_LOG(GLogSkylark, ESKLogVerbosity::Warning, "SKVulkanRHI: device function bootstrap incomplete.");
				Shutdown();
				return false;
			}

			GetDeviceQueueFn(Device, GraphicsQueueFamilyIndex, 0u, &GraphicsQueue);
			if (!GraphicsQueue)
			{
				SK_LOG(GLogSkylark, ESKLogVerbosity::Warning, "SKVulkanRHI: graphics queue acquisition failed.");
				Shutdown();
				return false;
			}

			if (!InitializeSubmissionObjects())
			{
				SK_LOG(GLogSkylark, ESKLogVerbosity::Warning, "SKVulkanRHI: command submission bootstrap incomplete.");
				Shutdown();
				return false;
			}

			BuildPipelineScaffolds();

			SK_LOG(
				GLogSkylark,
				ESKLogVerbosity::Display,
				"SKVulkanRHI: device ready. Swapchain path %s. Pipeline scaffolds: %zu.",
				bPresentationRuntimeReady ? "enabled" : "disabled",
				static_cast<SIZE_T>(PipelineScaffolds.size()));
			return true;
		}

		void Shutdown() override
		{
			using namespace VulkanMini;
			if (Device && DeviceWaitIdleFn)
			{
				DeviceWaitIdleFn(Device);
			}

			DestroySubmissionObjects();

			if (Device && DestroyDeviceFn)
			{
				DestroyDeviceFn(Device, nullptr);
			}

			Device = nullptr;
			GraphicsQueue = nullptr;
			DestroyDeviceFn = nullptr;
			GetDeviceQueueFn = nullptr;
			CreateSwapchainFn = nullptr;
			DestroySwapchainFn = nullptr;
			GetSwapchainImagesFn = nullptr;
			AcquireNextImageFn = nullptr;
			QueuePresentFn = nullptr;
			CreateSemaphoreFn = nullptr;
			DestroySemaphoreFn = nullptr;
			DeviceWaitIdleFn = nullptr;
			CreateCommandPoolFn = nullptr;
			DestroyCommandPoolFn = nullptr;
			AllocateCommandBuffersFn = nullptr;
			BeginCommandBufferFn = nullptr;
			EndCommandBufferFn = nullptr;
			QueueSubmitFn = nullptr;

			if (Instance && DestroyInstanceFn)
			{
				DestroyInstanceFn(Instance, nullptr);
			}

			Instance = nullptr;
			PhysicalDevice = nullptr;
			DestroyInstanceFn = nullptr;
			EnumeratePhysicalDevices = nullptr;
			GetPhysicalDeviceProperties = nullptr;
			GetPhysicalDeviceQueueFamilyProperties = nullptr;
			EnumerateDeviceExtensionProperties = nullptr;
			CreateDeviceFn = nullptr;
			GetPhysicalDeviceSurfaceSupportFn = nullptr;
			GetPhysicalDeviceSurfaceCapabilitiesFn = nullptr;
			GetPhysicalDeviceSurfaceFormatsFn = nullptr;
			GetPhysicalDeviceSurfacePresentModesFn = nullptr;
			DestroySurfaceFn = nullptr;
#if defined(_WIN32)
			CreateWin32SurfaceFn = nullptr;
#endif
			GraphicsQueueFamilyIndex = UINT32_MAX;
			PhysicalDeviceProperties = {};
			AvailableInstanceExtensions.clear();
			AvailableDeviceExtensions.clear();
			PipelineScaffolds.clear();
			bSurfaceInstanceExtensionsEnabled = false;
			bSwapchainDeviceExtensionEnabled = false;
			bPresentationRuntimeReady = false;
		}

		void BeginFrame() override {}
		void EndFrame() override {}

		ISKRHICommandList& GetImmediateCommandList() override { return Immediate; }

		TUniquePtr<ISKRHISwapChain> CreateSwapChain(const FSKRHISwapChainDesc& InDesc) override
		{
			auto SwapChain = std::make_unique<FSKVulkanSwapChain>(this, InDesc);
			InitializeSwapChain(*SwapChain);
			return SwapChain;
		}

		TUniquePtr<ISKRHITexture2D> CreateTexture2D(const FSKRHITextureDesc& InDesc) override
		{
			return std::make_unique<FSKVulkanTexture2D>(InDesc);
		}

		bool ReadbackTexturePixelRGBA8(ISKRHITexture2D& Texture, uint32 X, uint32 Y, uint8 OutRGBA[4]) override
		{
			auto* Tex = dynamic_cast<FSKVulkanTexture2D*>(&Texture);
			if (!Tex || !OutRGBA)
			{
				return false;
			}

			uint8* P = Tex->GetPixelPtr(X, Y);
			std::memcpy(OutRGBA, P, 4u);
			return true;
		}

		TUniquePtr<ISKRHIShaderModule> CreateShaderModule(const FSKRHIShaderModuleDesc& InDesc) override
		{
			if (!InDesc.Bytecode || InDesc.BytecodeSize == 0)
			{
				return {};
			}
			return std::make_unique<FSKVulkanShaderModule>(InDesc);
		}

		TUniquePtr<ISKRHIGraphicsPipelineState> CreateGraphicsPipeline(const FSKRHIGraphicsPipelineDesc& InDesc) override
		{
			if (!InDesc.VertexShader || !InDesc.PixelShader)
			{
				return {};
			}
			return std::make_unique<FSKVulkanGraphicsPipelineState>(InDesc);
		}

		ESKRHIApi GetApi() const override { return ESKRHIApi::Vulkan; }

		void PresentSwapChain(FSKVulkanSwapChain& SwapChain)
		{
			using namespace VulkanMini;
			if (SwapChain.bHeadless || !bPresentationRuntimeReady)
			{
				return;
			}

			if (!SwapChain.bInitialized || SwapChain.bResizeRequested)
			{
				InitializeSwapChain(SwapChain);
			}

			if (!SwapChain.Swapchain || !AcquireNextImageFn || !QueuePresentFn)
			{
				return;
			}

			const VkResult AcquireResult = AcquireNextImageFn(
				Device,
				SwapChain.Swapchain,
				~0ull,
				SwapChain.AcquireSemaphore,
				nullptr,
				&SwapChain.AcquiredImageIndex);

			if (AcquireResult == VK_ERROR_OUT_OF_DATE_KHR)
			{
				SwapChain.bResizeRequested = true;
				return;
			}

			if (AcquireResult != VK_SUCCESS && AcquireResult != VK_SUBOPTIMAL_KHR)
			{
				SK_LOG(GLogSkylark, ESKLogVerbosity::Warning, "SKVulkanRHI: vkAcquireNextImageKHR failed (%d).", (int32)AcquireResult);
				return;
			}

			const VkSemaphore WaitSemaphore = SwapChain.AcquireSemaphore;
			const VkSwapchainKHR SwapchainHandle = SwapChain.Swapchain;
			const uint32 ImageIndex = SwapChain.AcquiredImageIndex;

			VkPresentInfoKHR PresentInfo{};
			PresentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
			PresentInfo.waitSemaphoreCount = 1;
			PresentInfo.pWaitSemaphores = &WaitSemaphore;
			PresentInfo.swapchainCount = 1;
			PresentInfo.pSwapchains = &SwapchainHandle;
			PresentInfo.pImageIndices = &ImageIndex;

			const VkResult PresentResult = QueuePresentFn(GraphicsQueue, &PresentInfo);
			if (PresentResult == VK_ERROR_OUT_OF_DATE_KHR || PresentResult == VK_SUBOPTIMAL_KHR)
			{
				SwapChain.bResizeRequested = true;
				return;
			}

			if (PresentResult != VK_SUCCESS)
			{
				SK_LOG(GLogSkylark, ESKLogVerbosity::Warning, "SKVulkanRHI: vkQueuePresentKHR failed (%d).", (int32)PresentResult);
			}
		}

		void DestroySwapChain(FSKVulkanSwapChain& SwapChain)
		{
			using namespace VulkanMini;
			if (Device && DeviceWaitIdleFn)
			{
				DeviceWaitIdleFn(Device);
			}

			if (SwapChain.AcquireSemaphore && DestroySemaphoreFn && Device)
			{
				DestroySemaphoreFn(Device, SwapChain.AcquireSemaphore, nullptr);
			}
			SwapChain.AcquireSemaphore = nullptr;

			if (SwapChain.Swapchain && DestroySwapchainFn && Device)
			{
				DestroySwapchainFn(Device, SwapChain.Swapchain, nullptr);
			}
			SwapChain.Swapchain = nullptr;

			if (SwapChain.Surface && DestroySurfaceFn && Instance)
			{
				DestroySurfaceFn(Instance, SwapChain.Surface, nullptr);
			}
			SwapChain.Surface = nullptr;
			SwapChain.Images.clear();
			SwapChain.ImageCount = 0;
			SwapChain.bInitialized = false;
			SwapChain.bHeadless = true;
		}

	private:
		std::vector<VulkanMini::VkExtensionProperties> QueryAvailableInstanceExtensions() const
		{
			using namespace VulkanMini;
			std::vector<VkExtensionProperties> Extensions;
			if (!GetLoader().EnumerateInstanceExtensionProperties)
			{
				return Extensions;
			}

			uint32 Count = 0;
			if (GetLoader().EnumerateInstanceExtensionProperties(nullptr, &Count, nullptr) != VK_SUCCESS || Count == 0)
			{
				return Extensions;
			}

			Extensions.resize(Count);
			if (GetLoader().EnumerateInstanceExtensionProperties(nullptr, &Count, Extensions.data()) != VK_SUCCESS)
			{
				Extensions.clear();
			}
			return Extensions;
		}

		std::vector<VulkanMini::VkExtensionProperties> QueryAvailableDeviceExtensions() const
		{
			using namespace VulkanMini;
			std::vector<VkExtensionProperties> Extensions;
			if (!EnumerateDeviceExtensionProperties || !PhysicalDevice)
			{
				return Extensions;
			}

			uint32 Count = 0;
			if (EnumerateDeviceExtensionProperties(PhysicalDevice, nullptr, &Count, nullptr) != VK_SUCCESS || Count == 0)
			{
				return Extensions;
			}

			Extensions.resize(Count);
			if (EnumerateDeviceExtensionProperties(PhysicalDevice, nullptr, &Count, Extensions.data()) != VK_SUCCESS)
			{
				Extensions.clear();
			}
			return Extensions;
		}

		void LoadInstanceFunctions()
		{
			using namespace VulkanMini;
			EnumeratePhysicalDevices = reinterpret_cast<PFN_vkEnumeratePhysicalDevices>(GetInstanceProc(Instance, "vkEnumeratePhysicalDevices"));
			GetPhysicalDeviceProperties = reinterpret_cast<PFN_vkGetPhysicalDeviceProperties>(GetInstanceProc(Instance, "vkGetPhysicalDeviceProperties"));
			GetPhysicalDeviceQueueFamilyProperties = reinterpret_cast<PFN_vkGetPhysicalDeviceQueueFamilyProperties>(GetInstanceProc(Instance, "vkGetPhysicalDeviceQueueFamilyProperties"));
			EnumerateDeviceExtensionProperties = reinterpret_cast<PFN_vkEnumerateDeviceExtensionProperties>(GetInstanceProc(Instance, "vkEnumerateDeviceExtensionProperties"));
			CreateDeviceFn = reinterpret_cast<PFN_vkCreateDevice>(GetInstanceProc(Instance, "vkCreateDevice"));
			DestroyInstanceFn = reinterpret_cast<PFN_vkDestroyInstance>(GetInstanceProc(Instance, "vkDestroyInstance"));
			GetPhysicalDeviceSurfaceSupportFn = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceSupportKHR>(GetInstanceProc(Instance, "vkGetPhysicalDeviceSurfaceSupportKHR"));
			GetPhysicalDeviceSurfaceCapabilitiesFn = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR>(GetInstanceProc(Instance, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR"));
			GetPhysicalDeviceSurfaceFormatsFn = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceFormatsKHR>(GetInstanceProc(Instance, "vkGetPhysicalDeviceSurfaceFormatsKHR"));
			GetPhysicalDeviceSurfacePresentModesFn = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfacePresentModesKHR>(GetInstanceProc(Instance, "vkGetPhysicalDeviceSurfacePresentModesKHR"));
			DestroySurfaceFn = reinterpret_cast<PFN_vkDestroySurfaceKHR>(GetInstanceProc(Instance, "vkDestroySurfaceKHR"));
#if defined(_WIN32)
			CreateWin32SurfaceFn = reinterpret_cast<PFN_vkCreateWin32SurfaceKHR>(GetInstanceProc(Instance, "vkCreateWin32SurfaceKHR"));
#endif
		}

		void LoadDeviceFunctions()
		{
			using namespace VulkanMini;
			DestroyDeviceFn = GetLoader().GetDeviceProcAddr
				? reinterpret_cast<PFN_vkDestroyDevice>(GetLoader().GetDeviceProcAddr(Device, "vkDestroyDevice"))
				: nullptr;
			GetDeviceQueueFn = GetLoader().GetDeviceProcAddr
				? reinterpret_cast<PFN_vkGetDeviceQueue>(GetLoader().GetDeviceProcAddr(Device, "vkGetDeviceQueue"))
				: nullptr;
			CreateSwapchainFn = GetLoader().GetDeviceProcAddr
				? reinterpret_cast<PFN_vkCreateSwapchainKHR>(GetLoader().GetDeviceProcAddr(Device, "vkCreateSwapchainKHR"))
				: nullptr;
			DestroySwapchainFn = GetLoader().GetDeviceProcAddr
				? reinterpret_cast<PFN_vkDestroySwapchainKHR>(GetLoader().GetDeviceProcAddr(Device, "vkDestroySwapchainKHR"))
				: nullptr;
			GetSwapchainImagesFn = GetLoader().GetDeviceProcAddr
				? reinterpret_cast<PFN_vkGetSwapchainImagesKHR>(GetLoader().GetDeviceProcAddr(Device, "vkGetSwapchainImagesKHR"))
				: nullptr;
			AcquireNextImageFn = GetLoader().GetDeviceProcAddr
				? reinterpret_cast<PFN_vkAcquireNextImageKHR>(GetLoader().GetDeviceProcAddr(Device, "vkAcquireNextImageKHR"))
				: nullptr;
			QueuePresentFn = GetLoader().GetDeviceProcAddr
				? reinterpret_cast<PFN_vkQueuePresentKHR>(GetLoader().GetDeviceProcAddr(Device, "vkQueuePresentKHR"))
				: nullptr;
			CreateSemaphoreFn = GetLoader().GetDeviceProcAddr
				? reinterpret_cast<PFN_vkCreateSemaphore>(GetLoader().GetDeviceProcAddr(Device, "vkCreateSemaphore"))
				: nullptr;
			DestroySemaphoreFn = GetLoader().GetDeviceProcAddr
				? reinterpret_cast<PFN_vkDestroySemaphore>(GetLoader().GetDeviceProcAddr(Device, "vkDestroySemaphore"))
				: nullptr;
			DeviceWaitIdleFn = GetLoader().GetDeviceProcAddr
				? reinterpret_cast<PFN_vkDeviceWaitIdle>(GetLoader().GetDeviceProcAddr(Device, "vkDeviceWaitIdle"))
				: nullptr;
			CreateCommandPoolFn = GetLoader().GetDeviceProcAddr
				? reinterpret_cast<PFN_vkCreateCommandPool>(GetLoader().GetDeviceProcAddr(Device, "vkCreateCommandPool"))
				: nullptr;
			DestroyCommandPoolFn = GetLoader().GetDeviceProcAddr
				? reinterpret_cast<PFN_vkDestroyCommandPool>(GetLoader().GetDeviceProcAddr(Device, "vkDestroyCommandPool"))
				: nullptr;
			AllocateCommandBuffersFn = GetLoader().GetDeviceProcAddr
				? reinterpret_cast<PFN_vkAllocateCommandBuffers>(GetLoader().GetDeviceProcAddr(Device, "vkAllocateCommandBuffers"))
				: nullptr;
			BeginCommandBufferFn = GetLoader().GetDeviceProcAddr
				? reinterpret_cast<PFN_vkBeginCommandBuffer>(GetLoader().GetDeviceProcAddr(Device, "vkBeginCommandBuffer"))
				: nullptr;
			EndCommandBufferFn = GetLoader().GetDeviceProcAddr
				? reinterpret_cast<PFN_vkEndCommandBuffer>(GetLoader().GetDeviceProcAddr(Device, "vkEndCommandBuffer"))
				: nullptr;
			QueueSubmitFn = GetLoader().GetDeviceProcAddr
				? reinterpret_cast<PFN_vkQueueSubmit>(GetLoader().GetDeviceProcAddr(Device, "vkQueueSubmit"))
				: nullptr;

			bPresentationRuntimeReady = bSurfaceInstanceExtensionsEnabled
				&& bSwapchainDeviceExtensionEnabled
				&& GetPhysicalDeviceSurfaceSupportFn
				&& GetPhysicalDeviceSurfaceCapabilitiesFn
				&& GetPhysicalDeviceSurfaceFormatsFn
				&& GetPhysicalDeviceSurfacePresentModesFn
				&& DestroySurfaceFn
				&& CreateSwapchainFn
				&& DestroySwapchainFn
				&& GetSwapchainImagesFn
				&& AcquireNextImageFn
				&& QueuePresentFn
				&& CreateSemaphoreFn
				&& DestroySemaphoreFn
				&& DeviceWaitIdleFn
#if defined(_WIN32)
				&& CreateWin32SurfaceFn
#endif
				;
		}

		bool InitializeSubmissionObjects()
		{
			using namespace VulkanMini;
			if (!Device || !CreateCommandPoolFn || !DestroyCommandPoolFn || !AllocateCommandBuffersFn || !BeginCommandBufferFn || !EndCommandBufferFn || !QueueSubmitFn)
			{
				return false;
			}

			VkCommandPoolCreateInfo PoolInfo{};
			PoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			PoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
			PoolInfo.queueFamilyIndex = GraphicsQueueFamilyIndex;

			if (CreateCommandPoolFn(Device, &PoolInfo, nullptr, &ImmediateCommandPool) != VK_SUCCESS || !ImmediateCommandPool)
			{
				return false;
			}

			VkCommandBufferAllocateInfo AllocateInfo{};
			AllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			AllocateInfo.commandPool = ImmediateCommandPool;
			AllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			AllocateInfo.commandBufferCount = 1;

			if (AllocateCommandBuffersFn(Device, &AllocateInfo, &ImmediateCommandBuffer) != VK_SUCCESS || !ImmediateCommandBuffer)
			{
				DestroySubmissionObjects();
				return false;
			}

			return true;
		}

		void DestroySubmissionObjects()
		{
			using namespace VulkanMini;
			ImmediateCommandBuffer = nullptr;
			if (ImmediateCommandPool && DestroyCommandPoolFn && Device)
			{
				DestroyCommandPoolFn(Device, ImmediateCommandPool, nullptr);
			}
			ImmediateCommandPool = nullptr;
		}

		bool SubmitImmediateCommandList(const FSKVulkanCommandList& CommandList)
		{
			using namespace VulkanMini;
			(void)CommandList;
			if (!Device || !GraphicsQueue || !ImmediateCommandBuffer || !BeginCommandBufferFn || !EndCommandBufferFn || !QueueSubmitFn)
			{
				return false;
			}

			VkCommandBufferBeginInfo BeginInfo{};
			BeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			BeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

			if (BeginCommandBufferFn(ImmediateCommandBuffer, &BeginInfo) != VK_SUCCESS)
			{
				return false;
			}

			if (EndCommandBufferFn(ImmediateCommandBuffer) != VK_SUCCESS)
			{
				return false;
			}

			VkSubmitInfo SubmitInfo{};
			SubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			SubmitInfo.commandBufferCount = 1;
			SubmitInfo.pCommandBuffers = &ImmediateCommandBuffer;

			const VkResult SubmitResult = QueueSubmitFn(GraphicsQueue, 1u, &SubmitInfo, nullptr);
			if (SubmitResult != VK_SUCCESS)
			{
				SK_LOG(GLogSkylark, ESKLogVerbosity::Warning, "SKVulkanRHI: vkQueueSubmit failed (%d).", (int32)SubmitResult);
				return false;
			}

			if (DeviceWaitIdleFn)
			{
				DeviceWaitIdleFn(Device);
			}

			return true;
		}

		void BuildPipelineScaffolds()
		{
			PipelineScaffolds.clear();

			FSKVulkanPipelineScaffold LineOverlay;
			LineOverlay.DebugName = "Vulkan.LineOverlay";
			LineOverlay.bGraphics = true;
			LineOverlay.bPresentPath = false;
			LineOverlay.Stages.push_back({ "VS", {} });
			LineOverlay.Stages.push_back({ "PS", {} });
			PipelineScaffolds.push_back(std::move(LineOverlay));

			FSKVulkanPipelineScaffold PresentBlit;
			PresentBlit.DebugName = "Vulkan.PresentBlit";
			PresentBlit.bGraphics = true;
			PresentBlit.bPresentPath = true;
			PresentBlit.Stages.push_back({ "VS", {} });
			PresentBlit.Stages.push_back({ "PS", {} });
			PipelineScaffolds.push_back(std::move(PresentBlit));
		}

		bool InitializeSwapChain(FSKVulkanSwapChain& SwapChain)
		{
			using namespace VulkanMini;
			DestroySwapChain(SwapChain);
			SwapChain.bResizeRequested = false;

			if (!bPresentationRuntimeReady || !SwapChain.Owner || !SwapChain.Desc.Window.Handle)
			{
				SwapChain.bHeadless = true;
				return true;
			}

#if defined(_WIN32)
			HWND Hwnd = static_cast<HWND>(SwapChain.Desc.Window.Handle);
			if (!Hwnd)
			{
				SwapChain.bHeadless = true;
				return true;
			}

			VkWin32SurfaceCreateInfoKHR SurfaceInfo{};
			SurfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
			SurfaceInfo.hinstance = ::GetModuleHandleW(nullptr);
			SurfaceInfo.hwnd = Hwnd;

			if (CreateWin32SurfaceFn(Instance, &SurfaceInfo, nullptr, &SwapChain.Surface) != VK_SUCCESS || !SwapChain.Surface)
			{
				SK_LOG(GLogSkylark, ESKLogVerbosity::Warning, "SKVulkanRHI: failed to create Win32 surface.");
				SwapChain.bHeadless = true;
				return false;
			}
#else
			SwapChain.bHeadless = true;
			return true;
#endif

			VkBool32 bPresentSupported = VK_FALSE;
			if (GetPhysicalDeviceSurfaceSupportFn(PhysicalDevice, GraphicsQueueFamilyIndex, SwapChain.Surface, &bPresentSupported) != VK_SUCCESS || bPresentSupported == VK_FALSE)
			{
				SK_LOG(GLogSkylark, ESKLogVerbosity::Warning, "SKVulkanRHI: graphics queue cannot present to the requested surface.");
				DestroySwapChain(SwapChain);
				return false;
			}

			VkSurfaceCapabilitiesKHR Caps{};
			if (GetPhysicalDeviceSurfaceCapabilitiesFn(PhysicalDevice, SwapChain.Surface, &Caps) != VK_SUCCESS)
			{
				DestroySwapChain(SwapChain);
				return false;
			}

			uint32 FormatCount = 0;
			if (GetPhysicalDeviceSurfaceFormatsFn(PhysicalDevice, SwapChain.Surface, &FormatCount, nullptr) != VK_SUCCESS || FormatCount == 0)
			{
				DestroySwapChain(SwapChain);
				return false;
			}

			std::vector<VkSurfaceFormatKHR> Formats(FormatCount);
			GetPhysicalDeviceSurfaceFormatsFn(PhysicalDevice, SwapChain.Surface, &FormatCount, Formats.data());
			VkSurfaceFormatKHR ChosenFormat = Formats.front();
			for (const auto& Candidate : Formats)
			{
				if (Candidate.format == VK_FORMAT_B8G8R8A8_UNORM && Candidate.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
				{
					ChosenFormat = Candidate;
					break;
				}
			}

			uint32 PresentModeCount = 0;
			VkPresentModeKHR ChosenPresentMode = VK_PRESENT_MODE_FIFO_KHR;
			if (GetPhysicalDeviceSurfacePresentModesFn(PhysicalDevice, SwapChain.Surface, &PresentModeCount, nullptr) == VK_SUCCESS && PresentModeCount > 0)
			{
				std::vector<VkPresentModeKHR> PresentModes(PresentModeCount);
				GetPhysicalDeviceSurfacePresentModesFn(PhysicalDevice, SwapChain.Surface, &PresentModeCount, PresentModes.data());
				for (VkPresentModeKHR Mode : PresentModes)
				{
					if (Mode == VK_PRESENT_MODE_MAILBOX_KHR)
					{
						ChosenPresentMode = Mode;
						break;
					}
				}
			}

			VkExtent2D ChosenExtent{};
			if (Caps.currentExtent.width != VK_SURFACE_UNDEFINED_EXTENT)
			{
				ChosenExtent = Caps.currentExtent;
			}
			else
			{
				ChosenExtent.width = std::clamp(SwapChain.Desc.Width, Caps.minImageExtent.width, Caps.maxImageExtent.width);
				ChosenExtent.height = std::clamp(SwapChain.Desc.Height, Caps.minImageExtent.height, Caps.maxImageExtent.height);
			}

			uint32 DesiredImageCount = std::max(2u, Caps.minImageCount + 1u);
			if (Caps.maxImageCount > 0 && DesiredImageCount > Caps.maxImageCount)
			{
				DesiredImageCount = Caps.maxImageCount;
			}

			VkSwapchainCreateInfoKHR SwapchainInfo{};
			SwapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			SwapchainInfo.surface = SwapChain.Surface;
			SwapchainInfo.minImageCount = DesiredImageCount;
			SwapchainInfo.imageFormat = ChosenFormat.format;
			SwapchainInfo.imageColorSpace = ChosenFormat.colorSpace;
			SwapchainInfo.imageExtent = ChosenExtent;
			SwapchainInfo.imageArrayLayers = 1;
			SwapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
			SwapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			SwapchainInfo.preTransform = Caps.currentTransform ? Caps.currentTransform : VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
			SwapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
			SwapchainInfo.presentMode = ChosenPresentMode;
			SwapchainInfo.clipped = VK_TRUE;
			SwapchainInfo.oldSwapchain = nullptr;

			if (CreateSwapchainFn(Device, &SwapchainInfo, nullptr, &SwapChain.Swapchain) != VK_SUCCESS || !SwapChain.Swapchain)
			{
				SK_LOG(GLogSkylark, ESKLogVerbosity::Warning, "SKVulkanRHI: vkCreateSwapchainKHR failed.");
				DestroySwapChain(SwapChain);
				return false;
			}

			if (!SwapChain.AcquireSemaphore)
			{
				VkSemaphoreCreateInfo SemaphoreInfo{};
				SemaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
				if (CreateSemaphoreFn(Device, &SemaphoreInfo, nullptr, &SwapChain.AcquireSemaphore) != VK_SUCCESS || !SwapChain.AcquireSemaphore)
				{
					DestroySwapChain(SwapChain);
					return false;
				}
			}

			uint32 ImageCount = 0;
			if (GetSwapchainImagesFn(Device, SwapChain.Swapchain, &ImageCount, nullptr) != VK_SUCCESS || ImageCount == 0)
			{
				DestroySwapChain(SwapChain);
				return false;
			}

			SwapChain.Images.resize(ImageCount);
			if (GetSwapchainImagesFn(Device, SwapChain.Swapchain, &ImageCount, SwapChain.Images.data()) != VK_SUCCESS)
			{
				DestroySwapChain(SwapChain);
				return false;
			}

			SwapChain.ImageCount = ImageCount;
			SwapChain.bHeadless = false;
			SwapChain.bInitialized = true;
			return true;
		}

	private:
		FSKRHIDeviceDesc Desc{};
		FSKVulkanCommandList Immediate{};
		TArray<FSKVulkanPipelineScaffold> PipelineScaffolds;

		VulkanMini::VkInstance Instance = nullptr;
		VulkanMini::VkPhysicalDevice PhysicalDevice = nullptr;
		VulkanMini::VkDevice Device = nullptr;
		VulkanMini::VkQueue GraphicsQueue = nullptr;
		uint32 GraphicsQueueFamilyIndex = UINT32_MAX;
		VulkanMini::VkPhysicalDeviceProperties PhysicalDeviceProperties{};
		TArray<VulkanMini::VkExtensionProperties> AvailableInstanceExtensions;
		TArray<VulkanMini::VkExtensionProperties> AvailableDeviceExtensions;

		bool bSurfaceInstanceExtensionsEnabled = false;
		bool bSwapchainDeviceExtensionEnabled = false;
		bool bPresentationRuntimeReady = false;

		VulkanMini::PFN_vkEnumeratePhysicalDevices EnumeratePhysicalDevices = nullptr;
		VulkanMini::PFN_vkGetPhysicalDeviceProperties GetPhysicalDeviceProperties = nullptr;
		VulkanMini::PFN_vkGetPhysicalDeviceQueueFamilyProperties GetPhysicalDeviceQueueFamilyProperties = nullptr;
		VulkanMini::PFN_vkEnumerateDeviceExtensionProperties EnumerateDeviceExtensionProperties = nullptr;
		VulkanMini::PFN_vkCreateDevice CreateDeviceFn = nullptr;
		VulkanMini::PFN_vkDestroyInstance DestroyInstanceFn = nullptr;
		VulkanMini::PFN_vkDestroyDevice DestroyDeviceFn = nullptr;
		VulkanMini::PFN_vkGetDeviceQueue GetDeviceQueueFn = nullptr;
		VulkanMini::PFN_vkGetPhysicalDeviceSurfaceSupportKHR GetPhysicalDeviceSurfaceSupportFn = nullptr;
		VulkanMini::PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR GetPhysicalDeviceSurfaceCapabilitiesFn = nullptr;
		VulkanMini::PFN_vkGetPhysicalDeviceSurfaceFormatsKHR GetPhysicalDeviceSurfaceFormatsFn = nullptr;
		VulkanMini::PFN_vkGetPhysicalDeviceSurfacePresentModesKHR GetPhysicalDeviceSurfacePresentModesFn = nullptr;
		VulkanMini::PFN_vkDestroySurfaceKHR DestroySurfaceFn = nullptr;
#if defined(_WIN32)
		VulkanMini::PFN_vkCreateWin32SurfaceKHR CreateWin32SurfaceFn = nullptr;
#endif
		VulkanMini::PFN_vkCreateSwapchainKHR CreateSwapchainFn = nullptr;
		VulkanMini::PFN_vkDestroySwapchainKHR DestroySwapchainFn = nullptr;
		VulkanMini::PFN_vkGetSwapchainImagesKHR GetSwapchainImagesFn = nullptr;
		VulkanMini::PFN_vkAcquireNextImageKHR AcquireNextImageFn = nullptr;
		VulkanMini::PFN_vkQueuePresentKHR QueuePresentFn = nullptr;
		VulkanMini::PFN_vkCreateSemaphore CreateSemaphoreFn = nullptr;
		VulkanMini::PFN_vkDestroySemaphore DestroySemaphoreFn = nullptr;
		VulkanMini::PFN_vkDeviceWaitIdle DeviceWaitIdleFn = nullptr;
		VulkanMini::PFN_vkCreateCommandPool CreateCommandPoolFn = nullptr;
		VulkanMini::PFN_vkDestroyCommandPool DestroyCommandPoolFn = nullptr;
		VulkanMini::PFN_vkAllocateCommandBuffers AllocateCommandBuffersFn = nullptr;
		VulkanMini::PFN_vkBeginCommandBuffer BeginCommandBufferFn = nullptr;
		VulkanMini::PFN_vkEndCommandBuffer EndCommandBufferFn = nullptr;
		VulkanMini::PFN_vkQueueSubmit QueueSubmitFn = nullptr;
		VulkanMini::VkCommandPool ImmediateCommandPool = nullptr;
		VulkanMini::VkCommandBuffer ImmediateCommandBuffer = nullptr;

		friend class FSKVulkanSwapChain;
		friend class FSKVulkanCommandList;
	};

	void FSKVulkanCommandList::Flush()
	{
		if (Owner)
		{
			Owner->SubmitImmediateCommandList(*this);
		}
	}

	FSKVulkanSwapChain::~FSKVulkanSwapChain()
	{
		if (Owner)
		{
			Owner->DestroySwapChain(*this);
		}
	}

	void FSKVulkanSwapChain::Present()
	{
		if (Owner)
		{
			Owner->PresentSwapChain(*this);
		}
	}

	bool SKIsVulkanRHISupported()
	{
		using namespace VulkanMini;

		if (!EnsureLoader() || !GetLoader().CreateInstance)
		{
			return false;
		}

		VkApplicationInfo AppInfo{};
		AppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		AppInfo.pApplicationName = "SkylarkProbe";
		AppInfo.pEngineName = "SkylarkEngine";

		VkInstanceCreateInfo InstanceInfo{};
		InstanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		InstanceInfo.pApplicationInfo = &AppInfo;

		VkInstance ProbeInstance = nullptr;
		if (GetLoader().CreateInstance(&InstanceInfo, nullptr, &ProbeInstance) != VK_SUCCESS || !ProbeInstance)
		{
			return false;
		}

		auto EnumeratePhysicalDevices = reinterpret_cast<PFN_vkEnumeratePhysicalDevices>(GetInstanceProc(ProbeInstance, "vkEnumeratePhysicalDevices"));
		auto DestroyInstance = reinterpret_cast<PFN_vkDestroyInstance>(GetInstanceProc(ProbeInstance, "vkDestroyInstance"));

		uint32 PhysicalDeviceCount = 0;
		const bool bHasDevices = EnumeratePhysicalDevices
			&& (EnumeratePhysicalDevices(ProbeInstance, &PhysicalDeviceCount, nullptr) == VK_SUCCESS)
			&& (PhysicalDeviceCount > 0);

		if (DestroyInstance)
		{
			DestroyInstance(ProbeInstance, nullptr);
		}

		return bHasDevices;
	}

	TUniquePtr<ISKRHIDevice> SKCreateVulkanRHIDevice()
	{
		return std::make_unique<FSKVulkanRHIDevice>();
	}
}
