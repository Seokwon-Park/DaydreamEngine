#pragma once

#include "Daydream/Graphics/Resources/Texture/Texture.h"

namespace Daydream
{
	class Texture2D;

	struct TextureViewDesc
	{
		TextureViewType type = TextureViewType::ShaderResource;
		RenderFormat format = RenderFormat::UNKNOWN; // UNKNOWN이면 원본 texture format 사용

		UInt32 baseMip = 0;
		UInt32 mipCount = 1;

		UInt32 baseLayer = 0;
		UInt32 layerCount = 1;

		bool readOnlyDepth = false;
		bool readOnlyStencil = false;
	};

	class TextureView
	{
	public:
		TextureView(Shared<GPUTexture> _texture, const TextureViewDesc& _desc);
		virtual ~TextureView();

		inline Shared<GPUTexture> GetOriginTexture() const { return originTexture; }
		inline const TextureViewDesc& GetDesc() { return desc; }

		static Shared<TextureView> Create(Shared<Texture2D> _texture, const TextureViewDesc& _desc);
	protected:
		TextureViewDesc desc;
		Shared<GPUTexture> originTexture;
	private:

	};
}
