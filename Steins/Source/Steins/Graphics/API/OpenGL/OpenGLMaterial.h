#pragma once
#include "Steins/Graphics/Resources/Material.h"
#include "glad/glad.h"
#include "OpenGLPipelineState.h"

namespace Steins
{
	class OpenGLMaterial : public Material
	{
	public:
		OpenGLMaterial(OpenGLPipelineState* _pso);

		virtual void Bind() override;

		virtual void SetTexture2D(const std::string& _name, Shared<Texture2D> _texture)override;
		virtual void SetConstantBuffer(const std::string& _name, Shared<ConstantBuffer> _buffer)override;
	private:
		HashMap<String, Shared<ConstantBuffer>> cbuffers;
		HashMap<String, Shared<Texture2D>> textures;
	};
}