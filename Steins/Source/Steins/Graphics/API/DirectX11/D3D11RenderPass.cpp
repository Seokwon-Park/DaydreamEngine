#include "SteinsPCH.h"
#include "D3D11RenderPass.h"
#include "D3D11Framebuffer.h"

namespace Steins
{
    D3D11RenderPass::D3D11RenderPass(D3D11RenderDevice* _device, const RenderPassDesc& _desc)
    {
        device = _device;
        desc = _desc;
    }
    D3D11RenderPass::~D3D11RenderPass()
    {
        device = nullptr;
    }
    void D3D11RenderPass::Begin(Shared<Framebuffer> _framebuffer)
    {
        currentFramebuffer = static_pointer_cast<D3D11Framebuffer>(_framebuffer);
        Array<ID3D11RenderTargetView*> rtvs = currentFramebuffer->GetRenderTargetViews();
        for (auto rtv : rtvs)
        {
            device->GetContext()->ClearRenderTargetView(rtv, clearColor.color);
        }
        device->GetContext()->OMSetRenderTargets(rtvs.size(), rtvs.data(), nullptr);

        D3D11_VIEWPORT viewport;
        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        viewport.Width = Cast<Float32>(_framebuffer->GetWidth());
        viewport.Height = Cast<Float32>(_framebuffer->GetHeight());
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;

        device->GetContext()->RSSetViewports(1, &viewport);
    }
    void D3D11RenderPass::End()
    {
        device->GetContext()->OMSetRenderTargets(0, nullptr, nullptr);
    }
    Shared<Framebuffer> D3D11RenderPass::CreateFramebuffer(const FramebufferDesc& _desc)
    {
        return MakeShared<D3D11Framebuffer>(device, this, _desc);
    }
}
