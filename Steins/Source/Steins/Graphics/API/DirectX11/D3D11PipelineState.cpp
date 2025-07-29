#include "SteinsPCH.h"
#include "D3D11PipelineState.h"
#include "D3D11Material.h"

#include <Steins/Graphics/Utility/GraphicsUtil.h>

namespace Steins
{
	D3D11PipelineState::D3D11PipelineState(D3D11RenderDevice* _device, PipelineStateDesc _desc)
		:PipelineState(_desc)
	{
		device = _device;

		// 입력 파라미터 정보를 저장할 벡터
		Array<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;

		for (const auto& info : vertexShader->GetReflectionInfo())
		{
			if (info.shaderResourceType != ShaderResourceType::Input) continue;
			D3D11_INPUT_ELEMENT_DESC elementDesc;
			elementDesc.SemanticName = info.name.c_str();
			elementDesc.SemanticIndex = info.binding;
			elementDesc.InputSlot = 0;
			elementDesc.Format = GraphicsUtil::ConvertRenderFormatToDXGIFormat(info.format);
			elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			elementDesc.InstanceDataStepRate = 0;

			inputLayoutDesc.push_back(elementDesc);
		}

		// 입력 레이아웃 생성
		HRESULT hr = device->GetDevice()->CreateInputLayout(
			inputLayoutDesc.data(),
			(UINT)inputLayoutDesc.size(),
			static_cast<ID3DBlob*>(vertexShader->GetNativeHandle())->GetBufferPointer(),
			static_cast<ID3DBlob*>(vertexShader->GetNativeHandle())->GetBufferSize(),
			inputLayout.GetAddressOf()
		);
		STEINS_CORE_ASSERT(SUCCEEDED(hr), "Failed to create inputlayout!");

		//CW
		D3D11_RASTERIZER_DESC desc{};
		desc.FrontCounterClockwise = false;
		desc.CullMode = D3D11_CULL_BACK;
		desc.FillMode = D3D11_FILL_SOLID;
		_device->GetDevice()->CreateRasterizerState(&desc, rasterizer.GetAddressOf());

		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(samplerDesc));
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		device->GetDevice()->CreateSamplerState(&samplerDesc, sampler.GetAddressOf());
	}

	void D3D11PipelineState::Bind() const
	{
		for (auto shader : shaders)
		{
			shader->Bind();
		}
		device->GetContext()->IASetInputLayout(inputLayout.Get());
		device->GetContext()->RSSetState(rasterizer.Get());
		device->GetContext()->PSSetSamplers(1, 1, sampler.GetAddressOf());
	}
	Shared<Material> D3D11PipelineState::CreateMaterial()
	{
		return MakeShared<D3D11Material>(device, this);
	}
}
