#include "DaydreamPCH.h"
#include "Daydream/Graphics/Resources/TextureCube.h"

#include "Daydream/Graphics/Core/Renderer.h"

#include "Daydream/Graphics/Utility/ImageLoader.h"

#include "Daydream/Graphics/Manager/ResourceManager.h"

namespace Daydream
{
	TextureCube::TextureCube(const TextureDesc& _desc)
		:Texture(_desc)
	{
		textures.resize(6);
	}
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


	Shared<TextureCube> TextureCube::Create(const Array<Path>& _paths, const TextureDesc& _desc)
	{
		Array<const void*> imageDatas;
		Array<ImageData> temp;
		TextureDesc finalDesc = _desc;
		for (auto path : _paths)
		{
			ImageData imageData = ImageLoader::LoadImageFile(path);

			if (std::get<Array<UInt8>>(imageData.data).data() != nullptr)
			{
				finalDesc.width = imageData.width;
				finalDesc.height = imageData.height;

				temp.push_back(imageData);
			}
		}
		for (int i = 0; i < temp.size(); i++)
		{
			imageDatas.push_back(std::get<Array<UInt8>>(temp[i].data).data()); 
		}
		Shared<TextureCube> textureCube = Renderer::GetRenderDevice()->CreateTextureCube(imageDatas, finalDesc);
		for (int i = 0; i < 6; i++)
		{
			Path path = _paths[i];
			textureCube->textures[i] = ResourceManager::GetResource<Texture2D>(path.make_preferred().string());
		}
		return textureCube;
	}

	Shared<TextureCube> TextureCube::Create(const Array<Shared<Texture2D>>& _textures, const TextureDesc& _desc)
	{
		Shared<TextureCube> textureCube = Renderer::GetRenderDevice()->CreateTextureCube(_textures, _desc);
		textureCube->textures = _textures;
		return textureCube;
	}

	Shared<TextureCube> TextureCube::CreateEmpty(const TextureDesc& _desc)
	{
		Shared<TextureCube> textureCube = Renderer::GetRenderDevice()->CreateEmptyTextureCube(_desc);
		return textureCube;
	}
}