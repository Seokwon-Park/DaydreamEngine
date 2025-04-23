#include "SteinsPCH.h"
#include "VulkanShader.h"

namespace Steins
{
	VulkanShader::VulkanShader(VulkanGraphicsDevice* _device, const std::string& _src, const ShaderType& _type, const ShaderLoadMode& _mode)
	{
		device = _device;
		switch (_mode)
		{
		case Steins::ShaderLoadMode::Source:
		{
			STEINS_CORE_ERROR("Steins Vulkan API runtime compile is not supported!");
			break;
		}
		case Steins::ShaderLoadMode::File:
		{
			std::ifstream file(_src, std::ios::ate | std::ios::binary);

			if (file.is_open() == false)
			{
				STEINS_CORE_ERROR("Failed to open file!");
			}

			//tellg->파일의 입력위치 지정자를 리턴(사실상 size?)
			UInt64 fileSize = (UInt64)file.tellg();
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
			break;
		}
		default:
			break;
		}
		
	}
	VulkanShader::~VulkanShader()
	{
		vkDestroyShaderModule(device->GetLogicalDevice(), shader, nullptr);
	}
}
