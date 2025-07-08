#pragma once

#include "Steins/Graphics/Resources/Shader.h"
#include "Steins/Graphics/Resources/Buffer.h"
#include "Steins/Graphics/Resources/Texture.h"
#include "Steins/Graphics/Resources/Material.h"

namespace Steins
{
	struct ConstantBufferInfo
	{
		UInt32 bindingSlot;
		Shared<ConstantBuffer> constantBuffer;
	};

	struct PipelineStateDesc
	{
		Shared<Shader> vertexShader = nullptr;
		Shared<Shader> hullShader = nullptr;
		Shared<Shader> domainShader = nullptr;
		Shared<Shader> geometryShader = nullptr;
		Shared<Shader> pixelShader = nullptr;
		//Shared<Shader> computeShader; //??

		Array<ConstantBufferInfo> constantBuffers;
		Array<Shared<Texture2D>> textures;

		BufferLayout inputLayout;
		//InputLayoutDesc inputLayout;
		//Shared<ResourceBindingLayout> resourceBindingLayout; // RootSignature/PipelineLayout
		//RasterizerDesc rasterizerState;
		//BlendDesc blendState;
		//DepthStencilDesc depthStencilState;
		//Array<GraphicsFormat> renderTargetFormats; // RTV 포맷들
		//GraphicsFormat depthStencilFormat = GraphicsFormat::Unknown; // DSV 포맷
		uint32_t sampleCount = 1;
		//PrimitiveTopologyType topologyType = PrimitiveTopologyType::TriangleList;
	};

	class PipelineState
	{
	public:
		PipelineState(PipelineStateDesc _desc);
		virtual ~PipelineState() = default;

		virtual void Bind() const = 0;
		virtual Shared<Material> CreateMaterial() = 0;

		Array<Shared<Shader>> GetShaders() { return shaders; }

		static Shared<PipelineState> Create(PipelineStateDesc _desc);
	protected:
		Shared<Shader> vertexShader;
		Shared<Shader> hullShader;
		Shared<Shader> domainShader;
		Shared<Shader> geometryShader;
		Shared<Shader> pixelShader;

		Array<Shared<Shader>> shaders;

		//root signature??

		Array<ConstantBuffer*> cbfs;

		//rtv, dsv;
		//blend, rast, ds;

	};
}