#pragma once

#include "Daydream/Graphics/Resources/Material.h"
#include "D3D12RenderDevice.h"
#include "D3D12PipelineState.h"

namespace Daydream
{
	class D3D12Material : public Material
	{
	public:
		D3D12Material(D3D12RenderDevice* _device, D3D12PipelineState* _pso);

		virtual void Bind() override;

		//virtual void SetTexture2D(const std::string& _name, Shared<Texture2D> _texture)override;
		//virtual void SetTextureCube(const std::string& _name, Shared<TextureCube> _texture)override;
		//virtual void SetConstantBuffer(const std::string& _name, Shared<ConstantBuffer> _buffer)override;
	private:
		D3D12RenderDevice* device;
		D3D12PipelineState* pso;

		//HashMap<String, Shared<ConstantBuffer>> cbuffers;
		//HashMap<String, Shared<Texture2D>> textures;
		//HashMap<String, Shared<TextureCube>> textureCubes;
	};
}