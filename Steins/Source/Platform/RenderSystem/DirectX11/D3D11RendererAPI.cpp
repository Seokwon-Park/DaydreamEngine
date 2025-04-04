#include "SteinsPCH.h"
#include "D3D11RendererAPI.h"

namespace Steins
{
	void D3D11RendererAPI::SetClearColor(const Color& _color)
	{
		clearColor = _color;
	}

	void D3D11RendererAPI::Clear()
	{

	}

	void D3D11RendererAPI::DrawIndexed(UInt32 _indexCount, UInt32 _startIndex, UInt32 _baseVertex)
	{
		device->DrawIndexed(_indexCount, _startIndex, _baseVertex);
	}
}
