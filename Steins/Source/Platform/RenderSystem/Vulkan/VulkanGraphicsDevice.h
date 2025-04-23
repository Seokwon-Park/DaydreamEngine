#pragma once

#include "vulkan/vulkan.h"
#include "vulkan/vulkan.hpp"
#include "Steins/Render/GraphicsDevice.h"

namespace Steins
{
	struct QueueFamilyIndices
	{
		std::optional<UInt32> graphicsFamily;
		//std::optional<UInt32> presentFamily; // 지금은 필요없는듯.

		bool IsComplete() {
			return graphicsFamily.has_value();
		}
	};

	struct SwapChainSupportDetails
	{
		//gpu와 surface 조합에서 지원되는 최소/최대 이미지 크기정보
		VkSurfaceCapabilitiesKHR capabilities;
		//색상 format
		std::vector<VkSurfaceFormatKHR> formats;
		//present 방식
		std::vector<VkPresentModeKHR> presentModes;
	};

	class VulkanGraphicsDevice :public GraphicsDevice
	{
	public:

		VulkanGraphicsDevice();
		virtual ~VulkanGraphicsDevice() override;

		virtual void Init() override;
		virtual void Shutdown() override;
		virtual void Render() override;

		virtual Shared<VertexBuffer> CreateVertexBuffer(Float32* _vertices, UInt32 _size) override;
		virtual Shared<IndexBuffer> CreateIndexBuffer(UInt32* _indices, UInt32 _count) override;
		virtual Shared<Framebuffer> CreateFramebuffer(FramebufferSpecification _spec)override;
		virtual Shared<PipelineState> CreatePipelineState(PipelineStateDesc _desc)override;
		virtual Shared<Shader> CreateShader(const std::string& _src, const ShaderType& _type, ShaderLoadMode _mode) override;
		virtual Shared<SwapChain> CreateSwapChain(SwapChainSpecification* _desc, SteinsWindow* _window)override;
		virtual Shared<Texture2D> CreateTexture2D(const FilePath& _path)override;
		virtual Unique<ImGuiRenderer> CreateImGuiRenderer() override;


		VkInstance GetInstance() const { return instance; }
		VkPhysicalDevice GetPhysicalDevice() const { return physicalDevice; }
		VkDevice GetLogicalDevice() const { return device; }
		UInt32 GetQueueFamily() const { return queueFamilyIndices.graphicsFamily.value(); }
		VkQueue GetQueue() const { return graphicsQueue; }
		VkRenderPass GetRenderPass() const { return renderPass; }
		VkDescriptorPool GetDescriptorPool() const { return descriptorPool; }
		VkCommandBuffer GetCommandBuffer() const { return commandBuffer; }

		SwapChainSupportDetails QuerySwapChainSupport(VkSurfaceKHR _surface);
		//SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice _physicalDevice, VkSurfaceKHR _surface);

	private:
		void CreateInstance();
		void SetupDebugMessenger();
		void CreateLogicalDevice();
		void PickPhysicalDevice();

		bool CheckValidationLayerSupport();
		std::vector<const char*> GetRequiredExtensions();
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& _createInfo);
		bool IsDeviceSuitable(VkPhysicalDevice _physicalDevice);
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice _physicalDevice);
		bool CheckDeviceExtensionSupport(VkPhysicalDevice _physicalDevice);

#if defined(STEINS_DEBUG) || defined(_DEBUG)
		const bool enableValidationLayers = true;
		const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
#else
		const bool enableValidationLayers = false;
		const std::vector<const char*> validationLayers = {};
#endif
		const std::vector<const char*> deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};
		VkInstance instance; // Vulkan library handle
		VkDebugUtilsMessengerEXT debugMessenger;
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE; // GPU chosen as the default device
		VkDevice device; // Vulkan device for commands
		QueueFamilyIndices queueFamilyIndices;
		VkQueue graphicsQueue; // vulkan graphics Queue
		VkRenderPass renderPass;
		VkCommandPool commandPool;
		VkCommandBuffer commandBuffer;
		VkDescriptorPool descriptorPool;
	};
}

