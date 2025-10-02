#pragma once

#include "OpenGLSampler.h"
#include "Daydream/Graphics/Resources/TextureCube.h"

namespace Daydream
{
	class OpenGLTextureCube : public TextureCube
	{
	public:
		// Constrcuter Destructer
		OpenGLTextureCube(const TextureDesc& _desc, const Array<const void*>& _initialData);
		virtual ~OpenGLTextureCube();

		virtual void Update(UInt32 _faceIndex, Shared<Texture2D> _texture);

		virtual void SetSampler(Shared<Sampler> _sampler) override;

		virtual inline void* GetNativeHandle() override { return reinterpret_cast<void*>(static_cast<UInt64>(textureID)); }

		UInt32 GetTextureID() const { return textureID; }
		UInt32 GetSamplerID() const { return textureSampler->GetSamplerID(); }
	private:
		UInt32 textureID;
		OpenGLSampler* textureSampler;
	};
}