#include "SteinsPCH.h"
#include "RendererAPI.h"



namespace Steins
{
	RendererAPIType RendererAPI::API = RendererAPIType::None;

	void RendererAPI::Init(GraphicsDevice* _device)
	{
		device = _device;
	}

	/*std::string RendererAPI::GetAPIName() const
	{
		switch (graphicsAPI)
		{
		case Steins::RendererAPI::API::None: return "No API";
		case Steins::RendererAPI::API::OpenGL: return "OpenGL";
		case Steins::RendererAPI::API::DirectX11: return "DirectX11";
		default:
			break;
		}
		STEINS_CORE_ERROR("Wrong API");
		return "Error";
	}*/
}
