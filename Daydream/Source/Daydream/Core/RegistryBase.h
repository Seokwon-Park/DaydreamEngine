#pragma once

namespace Daydream
{
	template<typename ValueType>
	class RegistryBase
	{
	public:
		RegistryBase() = default;
		virtual ~RegistryBase() = default;

		Shared<ValueType> Get(const String& _name)
		{
			if (registry.find(_name) != registry.end())
			{
				return registry[_name];
			}
			DAYDREAM_CORE_WARN("Failed to find resource! Name = {}", _name);
			return nullptr;
		}

	protected:
		HashMap<String, Shared<ValueType>> registry;
	};
}