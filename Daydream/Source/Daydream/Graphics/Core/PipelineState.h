#pragma once

#include "Daydream/Graphics/Resources/Shader.h"
#include "Daydream/Graphics/Resources/ShaderGroup.h"
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
		Shared<ShaderGroup> shaderGroup;
		//Shared<Shader> computeShader; //??

		//BufferLayout inputLayout;
		//InputLayoutDesc inputLayout;
		//Shared<ResourceBindingLayout> resourceBindingLayout; // RootSignature/PipelineLayout
		Shared<RenderPass> renderPass;
		RasterizerStateDesc rasterizerState;
		//BlendDesc blendState;
		//DepthStencilDesc depthStencilState;
		//Array<GraphicsFormat> renderTargetFormats; // RTV ���˵�
		//GraphicsFormat depthStencilFormat = GraphicsFormat::Unknown; // DSV ����
		UInt32 sampleCount = 1;
		PrimitiveTopologyType topologyType = PrimitiveTopologyType::TriangleList;
	};

	class PipelineState
	{
	public:
		PipelineState(const PipelineStateDesc& _desc);
		virtual ~PipelineState() = default;

		virtual void Bind() const = 0;
		virtual Shared<Material> CreateMaterial() = 0;

		const Array<Shared<Shader>>& GetShaders() { return shaderGroup->GetShaders(); };
				
		static Shared<PipelineState> Create(const PipelineStateDesc& _desc);
	protected:
		Shared<ShaderGroup> shaderGroup;

		Shared<RenderPass> renderPass;

		//rtv, dsv;
		//blend, rast, ds;

	};
}