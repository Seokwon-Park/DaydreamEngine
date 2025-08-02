#pragma once

#include "Daydream/Graphics/Resources/Shader.h"
#include "VulkanRenderDevice.h"

#include "vulkan/vulkan.h"

namespace Daydream
{
	class VulkanShader : public Shader
	{
	public:
		VulkanShader(VulkanRenderDevice* _device, const std::string& _src, const ShaderType& _type, const ShaderLoadMode& _mode);
		virtual ~VulkanShader() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void* GetNativeHandle() override { return shader; }
	private:
		VulkanRenderDevice* device;
		VkShaderModule shader;
		VkShaderStageFlagBits stageBit;


		// Inherited via Shader
	};
}