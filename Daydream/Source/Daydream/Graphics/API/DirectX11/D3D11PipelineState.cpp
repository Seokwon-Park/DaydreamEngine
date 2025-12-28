#include "DaydreamPCH.h"
#include "D3D11PipelineState.h"
#include "D3D11Material.h"

#include "D3D11Utility.h"

namespace Daydream
{
	D3D11PipelineState::D3D11PipelineState(D3D11RenderDevice* _device, PipelineStateDesc _desc)
		:PipelineState(_desc)
	{
		device = _device;

		// 입력 파라미터 정보를 저장할 벡터
		Array<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;

		for (const auto& info : shaderGroup->GetInputData())
		{
			D3D11_INPUT_ELEMENT_DESC elementDesc;
			elementDesc.SemanticName = info.name.c_str();
			elementDesc.SemanticIndex = info.binding;
			elementDesc.InputSlot = 0;
			elementDesc.Format = GraphicsUtility::DirectX::ConvertRenderFormatToDXGIFormat(info.format);
			elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			elementDesc.InstanceDataStepRate = 0;

			inputLayoutDesc.push_back(elementDesc);
		}

		// 입력 레이아웃 생성
		HRESULT hr = device->GetDevice()->CreateInputLayout(
			inputLayoutDesc.data(),
			(UINT)inputLayoutDesc.size(),
			static_cast<ID3DBlob*>(shaderGroup->GetShader(ShaderType::Vertex)->GetNativeHandle())->GetBufferPointer(),
			static_cast<ID3DBlob*>(shaderGroup->GetShader(ShaderType::Vertex)->GetNativeHandle())->GetBufferSize(),
			inputLayout.GetAddressOf()
		);
		DAYDREAM_CORE_ASSERT(SUCCEEDED(hr), "Failed to create inputlayout!");

		//CW
		D3D11_RASTERIZER_DESC rastDesc = GraphicsUtility::DirectX11::TranslateToD3D11RasterizerDesc(_desc.rasterizerState);
		
		
		_device->GetDevice()->CreateRasterizerState(&rastDesc, rasterizer.GetAddressOf());


	}

	void D3D11PipelineState::Bind() const
	{
		for (auto shader : shaderGroup->GetShaders())
		{
			shader->Bind();
		}
		device->GetContext()->IASetInputLayout(inputLayout.Get());
		device->GetContext()->RSSetState(rasterizer.Get());
	}
	Shared<Material> D3D11PipelineState::CreateMaterial()
	{
		return MakeShared<D3D11Material>(device, this);
	}
}
