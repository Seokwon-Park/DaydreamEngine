#pragma once

#include "Steins/Window.h"
#include "GLFW/glfw3.h"

namespace Steins
{
	class WindowsWindow : public SteinsWindow
	{
	public:
		// constrcuter destructer
		WindowsWindow(const WindowProps& _props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline uint32 GetWidth() const override { return windowData.width; }
		inline uint32 GetHeight() const override { return windowData.height; }

		inline void SetEventCallback(const EventCallbackFn& _callbackFn) override { windowData.eventCallbackFn = _callbackFn; }

		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		int GetKeyState(int _key){ return glfwGetKey(glfwWindow, static_cast<int32>(_key)); }
	private:
		virtual void Init(const WindowProps& _props);
		virtual void Shutdown();

		GLFWwindow* glfwWindow;

		HWND windowHandle;

		struct WindowData
		{
			std::string title;
			uint32 width, height;
			bool isVSync;
			EventCallbackFn eventCallbackFn;
		};
		WindowData windowData;
	};
}

