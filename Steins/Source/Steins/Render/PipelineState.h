#pragma once

#include "Shader.h"

namespace Steins
{
	struct PipelineStateDesc
	{
		std::vector<Pair<Shared<Shader>, ShaderType>> shaders;
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
		virtual ~PipelineState() = 0;

		virtual void Bind() const = 0;

		static Shared<PipelineState> Create(PipelineStateDesc _desc);
	private:
		Shared<Shader> vertexShader;
		Shared<Shader> hullShader;
		Shared<Shader> domainShader;
		Shared<Shader> geometryShader;
		Shared<Shader> pixelShader;

		//root signature??

		VertexArray* vertexArray;//input layout

		//rtv, dsv;
		//blend, rast, ds;

	};
}