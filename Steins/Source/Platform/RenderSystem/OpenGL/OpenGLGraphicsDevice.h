#pragma once
#include "Steins/Render/GraphicsDevice.h"
#include "GLFW/glfw3.h"
//#include "glad/glad.h"

namespace Steins
{
	class OpenGLGraphicsDevice: public GraphicsDevice
	{
	public:
		OpenGLGraphicsDevice(SteinsWindow* _window);
		~OpenGLGraphicsDevice() override;

		virtual void Init() override;
		virtual void Shutdown() override;
		virtual void Render() override;
		virtual void SwapBuffers() override;

		virtual void SetPrimitiveTopology(PrimitiveTopology _primitiveTopology) override;
		void DrawIndexed(UInt32 _indexCount, UInt32 _startIndex, UInt32 _baseVertex) override;

		std::string GetVersion() const { return version; }
	protected:

	private:
		GLFWwindow* nativeWindow;

		std::string version = "#version 410";


		//temp
		unsigned int VAO;

		// Inherited via GraphicsDevice

		// Inherited via GraphicsDevice
	};
}
