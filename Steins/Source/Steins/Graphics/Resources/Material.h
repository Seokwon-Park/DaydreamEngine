#pragma once

#include "Steins/Graphics/Resources/Resource.h"
#include "Steins/Graphics/Resources/Shader.h"


namespace Steins
{
	class PipelineState;
	class Texture2D;
	class ConstantBuffer;

	class Material : public Resource
	{
	public:
		virtual ~Material() = default;
		virtual void Bind() = 0;

		virtual void SetTexture2D(const std::string& _name, Shared<Texture2D> _texture) {};
		virtual void SetConstantBuffer(const std::string& _name, Shared<ConstantBuffer> _buffer) {};

		static Shared<Material> Create(Shared<PipelineState> _pipeline);
	protected:
		HashMap<String, ShaderReflectionInfo> bindingMap;
	private:

	};
}