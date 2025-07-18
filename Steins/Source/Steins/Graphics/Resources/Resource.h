#pragma once

namespace Steins
{
	class Resource
	{
	public:
		Resource() {};
		Resource(const std::string& _name);

		inline void SetName(const std::string& _name) { name = _name; }
		inline std::string GetName() const { return name; }
	private:
		std::string name;

	};
}