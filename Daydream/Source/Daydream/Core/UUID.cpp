#include "DaydreamPCH.h"
#include "UUID.h"

namespace Daydream
{
	AssetHandle::AssetHandle()
		:uuid()
	{
	}

	AssetHandle::AssetHandle(const String& _string)
	{
		uuid = uuids::uuid::from_string(_string).value();
	}

	AssetHandle::AssetHandle(const uuids::uuid& _other)
	{
	}

	std::string AssetHandle::ToString() const
	{
		return uuids::to_string(uuid);
	}

	AssetHandle AssetHandle::StringToUUID(const String& _string) const
	{
		return AssetHandle(_string);
	}

	bool AssetHandle::IsValid() const
	{
		return !uuid.is_nil();
	}

	bool AssetHandle::operator==(const AssetHandle& _other) const
	{
		return uuid == _other.uuid;
	}

	bool AssetHandle::operator!=(const AssetHandle& _other) const
	{
		return uuid == _other.uuid;
	}

	bool AssetHandle::operator<(const AssetHandle& _other) const
	{
		return uuid < _other.uuid;
	}

	AssetHandle AssetHandle::Generate()
	{
		AssetHandle handle;
		handle.uuid = GenerateUUID();
		return handle;
	}

	uuids::uuid AssetHandle::GenerateUUID()
	{
		static uuids::uuid_system_generator gen;
		return gen(); 
	}
}
