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
	D3D11Shader::D3D11Shader(GraphicsDevice* _device, const FilePath& _filepath, const ShaderType& _type)
	{
		device = _device->Get<D3D11GraphicsDevice>();
		type = _type;

		STEINS_CORE_INFO(_filepath.GetCurrentPath());

		std::string target = chooseTarget();
		std::string entryPoint = chooseEntryPoint();

		HRESULT hr = D3DCompileFromFile(_filepath.ToCStr(), nullptr, nullptr, entryPoint.c_str(), target.c_str(), 0, 0, shaderBlob.GetAddressOf(), errorBlob.GetAddressOf());
		STEINS_CORE_ASSERT(SUCCEEDED(hr), "Failed to compile shader!");

	}
	D3D11Shader::D3D11Shader(GraphicsDevice* _device, const std::string& _src, const ShaderType& _type)
	{
		device = _device->Get<D3D11GraphicsDevice>();
		type = _type;

		std::string target = chooseTarget();
		std::string entryPoint = chooseEntryPoint();
		
		HRESULT hr = D3DCompile(_src.c_str(), _src.size(), nullptr, nullptr, nullptr, entryPoint.c_str(), target.c_str(), 0, 0, shaderBlob.GetAddressOf(), errorBlob.GetAddressOf());
		STEINS_CORE_ASSERT(SUCCEEDED(hr), "Failed to compile shader!");


	}
	void D3D11Shader::Bind() const
	{
	}

	void D3D11Shader::Unbind() const
	{
	}

	std::string D3D11Shader::chooseTarget()
	{
		switch (type)
		{
		case ShaderType::None: STEINS_CORE_ASSERT(false, "Invalid type") break;
		case ShaderType::Vertex: return "vs_5_0";;
		case ShaderType::Hull: return "hs_5_0";
		case ShaderType::Domain: return "ds_5_0";
		case ShaderType::Geometry: return "gs_5_0";
		case ShaderType::Pixel: return "ps_5_0";
		case ShaderType::Compute: return "cs_5_0";
		default:
			break;
		}
		return "";
	}

	std::string D3D11Shader::chooseEntryPoint()
	{
		std::string result = "Main";
		switch (type)
		{
		case ShaderType::None:
		{
			STEINS_CORE_ASSERT(false, "Invalid type");
			return result;
			break;
		}
		case ShaderType::Vertex: return "VS" + result;
		case ShaderType::Hull: return "HS" + result;
		case ShaderType::Domain: return "DS" + result;
		case ShaderType::Geometry: return "GS" + result;
		case ShaderType::Pixel: return "PS" + result;
		case ShaderType::Compute: return "CS" + result;
		default:
			break;
		}
		return "";
	}



	Shared<Shader> D3D11Shader::CreateShader(GraphicsDevice* _device, const std::string& _src, const ShaderType& _type)
	{
		switch (_type)
		{
		case Steins::ShaderType::None:
			break;
		case Steins::ShaderType::Vertex:return MakeShared<D3D11VertexShader>(_device, _src);
			//case Steins::ShaderType::Hull: return MakeShared<
			//case Steins::ShaderType::Domain:;
			//case Steins::ShaderType::Geometry:;
		case Steins::ShaderType::Pixel:return MakeShared<D3D11PixelShader>(_device, _src);
			//case Steins::ShaderType::Compute:;
		default:
			break;
		}
		return nullptr;
	}

	Shared<Shader> D3D11Shader::CreateShader(GraphicsDevice* _device, const FilePath& _filepath, const ShaderType& _type)
	{
		switch (_type)
		{
		case Steins::ShaderType::None:
			break;
		case Steins::ShaderType::Vertex:return MakeShared<D3D11VertexShader>(_device, _filepath);
			//case Steins::ShaderType::Hull: return MakeShared<
			//case Steins::ShaderType::Domain:;
			//case Steins::ShaderType::Geometry:;
		case Steins::ShaderType::Pixel:return MakeShared<D3D11PixelShader>(_device, _filepath);
			//case Steins::ShaderType::Compute:;
		default:
			break;
		}
		return nullptr;
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
