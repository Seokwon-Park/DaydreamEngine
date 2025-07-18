#include "SteinsPCH.h"
#include "D3D12Shader.h"

#include "Steins/Graphics/Utility/GraphicsUtil.h"

namespace Steins
{
    D3D12Shader::D3D12Shader(D3D12RenderDevice* _device, const std::string& _src, const ShaderType& _type, const ShaderLoadMode& _mode)
    {
        device = _device;
        shaderType = _type;
		DXShaderCompileParam param = GraphicsUtil::GetDXShaderCompileParam(shaderType);
		HRESULT hr;
		ComPtr<ID3DBlob> errorBlob;
		switch (_mode)
		{
		case ShaderLoadMode::Source:
		{
			hr = D3DCompile(_src.c_str(), _src.size(), nullptr, nullptr, nullptr, param.entryPoint.c_str(), param.target.c_str(), 0, 0,shaderBlob.GetAddressOf(), errorBlob.GetAddressOf());
			STEINS_CORE_ASSERT(SUCCEEDED(hr), "Failed to compile shader!");
			break;
		}
		case ShaderLoadMode::File:
		{
			FilePath path = FilePath(_src);
			//STEINS_CORE_INFO(path.GetCurrentPath());
			hr = D3DCompileFromFile(path.ToCStr(), nullptr, nullptr, param.entryPoint.c_str(), param.target.c_str(), 0, 0, shaderBlob.GetAddressOf(), errorBlob.GetAddressOf());
			STEINS_CORE_ASSERT(SUCCEEDED(hr), "Failed to compile shader!");
			break;
		}
		default:
			break;
		}
		//std::ifstream fin(file, std::ios::binary);

		//fin.seekg(0, std::ios_base::end);
		//std::ifstream::pos_type size = (int)fin.tellg();
		//fin.seekg(0, std::ios_base::beg);

		//ComPtr<ID3DBlob> blob;
		//ThrowIfFailed(D3DCreateBlob(size, blob.GetAddressOf()));

		//fin.read((char*)blob->GetBufferPointer(), size);
		//fin.close();

		//return blob;

		ID3D12ShaderReflection* reflection = nullptr;
		hr = D3DReflect(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), IID_PPV_ARGS(&reflection));

		// 셰이더 입력 시그니처 정보 얻기
		D3D12_SHADER_DESC shaderDesc;
		reflection->GetDesc(&shaderDesc);

		// cbuffer의 바인딩 정보 찾기
		D3D12_SHADER_INPUT_BIND_DESC bindDesc;
		for (UInt32 i = 0; i < shaderDesc.BoundResources; i++)
		{
			reflection->GetResourceBindingDesc(i, &bindDesc);

			String name = bindDesc.Name;

			D3D_SHADER_INPUT_TYPE type = bindDesc.Type;

			switch (type)
			{
			case D3D_SIT_CBUFFER:
			{
				ID3D12ShaderReflectionConstantBuffer* cbuffer = reflection->GetConstantBufferByIndex(i);
				D3D12_SHADER_BUFFER_DESC cbufferDesc;
				cbuffer->GetDesc(&cbufferDesc);

				ShaderResourceDesc sr{};
				sr.name = name;
				sr.type = ShaderResourceType::ConstantBuffer;
				sr.set = 0; // D3D11에서는 set 개념이 없음
				sr.binding = bindDesc.BindPoint;
				sr.count = bindDesc.BindCount;
				sr.size = cbufferDesc.Size;
				resourceInfo.push_back(sr);
				break;
			}
			case D3D_SIT_TEXTURE:
			{
				ShaderResourceDesc sr{};
				sr.name = name;
				sr.type = ShaderResourceType::Texture2D;
				sr.set = 0; // D3D11에서는 set 개념이 없음
				sr.binding = bindDesc.BindPoint;
				sr.count = bindDesc.BindCount;
				resourceInfo.push_back(sr);
				break;

			}
			case D3D_SIT_SAMPLER:
			{
				ShaderResourceDesc sr{};
				sr.name = name;
				sr.type = ShaderResourceType::Sampler;
				sr.set = 0; // D3D11에서는 set 개념이 없음
				sr.binding = bindDesc.BindPoint;
				sr.count = bindDesc.BindCount;
				resourceInfo.push_back(sr);
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
