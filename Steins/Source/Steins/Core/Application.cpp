#include "SteinsPCH.h"

#include "Application.h"

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

namespace Steins
{

	Application::Application()
	{
		mainWindow = SteinsWindow::Create();
	}

	Application::~Application()
	{

	}

	bool Application::Init()
	{
		if (mainWindow == nullptr)
		{
			STEINS_CORE_ASSERT(false, "No Main Window")
			return false;
		}
		mainWindow->SetEventCallback(BIND_EVENT_FN(OnEvent));
		isRunning = true;

		return true;
	}
	bool Application::Run()
	{
		while (isRunning)
		{
			mainWindow->OnUpdate();
		}
		return true;
	}
	bool Application::Exit()
	{
		return true;
	}

	//
	void Application::OnEvent(Event& _event)
	{
		EventDispatcher dispatcher(_event);
		//여기는 이벤트가 발생할 때마다 매번 일치하는지 탐색한다.
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		STEINS_CORE_TRACE("{0}", _event);
	}

	bool Application::OnWindowClose(WindowCloseEvent& _event)
	{
		isRunning = false;
		return true;
	}
}
