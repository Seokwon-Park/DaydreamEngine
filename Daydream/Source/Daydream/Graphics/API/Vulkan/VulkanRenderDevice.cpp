#include "DaydreamPCH.h"
#include "VulkanRenderDevice.h"
#include "VulkanRenderContext.h"
#include "VulkanBuffer.h"
#include "VulkanRenderPass.h"
#include "VulkanFramebuffer.h"
#include "VulkanPipelineState.h"
#include "VulkanShader.h"
#include "VulkanSwapchain.h"
#include "VulkanImGuiRenderer.h"
#include "VulkanTexture.h"
#include "VulkanMaterial.h"
#include "Daydream/Graphics/Utility/GraphicsUtility.h"
#include "Daydream/Graphics/Utility/ImageLoader.h"

#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"

#include "backends/imgui_impl_vulkan.h"

namespace vk
{
	namespace detail
	{
		DispatchLoaderDynamic defaultDispatchLoaderDynamic;
	}
}

namespace Daydream
{
	namespace
	{

		static VKAPI_ATTR vk::Bool32 VKAPI_CALL DebugCallback(
			vk::DebugUtilsMessageSeverityFlagBitsEXT _messageSeverity,
			vk::DebugUtilsMessageTypeFlagsEXT _messageType,
			const vk::DebugUtilsMessengerCallbackDataEXT* _pCallbackData,
			void* _pUserData)
		{
			if (_messageSeverity & vk::DebugUtilsMessageSeverityFlagBitsEXT::eError)
			{
				DAYDREAM_CORE_ERROR("Validation layer: {0}", _pCallbackData->pMessage);
			}
			else if (_messageSeverity & vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning)
			{
				DAYDREAM_CORE_WARN("Validation layer: {0}", _pCallbackData->pMessage);
			}
			else if (_messageSeverity & vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo)
			{
				DAYDREAM_CORE_INFO("Validation layer: {0}", _pCallbackData->pMessage);
			}
			else if (_messageSeverity & vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose)
			{
				DAYDREAM_CORE_TRACE("Validation layer: {0}", _pCallbackData->pMessage);
			}

			return VK_FALSE;
		}

		static VkResult CreateDebugUtilsMessengerEXT(VkInstance _instance, const VkDebugUtilsMessengerCreateInfoEXT* _pCreateInfo, const VkAllocationCallbacks* _pAllocator, VkDebugUtilsMessengerEXT* _pDebugMessenger)
		{
			auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(_instance, "vkCreateDebugUtilsMessengerEXT");
			if (func != nullptr)
			{
				return func(_instance, _pCreateInfo, _pAllocator, _pDebugMessenger);
			}
			else
			{
				return VK_ERROR_EXTENSION_NOT_PRESENT;
			}
		}

		static void DestroyDebugUtilsMessengerEXT(VkInstance _instance, VkDebugUtilsMessengerEXT _debugMessenger, const VkAllocationCallbacks* _pAllocator)
		{
			auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(_instance, "vkDestroyDebugUtilsMessengerEXT");
			if (func != nullptr)
			{
				func(_instance, _debugMessenger, _pAllocator);
			}
		}
	}

	VulkanRenderDevice::VulkanRenderDevice()
	{
		API = RendererAPIType::Vulkan;
	}

	VulkanRenderDevice::~VulkanRenderDevice()
	{

	}
	void VulkanRenderDevice::Init()
	{
		CreateInstance();
		SetupDebugMessenger();
		PickPhysicalDevice();
		CreateLogicalDevice();

		vk::CommandPoolCreateInfo poolCreateInfo{};
		poolCreateInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
		poolCreateInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

		commandPool = device->createCommandPoolUnique(poolCreateInfo, nullptr);

		{
			Array<vk::DescriptorPoolSize> poolSizes =
			{
				{ vk::DescriptorType::eCombinedImageSampler , IMGUI_IMPL_VULKAN_MINIMUM_IMAGE_SAMPLER_POOL_SIZE },
				{ vk::DescriptorType::eUniformBuffer , 3 },
				{ vk::DescriptorType::eCombinedImageSampler , 2 },
			};


			vk::DescriptorPoolCreateInfo poolInfo = {};
			poolInfo.flags = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet | vk::DescriptorPoolCreateFlagBits::eUpdateAfterBind;
			poolInfo.maxSets = 0;
			for (vk::DescriptorPoolSize& poolSize : poolSizes)
				poolInfo.maxSets += poolSize.descriptorCount;
			poolInfo.poolSizeCount = (uint32_t)poolSizes.size();
			poolInfo.pPoolSizes = poolSizes.data();
			descriptorPool = device->createDescriptorPoolUnique(poolInfo, nullptr);
		}

		vma::AllocatorCreateInfo allocatorInfo;
		allocatorInfo.physicalDevice = physicalDevice; // vk::PhysicalDevice
		allocatorInfo.device = device.get();             // vk::Device
		allocatorInfo.instance = instance.get();           // vk::Instance
		allocatorInfo.vulkanApiVersion = VK_API_VERSION_1_4; // 사용 중인 Vulkan 버전

		allocator = vma::createAllocatorUnique(allocatorInfo);

		vk::PhysicalDeviceProperties properties = physicalDevice.getProperties();
		DAYDREAM_CORE_INFO("Vulkan Info:");
		DAYDREAM_CORE_INFO("  Vendor: {0}", GraphicsUtility::GetVendor(properties.vendorID));
		DAYDREAM_CORE_INFO("  Renderer: {0}", properties.deviceName.data());
		DAYDREAM_CORE_INFO("  Version: {0}.{1}.{2}",
			VK_API_VERSION_MAJOR(properties.apiVersion),
			VK_API_VERSION_MINOR(properties.apiVersion),
			VK_API_VERSION_PATCH(properties.apiVersion));
	}

	void VulkanRenderDevice::Shutdown()
	{
	}

	void VulkanRenderDevice::Render()
	{
	}

	Shared<RenderContext> VulkanRenderDevice::CreateContext()
	{
		return MakeShared<VulkanGraphicsContext>(this);
	}

	Shared<VertexBuffer> VulkanRenderDevice::CreateDynamicVertexBuffer(UInt32 _size, UInt32 _stride, UInt32 _initialDataSize, const void* _initialData)
	{
		auto vertexBuffer = MakeShared<VulkanVertexBuffer>(this, BufferUsage::Dynamic, _size);
		if (_initialData)
		{
			vertexBuffer->SetData(_initialData, _initialDataSize);
		}
		return vertexBuffer;
	}

	Shared<VertexBuffer> VulkanRenderDevice::CreateStaticVertexBuffer(UInt32 _size, UInt32 _stride, const void* _initialData)
	{
		auto vertexBuffer = MakeShared<VulkanVertexBuffer>(this, BufferUsage::Static, _size);

		auto [uploadBuffer, uploadBufferAllocation] = CreateBuffer(
			_size,
			vk::BufferUsageFlagBits::eTransferSrc,
			vma::MemoryUsage::eCpuOnly,
			vma::AllocationCreateFlagBits::eMapped);

		vma::AllocationInfo allocationInfo;
		allocator->getAllocationInfo(uploadBufferAllocation.get(), &allocationInfo);
		memcpy(allocationInfo.pMappedData, _initialData, _size);
		this->CopyBuffer(uploadBuffer.get(), vertexBuffer->GetVkBuffer(), _size);

		return vertexBuffer;
	}

	Shared<IndexBuffer> Daydream::VulkanRenderDevice::CreateIndexBuffer(const UInt32 * _indices, UInt32 _count)
	{
		auto indexBuffer = MakeShared<VulkanIndexBuffer>(this, _count);

		UInt32 bufferSize = sizeof(UInt32) * _count;
		auto [uploadBuffer, uploadBufferAllocation] = CreateBuffer(
			bufferSize,
			vk::BufferUsageFlagBits::eTransferSrc,
			vma::MemoryUsage::eCpuOnly,
			vma::AllocationCreateFlagBits::eMapped);

		vma::AllocationInfo allocationInfo;
		// GetAllocator()는 VmaAllocator 핸들을 반환하는 함수라고 가정합니다.
		allocator->getAllocationInfo(uploadBufferAllocation.get(), &allocationInfo);
		memcpy(allocationInfo.pMappedData, _indices, bufferSize);

		CopyBuffer(uploadBuffer.get(), indexBuffer->GetVkBuffer(), bufferSize); // CopyBuffer는 vkCmdCopyBuffer를 호출하는 헬퍼 함수

		return indexBuffer;
	}

	Shared<RenderPass> VulkanRenderDevice::CreateRenderPass(const RenderPassDesc& _desc)
	{
		return MakeShared<VulkanRenderPass>(this, _desc);
	}

	Shared<Framebuffer> VulkanRenderDevice::CreateFramebuffer(Shared<RenderPass> _renderPass, const FramebufferDesc& _desc)
	{
		return _renderPass->CreateFramebuffer(_desc);
	}

	Shared<PipelineState> VulkanRenderDevice::CreatePipelineState(const PipelineStateDesc& _desc)
	{
		return MakeShared<VulkanPipelineState>(this, _desc);
	}

	Shared<Shader> VulkanRenderDevice::CreateShader(const std::string& _src, const ShaderType& _type, ShaderLoadMode _mode)
	{
		return MakeShared<VulkanShader>(this, _src, _type, _mode);
	}

	Shared<Swapchain> VulkanRenderDevice::CreateSwapchain(DaydreamWindow* _window, const SwapchainDesc& _desc)
	{
		return MakeShared<VulkanSwapchain>(this, _window, _desc);
	}

	Shared<Texture2D> VulkanRenderDevice::CreateTexture2D(const void* _imageData, const TextureDesc& _desc)
	{
		auto texture = MakeShared<VulkanTexture2D>(this, _desc);

		if (_imageData != nullptr)
		{
			UInt32 imageSize = _desc.width * _desc.height * 4;
			auto [uploadBuffer, uploadBufferAllocation] = CreateBuffer(
				imageSize,
				vk::BufferUsageFlagBits::eTransferSrc,
				vma::MemoryUsage::eCpuOnly,
				vma::AllocationCreateFlagBits::eMapped);
			
			vma::AllocationInfo allocationInfo;
			allocator->getAllocationInfo(uploadBufferAllocation.get(), &allocationInfo);
			memcpy(allocationInfo.pMappedData, _imageData, imageSize);

			CopyBufferToImage(uploadBuffer.get(), texture->GetImage(), _desc.width, _desc.height);
		}
		return texture;
	}

	Unique<ImGuiRenderer> VulkanRenderDevice::CreateImGuiRenderer()
	{
		return MakeUnique<VulkanImGuiRenderer>(this);
	}

	Shared<ConstantBuffer> VulkanRenderDevice::CreateConstantBuffer(UInt32 _size)
	{
		return MakeShared<VulkanConstantBuffer>(this, _size);
	}

	Shared<Material> VulkanRenderDevice::CreateMaterial(Shared<PipelineState> _pipeline)
	{
		return _pipeline->CreateMaterial();
	}


	vk::CommandBuffer VulkanRenderDevice::BeginSingleTimeCommands()
	{
		vk::CommandBufferAllocateInfo allocInfo{};
		allocInfo.level = vk::CommandBufferLevel::ePrimary;
		allocInfo.commandPool = commandPool.get();
		allocInfo.commandBufferCount = 1;

		std::vector<vk::CommandBuffer> commandBuffers(1);
		commandBuffers = device->allocateCommandBuffers(allocInfo);

		vk::CommandBufferBeginInfo beginInfo{};
		beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;

		commandBuffers[0].begin(beginInfo);

		return commandBuffers[0];
	}

	void VulkanRenderDevice::EndSingleTimeCommands(vk::CommandBuffer _commandBuffer)
	{
		_commandBuffer.end();

		vk::SubmitInfo submitInfo{};
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &_commandBuffer;

		vk::Result result = graphicsQueue.submit(1, &submitInfo, {});
		vkQueueWaitIdle(graphicsQueue);

		device->freeCommandBuffers(commandPool.get(), 1, &_commandBuffer);
	}

	SwapchainSupportDetails VulkanRenderDevice::QuerySwapchainSupport(vk::SurfaceKHR _surface)
	{
		SwapchainSupportDetails details;
		details.capabilities = physicalDevice.getSurfaceCapabilitiesKHR(_surface);
		details.formats = physicalDevice.getSurfaceFormatsKHR(_surface);
		details.presentModes = physicalDevice.getSurfacePresentModesKHR(_surface);

		return details;
	}

	UInt32 VulkanRenderDevice::FindMemoryType(UInt32 _typeFilter, vk::MemoryPropertyFlags _properties)
	{
		vk::PhysicalDeviceMemoryProperties memProperties;
		memProperties = physicalDevice.getMemoryProperties();

		for (UInt32 i = 0; i < memProperties.memoryTypeCount; i++) {
			if ((_typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & _properties) == _properties)
			{
				return i;
			}
		}
		return 0;
	}

	Pair<vma::UniqueBuffer, vma::UniqueAllocation> VulkanRenderDevice::CreateBuffer(vk::DeviceSize _size, vk::BufferUsageFlags _usage,
		vma::MemoryUsage _memoryUsage, vma::AllocationCreateFlags _flags)
	{
		vk::BufferCreateInfo bufferInfo{};
		bufferInfo.size = _size;
		bufferInfo.usage = _usage;
		bufferInfo.sharingMode = vk::SharingMode::eExclusive;

		vma::AllocationCreateInfo allocInfo{};
		allocInfo.usage = _memoryUsage;
		allocInfo.flags = _flags;

		//vk::MemoryRequirements memRequirements = device->getBufferMemoryRequirements(buffer);

		//vk::MemoryAllocateInfo allocInfo{};
		//allocInfo.allocationSize = memRequirements.size;
		//allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, _properties);

		//vk::DeviceMemory bufferMemory = device->allocateMemory(allocInfo);
		//device->bindBufferMemory(buffer, bufferMemory, 0);

		return allocator->createBufferUnique(bufferInfo, allocInfo);
	}

	VulkanImageResource VulkanRenderDevice::CreateImage(UInt32 _width, UInt32 _height, vk::Format _format, vk::ImageTiling _tiling, vk::ImageUsageFlags _usage, vk::MemoryPropertyFlags _properties)
	{
		vk::ImageCreateInfo imageInfo{};
		imageInfo.imageType = vk::ImageType::e2D;
		imageInfo.extent.width = static_cast<UInt32>(_width);
		imageInfo.extent.height = static_cast<UInt32>(_height);
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = _format;
		imageInfo.tiling = _tiling;
		imageInfo.initialLayout = vk::ImageLayout::eUndefined;
		imageInfo.usage = _usage;
		imageInfo.sharingMode = vk::SharingMode::eExclusive;
		imageInfo.samples = vk::SampleCountFlagBits::e1;

		vk::Image image = device->createImage(imageInfo);

		vk::MemoryRequirements memRequirements = device->getImageMemoryRequirements(image);

		vk::MemoryAllocateInfo allocInfo{};
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, _properties);

		vk::DeviceMemory imageMemory = device->allocateMemory(allocInfo);
		device->bindImageMemory(image, imageMemory, 0);

		return { image, imageMemory };
	}

	vk::ImageView VulkanRenderDevice::CreateImageView(vk::Image _image, vk::Format _format, vk::ImageAspectFlags _aspectMask)
	{
		vk::ImageViewCreateInfo viewInfo{};
		viewInfo.image = _image;
		viewInfo.viewType = vk::ImageViewType::e2D;
		viewInfo.format = _format;
		viewInfo.subresourceRange.aspectMask = _aspectMask;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		vk::ImageView imageView = device->createImageView(viewInfo);

		return imageView;
	}

	void VulkanRenderDevice::CopyBuffer(vk::Buffer _src, vk::Buffer _dst, vk::DeviceSize _size)
	{
		vk::CommandBuffer copyCommandBuffer = BeginSingleTimeCommands();

		vk::BufferCopy copyRegion{};
		copyRegion.srcOffset = 0; // Optional
		copyRegion.dstOffset = 0; // Optional
		copyRegion.size = _size;

		copyCommandBuffer.copyBuffer(_src, _dst, 1, &copyRegion);

		EndSingleTimeCommands(copyCommandBuffer);
	}

	void VulkanRenderDevice::CopyBufferToImage(vk::Buffer _src, vk::Image _dst, UInt32 _width, UInt32 _height)
	{
		vk::CommandBuffer copyCommandBuffer = BeginSingleTimeCommands();

		vk::BufferImageCopy region{};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;

		region.imageSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;

		region.imageOffset = vk::Offset3D();
		region.imageExtent = vk::Extent3D(
			_width,
			_height,
			1
		);

		copyCommandBuffer.copyBufferToImage(_src, _dst, vk::ImageLayout::eTransferDstOptimal, 1, &region);

		EndSingleTimeCommands(copyCommandBuffer);
	}

	void VulkanRenderDevice::TransitionImageLayout(vk::Image _image, vk::Format _format, vk::ImageLayout _oldLayout, vk::ImageLayout _newLayout)
	{
		vk::CommandBuffer transCommandBuffer = BeginSingleTimeCommands();

		vk::ImageMemoryBarrier barrier{};
		barrier.oldLayout = _oldLayout;
		barrier.newLayout = _newLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = _image;
		if (_format == vk::Format::eD32SfloatS8Uint || _format == vk::Format::eD24UnormS8Uint)
		{
			barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil;
		}
		else
		{
			barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
		}
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.layerCount = 1;
		barrier.subresourceRange.levelCount = 1;

		vk::AccessFlags srcAccess;
		vk::AccessFlags dstAccess;

		vk::PipelineStageFlags srcStage{};
		vk::PipelineStageFlags dstStage{};

		using enum vk::PipelineStageFlagBits;
		// 패턴별로 그룹화
		switch (_oldLayout)
		{
		case vk::ImageLayout::eUndefined:
			srcAccess = {};
			srcStage = vk::PipelineStageFlagBits::eTopOfPipe;
			break;
		case vk::ImageLayout::eTransferDstOptimal:
			srcAccess = vk::AccessFlagBits::eTransferWrite;
			srcStage = vk::PipelineStageFlagBits::eTransfer;
			break;
		case vk::ImageLayout::eColorAttachmentOptimal:
			srcAccess = vk::AccessFlagBits::eColorAttachmentWrite;
			srcStage = vk::PipelineStageFlagBits::eColorAttachmentOutput;
			break;
		case vk::ImageLayout::eShaderReadOnlyOptimal:
			srcAccess = vk::AccessFlagBits::eShaderRead;
			srcStage = vk::PipelineStageFlagBits::eFragmentShader;
			break;
		default:
			throw std::invalid_argument("Unsupported old layout!");
		}

		switch (_newLayout)
		{
		case vk::ImageLayout::eTransferDstOptimal:
			dstAccess = vk::AccessFlagBits::eTransferWrite;
			dstStage = vk::PipelineStageFlagBits::eTransfer;
			break;
		case vk::ImageLayout::eShaderReadOnlyOptimal:
			dstAccess = vk::AccessFlagBits::eShaderRead;
			dstStage = vk::PipelineStageFlagBits::eFragmentShader;
			break;
		case vk::ImageLayout::eColorAttachmentOptimal:
			dstAccess = vk::AccessFlagBits::eColorAttachmentWrite;
			dstStage = vk::PipelineStageFlagBits::eColorAttachmentOutput;
			break;
		case vk::ImageLayout::eDepthStencilAttachmentOptimal:
			dstAccess = vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite;
			dstStage = vk::PipelineStageFlagBits::eEarlyFragmentTests;
			break;
		case vk::ImageLayout::ePresentSrcKHR:
			dstAccess = {}; // 또는 VK_ACCESS_NONE (Vulkan 1.3 이상)
			dstStage = vk::PipelineStageFlagBits::eBottomOfPipe; // 또는 VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT
			break;
		default:
			throw std::invalid_argument("Unsupported new layout!");
		}

		barrier.srcAccessMask = srcAccess;
		barrier.dstAccessMask = dstAccess;

		transCommandBuffer.pipelineBarrier(
			srcStage, dstStage,
			{},
			0, nullptr,
			0, nullptr,
			1, &barrier
		);

		EndSingleTimeCommands(transCommandBuffer);
	}


	void VulkanRenderDevice::CreateInstance()
	{
		vk::detail::defaultDispatchLoaderDynamic.init();
		if (enableValidationLayers == true && CheckValidationLayerSupport() == false)
		{
			DAYDREAM_CORE_ERROR("Validation layers requested, but not available!");
			throw std::runtime_error("Validation layers not supported!");
		}

		vk::ApplicationInfo appInfo(
			"My Vulkan App",
			VK_VERSION_1_4,
			"DaydreamEngine",
			VK_VERSION_1_4,
			VK_API_VERSION_1_3
		);

		Array<const char*> extensions = GetRequiredExtensions();

		vk::InstanceCreateInfo createInfo(
			{},
			&appInfo,
			0,
			{},
			(UInt32)extensions.size(),
			extensions.data()
		);

		vk::DebugUtilsMessengerCreateInfoEXT debugCreateInfo;
		if (enableValidationLayers)
		{
			createInfo.enabledLayerCount = static_cast<UInt32>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
			PopulateDebugMessengerCreateInfo(debugCreateInfo);
			createInfo.pNext = &debugCreateInfo;
		}

		instance = vk::createInstanceUnique(createInfo);
		vk::detail::defaultDispatchLoaderDynamic.init(*instance);
	}
	void VulkanRenderDevice::SetupDebugMessenger()
	{
		if (!enableValidationLayers) return;

		vk::DebugUtilsMessengerCreateInfoEXT createInfo;
		PopulateDebugMessengerCreateInfo(createInfo);

		debugMessenger = instance->createDebugUtilsMessengerEXTUnique(createInfo);
	}
	void VulkanRenderDevice::PickPhysicalDevice()
	{
		auto physicalDevices = instance->enumeratePhysicalDevices();
		if (physicalDevices.empty())
		{
			DAYDREAM_CORE_ERROR("Failed to find GPUs with Vulkan support!");
		}

		for (const vk::PhysicalDevice& device : physicalDevices)
		{
			if (IsDeviceSuitable(device))
			{
				physicalDevice = device;
				break;
			}
		}

		if (physicalDevice == VK_NULL_HANDLE)
		{
			DAYDREAM_CORE_ERROR("Failed to find a suitable GPU!");
		}
	}
	void VulkanRenderDevice::CreateLogicalDevice()
	{
		Float32 queuePriority = 1.0f;
		queueFamilyIndices = FindQueueFamilies(physicalDevice);
		vk::DeviceQueueCreateInfo queueCreateInfo(
			{},
			queueFamilyIndices.graphicsFamily.value(),
			1,
			&queuePriority
		);

		vk::PhysicalDeviceDescriptorIndexingFeatures descriptorIndexingFeatures;
		descriptorIndexingFeatures.runtimeDescriptorArray = VK_TRUE;
		descriptorIndexingFeatures.descriptorBindingPartiallyBound = VK_TRUE;
		descriptorIndexingFeatures.descriptorBindingUniformBufferUpdateAfterBind = VK_TRUE;
		descriptorIndexingFeatures.descriptorBindingSampledImageUpdateAfterBind = VK_TRUE;

		vk::PhysicalDeviceFeatures2 deviceFeatures{};
		deviceFeatures.features.samplerAnisotropy = VK_TRUE;
		deviceFeatures.pNext = &descriptorIndexingFeatures;

		vk::DeviceCreateInfo createInfo{};
		createInfo.queueCreateInfoCount = 1;
		createInfo.pQueueCreateInfos = &queueCreateInfo;
		createInfo.enabledLayerCount = Cast<UInt32>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.empty() ? nullptr : validationLayers.data();
		createInfo.enabledExtensionCount = Cast<UInt32>(deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = deviceExtensions.empty() ? nullptr : deviceExtensions.data();
		createInfo.pEnabledFeatures = nullptr;
		createInfo.pNext = &deviceFeatures;

		device = physicalDevice.createDeviceUnique(createInfo);
		graphicsQueue = device->getQueue(queueFamilyIndices.graphicsFamily.value(), 0);
	}

	bool VulkanRenderDevice::CheckValidationLayerSupport()
	{
		Array<vk::LayerProperties> availableLayers = vk::enumerateInstanceLayerProperties();

		for (const char* layerName : validationLayers) {
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers) {
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}

			if (!layerFound) {
				return false;
			}
		}

		return true;
	}
	Array<const char*> VulkanRenderDevice::GetRequiredExtensions()
	{
		UInt32 glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		Array<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (enableValidationLayers) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}
	void VulkanRenderDevice::PopulateDebugMessengerCreateInfo(vk::DebugUtilsMessengerCreateInfoEXT& _createInfo)
	{
		using enum vk::DebugUtilsMessageSeverityFlagBitsEXT;
		using enum vk::DebugUtilsMessageTypeFlagBitsEXT;

		_createInfo.messageSeverity = eVerbose | eWarning | eError;
		_createInfo.messageType = eGeneral | eValidation | ePerformance;
		_createInfo.pfnUserCallback = DebugCallback;
		_createInfo.pUserData = nullptr; // 원한다면 this 포인터 등을 전달할 수 있음
	}
	bool VulkanRenderDevice::IsDeviceSuitable(vk::PhysicalDevice _physicalDevice)
	{
		vk::PhysicalDeviceProperties deviceProperties;
		vk::PhysicalDeviceFeatures deviceFeatures;
		vk::PhysicalDeviceProperties properties = _physicalDevice.getProperties();
		auto feautures = _physicalDevice.getFeatures();

		//이 GPU가 swapchain을 지원할 수 있는지 확인?
		bool extensionSupported = CheckDeviceExtensionSupport(_physicalDevice);

		QueueFamilyIndices indices = FindQueueFamilies(_physicalDevice);
		//return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
		//	deviceFeatures.geometryShader;
		vk::PhysicalDeviceFeatures supportedFeatures = _physicalDevice.getFeatures();

		return indices.IsComplete() && extensionSupported && supportedFeatures.samplerAnisotropy;
	}
	QueueFamilyIndices VulkanRenderDevice::FindQueueFamilies(vk::PhysicalDevice _physicalDevice)
	{
		QueueFamilyIndices indices;

		UInt32 queueFamilyCount = 0;
		Array<vk::QueueFamilyProperties> queueFamilies = _physicalDevice.getQueueFamilyProperties();

		int i = 0;
		for (UInt32 i = 0; i < (UInt32)queueFamilies.size(); i++)
		{
			if (queueFamilies[i].queueFlags & vk::QueueFlagBits::eGraphics)
			{
				indices.graphicsFamily = i;
			}
			if (indices.IsComplete())
			{
				break;
			}
		}

		return indices;
	}

	bool VulkanRenderDevice::CheckDeviceExtensionSupport(vk::PhysicalDevice _physicalDevice)
	{
		UInt32 extensionCount = 0;
		Array<vk::ExtensionProperties> availableExtensions = _physicalDevice.enumerateDeviceExtensionProperties();

		std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

		for (const vk::ExtensionProperties& extension : availableExtensions)
		{
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}



}
