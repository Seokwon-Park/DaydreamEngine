#include "SteinsPCH.h"
#include "D3D12PipelineState.h"

#include "Steins/Graphics/Utility/GraphicsUtil.h"
#include "D3D12Shader.h"
#include "D3D12Material.h"

namespace Steins
{
	D3D12PipelineState::D3D12PipelineState(D3D12RenderDevice* _device, PipelineStateDesc _desc)
		:PipelineState(_desc)
	{
		device = _device;

		Array<D3D12_ROOT_PARAMETER> rootParameters;
		Array<D3D12_DESCRIPTOR_RANGE> descriptorRanges;
		Array<D3D12_INPUT_ELEMENT_DESC> inputLayoutDesc;

		for (const auto& info : vertexShader->GetReflectionInfo())
		{
			if (info.shaderResourceType != ShaderResourceType::Input) continue;
			D3D12_INPUT_ELEMENT_DESC elementDesc;
			elementDesc.SemanticName = info.name.c_str();
			elementDesc.SemanticIndex = info.binding;
			elementDesc.Format = GraphicsUtil::ConvertRenderFormatToDXGIFormat(info.format);
			elementDesc.InputSlot = 0;
			elementDesc.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
			elementDesc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
			elementDesc.InstanceDataStepRate = 0;

			inputLayoutDesc.push_back(elementDesc);
		}

		UInt32 index = 0;
		for (auto shader : shaders)
		{
			for (auto& info : shader->GetReflectionInfo())
			{
				switch (info.shaderResourceType)
				{
				case ShaderResourceType::ConstantBuffer:
				{
					D3D12_ROOT_PARAMETER rootParam = {};
					rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
					rootParam.Descriptor.ShaderRegister = info.binding;
					rootParam.Descriptor.RegisterSpace = 0;
					rootParam.ShaderVisibility = GraphicsUtil::GetDX12ShaderVisibility(shader->GetType());
					rootParameters.push_back(rootParam);
					break;
				}
				case ShaderResourceType::Texture2D:
				{
					D3D12_DESCRIPTOR_RANGE srvRange{};
					srvRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
					srvRange.NumDescriptors = 1;
					srvRange.BaseShaderRegister = info.binding;  // t0
					srvRange.RegisterSpace = 0;
					srvRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

					D3D12_ROOT_PARAMETER rootParam{};
					rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
					rootParam.DescriptorTable.NumDescriptorRanges = 1;
					rootParam.DescriptorTable.pDescriptorRanges = &srvRange;
					rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
					rootParameters.push_back(rootParam);
					break;
				}
				case ShaderResourceType::Sampler:
					//D3D12_DESCRIPTOR_RANGE samplerRange{};
					//samplerRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
					//samplerRange.NumDescriptors = 1;
					//samplerRange.BaseShaderRegister = 0; // s0부터 시작
					//samplerRange.RegisterSpace = 0;
					//samplerRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
					//descriptorRanges.push_back(samplerRange);

					//D3D12_ROOT_PARAMETER rootParam = {};
					//rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
					//rootParam.DescriptorTable.NumDescriptorRanges = 1;
					//rootParam.DescriptorTable.pDescriptorRanges = &samplerRange;
					//rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
					//rootParameters.push_back(rootParam);
					continue;
				case ShaderResourceType::Input:
					continue;
				}
				info.set = index++;
			}
		}


		D3D12_ROOT_DESCRIPTOR rootDescriptor;
		rootDescriptor.ShaderRegister = 0;    // HLSL의 b0 레지스터
		rootDescriptor.RegisterSpace = 0;     // 기본 레지스터 공간




		//// 2. 루트 파라미터(Root Parameter) 정의
		//D3D12_ROOT_PARAMETER rootParameters[2];
		//rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // 루트 CBV 타입
		//rootParameters[0].Descriptor = rootDescriptor;
		//rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; // 예: 정점 셰이더에서만 사용
		//		
		//rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		//rootParameters[1].DescriptorTable.NumDescriptorRanges = 1;
		//rootParameters[1].DescriptorTable.pDescriptorRanges = &range;
		//rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

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
		rootSignatureDesc.NumParameters = (UINT)rootParameters.size();
		rootSignatureDesc.pParameters = rootParameters.data();
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



		D3D12_GRAPHICS_PIPELINE_STATE_DESC desc{};
		desc.pRootSignature = rootSignature.Get();
		desc.VS = static_cast<D3D12Shader*>(_desc.vertexShader.get())->GetShaderBytecode();
		desc.PS = static_cast<D3D12Shader*>(_desc.pixelShader.get())->GetShaderBytecode();
		desc.RasterizerState = rasterizerDesc;
		desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		desc.NumRenderTargets = 1;
		desc.SampleDesc = sampleDesc;
		desc.InputLayout.NumElements = static_cast<UInt32>(inputLayoutDesc.size());
		desc.InputLayout.pInputElementDescs = inputLayoutDesc.data();
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
	Shared<Material> D3D12PipelineState::CreateMaterial()
	{
		return MakeShared<D3D12Material>(device, this);
	}
}

