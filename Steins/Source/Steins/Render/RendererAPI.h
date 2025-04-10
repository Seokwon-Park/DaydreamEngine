#pragma once
#include "GraphicsDevice.h"
#include "Steins/Enum/RendererEnums.h"

namespace Steins
{
	class RendererAPI
	{
	public:
		virtual void Init(GraphicsDevice* _device);
		virtual void SetClearColor(const Color& _color) = 0;
		virtual void Clear() = 0;
		virtual void DrawIndexed(UInt32 _indexCount, UInt32 _startIndex, UInt32 _baseVertex) = 0;

		inline static void SetRendererAPI(RendererAPIType _Type) { API = _Type; }
		inline static RendererAPIType GetRendererAPI() { return API; }
	protected:
		GraphicsDevice* device;
	private:
		static RendererAPIType API;
	};
}