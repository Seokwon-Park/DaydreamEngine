#include "SteinsPCH.h"
#include "D3D11Shader.h"

#include "D3D11GraphicsDevice.h"
#include "Platform/RenderSystem/GraphicsUtil.h"

namespace Steins
{
	//D3D11GraphicsDevice::GetDevice()->CreateInputLayout(
	//	m_Context->GetInputElements().data(), UINT(m_Context->GetInputElements().size()),
	//	vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(),
	//	&m_InputLayout);
	//ID3DBlob* psBlob;
	//hr = D3DCompile(
	//	pixelSrc.c_str(),
	//	pixelSrc.length(),
	//	nullptr,
	//	nullptr,
	//	nullptr,
	//	"ps_main", "ps_5_0",
	//	0, 0,
	//	&psBlob,
	//	nullptr);

	//hr = m_Context->GetD3DDevice()->CreatePixelShader(
	//	psBlob->GetBufferPointer(),
	//	psBlob->GetBufferSize(),
	//	nullptr,
	//	&m_PixelShader);

	//vsBlob->Release();
	//psBlob->Release();*/
	D3D11Shader::D3D11Shader(D3D11GraphicsDevice* _device, const std::string& _src, const ShaderType& _type, const ShaderLoadMode& _mode)
	{
		device = _device;
		type = _type;
		DXShaderCompileParam param = GraphicsUtil::GetDXShaderCompileParam(type);
		HRESULT hr;
		switch (_mode)
		{
		case ShaderLoadMode::Source:
		{
			hr = D3DCompile(_src.c_str(), _src.size(), nullptr, nullptr, nullptr, param.entryPoint.c_str(), param.target.c_str(), 0, 0, shaderBlob.GetAddressOf(), errorBlob.GetAddressOf());
			STEINS_CORE_ASSERT(SUCCEEDED(hr), "Failed to compile shader!");
			break;
		}
		case ShaderLoadMode::File:
		{
			FilePath path = FilePath(_src);
			STEINS_CORE_INFO(path.GetCurrentPath());
			hr = D3DCompileFromFile(path.ToCStr(), nullptr, nullptr, param.entryPoint.c_str(), param.target.c_str(), 0, 0, shaderBlob.GetAddressOf(), errorBlob.GetAddressOf());
			STEINS_CORE_ASSERT(SUCCEEDED(hr), "Failed to compile shader!");
			break;
		}
		default:
			break;
		}
	

	

	}
	void D3D11Shader::Bind() const
	{
	}

	void D3D11Shader::Unbind() const
	{
	}




		//HRESULT hr = D3DCompile(
		//	_src.c_str(),
		//	_src.length(),
		//	nullptr,
		//	nullptr,
		//	nullptr,
		//	"vs_main", "vs_5_0",
		//	0, 0,
		//	shaderBlob.GetAddressOf(),
		//	nullptr);

		//hr = D3D11GraphicsDevice::GetDevice()->CreateVertexShader(
		//	shaderBlob->GetBufferPointer(),
		//	shaderBlob->GetBufferSize(),
		//	nullptr,
		//	&vs);




}
