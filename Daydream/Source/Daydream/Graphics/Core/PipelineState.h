#pragma once

#include "Daydream/Graphics/Resources/Shader.h"
#include "Daydream/Graphics/Resources/Buffer.h"
#include "Daydream/Graphics/Resources/Texture.h"
#include "Daydream/Graphics/Resources/Material.h"
#include "Daydream/Graphics/States/BlendState.h"
#include "Daydream/Graphics/States/RasterizerState.h"
#include "Daydream/Graphics/States/DepthStencilState.h"
#include "RenderPass.h"


namespace Daydream
{
	enum class PrimitiveTopologyType
	{
		TriangleList,
	};

	struct PipelineStateDesc
	{
		Shared<Shader> vertexShader = nullptr;
		Shared<Shader> hullShader = nullptr;
		Shared<Shader> domainShader = nullptr;
		Shared<Shader> geometryShader = nullptr;
		Shared<Shader> pixelShader = nullptr;
		//Shared<Shader> computeShader; //??

		//BufferLayout inputLayout;
		//InputLayoutDesc inputLayout;
		//Shared<ResourceBindingLayout> resourceBindingLayout; // RootSignature/PipelineLayout
		Shared<RenderPass> renderPass;
		RasterizerStateDesc rasterizerState;
		//BlendDesc blendState;
		//DepthStencilDesc depthStencilState;
		//Array<GraphicsFormat> renderTargetFormats; // RTV 포맷들
		//GraphicsFormat depthStencilFormat = GraphicsFormat::Unknown; // DSV 포맷
		uint32_t sampleCount = 1;
		PrimitiveTopologyType topologyType = PrimitiveTopologyType::TriangleList;
	};

	class PipelineState
	{
	public:
		PipelineState(const PipelineStateDesc& _desc);
		virtual ~PipelineState() = default;

		virtual void Bind() const = 0;
		virtual Shared<Material> CreateMaterial() = 0;

		Array<Shared<Shader>> GetShaders() { return shaders; }

		static Shared<PipelineState> Create(const PipelineStateDesc& _desc);
	protected:
		Shared<Shader> vertexShader;
		Shared<Shader> hullShader;
		Shared<Shader> domainShader;
		Shared<Shader> geometryShader;
		Shared<Shader> pixelShader;
		Shared<RenderPass> renderPass;

		Array<Shared<Shader>> shaders;

		//rtv, dsv;
		//blend, rast, ds;

	};
}