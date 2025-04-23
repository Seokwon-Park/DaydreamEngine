#pragma once

#include "Steins/Render/Framebuffer.h"

namespace Steins
{
	class D3D12Framebuffer : public Framebuffer
	{
	public:
		virtual ~D3D12Framebuffer() override {};

		virtual void Bind() const {};

	private:
		std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> renderTargetHandles; 
		D3D12_CPU_DESCRIPTOR_HANDLE depthStencilHandle; 
		std::vector<ComPtr<ID3D12Resource>> colorAttachments;
		ComPtr<ID3D12Resource> depthAttachment;
	};
}

