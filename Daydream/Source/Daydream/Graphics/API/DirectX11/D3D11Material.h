#pragma once

#include "Daydream/Graphics/Resources/Material.h"
#include "D3D11RenderDevice.h"
#include "D3D11PipelineState.h"

namespace Daydream
{
	class D3D11Material : public Material
	{
	public:
		D3D11Material(D3D11RenderDevice* _device, Shared<PipelineState> _pso);

		virtual void Bind() override;
		virtual void Unbind() override;
		//virtual void SetTexture2D(const String& _name, Shared<Texture2D> _texture)override;
		//virtual void SetTextureCube(const String& _name, Shared<TextureCube> _textureCube)override;
		//virtual void SetConstantBuffer(const String& _name, Shared<ConstantBuffer> _buffer)override;
	private:
		D3D11RenderDevice* device;

		//HashMap<String, Shared<ConstantBuffer>> cbuffers;
		//HashMap<String, Shared<Texture2D>> textures;
		//HashMap<String, Shared<TextureCube>> textureCubes;
	};
}