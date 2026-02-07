#include "DaydreamPCH.h"

#include "Daydream/Graphics/Core/Renderer.h"
#include "Daydream/Graphics/Resources/PipelineState.h"
#include "Daydream/Graphics/Manager/ResourceManager.h"

namespace Daydream
{
    void RenderContext::BindPipelineState(Shared<PipelineState> _pipelineState)
    {
        currentPipelineState = _pipelineState;
    }

    void RenderContext::BindMesh(Shared<Mesh> _mesh)
    {
        BindVertexBuffer(_mesh->GetVertexBuffer());
        BindIndexBuffer(_mesh->GetIndexBuffer());
    }
	
	void RenderContext::BindMaterial(Shared<Material> _material)
	{
		const auto& textureInfo = _material->GetAllTexture2D();
		for (auto [name, texture] : textureInfo)
		{
			if (texture == nullptr) continue;
			SetTexture2D(name, texture);
		}

		const auto& textureCubeInfo = _material->GetAllTextureCube();
		for (auto [name, textureCube] : textureCubeInfo)
		{
			if (textureCube == nullptr) continue;
			SetTextureCube(name, textureCube);
		}

		const auto& constantBufferInfo = _material->GetAllConstantBuffer();
		for (auto [name, cbuffer] : constantBufferInfo)
		{
			if (cbuffer == nullptr) continue;
			SetConstantBuffer(name, cbuffer);
		}
	}

    void RenderContext::SetTexture2D(const String& _name, Shared<Texture2D> _texture)
    {
        if (!_texture->HasSampler())
        {
            _texture->SetSampler(ResourceManager::GetResource<Sampler>("LinearRepeat"));
        }
    }
}
