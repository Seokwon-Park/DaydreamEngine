#pragma once

#include "vulkan/vulkan.h"
#include "Steins/Graphics/Core/RenderDevice.h"

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
		Array<VkSurfaceFormatKHR> formats;
		//present 방식
		Array<VkPresentModeKHR> presentModes;
	};

	class VulkanRenderDevice :public RenderDevice
	{
	public:
		VulkanRenderDevice();
		virtual ~VulkanRenderDevice() override;

		virtual void Init() override;
		virtual void Shutdown() override;
		virtual void Render() override;

		virtual Shared<GraphicsContext> CreateContext() override;
		virtual Shared<VertexBuffer> CreateDynamicVertexBuffer(UInt32 _bufferSize, UInt32 _stride)override;
		virtual Shared<VertexBuffer> CreateStaticVertexBuffer(Float32* _vertices, UInt32 _size, UInt32 _stride) override;
		virtual Shared<IndexBuffer> CreateIndexBuffer(UInt32* _indices, UInt32 _count) override;
		virtual Shared<Framebuffer> CreateFramebuffer(FramebufferDesc _spec)override;
		virtual Shared<PipelineState> CreatePipelineState(PipelineStateDesc _desc)override;
		virtual Shared<Shader> CreateShader(const std::string& _src, const ShaderType& _type, ShaderLoadMode _mode) override;
		virtual Shared<SwapChain> CreateSwapChain(SwapChainSpecification* _desc, SteinsWindow* _window)override;
		virtual Shared<Texture2D> CreateTexture2D(const FilePath& _path)override;
		virtual Unique<ImGuiRenderer> CreateImGuiRenderer() override;
		virtual Shared<ConstantBuffer> CreateConstantBuffer(UInt32 _size) override;
		virtual Shared<Material> CreateMaterial(Shared<PipelineState> _pipeline) override;

		VkInstance GetInstance() const { return instance; }
		VkPhysicalDevice GetGPU() const { return physicalDevice; }
		VkDevice GetDevice() const { return device; }
		UInt32 GetGraphicsFamilyIndex() const { return queueFamilyIndices.graphicsFamily.value(); }
		VkQueue GetQueue() const { return graphicsQueue; }
		VkDescriptorPool GetDescriptorPool() const { return descriptorPool; }
		VkCommandBuffer GetCommandBuffer() const { return commandBuffer; }
		VkCommandPool GetCommandPool() const { return commandPool; }

		void SetCurrentRenderPass(VkRenderPass _renderPass) { renderPass = _renderPass; }
		VkRenderPass GetCurrentRenderPass() const { return renderPass; }

		VkCommandBuffer BeginSingleTimeCommands();
		void EndSingleTimeCommands(VkCommandBuffer _commandBuffer);

		SwapChainSupportDetails QuerySwapChainSupport(VkSurfaceKHR _surface);
		UInt32 FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		//Create a VkBuffer
		void CreateBuffer(VkDeviceSize _size, VkBufferUsageFlags _usage, VkMemoryPropertyFlags _properties, VkBuffer& _buffer, VkDeviceMemory& _bufferMemory);
		void CreateImage(UInt32 _width, UInt32 _height, VkFormat _format, VkImageTiling tiling, VkImageUsageFlags _usage, VkMemoryPropertyFlags _properties, VkImage& _image, VkDeviceMemory& _imageMemory);
		void CreateImageView(VkImage _image, VkFormat _format, VkImageView& _imageView);
		void CopyBuffer(VkBuffer _src, VkBuffer _dst, VkDeviceSize _size);
		void CopyBufferToImage(VkBuffer _src, VkImage _dst, UInt32 _width, UInt32 _height);
		void TransitionTextureLayout(VkImage _image, VkFormat _format, VkImageLayout _oldLayout, VkImageLayout _newLayout);

		//SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice _physicalDevice, VkSurfaceKHR _surface);
	private:
		void CreateInstance();
		void SetupDebugMessenger();
		void CreateLogicalDevice();
		void PickPhysicalDevice();

		bool CheckValidationLayerSupport();
		Array<const char*> GetRequiredExtensions();
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& _createInfo);
		bool IsDeviceSuitable(VkPhysicalDevice _physicalDevice);
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice _physicalDevice);
		bool CheckDeviceExtensionSupport(VkPhysicalDevice _physicalDevice);

#if defined(STEINS_DEBUG) || defined(_DEBUG)
		const bool enableValidationLayers = true;
		const Array<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
#else
		const bool enableValidationLayers = false;
		const Array<const char*> validationLayers = {};
#endif
		const Array<const char*> deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME,
			VK_GOOGLE_HLSL_FUNCTIONALITY_1_EXTENSION_NAME,
			VK_GOOGLE_USER_TYPE_EXTENSION_NAME
		};
		VkInstance instance; // Vulkan library handle
		VkDebugUtilsMessengerEXT debugMessenger;
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE; // GPU chosen as the default device
		VkDevice device; // Vulkan device for commands
		QueueFamilyIndices queueFamilyIndices;
		VkQueue graphicsQueue; // vulkan graphics Queue
		VkCommandPool commandPool;
		VkCommandBuffer commandBuffer;
		VkDescriptorPool descriptorPool;

		VkRenderPass renderPass;
	};
}

