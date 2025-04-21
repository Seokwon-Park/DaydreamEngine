#pragma once

#include "Steins/Render/Shader.h"

#include "vulkan/vulkan.h"

namespace Steins
{
	class VulkanShader : public Shader
	{
	public:
		VulkanShader(const FilePath& _filepath, const ShaderType& _type);
		VulkanShader(const std::string& _src, const ShaderType& _type);
	private:
		VkShaderModule shader;
	};
}