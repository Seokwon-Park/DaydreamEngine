#include "SteinsPCH.h"
#include "D3D12PipelineState.h"

#include "Platform/RenderSystem/GraphicsUtil.h"

namespace Steins
{
	D3D12PipelineState::D3D12PipelineState(D3D12GraphicsDevice* _device, PipelineStateDesc _desc)
		:PipelineState(_desc)
	{
		device = _device;

		D3D12_ROOT_DESCRIPTOR rootDescriptor;
		rootDescriptor.ShaderRegister = 0;    // HLSL의 b0 레지스터
		rootDescriptor.RegisterSpace = 0;     // 기본 레지스터 공간

		// 2. 루트 파라미터(Root Parameter) 정의
		D3D12_ROOT_PARAMETER rootParameters[1];
		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // 루트 CBV 타입
		rootParameters[0].Descriptor = rootDescriptor;
		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; // 예: 정점 셰이더에서만 사용

		D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
		rootSignatureDesc.NumParameters = 1;
		rootSignatureDesc.pParameters = &rootParameters[0];
		rootSignatureDesc.NumStaticSamplers = 0;
		rootSignatureDesc.pStaticSamplers = nullptr;
		rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		ComPtr<ID3DBlob> signature;
		ComPtr<ID3DBlob> error;
		HRESULT hr = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
		STEINS_CORE_ASSERT(SUCCEEDED(hr), "Failed to serialize root signature");

		hr = _device->GetDevice()->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(rootSignature.GetAddressOf()));
		STEINS_CORE_ASSERT(SUCCEEDED(hr), "Failed to create root signature");

		D3D12_RASTERIZER_DESC rasterizerDesc{};
		rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
		rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
		rasterizerDesc.FrontCounterClockwise = false;
		rasterizerDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
		rasterizerDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
		rasterizerDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		rasterizerDesc.DepthClipEnable = TRUE;
		rasterizerDesc.MultisampleEnable = FALSE;
		rasterizerDesc.AntialiasedLineEnable = FALSE;
		rasterizerDesc.ForcedSampleCount = 0;
		rasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

		DXGI_SAMPLE_DESC sampleDesc{};
		sampleDesc.Count = _desc.sampleCount;
		sampleDesc.Quality = 0;

		D3D12_INPUT_ELEMENT_DESC inputElementDescs[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // 위치(float3) 다음이므로 12바이트 오프셋
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 } // 위치(float3) 다음이므로 12바이트 오프셋
		};


		D3D12_GRAPHICS_PIPELINE_STATE_DESC desc{};
		desc.pRootSignature = rootSignature.Get();
		desc.VS.pShaderBytecode = ((ID3DBlob*)_desc.vertexShader->GetNativeHandle())->GetBufferPointer();
		desc.VS.BytecodeLength = ((ID3DBlob*)_desc.vertexShader->GetNativeHandle())->GetBufferSize();
		desc.PS.pShaderBytecode = ((ID3DBlob*)_desc.pixelShader->GetNativeHandle())->GetBufferPointer();
		desc.PS.BytecodeLength = ((ID3DBlob*)_desc.pixelShader->GetNativeHandle())->GetBufferSize();
		desc.RasterizerState = rasterizerDesc;
		desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		desc.NumRenderTargets = 1;
		desc.SampleDesc = sampleDesc;
		desc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
		desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleMask = UINT_MAX;

		desc.BlendState.AlphaToCoverageEnable = FALSE;
		desc.BlendState.IndependentBlendEnable = FALSE;
		const D3D12_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlendDesc = {
			FALSE,FALSE,
			D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
			D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
			D3D12_LOGIC_OP_NOOP,
			D3D12_COLOR_WRITE_ENABLE_ALL,
		};
		for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
			desc.BlendState.RenderTarget[i] = defaultRenderTargetBlendDesc;

		// 깊이/스텐실 상태 설정 (d3dx12 없이 직접 - 깊이 테스트 비활성화)
		desc.DepthStencilState.DepthEnable = FALSE; // 깊이 테스트 끔
		desc.DepthStencilState.StencilEnable = FALSE; // 스텐실 테스트 끔
		//// 아래 값들은 DepthEnable=FALSE 이므로 무시됨
		//desc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		//desc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_NEVER;

		hr = device->GetDevice()->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(pipeline.GetAddressOf()));
		STEINS_CORE_ASSERT(SUCCEEDED(hr), "Failed to create pipeline!");
	}

	D3D12PipelineState::~D3D12PipelineState()
	{
	}
	void D3D12PipelineState::Bind() const
	{
		device->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());
		device->GetCommandList()->SetPipelineState(pipeline.Get());
	}
}

