#include "SteinsPCH.h"
#include "D3D11Shader.h"

#include "D3D11RenderDevice.h"
#include "Steins/Graphics/Utility/GraphicsUtil.h"

namespace Steins
{
	//D3D11RenderDevice::GetDevice()->CreateInputLayout(
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
	D3D11Shader::D3D11Shader(D3D11RenderDevice* _device, const std::string& _src, const ShaderType& _type, const ShaderLoadMode& _mode)
	{
		device = _device;
		shaderType = _type;
		DXShaderCompileParam param = GraphicsUtil::GetDXShaderCompileParam(shaderType);
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
			//STEINS_CORE_INFO(path.GetCurrentPath());
			hr = D3DCompileFromFile(path.ToCStr(), nullptr, nullptr, param.entryPoint.c_str(), param.target.c_str(), 0, 0, shaderBlob.GetAddressOf(), errorBlob.GetAddressOf());
			STEINS_CORE_ASSERT(SUCCEEDED(hr), "Failed to compile shader!");
			break;
		}
		default:
			break;
		}

		ID3D11ShaderReflection* reflection = nullptr;
		hr = D3DReflect(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), IID_PPV_ARGS(&reflection));

		// 셰이더 입력 시그니처 정보 얻기
		D3D11_SHADER_DESC shaderDesc;
		reflection->GetDesc(&shaderDesc);

		// cbuffer의 바인딩 정보 찾기
		D3D11_SHADER_INPUT_BIND_DESC bindDesc;
		for (UInt32 i = 0; i < shaderDesc.BoundResources; i++)
		{
			reflection->GetResourceBindingDesc(i, &bindDesc);

			String name = bindDesc.Name;

			D3D_SHADER_INPUT_TYPE inputType = bindDesc.Type;

			switch (inputType)
			{
			case D3D_SIT_CBUFFER:
			{
				ID3D11ShaderReflectionConstantBuffer* cbuffer = reflection->GetConstantBufferByIndex(i);
				D3D11_SHADER_BUFFER_DESC cbufferDesc;
				cbuffer->GetDesc(&cbufferDesc);

				ShaderResourceDesc sr{};
				sr.name = name;
				sr.type = ShaderResourceType::ConstantBuffer;
				sr.set = 0; // D3D11에서는 set 개념이 없음
				sr.binding = bindDesc.BindPoint;
				sr.count = bindDesc.BindCount;
				sr.size = cbufferDesc.Size;
				sr.shaderType = shaderType;
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
				sr.shaderType = shaderType;
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

	//hr = D3D11RenderDevice::GetDevice()->CreateVertexShader(
	//	shaderBlob->GetBufferPointer(),
	//	shaderBlob->GetBufferSize(),
	//	nullptr,
	//	&vs);




}
