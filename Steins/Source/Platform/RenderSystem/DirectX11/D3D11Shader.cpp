#include "SteinsPCH.h"
#include "D3D11Shader.h"

#include "D3D11GraphicsDevice.h"

namespace Steins
{
	D3D11Shader::D3D11Shader(const FilePath& _filepath, const ShaderType& _type)
	{
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
	Shared<Shader> D3D11Shader::CreateShader(const std::string& _src, const ShaderType& _type)
	{
		switch (_type)
		{
		case Steins::ShaderType::None:
			break;
		case Steins::ShaderType::Vertex:return MakeShared<D3D11VertexShader>(_src);
		//case Steins::ShaderType::Hull: return MakeShared<
		//case Steins::ShaderType::Domain:;
		//case Steins::ShaderType::Geometry:;
		case Steins::ShaderType::Pixel:return MakeShared<D3D11PixelShader>(_src);
		//case Steins::ShaderType::Compute:;
		default:
			break;
		}
	}
	Shared<Shader> D3D11Shader::CreateShader(const FilePath& _filepath, const ShaderType& _type)
	{
		switch (_type)
		{
		case Steins::ShaderType::None:
			break;
		case Steins::ShaderType::Vertex:return MakeShared<D3D11VertexShader>(_filepath);
			//case Steins::ShaderType::Hull: return MakeShared<
			//case Steins::ShaderType::Domain:;
			//case Steins::ShaderType::Geometry:;
		case Steins::ShaderType::Pixel:return MakeShared<D3D11PixelShader>(_filepath);
			//case Steins::ShaderType::Compute:;
		default:
			break;
		}
	}


}
