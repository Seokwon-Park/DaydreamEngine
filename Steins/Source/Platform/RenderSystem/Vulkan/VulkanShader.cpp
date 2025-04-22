#include "SteinsPCH.h"
#include "VulkanShader.h"

namespace Steins
{
	VulkanShader::VulkanShader(GraphicsDevice* _device, const FilePath& _filepath, const ShaderType& _type)
	{
		std::ifstream file(_filepath.ToCStr(), std::ios::ate | std::ios::binary);

		if (file.is_open() == false) 
		{
			STEINS_CORE_ERROR("Failed to open file!");
		}

		//tellg->파일의 입력위치 지정자를 리턴(사실상 size?)
		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);
		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = buffer.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(buffer.data());

		VkResult result = vkCreateShaderModule(device->GetLogicalDevice(), &createInfo, nullptr, &shader);
		STEINS_CORE_ASSERT(result == VK_SUCCESS, "Failed to create shader module!");
	}
	VulkanShader::VulkanShader(GraphicsDevice* _device, const std::string& _src, const ShaderType& _type)
	{
		STEINS_CORE_ERROR("Steins Vulkan API runtime compile is not supported!");
		shader = nullptr;
	}
}
