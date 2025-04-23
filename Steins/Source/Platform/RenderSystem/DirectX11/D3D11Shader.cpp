#include "SteinsPCH.h"
#include "D3D11Shader.h"

#include "D3D11GraphicsDevice.h"

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
		device = _device->Get<D3D11GraphicsDevice>();
		type = _type;
		Pair<std::string, std::string> targetEntryPair = GetCompileParam();
		HRESULT hr;
		switch (_mode)
		{
		case ShaderLoadMode::Source:
		{
			hr = D3DCompile(_src.c_str(), _src.size(), nullptr, nullptr, nullptr, targetEntryPair.second.c_str(), targetEntryPair.first.c_str(), 0, 0, shaderBlob.GetAddressOf(), errorBlob.GetAddressOf());
			STEINS_CORE_ASSERT(SUCCEEDED(hr), "Failed to compile shader!");
			break;
		}
		case ShaderLoadMode::File:
		{
			FilePath path = FilePath(_src);
			STEINS_CORE_INFO(path.GetCurrentPath());
			hr = D3DCompileFromFile(path.ToCStr(), nullptr, nullptr, targetEntryPair.second.c_str(), targetEntryPair.first.c_str(), 0, 0, shaderBlob.GetAddressOf(), errorBlob.GetAddressOf());
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

	Pair<std::string, std::string> D3D11Shader::GetCompileParam()
	{
		switch (type)
		{
		case ShaderType::None:
		{
			STEINS_CORE_ASSERT(false, "Invalid type");
			return Pair<std::string, std::string>();
		}
		case ShaderType::Vertex:return MakePair<std::string, std::string>("vs_5_0", "VSMain");
		case ShaderType::Hull:return MakePair<std::string, std::string>("hs_5_0", "HSMain");
		case ShaderType::Domain:return MakePair<std::string, std::string>("ds_5_0", "DSMain");
		case ShaderType::Geometry:return MakePair<std::string, std::string>("gs_5_0", "GSMain");
		case ShaderType::Pixel:return MakePair<std::string, std::string>("ps_5_0", "PSMain");
		case ShaderType::Compute:return MakePair<std::string, std::string>("cs_5_0", "CSMain");
		default:
			break;
		}
		return Pair<std::string, std::string>();
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
