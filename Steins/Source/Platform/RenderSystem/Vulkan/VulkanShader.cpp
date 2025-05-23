#include "SteinsPCH.h"
#include "VulkanShader.h"

#include <spirv_cross/spirv_reflect.hpp>


namespace Steins
{
	VulkanShader::VulkanShader(VulkanGraphicsDevice* _device, const std::string& _src, const ShaderType& _type, const ShaderLoadMode& _mode)
	{
		device = _device;
		type = _type;
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

			STEINS_CORE_ASSERT(file.is_open() == true, "Failed to open file! Check directory");

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

			VkResult result = vkCreateShaderModule(device->GetDevice(), &createInfo, nullptr, &shader);
			STEINS_CORE_ASSERT(result == VK_SUCCESS, "Failed to create shader module!");
			break;
		}
		default:
			break;
		}
		
	}

	VulkanShader::~VulkanShader()
	{
		vkDestroyShaderModule(device->GetDevice(), shader, nullptr);
	}
	void VulkanShader::Bind() const
	{
	}
	void VulkanShader::Unbind() const
	{
	}

}
