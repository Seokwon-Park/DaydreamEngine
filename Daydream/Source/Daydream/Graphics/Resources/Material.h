#pragma once

#include "Daydream/Core/Resource.h"
#include "Daydream/Graphics/Resources/Shader.h"


namespace Daydream
{
	class Sampler;
	class PipelineState;
	class Texture2D;
	class TextureCube;
	class ConstantBuffer;

	class Material : public Resource
	{
	public:
		virtual ~Material() = default;
		virtual void Bind() = 0;

		virtual void SetSampler(const String& _name, Shared<Sampler> _sampler) {};
		virtual void SetTexture2D(const String& _name, Shared<Texture2D> _texture) {};
		virtual void SetTextureCube(const String& _name, Shared<TextureCube> _textureCube) {};
		virtual void SetConstantBuffer(const String& _name, Shared<ConstantBuffer> _buffer) {};

		static Shared<Material> Create(Shared<PipelineState> _pipeline);
	protected:
		HashMap<String, ShaderReflectionData> bindingMap;
	private:

	};
}