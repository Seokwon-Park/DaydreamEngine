#pragma once

#include "Daydream/Graphics/Resources/Shader.h"
#include "Daydream/Graphics/Resources/ShaderGroup.h"
#include "Daydream/Graphics/Resources/Buffer.h"
#include "Daydream/Graphics/Resources/Texture/Texture.h"
#include "Daydream/Graphics/Resources/Texture/TextureView.h"
#include "Daydream/Graphics/Resources/Material.h"
#include "Daydream/Graphics/States/BlendState.h"
#include "Daydream/Graphics/States/RasterizerState.h"
#include "Daydream/Graphics/States/DepthStencilState.h"


namespace Daydream
{
	enum class PrimitiveTopologyType
	{
		TriangleList,
	};

	enum class AttachmentType
	{
		None,
		EntityHandle,
	};



	struct PipelineStateDesc
	{
		Shared<ShaderGroup> shaderGroup;
		//Shared<Shader> computeShader; //??

		//BufferLayout inputLayout;
		//InputLayoutDesc inputLayout;
		//Shared<ResourceBindingLayout> resourceBindingLayout; // RootSignature/PipelineLayout
		RasterizerStateDesc rasterizerState;
		//BlendDesc blendState;
		//DepthStencilDesc depthStencilState;
		Array<RenderFormat> renderTargetFormats; // RTV 포맷들
		RenderFormat depthStencilFormat = RenderFormat::UNKNOWN; // DSV 포맷

		UInt32 sampleCount = 1;
		//GraphicsFormat depthStencilFormat = GraphicsFormat::Unknown; // DSV 포맷
		PrimitiveTopologyType topologyType = PrimitiveTopologyType::TriangleList;
	};

	class PipelineState
	{
	public:
		PipelineState(const PipelineStateDesc& _desc);
		virtual ~PipelineState() = default;

		virtual void Bind() const = 0;
		//virtual Shared<Material> CreateMaterial() = 0;

		//ShaderGroup Functions
		inline const Shared<ShaderGroup>& GetShaderGroup() const { return shaderGroup; }
		inline const Array<Shared<Shader>>& GetShaders() const { return shaderGroup->GetShaders(); };
		inline const ShaderReflectionData* GetBindingInfo(const String& _name) const {
			return shaderGroup->GetShaderBindingInfo(_name);
		}

		static Shared<PipelineState> Create(const PipelineStateDesc& _desc);
	protected:

		Shared<ShaderGroup> shaderGroup;

		PipelineStateDesc desc;
		//rtv, dsv;
		//blend, rast, ds;
	};
}