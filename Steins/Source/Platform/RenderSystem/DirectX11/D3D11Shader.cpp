#include "SteinsPCH.h"
#include "D3D11Shader.h"

#include "D3D11GraphicsDevice.h"

namespace Steins
{
	D3D11Shader::D3D11Shader(const FilePath& _filepath, const ShaderType& _type)
	{
		Compile(_filepath.ToString(), true);
	}
	D3D11Shader::D3D11Shader(const std::string& _src, const ShaderType& _type)
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
	}
	void D3D11Shader::Bind() const
	{
	}

	void D3D11Shader::Unbind() const
	{
	}

	void D3D11Shader::SetMat4(const std::string& _name, const Matrix4x4& _value)
	{
	}

	void D3D11Shader::Compile(const std::string& _src, bool _isFile)
	{
		HRESULT hr = D3DCompile(
			_src.c_str(),
			_src.length(),
			nullptr,
			nullptr,
			nullptr,
			"vs_main", "vs_5_0",
			0, 0,
			shaderBlob.GetAddressOf(),
			nullptr);

		//hr = D3D11GraphicsDevice::GetDevice()->CreateVertexShader(
		//	shaderBlob->GetBufferPointer(),
		//	shaderBlob->GetBufferSize(),
		//	nullptr,
		//	&vs);
	}
	Shared<Shader> D3D11Shader::CreateShader(const std::string& _src, const ShaderType& _type)
	{
		return nullptr;
	}
	Shared<Shader> D3D11Shader::CreateShader(const FilePath& _filepath, const ShaderType& _type)
	{
		return Shared<Shader>();
	}
}
