#include "SteinsPCH.h"
#include "D3D11GraphicsContext.h"
#include "D3D11RenderDevice.h"

#include "Steins/Render/SwapChain.h"

namespace Steins
{
	D3D11GraphicsContext::D3D11GraphicsContext(D3D11RenderDevice* _device)
	{
		device = _device;
	}
	void D3D11GraphicsContext::Init()
	{
	}
	void D3D11GraphicsContext::SetViewport(UInt32 _x, UInt32 _y, UInt32 _width, UInt32 _height)
	{
		
	}
	void D3D11GraphicsContext::SetClearColor(const Color& _color)
	{
		clearColor = _color;
	}

	void D3D11GraphicsContext::Clear()
	{
		device->GetSwapChain(0)->GetBackFramebuffer()->Begin();
		device->GetSwapChain(0)->GetBackFramebuffer()->Clear(clearColor);
	}

	void D3D11GraphicsContext::DrawIndexed(UInt32 _indexCount, UInt32 _startIndex, UInt32 _baseVertex)
	{
		device->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		device->GetContext()->DrawIndexed(_indexCount, _startIndex, _baseVertex);
	}
}
