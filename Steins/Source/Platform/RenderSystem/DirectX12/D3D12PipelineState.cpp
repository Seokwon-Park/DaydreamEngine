#include "SteinsPCH.h"
#include "D3D12PipelineState.h"

namespace Steins
{
	D3D12PipelineState::D3D12PipelineState(D3D12GraphicsDevice* _device, PipelineStateDesc _desc)
		:PipelineState(_desc)
	{
		device = _device;

		D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
		rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		ComPtr<ID3DBlob> signature;
		ComPtr<ID3DBlob> error;
		HRESULT hr = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
		STEINS_CORE_ASSERT(SUCCEEDED(hr), "Failed to serialize root signature");

		if (FAILED(_device->GetDevice()->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(rootSignature.GetAddressOf())))) {
			throw std::runtime_error("Failed to create root signature");
		}

		D3D12_RASTERIZER_DESC rasterizerDesc{};
		rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
		rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
		
		DXGI_SAMPLE_DESC sampleDesc{};
		sampleDesc.Count = 1;
		sampleDesc.Quality= 0;


		D3D12_GRAPHICS_PIPELINE_STATE_DESC desc{};
		desc.pRootSignature = rootSignature.Get();
		desc.VS = *(D3D12_SHADER_BYTECODE*)_desc.vertexShader->GetNativeHandle();
		desc.PS = *(D3D12_SHADER_BYTECODE*)_desc.pixelShader->GetNativeHandle();
		desc.RasterizerState = rasterizerDesc;
		desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		desc.NumRenderTargets = 1;
		desc.SampleDesc = sampleDesc;
		desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		device->GetDevice()->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(pipeline.GetAddressOf()));
	}

	D3D12PipelineState::~D3D12PipelineState()
	{
	}
	void D3D12PipelineState::Bind() const
	{
	}
}

