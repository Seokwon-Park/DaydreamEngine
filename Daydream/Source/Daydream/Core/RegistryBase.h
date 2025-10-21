#pragma once

namespace Daydream
{
	template<typename KeyType, typename ValueType>
	class RegistryBase
	{
	public:
		RegistryBase() = default;
		virtual ~RegistryBase() = default;

		inline Array<KeyType>& GetNameList() { return nameList; }

		void Register(const KeyType& _name, ValueType _value);

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

	template<typename KeyType, typename ValueType>
	inline void RegistryBase<KeyType, ValueType>::Register(const KeyType& _name, ValueType _value)
	{
		if (registry.find(_name) != registry.end())
		{
			DAYDREAM_CORE_INFO("Duplicate name!");
			return;
		}
		registry.insert({ _name,_value });
		nameList.push_back(_name);
		std::sort(nameList.begin(), nameList.end());
		
	}
}