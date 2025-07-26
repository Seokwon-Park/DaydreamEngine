#pragma once

#include "Steins/Graphics/Resources/Shader.h"
#include "Steins/Graphics/Resources/Buffer.h"
#include "Steins/Graphics/Resources/Texture.h"
#include "Steins/Graphics/Resources/Material.h"
#include "Steins/Graphics/Core/RenderPass.h"

namespace Steins
{
	enum class PrimitiveTopologyType
	{
		TriangleList,
	};

	struct RasterizerStateDesc
	{
		enum class FillMode {
			Solid,
			Wireframe
		};

		enum class CullMode {
			None,
			Front,
			Back
		};

		FillMode fillMode = FillMode::Solid;
		CullMode cullMode = CullMode::Back;
		bool frontCounterClockwise = false;
		bool depthClipEnable = true;
		bool scissorEnable = false;
		bool multisampleEnable = false;
		bool antialiasedLineEnable = false;
		int depthBias = 0;
		float depthBiasClamp = 0.0f;
		float slopeScaledDepthBias = 0.0f;
	};

	struct PipelineStateDesc
	{
		Shared<Shader> vertexShader = nullptr;
		Shared<Shader> hullShader = nullptr;
		Shared<Shader> domainShader = nullptr;
		Shared<Shader> geometryShader = nullptr;
		Shared<Shader> pixelShader = nullptr;
		//Shared<Shader> computeShader; //??

		BufferLayout inputLayout;
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