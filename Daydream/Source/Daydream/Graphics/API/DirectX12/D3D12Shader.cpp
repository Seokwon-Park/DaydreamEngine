#include "DaydreamPCH.h"
#include "D3D12Shader.h"

#include "Daydream/Graphics/Utility/GraphicsUtility.h"
#include "Daydream/Graphics/Utility/ShaderCompileHelper.h"

namespace Daydream
{
	namespace
	{
		RenderFormat ConvertToRenderFormat(const D3D12_SIGNATURE_PARAMETER_DESC& paramDesc)
		{
			int componentCount = Math::BitCount(paramDesc.Mask); // 비트 수 세는 함수

			switch (componentCount)
			{
			case 1:
				switch (paramDesc.ComponentType)
				{
				case D3D_REGISTER_COMPONENT_UINT32:  return RenderFormat::R32_UINT;
				case D3D_REGISTER_COMPONENT_SINT32:  return RenderFormat::R32_SINT;
				case D3D_REGISTER_COMPONENT_FLOAT32: return RenderFormat::R32_FLOAT;
				default: return RenderFormat::UNKNOWN;
				}

			case 2:
				switch (paramDesc.ComponentType)
				{
				case D3D_REGISTER_COMPONENT_UINT32:  return RenderFormat::R32G32_UINT;
				case D3D_REGISTER_COMPONENT_SINT32:  return RenderFormat::R32G32_SINT;
				case D3D_REGISTER_COMPONENT_FLOAT32: return RenderFormat::R32G32_FLOAT;
				default: return RenderFormat::UNKNOWN;
				}

			case 3:
				switch (paramDesc.ComponentType)
				{
				case D3D_REGISTER_COMPONENT_UINT32:  return RenderFormat::R32G32B32_UINT;
				case D3D_REGISTER_COMPONENT_SINT32:  return RenderFormat::R32G32B32_SINT;
				case D3D_REGISTER_COMPONENT_FLOAT32: return RenderFormat::R32G32B32_FLOAT;
				default: return RenderFormat::UNKNOWN;
				}

			case 4:
				switch (paramDesc.ComponentType)
				{
				case D3D_REGISTER_COMPONENT_UINT32:  return RenderFormat::R32G32B32A32_UINT;
				case D3D_REGISTER_COMPONENT_SINT32:  return RenderFormat::R32G32B32A32_SINT;
				case D3D_REGISTER_COMPONENT_FLOAT32: return RenderFormat::R32G32B32A32_FLOAT;
				default: return RenderFormat::UNKNOWN;
				}

			default:
				return RenderFormat::UNKNOWN;
			}
		}
	}

    D3D12Shader::D3D12Shader(D3D12RenderDevice* _device, const std::string& _src, const ShaderType& _type, const ShaderLoadMode& _mode)
    {
        device = _device;
        shaderType = _type;
		String target = GraphicsUtility::GetShaderTargetName(_type, "6_0");
		String entryPoint = GraphicsUtility::GetShaderEntryPointName(_type);
		HRESULT hr;
		//ComPtr<ID3DBlob> errorBlob;
		//switch (_mode)
		//{
		//case ShaderLoadMode::Source:
		//{
		//	hr = D3DCompile(_src.c_str(), _src.size(), nullptr, nullptr, nullptr, entryPoint.c_str(), target.c_str(), 0, 0,shaderBlob.GetAddressOf(), errorBlob.GetAddressOf());
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
		//std::ifstream fin(file, std::ios::binary);

		//fin.seekg(0, std::ios_base::end);
		//std::ifstream::pos_type size = (int)fin.tellg();
		//fin.seekg(0, std::ios_base::beg);

		//ComPtr<ID3DBlob> blob;
		//ThrowIfFailed(D3DCreateBlob(size, blob.GetAddressOf()));

		//fin.read((char*)blob->GetBufferPointer(), size);
		//fin.close();

		//return blob;
		ComPtr<ID3D12ShaderReflection> reflection = nullptr;

		Path path(_src);
		ShaderCompileHelper::GetDXIL(path, _type, shaderBlob);
		ShaderCompileHelper::GetDX12Reflection(shaderBlob, reflection);

		//hr = D3DReflect(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), IID_PPV_ARGS(&reflection));

		// 셰이더 입력 시그니처 정보 얻기
		D3D12_SHADER_DESC shaderDesc;
		hr = reflection->GetDesc(&shaderDesc);
		DAYDREAM_CORE_ASSERT(SUCCEEDED(hr), "Failed to get shader description")

		// 각 입력 파라미터에 대해 순회
		if (shaderType == ShaderType::Vertex)
		{
			for (UInt32 i = 0; i < shaderDesc.InputParameters; i++)
			{
				D3D12_SIGNATURE_PARAMETER_DESC paramDesc;
				reflection->GetInputParameterDesc(i, &paramDesc);

				ShaderReflectionData sr{};
				sr.name = paramDesc.SemanticName;
				sr.set = 0;
				sr.binding = paramDesc.SemanticIndex;
				sr.shaderResourceType = ShaderResourceType::Input;
				sr.format = ConvertToRenderFormat(paramDesc);
				sr.size = GraphicsUtility::GetRenderFormatSize(sr.format);
				sr.shaderType = shaderType;

				reflectionDatas.push_back(sr);
			}
		}

		// cbuffer의 바인딩 정보 찾기
		D3D12_SHADER_INPUT_BIND_DESC bindDesc;
		for (UInt32 i = 0; i < shaderDesc.BoundResources; i++)
		{
			reflection->GetResourceBindingDesc(i, &bindDesc);

			String name = bindDesc.Name;

			D3D_SHADER_INPUT_TYPE type = bindDesc.Type;

			ShaderReflectionData sr{};
			switch (type)
			{
			case D3D_SIT_CBUFFER:
			{
				ID3D12ShaderReflectionConstantBuffer* cbuffer = reflection->GetConstantBufferByIndex(i);
				D3D12_SHADER_BUFFER_DESC cbufferDesc;
				cbuffer->GetDesc(&cbufferDesc);

				sr.name = name;
				sr.shaderResourceType = ShaderResourceType::ConstantBuffer;
				sr.set = bindDesc.Space;
				sr.binding = bindDesc.BindPoint;
				sr.count = bindDesc.BindCount;
				sr.size = cbufferDesc.Size;
				sr.shaderType = shaderType;
				reflectionDatas.push_back(sr);
				break;
			}
			case D3D_SIT_TEXTURE:
			{
				sr.name = name;
				sr.shaderResourceType = ShaderResourceType::Texture;
				sr.set = bindDesc.Space;
				sr.binding = bindDesc.BindPoint;
				sr.count = bindDesc.BindCount;
				reflectionDatas.push_back(sr);
				break;

			}
			case D3D_SIT_SAMPLER:
			{
				sr.name = name;
				sr.shaderResourceType = ShaderResourceType::Sampler;
				sr.set = bindDesc.Space;
				sr.binding = bindDesc.BindPoint;
				sr.count = bindDesc.BindCount;
				reflectionDatas.push_back(sr);
				break;
			}
			}
		}

		shaderByteCode.pShaderBytecode = shaderBlob->GetBufferPointer();
		shaderByteCode.BytecodeLength = shaderBlob->GetBufferSize();

    }
    void D3D12Shader::Bind() const
    {
    }

    void D3D12Shader::Unbind() const
    {
    }
}
