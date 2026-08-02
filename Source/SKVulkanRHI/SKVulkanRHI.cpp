#include "SKVulkanRHI/SKVulkanRHI.h"
#include "SKCore/SKCoreLog.h"

#include <algorithm>
#include <cstring>
#include <vector>

#if defined(_WIN32)
	#ifndef VK_USE_PLATFORM_WIN32_KHR
		#define VK_USE_PLATFORM_WIN32_KHR
	#endif
#endif
#ifndef VK_NO_PROTOTYPES
	#define VK_NO_PROTOTYPES
#endif
#include <vulkan/vulkan.h>

#if defined(_WIN32)
	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif
	#ifndef NOMINMAX
		#define NOMINMAX
	#endif
	#include <windows.h>
#else
	#include <dlfcn.h>
#endif

namespace Skylark
{
	// Vulkan RHI deliberately uses the official Vulkan headers instead of hand-written ABI
	// structs.  The previous VulkanMini block duplicated Vk* structs manually; that is not
	// safe on Windows because driver calls write into ABI-defined layouts that must match
	// the SDK exactly.  A single wrong padding/alignment field can crash in
	// vkGetPhysicalDeviceProperties, vkCreateSwapchainKHR, vkQueueSubmit, or present.
	namespace VulkanMini
	{
		using ::VkAccessFlags;
		using ::VkApplicationInfo;
		using ::VkBool32;
		using ::VkBufferMemoryBarrier;
		using ::VkClearColorValue;
		using ::VkColorSpaceKHR;
		using ::VkCommandBuffer;
		using ::VkCommandBufferAllocateInfo;
		using ::VkCommandBufferBeginInfo;
		using ::VkCommandBufferResetFlags;
		using ::VkCommandBufferUsageFlags;
		using ::VkCommandPool;
		using ::VkCommandPoolCreateFlags;
		using ::VkCommandPoolCreateInfo;
		using ::VkCompositeAlphaFlagsKHR;
		using ::VkDependencyFlags;
		using ::VkDevice;
		using ::VkDeviceCreateFlags;
		using ::VkDeviceCreateInfo;
		using ::VkDeviceQueueCreateFlags;
		using ::VkDeviceQueueCreateInfo;
		using ::VkDeviceSize;
		using ::VkExtensionProperties;
		using ::VkExtent2D;
		using ::VkFence;
		using ::VkFlags;
		using ::VkFormat;
		using ::VkImage;
		using ::VkImageAspectFlags;
		using ::VkImageLayout;
		using ::VkImageMemoryBarrier;
		using ::VkImageSubresourceRange;
		using ::VkImageUsageFlags;
		using ::VkInstance;
		using ::VkInstanceCreateFlags;
		using ::VkInstanceCreateInfo;
		using ::VkMemoryBarrier;
		using ::VkPhysicalDevice;
		using ::VkPhysicalDeviceProperties;
		using ::VkPipelineStageFlags;
		using ::VkPresentInfoKHR;
		using ::VkPresentModeKHR;
		using ::VkQueue;
		using ::VkQueueFamilyProperties;
		using ::VkQueueFlags;
		using ::VkResult;
		using ::VkSemaphore;
		using ::VkSemaphoreCreateFlags;
		using ::VkSemaphoreCreateInfo;
		using ::VkSharingMode;
		using ::VkStructureType;
		using ::VkSubmitInfo;
		using ::VkSurfaceCapabilitiesKHR;
		using ::VkSurfaceFormatKHR;
		using ::VkSurfaceKHR;
		using ::VkSurfaceTransformFlagsKHR;
		using ::VkSwapchainCreateFlagsKHR;
		using ::VkSwapchainCreateInfoKHR;
		using ::VkSwapchainKHR;
#if defined(_WIN32)
		using ::VkWin32SurfaceCreateInfoKHR;
#endif

		using ::PFN_vkVoidFunction;
		using ::PFN_vkGetInstanceProcAddr;
		using ::PFN_vkCreateInstance;
		using ::PFN_vkEnumerateInstanceExtensionProperties;
		using ::PFN_vkDestroyInstance;
		using ::PFN_vkEnumeratePhysicalDevices;
		using ::PFN_vkGetPhysicalDeviceProperties;
		using ::PFN_vkGetPhysicalDeviceQueueFamilyProperties;
		using ::PFN_vkEnumerateDeviceExtensionProperties;
		using ::PFN_vkCreateDevice;
		using ::PFN_vkGetDeviceProcAddr;
		using ::PFN_vkDestroyDevice;
		using ::PFN_vkGetDeviceQueue;
		using ::PFN_vkGetPhysicalDeviceSurfaceSupportKHR;
		using ::PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
		using ::PFN_vkGetPhysicalDeviceSurfaceFormatsKHR;
		using ::PFN_vkGetPhysicalDeviceSurfacePresentModesKHR;
		using ::PFN_vkDestroySurfaceKHR;
#if defined(_WIN32)
		using ::PFN_vkCreateWin32SurfaceKHR;
#endif
		using ::PFN_vkCreateSwapchainKHR;
		using ::PFN_vkDestroySwapchainKHR;
		using ::PFN_vkGetSwapchainImagesKHR;
		using ::PFN_vkAcquireNextImageKHR;
		using ::PFN_vkQueuePresentKHR;
		using ::PFN_vkCreateSemaphore;
		using ::PFN_vkDestroySemaphore;
		using ::PFN_vkDeviceWaitIdle;
		using ::PFN_vkCreateCommandPool;
		using ::PFN_vkDestroyCommandPool;
		using ::PFN_vkAllocateCommandBuffers;
		using ::PFN_vkBeginCommandBuffer;
		using ::PFN_vkEndCommandBuffer;
		using ::PFN_vkQueueSubmit;
		using ::PFN_vkResetCommandBuffer;
		using ::PFN_vkCmdPipelineBarrier;
		using ::PFN_vkCmdClearColorImage;

		constexpr VkFormat VK_FORMAT_B8G8R8A8_UNORM = ::VK_FORMAT_B8G8R8A8_UNORM;
		constexpr uint32 VK_SURFACE_UNDEFINED_EXTENT = 0xFFFFFFFFu;

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
				if (std::strncmp(Ext.extensionName, Name, VK_MAX_EXTENSION_NAME_SIZE) == 0)
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
		VulkanMini::VkSemaphore RenderFinishedSemaphore = nullptr;
		VulkanMini::VkFormat ImageFormat = VulkanMini::VK_FORMAT_B8G8R8A8_UNORM;
		bool bSupportsTransferDst = false;
		std::vector<VulkanMini::VkImageLayout> ImageLayouts;

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


		void DrawTriangleList(const FSKRHITriangleVertex* Vertices, uint32 VertexCount, const FSKRHITriangleDrawParams& Params) override
		{
			(void)Vertices;
			RecordedVertexCount = VertexCount;
			RecordedIndexCount = 0;
			RecordedInstanceCount = 1;
			bRecordedTriangleDraw = (VertexCount >= 3);
			bRecordedIndexed = false;
			bRecordedInstanced = false;
			LastTriangleParams = Params;
		}

		void DrawIndexedTriangleList(const FSKRHITriangleVertex* Vertices, uint32 VertexCount, const uint32* Indices, uint32 IndexCount, const FSKRHITriangleDrawParams& Params) override
		{
			(void)Vertices;
			(void)Indices;
			RecordedVertexCount = VertexCount;
			RecordedIndexCount = IndexCount;
			RecordedInstanceCount = 1;
			bRecordedTriangleDraw = (IndexCount >= 3);
			bRecordedIndexed = true;
			bRecordedInstanced = false;
			LastTriangleParams = Params;
		}

		void DrawIndexedInstancedTriangleList(const FSKRHITriangleVertex* Vertices, uint32 VertexCount, const uint32* Indices, uint32 IndexCount, const FSKRHITriangleInstance* Instances, uint32 InstanceCount, const FSKRHITriangleDrawParams& Params) override
		{
			(void)Vertices;
			(void)Indices;
			(void)Instances;
			RecordedVertexCount = VertexCount;
			RecordedIndexCount = IndexCount;
			RecordedInstanceCount = InstanceCount;
			bRecordedTriangleDraw = (IndexCount >= 3) && (InstanceCount > 0);
			bRecordedIndexed = true;
			bRecordedInstanced = (InstanceCount > 1);
			LastTriangleParams = Params;
		}

		void Flush() override;

	private:
		friend class FSKVulkanRHIDevice;

		FSKVulkanRHIDevice* Owner = nullptr;
		FSKVulkanSwapChain* BoundSwapChain = nullptr;
		FSKVulkanTexture2D* BoundTexture = nullptr;
		ISKRHIGraphicsPipelineState* BoundPipeline = nullptr;
		bool bInsideRenderPass = false;
		uint32 RecordedVertexCount = 0;
		uint32 RecordedFirstVertex = 0;
		uint32 RecordedIndexCount = 0;
		uint32 RecordedInstanceCount = 0;
		bool bRecordedTriangleDraw = false;
		bool bRecordedIndexed = false;
		bool bRecordedInstanced = false;
		FSKRHITriangleDrawParams LastTriangleParams{};
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
			AppInfo.apiVersion = VK_API_VERSION_1_0;

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
			// Refresh vkGetDeviceProcAddr through the created instance. Some Windows Vulkan loaders do not expose
			// a complete global function table before an instance exists.
			GetLoader().GetDeviceProcAddr = reinterpret_cast<PFN_vkGetDeviceProcAddr>(GetInstanceProc(Instance, "vkGetDeviceProcAddr"));
			if (!EnumeratePhysicalDevices || !GetPhysicalDeviceQueueFamilyProperties || !CreateDeviceFn || !DestroyInstanceFn || !GetLoader().GetDeviceProcAddr)
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
				"SKVulkanRHI: device ready. Device=%s GraphicsQueueFamily=%u SurfaceExt=%s SwapchainExt=%s PresentRuntime=%s PipelineScaffolds=%zu.",
				PhysicalDeviceProperties.deviceName,
				GraphicsQueueFamilyIndex,
				bSurfaceInstanceExtensionsEnabled ? "on" : "off",
				bSwapchainDeviceExtensionEnabled ? "on" : "off",
				bPresentationRuntimeReady ? "on" : "off",
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
			ResetCommandBufferFn = nullptr;
			CmdPipelineBarrierFn = nullptr;
			CmdClearColorImageFn = nullptr;

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

			if (!SwapChain.Swapchain || !SwapChain.AcquireSemaphore || !SwapChain.RenderFinishedSemaphore || !SwapChain.bSupportsTransferDst)
			{
				return;
			}

			if (!AcquireNextImageFn || !QueuePresentFn || !ResetCommandBufferFn || !BeginCommandBufferFn
				|| !EndCommandBufferFn || !QueueSubmitFn || !CmdPipelineBarrierFn || !CmdClearColorImageFn)
			{
				SK_LOG(GLogSkylark, ESKLogVerbosity::Warning, "SKVulkanRHI: present chain function table is incomplete.");
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

			if (SwapChain.AcquiredImageIndex >= SwapChain.Images.size() || SwapChain.AcquiredImageIndex >= SwapChain.ImageLayouts.size())
			{
				SK_LOG(GLogSkylark, ESKLogVerbosity::Warning, "SKVulkanRHI: acquired image index is outside swapchain image arrays.");
				SwapChain.bResizeRequested = true;
				return;
			}

			VkCommandBuffer Cmd = ImmediateCommandBuffer;
			if (!Cmd)
			{
				return;
			}

			if (DeviceWaitIdleFn)
			{
				DeviceWaitIdleFn(Device);
			}

			const VkResult ResetResult = ResetCommandBufferFn(Cmd, 0u);
			if (ResetResult != VK_SUCCESS)
			{
				SK_LOG(GLogSkylark, ESKLogVerbosity::Warning, "SKVulkanRHI: vkResetCommandBuffer before present failed (%d).", (int32)ResetResult);
				return;
			}

			VkCommandBufferBeginInfo BeginInfo{};
			BeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			BeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
			const VkResult BeginResult = BeginCommandBufferFn(Cmd, &BeginInfo);
			if (BeginResult != VK_SUCCESS)
			{
				SK_LOG(GLogSkylark, ESKLogVerbosity::Warning, "SKVulkanRHI: vkBeginCommandBuffer for present failed (%d).", (int32)BeginResult);
				return;
			}

			const uint32 ImageIndex = SwapChain.AcquiredImageIndex;
			const VkImage Image = SwapChain.Images[ImageIndex];
			const VkImageLayout OldLayout = SwapChain.ImageLayouts[ImageIndex];

			VkImageSubresourceRange ColorRange{};
			ColorRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			ColorRange.baseMipLevel = 0u;
			ColorRange.levelCount = 1u;
			ColorRange.baseArrayLayer = 0u;
			ColorRange.layerCount = 1u;

			VkImageMemoryBarrier ToTransfer{};
			ToTransfer.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			ToTransfer.srcAccessMask = (OldLayout == VK_IMAGE_LAYOUT_UNDEFINED) ? 0u : VK_ACCESS_MEMORY_READ_BIT;
			ToTransfer.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			ToTransfer.oldLayout = OldLayout;
			ToTransfer.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			ToTransfer.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			ToTransfer.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			ToTransfer.image = Image;
			ToTransfer.subresourceRange = ColorRange;

			CmdPipelineBarrierFn(
				Cmd,
				(OldLayout == VK_IMAGE_LAYOUT_UNDEFINED) ? VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT : VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
				VK_PIPELINE_STAGE_TRANSFER_BIT,
				0u,
				0u, nullptr,
				0u, nullptr,
				1u, &ToTransfer);

			VkClearColorValue ClearValue{};
			ClearValue.float32[0] = std::clamp(SwapChain.PendingClear.R, 0.0f, 1.0f);
			ClearValue.float32[1] = std::clamp(SwapChain.PendingClear.G, 0.0f, 1.0f);
			ClearValue.float32[2] = std::clamp(SwapChain.PendingClear.B, 0.0f, 1.0f);
			ClearValue.float32[3] = std::clamp(SwapChain.PendingClear.A, 0.0f, 1.0f);

			CmdClearColorImageFn(Cmd, Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &ClearValue, 1u, &ColorRange);

			VkImageMemoryBarrier ToPresent{};
			ToPresent.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			ToPresent.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			ToPresent.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
			ToPresent.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			ToPresent.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
			ToPresent.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			ToPresent.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			ToPresent.image = Image;
			ToPresent.subresourceRange = ColorRange;

			CmdPipelineBarrierFn(
				Cmd,
				VK_PIPELINE_STAGE_TRANSFER_BIT,
				VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
				0u,
				0u, nullptr,
				0u, nullptr,
				1u, &ToPresent);

			const VkResult EndResult = EndCommandBufferFn(Cmd);
			if (EndResult != VK_SUCCESS)
			{
				SK_LOG(GLogSkylark, ESKLogVerbosity::Warning, "SKVulkanRHI: vkEndCommandBuffer for present failed (%d).", (int32)EndResult);
				return;
			}

			const VkSemaphore WaitSemaphore = SwapChain.AcquireSemaphore;
			const VkSemaphore SignalSemaphore = SwapChain.RenderFinishedSemaphore;
			const VkPipelineStageFlags WaitStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			VkSubmitInfo SubmitInfo{};
			SubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			SubmitInfo.waitSemaphoreCount = 1u;
			SubmitInfo.pWaitSemaphores = &WaitSemaphore;
			SubmitInfo.pWaitDstStageMask = &WaitStage;
			SubmitInfo.commandBufferCount = 1u;
			SubmitInfo.pCommandBuffers = &Cmd;
			SubmitInfo.signalSemaphoreCount = 1u;
			SubmitInfo.pSignalSemaphores = &SignalSemaphore;

			const VkResult SubmitResult = QueueSubmitFn(GraphicsQueue, 1u, &SubmitInfo, nullptr);
			if (SubmitResult != VK_SUCCESS)
			{
				SK_LOG(GLogSkylark, ESKLogVerbosity::Warning, "SKVulkanRHI: vkQueueSubmit for present clear failed (%d).", (int32)SubmitResult);
				return;
			}

			const VkSwapchainKHR SwapchainHandle = SwapChain.Swapchain;
			VkPresentInfoKHR PresentInfo{};
			PresentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
			PresentInfo.waitSemaphoreCount = 1u;
			PresentInfo.pWaitSemaphores = &SignalSemaphore;
			PresentInfo.swapchainCount = 1u;
			PresentInfo.pSwapchains = &SwapchainHandle;
			PresentInfo.pImageIndices = &ImageIndex;

			const VkResult PresentResult = QueuePresentFn(GraphicsQueue, &PresentInfo);
			if (PresentResult == VK_ERROR_OUT_OF_DATE_KHR || PresentResult == VK_SUBOPTIMAL_KHR)
			{
				SwapChain.bResizeRequested = true;
			}
			else if (PresentResult != VK_SUCCESS)
			{
				SK_LOG(GLogSkylark, ESKLogVerbosity::Warning, "SKVulkanRHI: vkQueuePresentKHR failed (%d).", (int32)PresentResult);
			}
			else
			{
				SwapChain.ImageLayouts[ImageIndex] = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
			}

			if (DeviceWaitIdleFn)
			{
				DeviceWaitIdleFn(Device);
			}
		}

		void DestroySwapChain(FSKVulkanSwapChain& SwapChain)
		{
			using namespace VulkanMini;
			if (Device && DeviceWaitIdleFn)
			{
				DeviceWaitIdleFn(Device);
			}

			if (SwapChain.RenderFinishedSemaphore && DestroySemaphoreFn && Device)
			{
				DestroySemaphoreFn(Device, SwapChain.RenderFinishedSemaphore, nullptr);
			}
			SwapChain.RenderFinishedSemaphore = nullptr;

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
			SwapChain.ImageLayouts.clear();
			SwapChain.ImageCount = 0;
			SwapChain.bSupportsTransferDst = false;
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
			ResetCommandBufferFn = GetLoader().GetDeviceProcAddr
				? reinterpret_cast<PFN_vkResetCommandBuffer>(GetLoader().GetDeviceProcAddr(Device, "vkResetCommandBuffer"))
				: nullptr;
			CmdPipelineBarrierFn = GetLoader().GetDeviceProcAddr
				? reinterpret_cast<PFN_vkCmdPipelineBarrier>(GetLoader().GetDeviceProcAddr(Device, "vkCmdPipelineBarrier"))
				: nullptr;
			CmdClearColorImageFn = GetLoader().GetDeviceProcAddr
				? reinterpret_cast<PFN_vkCmdClearColorImage>(GetLoader().GetDeviceProcAddr(Device, "vkCmdClearColorImage"))
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
				&& BeginCommandBufferFn
				&& EndCommandBufferFn
				&& QueueSubmitFn
				&& ResetCommandBufferFn
				&& CmdPipelineBarrierFn
				&& CmdClearColorImageFn
#if defined(_WIN32)
				&& CreateWin32SurfaceFn
#endif
				;
		}

		bool InitializeSubmissionObjects()
		{
			using namespace VulkanMini;
			if (!Device || !CreateCommandPoolFn || !DestroyCommandPoolFn || !AllocateCommandBuffersFn || !BeginCommandBufferFn || !EndCommandBufferFn || !QueueSubmitFn || !ResetCommandBufferFn)
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
			if (CommandList.bRecordedTriangleDraw)
			{
				SK_LOG(GLogSkylark, ESKLogVerbosity::Verbose, "SKVulkanRHI: recorded %s triangle draw (V=%u I=%u Inst=%u).", CommandList.bRecordedInstanced ? "instanced" : (CommandList.bRecordedIndexed ? "indexed" : "non-indexed"), CommandList.RecordedVertexCount, CommandList.RecordedIndexCount, CommandList.RecordedInstanceCount);
			}
			if (!Device || !GraphicsQueue || !ImmediateCommandBuffer || !ResetCommandBufferFn || !BeginCommandBufferFn || !EndCommandBufferFn || !QueueSubmitFn)
			{
				return false;
			}

			if (DeviceWaitIdleFn)
			{
				DeviceWaitIdleFn(Device);
			}

			const VkResult ResetResult = ResetCommandBufferFn(ImmediateCommandBuffer, 0u);
			if (ResetResult != VK_SUCCESS)
			{
				SK_LOG(GLogSkylark, ESKLogVerbosity::Warning, "SKVulkanRHI: vkResetCommandBuffer failed (%d).", (int32)ResetResult);
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

			FSKVulkanPipelineScaffold SolidTriangles;
			SolidTriangles.DebugName = "Vulkan.SolidTriangles";
			SolidTriangles.bGraphics = true;
			SolidTriangles.bPresentPath = false;
			SolidTriangles.Stages.push_back({ "VS", {} });
			SolidTriangles.Stages.push_back({ "PS", {} });
			PipelineScaffolds.push_back(std::move(SolidTriangles));

			FSKVulkanPipelineScaffold InstancedTriangles;
			InstancedTriangles.DebugName = "Vulkan.InstancedTriangles";
			InstancedTriangles.bGraphics = true;
			InstancedTriangles.bPresentPath = false;
			InstancedTriangles.Stages.push_back({ "VS", {} });
			InstancedTriangles.Stages.push_back({ "PS", {} });
			PipelineScaffolds.push_back(std::move(InstancedTriangles));

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

			if ((Caps.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT) == 0u)
			{
				SK_LOG(GLogSkylark, ESKLogVerbosity::Warning, "SKVulkanRHI: swapchain does not support VK_IMAGE_USAGE_TRANSFER_DST_BIT; minimal present clear path cannot run.");
				DestroySwapChain(SwapChain);
				return false;
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
			SwapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
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

			VkSemaphoreCreateInfo SemaphoreInfo{};
			SemaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
			if (!SwapChain.AcquireSemaphore)
			{
				if (CreateSemaphoreFn(Device, &SemaphoreInfo, nullptr, &SwapChain.AcquireSemaphore) != VK_SUCCESS || !SwapChain.AcquireSemaphore)
				{
					DestroySwapChain(SwapChain);
					return false;
				}
			}
			if (!SwapChain.RenderFinishedSemaphore)
			{
				if (CreateSemaphoreFn(Device, &SemaphoreInfo, nullptr, &SwapChain.RenderFinishedSemaphore) != VK_SUCCESS || !SwapChain.RenderFinishedSemaphore)
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
			SwapChain.ImageFormat = ChosenFormat.format;
			SwapChain.bSupportsTransferDst = true;
			SwapChain.ImageLayouts.assign(ImageCount, VK_IMAGE_LAYOUT_UNDEFINED);
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
		VulkanMini::PFN_vkResetCommandBuffer ResetCommandBufferFn = nullptr;
		VulkanMini::PFN_vkCmdPipelineBarrier CmdPipelineBarrierFn = nullptr;
		VulkanMini::PFN_vkCmdClearColorImage CmdClearColorImageFn = nullptr;
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
