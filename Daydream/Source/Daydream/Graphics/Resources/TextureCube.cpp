#include "DaydreamPCH.h"
#include "Daydream/Graphics/Resources/TextureCube.h"

#include "Daydream/Graphics/Core/Renderer.h"

#include "Daydream/Graphics/Utility/ImageLoader.h"

#include "Daydream/Graphics/Manager/ResourceManager.h"

namespace Daydream
{


	TextureCube::~TextureCube()
	{
		textures.clear();
	}

	void TextureCube::Update(UInt32 _faceIndex, Shared<Texture2D> _texture)
	{
		textures[_faceIndex] = _texture;
		textures[_faceIndex]->SetSampler(ResourceManager::GetResource<Sampler>("LinearClampToEdge"));
	}

	void* TextureCube::GetImGuiHandle(UInt32 _faceIndex)
	{
		if (textures[_faceIndex] == nullptr)
		{
			return nullptr;
		}
		return textures[_faceIndex]->GetImGuiHandle();
	}
}