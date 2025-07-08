#pragma once

#include "Steins/Graphics/Core/Asset.h"


namespace Steins
{
	class PipelineState;

	class Material : public Asset
	{
	public:
		virtual ~Material() = default;
		virtual void Bind() = 0;

		static Shared<Material> Create(Shared<PipelineState> _pipeline);
	private:
	};
}