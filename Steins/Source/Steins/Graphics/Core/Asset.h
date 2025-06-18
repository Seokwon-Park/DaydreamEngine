#pragma once

namespace Steins
{
	class Asset
	{
	public:
		Asset() {};
		Asset(const std::string& _name);

		inline void SetName(const std::string& _name) { name = _name; }
		inline std::string GetName() const { return name; }
	private:
		std::string name;

	};
}