#include "SteinsPCH.h"
#include "VulkanShader.h"

#include "spirv_cross/spirv_cross.hpp"

#include "Steins/Graphics/Utility/GraphicsUtil.h"
#include "Steins/Graphics/Utility/ShaderCompileHelper.h"


namespace Steins
{
	VulkanShader::VulkanShader(VulkanRenderDevice* _device, const std::string& _src, const ShaderType& _type, const ShaderLoadMode& _mode)
	{
		device = _device;
		shaderType = _type;
		stageBit = GraphicsUtil::GetVKShaderStage(_type);

		FilePath path(_src);
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
			//UInt64 fileSize = (UInt64)file.tellg();
			Array<uint32_t> reflect;
			//Array<char> buffer(fileSize);
			//file.seekg(0);
			//file.read(buffer.data(), fileSize);
			//file.seekg(0);
			//file.read((char*)reflect.data(), fileSize);
			//file.close();

			ShaderCompileHelper::ConvertHLSLtoSPIRV(path, _type, reflect);

			VkShaderModuleCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.codeSize = reflect.size() * 4;
			createInfo.pCode = reflect.data();

			spirv_cross::Compiler compiler(reflect);
			spirv_cross::ShaderResources res = compiler.get_shader_resources();

			if (shaderType == ShaderType::Vertex)
			{
				for (const spirv_cross::Resource& resource : res.stage_inputs)
				{
					const spirv_cross::SPIRType& spirType = compiler.get_type(resource.type_id);

					ShaderReflectionInfo sr{};
					sr.name = compiler.get_name(resource.id);
					sr.set = compiler.get_decoration(resource.id, spv::DecorationLocation);
					sr.binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
					sr.shaderResourceType = ShaderResourceType::Input;
					 
					UInt32 componentCount = spirType.vecsize;
					spirv_cross::SPIRType::BaseType baseType = spirType.basetype;
					sr.format = GraphicsUtil::ConvertSPIRVTypeToRenderFormat(baseType, componentCount);
					sr.size = GraphicsUtil::GetRenderFormatSize(sr.format);
					sr.shaderType = shaderType;

					reflectionInfo.push_back(sr);
				}
			}


			for (const spirv_cross::Resource& resource : res.uniform_buffers)
			{
				ShaderReflectionInfo sr{};
				sr.name = compiler.get_name(resource.id);
				sr.shaderResourceType = ShaderResourceType::ConstantBuffer;
				sr.set = compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
				sr.binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
				sr.size = compiler.get_declared_struct_size(compiler.get_type(resource.type_id));

				reflectionInfo.push_back(sr);
			}

			for (const spirv_cross::Resource& resource : res.sampled_images)
			{
				ShaderReflectionInfo sr{};
				sr.name = compiler.get_name(resource.id);
				sr.shaderResourceType = ShaderResourceType::Texture2D;
				sr.set = compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
				sr.binding = compiler.get_decoration(resource.id, spv::DecorationBinding);

				const auto& type = compiler.get_type(resource.type_id);
				UInt32 count = 1;
				if (!type.array.empty())
				{
					count = type.array[0];
				}

				reflectionInfo.push_back(sr);
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
