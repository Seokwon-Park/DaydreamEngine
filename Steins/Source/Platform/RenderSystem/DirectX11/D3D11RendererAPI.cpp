#include "SteinsPCH.h"
#include "D3D11RendererAPI.h"
#include "D3D11GraphicsDevice.h"

namespace Steins
{
	D3D11RendererAPI::D3D11RendererAPI(GraphicsDevice* _device)
	{
		device = Cast<D3D11GraphicsDevice>(_device);
	}
	void D3D11RendererAPI::Init()
	{
	}
	void D3D11RendererAPI::SetClearColor(const Color& _color)
	{
		clearColor = _color;
	}

	void D3D11RendererAPI::Clear()
	{
		((D3D11GraphicsDevice*)device)->ClearRenderTargetViews(clearColor);
	}

	void D3D11RendererAPI::DrawIndexed(UInt32 _indexCount, UInt32 _startIndex, UInt32 _baseVertex)
	{
		((D3D11GraphicsDevice*)device)->BindRenderTargets();
		device->GetContext()->DrawIndexed(_indexCount, _startIndex, _baseVertex);
	}
}
