#pragma once

#include "Steins/Window.h"

#define GLFW_EXPOSE_NATIVE_WIN32

#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"

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

		inline int GetKeyState(int _key) override { return windowData.keyStates[_key]; }
		inline int GetMouseState(int _mousebutton) override { return glfwGetMouseButton(glfwWindow, _mousebutton); }
		inline Pair<float32, float32> GetMousePos() override
		{
			double xpos, ypos;
			glfwGetCursorPos(glfwWindow, &xpos, &ypos);
			return MakePair<float32, float32>(static_cast<float32>(xpos), static_cast<float32>(ypos));
		}
		inline void* GetNativeWindow() override { return glfwWindow; }

		inline void SetKeyState(int _key, int _state)override { windowData.keyStates[_key] = _state; }
		inline bool GetIsKeyDown(int _key)override { return windowData.keyDownChecker[_key]; }
		inline void SetKeyDown(int _key) override { windowData.keyDownChecker[_key] = true; }
		void OnUpdateKeyState() override;

		HWND& GetWindowHandle() { return windowHandle; }
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
			std::vector<uint16> keyStates;
			std::vector<bool> keyDownChecker;
		};
		WindowData windowData;

	};
}

