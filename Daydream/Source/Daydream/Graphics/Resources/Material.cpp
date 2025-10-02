#include "DaydreamPCH.h"
#include "Material.h"

#include "Daydream/Graphics/Core/Renderer.h"
#include "Daydream/Graphics/Core/PipelineState.h"

namespace Daydream
{
    Shared<Material> Material::Create(Shared<PipelineState> _pipeline)
    {
        return Renderer::GetRenderDevice()->CreateMaterial(_pipeline);
    }

	void Material::SetTexture2D(const String& _name, Shared<Texture2D> _texture)
	{
		if (bindingMap.find(_name) != bindingMap.end())
		{
			textures[_name] = _texture;
		}
	}

	void Material::SetTextureCube(const String& _name, Shared<TextureCube> _texture)
	{
		if (bindingMap.find(_name) != bindingMap.end())
		{
			textureCubes[_name] = _texture;
		}
	}
	void Material::SetConstantBuffer(const String& _name, Shared<ConstantBuffer> _buffer)
	{
		if (bindingMap.find(_name) != bindingMap.end())
		{
			cbuffers[_name] = _buffer;
		}
	}
}
