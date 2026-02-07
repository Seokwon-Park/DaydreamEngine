#include "DaydreamPCH.h"

#include "Application.h"

#include "Input.h"
#include "KeyCodes.h"
#include "Daydream/ImGui/ImGuiLayer.h"
#include "Daydream/Asset/AssetManager.h"
#include "Daydream/Graphics/Core/Renderer.h"
#include "Daydream/Scene/Components/ComponentRegistry.h"
#include "Daydream/Graphics/Manager/ResourceManager.h"


namespace Daydream
{
	Application* Application::instance = nullptr;

	Application::Application(ApplicationSpecification _specification)
	{
		DAYDREAM_CORE_ASSERT(!instance, "Application already exists!");
		instance = this;

		prop.width = 1280;
		prop.height = 720;
		prop.title = _specification.Name;
		prop.rendererAPI = _specification.rendererAPI;

		
		//prop.width = 960;
		//prop.height = 540;
		//prop.title = "TestWindow";
		//prop.rendererAPI = _specification.rendererAPI;

		//testWindow = DaydreamWindow::Create(prop);
		//testWindow->SetEventCallback(BIND_EVENT_FN(OnEvent));


	}

	Application::~Application()
	{
		
	}

	void Application::AttachLayer(Layer* _layer)
	{
		layerStack.PushLayer(_layer);
	}

	void Application::AttachOverlay(Layer* _overlay)
	{
		layerStack.PushOverlay(_overlay);
	}

	void Application::ReadConfig(const String& _fileName)
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

		//1. 윈도우 매니저 초기화
		WindowManager::Init();
		//2. 메인 윈도우 생성 및 등록
		mainWindow = DaydreamWindow::Create(prop);
		if (mainWindow == nullptr)
		{
			DAYDREAM_CORE_ASSERT(false, "No Main Window")
		}
		mainWindow->SetEventCallback(BIND_EVENT_FN(OnEvent));
		mainWindow->SetVSync(true);
		WindowManager::RegisterWindow(prop.title, mainWindow.get());

		//에셋 매니저 초기화
		AssetManager::Init();
		AssetManager::LoadAssetMetadataFromDirectory("Asset");

		//렌더러 초기화
		Renderer::Init(prop.rendererAPI);

		AssetManager::LoadAssets(LoadPhase::Early);// 셰이더 때문에 renderer초기화 이후로 미룸
		
		//렌더러에서 윈도우에 대한 스왑체인 생성
		Renderer::CreateSwapchainForWindow(mainWindow.get());
		//렌더러가 사용할 윈도우 설정
		Renderer::SetCurrentWindow(mainWindow.get());
		//Renderer::RegisterWindow("TestWindow", testWindow.get());

		ResourceManager::Init();

		AssetManager::LoadAssets(LoadPhase::Normal);// 모델을 로드하기 위해서는 pipeline이 빌드된 상태여야함

		imGuiLayer = new ImGuiLayer();
		AttachOverlay(imGuiLayer);

		ComponentRegistry::Init();

		return true;
	}
	bool Application::Run()
	{
		while (isRunning)
		{
			timeStep.UpdateTime();
			float deltaTime = timeStep.GetDeltaTime();

			Renderer::BeginCommandList();
			for (Layer* layer : layerStack)
			{
				layer->OnUpdate(deltaTime);
			}
			Renderer::EndCommandList();

			
			Renderer::BeginFrame(mainWindow->GetSwapchain());
			imGuiLayer->BeginImGui();
			{
				for (Layer* layer : layerStack)
					layer->OnImGuiRender();
			}
			imGuiLayer->EndImGui();
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
			Renderer::EndFrame(mainWindow->GetSwapchain());
			
			//testWindow->OnUpdate();
		}
		return true;
	}
	bool Application::Exit()
	{
		mainWindow->SetSwapchain(nullptr);
		ComponentRegistry::Shutdown();
		layerStack.Release();
		AssetManager::Shutdown();
		ResourceManager::Shutdown();
		Renderer::Shutdown();
		WindowManager::Shutdown();
		mainWindow = nullptr;

		return true;
	}

	void Application::OnEvent(Event& _event)
	{
		EventDispatcher dispatcher(_event);
		//_event의 타입이 WindowCloseEvent에 해당하면 OnWindowClose를 실행시킨다.
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
