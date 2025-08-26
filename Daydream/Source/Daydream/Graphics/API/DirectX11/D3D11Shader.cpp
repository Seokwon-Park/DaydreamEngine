#include "DaydreamPCH.h"
#include "D3D11Shader.h"

#include "D3D11RenderDevice.h"
#include "Daydream/Graphics/Utility/GraphicsUtility.h"
#include "Daydream/Graphics/Utility/ShaderCompileHelper.h"

#include <spirv_cross/spirv_hlsl.hpp>

namespace Daydream
{
	D3D11Shader::D3D11Shader(D3D11RenderDevice* _device, const std::string& _src, const ShaderType& _type, const ShaderLoadMode& _mode)
	{
		device = _device;
		shaderType = _type;
		String target = GraphicsUtility::GetShaderTargetName(_type, "5_0");
		String entryPoint = GraphicsUtility::GetShaderEntryPointName(_type);
		HRESULT hr;

		FilePath path(_src);
		String src;
		Array<UInt32> spirvData;
		ShaderCompileHelper::ConvertHLSLtoSPIRV(path, _type, spirvData);
		//spirv_cross::CompilerHLSL compiler(spirvData);
		Unique<spirv_cross::CompilerHLSL> compiler = MakeUnique<spirv_cross::CompilerHLSL>(spirvData);
		Unique<spirv_cross::ShaderResources> res = MakeUnique <spirv_cross::ShaderResources>(compiler->get_shader_resources());
		for (const spirv_cross::Resource& resource : res->uniform_buffers)
		{
			ShaderReflectionInfo sr{};
			sr.name = compiler->get_name(resource.id);
			sr.shaderResourceType = ShaderResourceType::ConstantBuffer;
			sr.set = compiler->get_decoration(resource.id, spv::DecorationDescriptorSet);
			sr.binding = compiler->get_decoration(resource.id, spv::DecorationBinding);
			sr.size = compiler->get_declared_struct_size(compiler->get_type(resource.type_id));
			sr.shaderType = _type;

			reflectionInfo.push_back(sr);
		}

		for (const spirv_cross::Resource& resource : res->sampled_images)
		{
			ShaderReflectionInfo sr{};
			sr.name = compiler->get_name(resource.id);
			sr.shaderResourceType = ShaderResourceType::Texture;
			sr.set = compiler->get_decoration(resource.id, spv::DecorationDescriptorSet);
			sr.binding = compiler->get_decoration(resource.id, spv::DecorationBinding);
			sr.shaderType = _type;

			const auto& type = compiler->get_type(resource.type_id);
			UInt32 count = 1;
			if (!type.array.empty())
			{
				count = type.array[0];
			}

			reflectionInfo.push_back(sr);
		}
		ShaderCompileHelper::ConvertSPIRVtoDXBC(spirvData, _type, src);

		hr = D3DCompile(src.c_str(), src.size(), nullptr, nullptr, nullptr, entryPoint.c_str(), target.c_str(), 0, 0, shaderBlob.GetAddressOf(), errorBlob.GetAddressOf());
		DAYDREAM_CORE_ASSERT(SUCCEEDED(hr), "Failed to compile shader!");
		//switch (_mode)
		//{
		//case ShaderLoadMode::Source:
		//{
		//	hr = D3DCompile(_src.c_str(), _src.size(), nullptr, nullptr, nullptr, entryPoint.c_str(), target.c_str(), 0, 0, shaderBlob.GetAddressOf(), errorBlob.GetAddressOf());
		//	DAYDREAM_CORE_ASSERT(SUCCEEDED(hr), "Failed to compile shader!");
		//	break;
		//}
		//case ShaderLoadMode::File:
		//{
		//	FilePath path = FilePath(_src);
		//	//DAYDREAM_CORE_INFO(path.GetCurrentPath());
		//	hr = D3DCompileFromFile(path.ToWString().c_str(), nullptr, nullptr, entryPoint.c_str(), target.c_str(), 0, 0, shaderBlob.GetAddressOf(), errorBlob.GetAddressOf());
		//	DAYDREAM_CORE_ASSERT(SUCCEEDED(hr), "Failed to compile shader!");
		//	break;
		//}
		//default:
		//	break;
		//}



		hr = D3DReflect(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), IID_PPV_ARGS(reflection.GetAddressOf()));
		DAYDREAM_CORE_ASSERT(SUCCEEDED(hr), "Failed to reflection");

		//// 셰이더 입력 시그니처 정보 얻기
		//D3D11_SHADER_DESC shaderDesc;
		//reflection->GetDesc(&shaderDesc);

		//// cbuffer의 바인딩 정보 찾기
		//D3D11_SHADER_INPUT_BIND_DESC bindDesc;
		//for (UInt32 i = 0; i < shaderDesc.BoundResources; i++)
		//{
		//	reflection->GetResourceBindingDesc(i, &bindDesc);

		//	String name = bindDesc.Name;

		//	D3D_SHADER_INPUT_TYPE inputType = bindDesc.Type;

		//	switch (inputType)
		//	{
		//	case D3D_SIT_CBUFFER:
		//	{
		//		ID3D11ShaderReflectionConstantBuffer* cbuffer = reflection->GetConstantBufferByIndex(i);
		//		D3D11_SHADER_BUFFER_DESC cbufferDesc;
		//		cbuffer->GetDesc(&cbufferDesc);

		//		ShaderReflectionInfo sr{};
		//		sr.name = name;
		//		sr.shaderResourceType = ShaderResourceType::ConstantBuffer;
		//		sr.set = 0; // D3D11에서는 set 개념이 없음
		//		sr.binding = bindDesc.BindPoint;
		//		sr.count = bindDesc.BindCount;
		//		sr.size = cbufferDesc.Size;
		//		sr.shaderType = shaderType;
		//		reflectionInfo.push_back(sr);
		//		break;
		//	}
		//	case D3D_SIT_TEXTURE:
		//	{
		//		ShaderReflectionInfo sr{};
		//		sr.name = name;
		//		sr.shaderResourceType = ShaderResourceType::Texture2D;
		//		sr.set = 0; // D3D11에서는 set 개념이 없음
		//		sr.binding = bindDesc.BindPoint;
		//		sr.count = bindDesc.BindCount;
		//		sr.shaderType = shaderType;
		//		reflectionInfo.push_back(sr);
		//		break;
		//	}
		//	case D3D_SIT_SAMPLER:
		//	{
		//		ShaderReflectionInfo sr{};
		//		sr.name = name;
		//		sr.shaderResourceType = ShaderResourceType::Sampler;
		//		sr.set = 0; // D3D11에서는 set 개념이 없음
		//		sr.binding = bindDesc.BindPoint;
		//		sr.count = bindDesc.BindCount;
		//		reflectionInfo.push_back(sr);
		//		break;
		//	}
		//	}
		//}
	}

	D3D11Shader::~D3D11Shader()
	{
		shaderBlob.Reset();
		errorBlob.Reset();
	}

	void D3D11Shader::Bind() const
	{}

	void D3D11Shader::Unbind() const
	{
	}
}
