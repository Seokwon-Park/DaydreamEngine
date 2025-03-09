#pragma once
#include "Platform/RenderSystem/RenderSystem.h"
#include "Steins/Window.h"
#include "GLFW/glfw3.h"
//#include "glad/glad.h"

namespace Steins
{
	class RenderSystemGL : public RenderSystem
	{
	public:
		RenderSystemGL(SteinsWindow* Window);
		~RenderSystemGL() override;

		virtual void Init() override;
		virtual void Shutdown() override;
		virtual void Render() override;
		virtual void SwapBuffers() override;
	protected:

	private:

	};
}
