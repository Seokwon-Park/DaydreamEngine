#include "SteinsPCH.h"
#include "D3D11RendererAPI.h"
#include "D3D11GraphicsDevice.h"

namespace Steins
{
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
		device->DrawIndexed(_indexCount, _startIndex, _baseVertex);
	}
}
