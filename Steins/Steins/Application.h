#pragma once

// Ό³Έν :
class Application
{
public:
	// constrcuter destructer
	Application();
	~Application();

	// delete Function
	Application(const Application& _Other) = delete;
	Application(Application&& _Other) noexcept = delete;
	Application& operator=(const Application& _Other) = delete;
	Application& operator=(Application&& _Other) noexcept = delete;

	bool Init();
	bool Run();
	bool Exit();
protected:

private:

};

