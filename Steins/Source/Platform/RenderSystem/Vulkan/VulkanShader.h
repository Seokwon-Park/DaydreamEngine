#pragma once

#include "Steins/Render/Shader.h"
#include "VulkanGraphicsDevice.h"

#include "vulkan/vulkan.h"

namespace Steins
{
	class VulkanShader : public Shader
	{
	public:
		VulkanShader(GraphicsDevice* _device, const FilePath& _filepath, const ShaderType& _type);
		VulkanShader(GraphicsDevice* _device, const std::string& _src, const ShaderType& _type);
	private:
		VulkanGraphicsDevice* device;
		VkShaderModule shader;
	};
}