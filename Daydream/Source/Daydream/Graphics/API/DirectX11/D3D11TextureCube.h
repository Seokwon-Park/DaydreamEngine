#pragma once

#pragma once

#include "D3D11Texture.h"
#include "D3D11RenderDevice.h"
#include "Daydream/Graphics/Resources/TextureCube.h"


namespace Daydream
{
	class D3D11TextureCube : public TextureCube
	{
	public:
		D3D11TextureCube(D3D11RenderDevice* _device, const TextureDesc& _desc, Array<const void*> _initialData);

		virtual ~D3D11TextureCube() = default;

		virtual void* GetNativeHandle() { return nullptr; };

	protected:
		D3D11RenderDevice* device;
		ComPtr<ID3D11Texture2D> texture;
	};
}