#pragma once

#include "Shader.h"

namespace Steins
{
	struct PipelineStateDesc
	{
		Shared<Shader> vertexShader = nullptr;
		Shared<Shader> hullShader = nullptr;
		Shared<Shader> domainShader = nullptr;
		Shared<Shader> geometryShader = nullptr;
		Shared<Shader> pixelShader = nullptr;
		//Shared<Shader> computeShader; //??

		//InputLayoutDesc inputLayout;
		//Shared<ResourceBindingLayout> resourceBindingLayout; // RootSignature/PipelineLayout
		//RasterizerDesc rasterizerState;
		//BlendDesc blendState;
		//DepthStencilDesc depthStencilState;
		//std::vector<GraphicsFormat> renderTargetFormats; // RTV 포맷들
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

		static Shared<PipelineState> Create(PipelineStateDesc _desc);
	protected:
		bool ValidateShaders();

		Shared<Shader> vertexShader;
		Shared<Shader> hullShader;
		Shared<Shader> domainShader;
		Shared<Shader> geometryShader;
		Shared<Shader> pixelShader;

		std::vector<Shared<Shader>> shaders;

		//root signature??

		VertexArray* vertexArray;//input layout

		//rtv, dsv;
		//blend, rast, ds;

	};
}