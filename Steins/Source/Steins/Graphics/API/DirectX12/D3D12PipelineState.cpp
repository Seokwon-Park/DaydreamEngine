#include "SteinsPCH.h"
#include "D3D12PipelineState.h"

#include "Steins/Graphics/Utility/GraphicsUtil.h"
#include "D3D12Shader.h"

namespace Steins
{
	D3D12PipelineState::D3D12PipelineState(D3D12RenderDevice* _device, PipelineStateDesc _desc)
		:PipelineState(_desc)
	{
		device = _device;

		D3D12_ROOT_DESCRIPTOR rootDescriptor;
		rootDescriptor.ShaderRegister = 0;    // HLSL�� b0 ��������
		rootDescriptor.RegisterSpace = 0;     // �⺻ �������� ����

		D3D12_DESCRIPTOR_RANGE range{};
		range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		range.NumDescriptors = 1;
		range.BaseShaderRegister = 0;  // t0
		range.RegisterSpace = 0;
		range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		// 2. ��Ʈ �Ķ����(Root Parameter) ����
		D3D12_ROOT_PARAMETER rootParameters[2];
		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // ��Ʈ CBV Ÿ��
		rootParameters[0].Descriptor = rootDescriptor;
		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; // ��: ���� ���̴������� ���
				
		rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParameters[1].DescriptorTable.NumDescriptorRanges = 1;
		rootParameters[1].DescriptorTable.pDescriptorRanges = &range;
		rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
		staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].MipLODBias = 0;
		staticSamplers[0].MaxAnisotropy = 0;
		staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		staticSamplers[0].BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		staticSamplers[0].MinLOD = 0;
		staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;
		staticSamplers[0].ShaderRegister = 0;  // s0
		staticSamplers[0].RegisterSpace = 0;
		staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
		rootSignatureDesc.NumParameters = 2;
		rootSignatureDesc.pParameters = rootParameters;
		rootSignatureDesc.NumStaticSamplers = 1;
		rootSignatureDesc.pStaticSamplers = staticSamplers;
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
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // ��ġ(float3) �����̹Ƿ� 12����Ʈ ������
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 } // ��ġ(float3) �����̹Ƿ� 12����Ʈ ������
		};



		D3D12_GRAPHICS_PIPELINE_STATE_DESC desc{};
		desc.pRootSignature = rootSignature.Get();
		desc.VS = static_cast<D3D12Shader*>(_desc.vertexShader.get())->GetShaderBytecode();
		desc.PS = static_cast<D3D12Shader*>(_desc.pixelShader.get())->GetShaderBytecode();
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

		// ����/���ٽ� ���� ���� (d3dx12 ���� ���� - ���� �׽�Ʈ ��Ȱ��ȭ)
		desc.DepthStencilState.DepthEnable = FALSE; // ���� �׽�Ʈ ��
		desc.DepthStencilState.StencilEnable = FALSE; // ���ٽ� �׽�Ʈ ��
		//// �Ʒ� ������ DepthEnable=FALSE �̹Ƿ� ���õ�
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

