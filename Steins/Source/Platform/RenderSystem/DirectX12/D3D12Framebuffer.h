#pragma once

#include "Steins/Render/Framebuffer.h"

namespace Steins
{
	class D3D12Framebuffer : public Framebuffer
	{
	public:
	private:
		std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> renderTargetHandles; 
		D3D12_CPU_DESCRIPTOR_HANDLE depthStencilHandle; 
		std::vector<ComPtr<ID3D12Resource>> colorAttachments;
		ComPtr<ID3D12Resource> depthAttachment;
	};
}

