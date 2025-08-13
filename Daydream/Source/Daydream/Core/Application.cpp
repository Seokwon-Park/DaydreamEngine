#include "DaydreamPCH.h"

#include "Application.h"

#include "Input.h"
#include "KeyCodes.h"
#include "Daydream/ImGui/ImGuiLayer.h"
#include "Daydream/Graphics/Core/Renderer.h"
#include "Daydream/Graphics/Utility/ShaderCompileHelper.h"


#include "glad/glad.h"

namespace Daydream
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::instance = nullptr;

	Application::Application(ApplicationSpecification _specification)
	{
		DAYDREAM_CORE_ASSERT(!instance, "Application already exists!");
		instance = this;

		WindowProps prop;
		prop.width = 1280;
		prop.height = 720;
		prop.title = _specification.Name;
		prop.rendererAPI = _specification.rendererAPI;

		//�ϴ� ���α׷� ������ ����
		mainWindow = DaydreamWindow::Create(prop);
		if (mainWindow == nullptr)
		{
			DAYDREAM_CORE_ASSERT(false, "No Main Window")
		}
		mainWindow->SetEventCallback(BIND_EVENT_FN(OnEvent));
		mainWindow->SetVSync(true);

		
		//prop.width = 960;
		//prop.height = 540;
		//prop.title = "TestWindow";
		//prop.rendererAPI = _specification.rendererAPI;

		//testWindow = DaydreamWindow::Create(prop);
		//testWindow->SetEventCallback(BIND_EVENT_FN(OnEvent));
		WindowManager::Init();
		WindowManager::RegisterWindow(prop.title, mainWindow.get());
		
		//������ �ʱ�ȭ
		Renderer::Init(_specification.rendererAPI);
		//�������� ������ 
		Renderer::CreateSwapchainForWindow(mainWindow.get());
		Renderer::SetCurrentWindow(mainWindow.get());
		//Renderer::RegisterWindow("TestWindow", testWindow.get());

		imGuiLayer = new ImGuiLayer();
		AttachOverlay(imGuiLayer);

		ShaderCompileHelper::Init();
	}

	Application::~Application()
	{
		mainWindow->SetSwapchain(nullptr);
		mainWindow = nullptr;
		layerStack.Release();
		Renderer::Shutdown();
	}

	void Application::AttachLayer(Layer* _layer)
	{
		layerStack.PushLayer(_layer);
	}

	void Application::AttachOverlay(Layer* _overlay)
	{
		layerStack.PushOverlay(_overlay);
	}

	void Application::ReadConfig(std::string_view _fileName)
	{
		std::ifstream file(_fileName.data());
		DAYDREAM_CORE_ASSERT(!file.is_open(), "Cannot Open Configuration File!")

			std::string line;

		/*while (std::getline(file, line)) {
			if (line.find("API=") != std::string::npos) {
				api = line.substr(line.find("=") + 1);
				break;
			}
		}*/
	}

	bool Application::Init()
	{
		isRunning = true;

		return true;
	}
	bool Application::Run()
	{
		while (isRunning)
		{
			timeStep.UpdateTime();
			float deltaTime = timeStep.GetDeltaTime();

			for (Layer* layer : layerStack)
			{
				layer->OnUpdate(deltaTime);
			}

			Renderer::BeginSwapchainRenderPass(mainWindow.get());

			imGuiLayer->BeginImGui();
			{
				for (Layer* layer : layerStack)
					layer->OnImGuiRender();
			}
			imGuiLayer->EndImGui();

			Renderer::EndSwapchainRenderPass(mainWindow.get());
			//auto [x, y] = Input::GetMousePosition();
			////DAYDREAM_CORE_TRACE("{0}, {1}", x, y);

			//if (Input::GetMouseDown(Mouse::ButtonLeft))
			//{
			//	DAYDREAM_CORE_TRACE("MOUSE BUTTON DOWN TEST");
			//}

			//if (Input::GetMouseReleased(Mouse::ButtonLeft))
			//{
			//	DAYDREAM_CORE_TRACE("MOUSE BUTTON RELEASE TEST");
			//}

			//if (Input::GetMousePressed(Mouse::ButtonLeft))
			//{
			//	DAYDREAM_CORE_TRACE("MOUSE DOWN TEST");
			//}

			//if (Input::GetKeyPress(Key::B))
			//{
			//	DAYDREAM_CORE_TRACE("KEY PRESS TEST");
			//}

			//if (Input::GetKeyUp(Key::C))
			//{
			//	DAYDREAM_CORE_TRACE("KEY UP TEST");
			//}

			mainWindow->OnUpdateInputState();
			mainWindow->OnUpdate();
			//testWindow->OnUpdate();
		}
		return true;
	}
	bool Application::Exit()
	{
		return true;
	}

	void Application::OnEvent(Event& _event)
	{
		EventDispatcher dispatcher(_event);
		//_event�� Ÿ���� WindowCloseEvent�� �ش��ϸ� OnWindowClose�� �����Ų��.
		dispatcher.Dispatch<WindowFocusEvent>(BIND_EVENT_FN(OnWindowFocused));
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));

		for (Array<Layer*>::iterator itr = layerStack.end(); itr != layerStack.begin();)
		{
			if (_event.handled) // if Event is already handled then break;
				break;
			//check event for layer
			(*--itr)->OnEvent(_event);
		}

		//DAYDREAM_CORE_TRACE("{0}", _event.ToString());
	}

	bool Application::OnWindowClose(WindowCloseEvent& _event)
	{
		isRunning = false;
		return true;
	}
	bool Application::OnWindowResize(WindowResizeEvent& _event)
	{
		if (_event.GetWidth() == 0 || _event.GetHeight() == 0)
		{
			isMinimized = true;
			return false; 
		}

		//DAYDREAM_CORE_INFO("Window Resized : [ {0} , {1} ]", _event.GetWidth(), _event.GetHeight());
		isMinimized = false;
		Renderer::OnWindowResize(_event.GetWidth(), _event.GetHeight());
		
		return false;
	}
	bool Application::OnWindowFocused(WindowFocusEvent& _e)
	{
		if (_e.GetIsFocused() == true)
		{
			DAYDREAM_CORE_INFO("{0} Window is now focused", _e.GetWindowName());
			//	Renderer::SetWindow(_e.GetWindowName());
		}
		return false;
	}
}
