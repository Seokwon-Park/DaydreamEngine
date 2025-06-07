#pragma once

// Ό³Έν :
class GameObject
{
public:
	// constrcuter destructer
	GameObject();
	~GameObject();

	inline void SetName(const std::string& _name) { name = _name; }

protected:

private:
	std::string name;
};

