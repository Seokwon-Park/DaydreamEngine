#pragma once

#include <cstdint>

struct EntityHandle
{
	UInt32 id;

	// 비트 마스크 및 시프트 상수화
	static const UInt32 indexBits = 22;
	static const UInt32 generationBits = 10;
	static const UInt32 indexMask = (1 << indexBits) - 1;
	static const UInt32 generationMask = (1 << generationBits) - 1;

	static const UInt32 invalidId = 0;

	// 기본 생성자는 유효하지 않은 ID로 초기화
	EntityHandle() : id(invalidId) {}

	// 명시적 ID 생성
	explicit EntityHandle(UInt32 _id) : id(_id) {}

	UInt32 GetIndex() const { return id & indexMask; }
	UInt32 GetGeneration() const { return (id >> indexBits) & generationMask; }
	UInt32 GetID() const { return id; }
	bool IsValid() const { return id != invalidId; }

	static EntityHandle Create(UInt32 _index, UInt32 _generation)
	{
		UInt32 newId = ((_generation & generationMask) << indexBits) | (_index & indexMask);
		// 0은 Invalid ID로 사용하므로, 만약 조합된 ID가 0이면 
		// (보통 index 0, generation 0일 때) 유효하지 않은 핸들을 반환
		return newId == invalidId ? EntityHandle() : EntityHandle(newId);
	}

	// 비교 연산자
	bool operator==(const EntityHandle& _other) const { return id == _other.id; }
	bool operator!=(const EntityHandle& _other) const { return id != _other.id; }
};