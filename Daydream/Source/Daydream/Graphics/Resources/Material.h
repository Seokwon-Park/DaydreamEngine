#pragma once

#include "Daydream/Core/Resource.h"
#include "Daydream/Graphics/Resources/Shader.h"


namespace Daydream
{
	class PipelineState;
	class Texture2D;
	class TextureCube;
	class ConstantBuffer;

	class Material : public Resource
	{
	public:
		virtual ~Material() = default;
		virtual void Bind() = 0;

		virtual void SetTexture2D(const std::string& _name, Shared<Texture2D> _texture) {};
		virtual void SetTextureCube(const std::string& _name, Shared<TextureCube> _textureCube) {};
		virtual void SetConstantBuffer(const std::string& _name, Shared<ConstantBuffer> _buffer) {};

		static Shared<Material> Create(Shared<PipelineState> _pipeline);
	protected:
		HashMap<String, ShaderReflectionInfo> bindingMap;
	private:

	};
}