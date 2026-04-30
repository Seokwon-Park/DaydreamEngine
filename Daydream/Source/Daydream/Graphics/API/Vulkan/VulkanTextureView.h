#pragma once

#include "Daydream/Graphics/Resources/TextureView.h"
#include "vulkan/vulkan.hpp"

namespace Daydream
{
	class VulkanTextureView : public TextureView
	{
	public:
		explicit VulkanTextureView(const TextureViewDesc& _desc);
		~VulkanTextureView() override = default;

		const TextureViewDesc& GetDesc() const override { return desc; }

		void SetImageView(vk::UniqueImageView _imageView) { imageView = std::move(_imageView); }
		vk::ImageView GetImageView() const { return imageView.get(); }

	private:
		TextureViewDesc desc;
		vk::UniqueImageView imageView;
	};
}