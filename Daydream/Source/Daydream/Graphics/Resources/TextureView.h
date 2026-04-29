#pragma once

namespace Daydream
{
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
		virtual ~TextureView() = default;
		virtual const TextureViewDesc& GetDesc() const = 0;
	protected:


	private:

	};
}
