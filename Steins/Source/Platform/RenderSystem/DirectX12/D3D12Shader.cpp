#include "SteinsPCH.h"
#include "D3D12Shader.h"

#include "Platform/RenderSystem/GraphicsUtil.h"

namespace Steins
{
    D3D12Shader::D3D12Shader(D3D12RenderDevice* _device, const std::string& _src, const ShaderType& _type, const ShaderLoadMode& _mode)
    {
        device = _device;
        type = _type;
		DXShaderCompileParam param = GraphicsUtil::GetDXShaderCompileParam(type);
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
