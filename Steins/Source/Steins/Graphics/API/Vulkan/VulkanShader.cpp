#include "SteinsPCH.h"
#include "VulkanShader.h"

#include "spirv_cross/spirv_cross.hpp"

#include "Steins/Graphics/Utility/GraphicsUtil.h"


namespace Steins
{
	
	
	VulkanShader::VulkanShader(VulkanRenderDevice* _device, const std::string& _src, const ShaderType& _type, const ShaderLoadMode& _mode)
	{
		device = _device;
		type = _type;
		stageBit = GraphicsUtil::GetVKShaderStage(_type);
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
			Array<uint32_t> reflect((fileSize + sizeof(uint32_t) - 1) / sizeof(uint32_t));
			Array<char> buffer(fileSize);
			file.seekg(0);
			file.read(buffer.data(), fileSize);
			file.seekg(0);

			file.read((char*)reflect.data(), fileSize);
			file.close();

			VkShaderModuleCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.codeSize = buffer.size();
			createInfo.pCode = reinterpret_cast<uint32_t*>(buffer.data());

			spirv_cross::Compiler comp(reflect); // const uint32_t *, size_t interface is also available.
			spirv_cross::ShaderResources res = comp.get_shader_resources();

			if (type == ShaderType::Vertex)
			{
				for (const spirv_cross::Resource& resource : res.stage_inputs)
				{
					comp.get_name(resource.id);
					comp.get_decoration(resource.id, spv::DecorationLocation);
				}
			}

			for (const spirv_cross::Resource& resource : res.uniform_buffers)
			{
				ShaderResource sr{};
				sr.name = comp.get_name(resource.base_type_id);
				sr.type = ShaderResourceType::ConstantBuffer;
				sr.set = comp.get_decoration(resource.id, spv::DecorationDescriptorSet);
				sr.binding = comp.get_decoration(resource.id, spv::DecorationBinding);
				sr.size = comp.get_declared_struct_size(comp.get_type(resource.type_id));

				resourceInfo.push_back(sr);
			}


			for (const spirv_cross::Resource& resource : res.sampled_images)
			{
				ShaderResource sr{};
				sr.name = comp.get_name(resource.id);
				sr.type = ShaderResourceType::Texture;
				sr.set = comp.get_decoration(resource.id, spv::DecorationDescriptorSet);
				sr.binding = comp.get_decoration(resource.id, spv::DecorationBinding);

				resourceInfo.push_back(sr);
			}

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
