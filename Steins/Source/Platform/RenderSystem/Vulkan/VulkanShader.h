#pragma once

#include "Steins/Render/Shader.h"
#include "VulkanGraphicsDevice.h"

#include "vulkan/vulkan.h"

namespace Steins
{
	class VulkanShader : public Shader
	{
	public:
		VulkanShader(VulkanGraphicsDevice* _device, const std::string& _src, const ShaderType& _type, const ShaderLoadMode& _mode);
		virtual ~VulkanShader() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void* GetNativeHandle() override { return shader; }
	private:
		VulkanGraphicsDevice* device;
		VkShaderModule shader;

		// Inherited via Shader
	};
}