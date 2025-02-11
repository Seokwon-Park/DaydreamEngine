#pragma once

// Ό³Έν :
class Sandbox2D
{
public:
	// constrcuter destructer
	Sandbox2D();
	~Sandbox2D();

	// delete Function
	Sandbox2D(const Sandbox2D& _Other) = delete;
	Sandbox2D(Sandbox2D&& _Other) noexcept = delete;
	Sandbox2D& operator=(const Sandbox2D& _Other) = delete;
	Sandbox2D& operator=(Sandbox2D&& _Other) noexcept = delete;

protected:

private:

};

