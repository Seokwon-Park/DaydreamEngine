#pragma once

#include "OpenGLSampler.h"

namespace Daydream
{
	//class OpenGLTextureCube : public TextureCube
	//{
	//public:
	//	// Constrcuter Destructer
	//	OpenGLTextureCube(const TextureDesc& _desc, const Array<const void*>& _initialData);
	//	virtual ~OpenGLTextureCube();

	//	virtual void SetSampler(Shared<Sampler> _sampler) override;
	//	virtual bool HasSampler() override { return textureSampler != nullptr; }
	//	virtual void GenerateMips() override;

	//	UInt32 GetTextureID() const { return textureID; }
	//	UInt32 GetSamplerID() const { return textureSampler->GetSamplerID(); }
	//private:
	//	UInt32 textureID;
	//	OpenGLSampler* textureSampler;
	//};
}