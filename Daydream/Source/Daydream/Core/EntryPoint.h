#include "Base/MathTypes.h"

//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

namespace Daydream
{
	extern Application* CreateApplication();
}

Int32 main(int argc, char** argv)
{
	//_CrtSetBreakAlloc(176);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Daydream::Logger::Init();

	Daydream::Application* application = Daydream::CreateApplication();

	if (application->Init() == false)
	{
		return 1;
	}

	application->Run();

	if (!application->Exit())
	{
		return 1;
	}

	if (application != nullptr)
	{
		delete application;
		application = nullptr;
	}

	Daydream::Logger::Shutdown();

	return 0;
}
