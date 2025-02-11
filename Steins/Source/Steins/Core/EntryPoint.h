namespace Steins
{
	extern Application* CreateApplication();
}

int main(int argc, char** argv)
{
	Steins::Application* application = Steins::CreateApplication();

	if (!application->Init())
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

	return 0;
}
