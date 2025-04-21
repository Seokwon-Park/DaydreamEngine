#pragma once

#include "Steins/Render/Texture.h"

namespace Steins
{
	class D3D11Texture2D :public Texture2D
	{
	public:
		// constrcuter destructer
		D3D11Texture2D(const FilePath& _path);
		virtual ~D3D11Texture2D();

		virtual void Bind(UInt32 _slot) const override;

	protected:

	private:

	};
}

