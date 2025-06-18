#pragma once

#include "Steins/Graphics/Core/Asset.h"

namespace Steins
{
	class Material : public Asset
	{
	public:
		virtual ~Material() = default;
		virtual void Bind() = 0;
	private:
	};
}