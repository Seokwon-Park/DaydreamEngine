#pragma once

#include "Steins/Graphics/Core/Asset.h"


namespace Steins
{
	class PipelineState;
	class Texture;
	class ConstantBuffer;

	class Material : public Asset
	{
	public:
		virtual ~Material() = default;
		virtual void Bind() = 0;

		virtual void SetTexture(const std::string& _name, Shared<Texture> _texture) {};
		virtual void SetConstantBuffer(const std::string& _name, Shared<ConstantBuffer> _buffer) {};

		static Shared<Material> Create(Shared<PipelineState> _pipeline);
	private:
	};
}