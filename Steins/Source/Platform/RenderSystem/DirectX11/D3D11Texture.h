#pragma once

#include "Steins/Render/Texture.h"
#include "D3D11GraphicsDevice.h"

namespace Steins
{
	class D3D11Texture2D :public Texture2D
	{
	public:
		// constrcuter destructer
		D3D11Texture2D(D3D11GraphicsDevice* _device, const FilePath& _path);
		virtual ~D3D11Texture2D();

		virtual void Bind(UInt32 _slot) const override;

		virtual void* GetNativeHandle() override { return texture.Get(); }
	protected:

	private:
		D3D11GraphicsDevice* device;
		ComPtr<ID3D11Texture2D> texture;
	};
}

