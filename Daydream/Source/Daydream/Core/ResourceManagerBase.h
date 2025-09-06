#pragma once

namespace Daydream
{
	template<typename ResourceType>
	class ResourceManagerBase
	{
	public:
		ResourceManagerBase() = default;
		virtual ~ResourceManagerBase() = default;

		Shared<ResourceType> Get(const String& _name)
		{
			if (resourceCache.find(_name) != resourceCache.end())
			{
				return resourceCache[_name];
			}
			DAYDREAM_CORE_WARN("Failed to find resource! Name = {}", _name);
			return nullptr;
		}

	protected:
		HashMap<String, Shared<ResourceType>> resourceCache;
	};
}