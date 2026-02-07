#pragma once

#include "Texture.h"


namespace Daydream
{
	class TextureCube : public Texture
	{
	public:
		TextureCube(const TextureDesc& _dsec);
		virtual ~TextureCube();

		virtual UInt32 GetLayerCount() const { return 6; }

		void Update(UInt32 _faceIndex, Shared<Texture2D> _texture);
		virtual void GenerateMips() {};

		virtual void* GetNativeHandle() override = 0;
		void* GetImGuiHandle(UInt32 _faceIndex);

		static Shared<TextureCube> Create(const Array<Path>& _paths, const TextureDesc& _desc);
		static Shared<TextureCube> Create(const Array<Shared<Texture2D>>& _textures, const TextureDesc& _desc);
		static Shared<TextureCube> CreateEmpty(const TextureDesc& _desc);
	protected:
		Array<Shared<Texture2D>> textures;
	};
}