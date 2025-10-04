#pragma once

namespace Daydream
{
	template<typename KeyType, typename ValueType>
	class RegistryBase
	{
	public:
		RegistryBase() = default;
		virtual ~RegistryBase() = default;

		Array<KeyType>& GetNameList() { return nameList; }

		void Add(const KeyType& _name, ValueType _value)
		{
			if (registry.find(_name) != registry.end())
			{
				DAYDREAM_CORE_INFO("Duplicate name!");
				return;
			}
			registry.insert({ _name,_value });
			nameList.push_back(_name);
		}

		ValueType Get(const KeyType& _name)
		{
			if (registry.find(_name) != registry.end())
			{
				return registry[_name];
			}
			DAYDREAM_CORE_WARN("Failed to find resource! Name = {}", _name);
			return nullptr;
		}

	protected:
		HashMap<KeyType, ValueType> registry;
		Array<KeyType> nameList;
	};
}