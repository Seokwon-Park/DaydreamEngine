#pragma once

#include "Component.h" // 모든 컴포넌트의 기반 클래스

namespace Daydream
{
    class ComponentRegistry
    {
    public:
        // 컴포넌트 이름으로 컴포넌트를 생성하는 함수 (팩토리)
        using ComponentCreateFunc = std::function<Shared<Component>()> ;

        // 싱글톤 인스턴스를 가져오는 함수
        static ComponentRegistry* Get()
        {
            static ComponentRegistry instance;
            return &instance;
        }

        // 컴포넌트 타입을 시스템에 등록하는 함수
        template<typename ComponentType>
        void RegisterComponent(const String& name)
        {
            static_assert(std::is_base_of<Component, ComponentType>::value, "Template argument must inherit from Component!");
            componentFunctionMap[name] = [](){ return MakeShared<ComponentType>(); };
        }

        // 등록된 모든 컴포넌트의 팩토리 맵을 반환
        const HashMap<String, ComponentCreateFunc>& GetFactoryMap() const
        {
            return componentFunctionMap;
        }

    private:
        HashMap<String, ComponentCreateFunc> componentFunctionMap;
    };
}