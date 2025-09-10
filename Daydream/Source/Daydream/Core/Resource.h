#pragma once

namespace Daydream
{
	class Resource
	{
	public:
		Resource() {};
		Resource(const String& _name);

		inline void SetName(const String& _name) { name = _name; }
		inline String GetName() const { return name; }
	private:
		String name;

	};
}