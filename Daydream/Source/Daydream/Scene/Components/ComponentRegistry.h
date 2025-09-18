#pragma once

#include "Component.h" // ��� ������Ʈ�� ��� Ŭ����

namespace Daydream
{
    class ComponentRegistry
    {
    public:
        // ������Ʈ �̸����� ������Ʈ�� �����ϴ� �Լ� (���丮)
        using ComponentCreateFunc = std::function<Shared<Component>()> ;

        // �̱��� �ν��Ͻ��� �������� �Լ�
        static ComponentRegistry* Get()
        {
            static ComponentRegistry instance;
            return &instance;
        }

        // ������Ʈ Ÿ���� �ý��ۿ� ����ϴ� �Լ�
        template<typename ComponentType>
        void RegisterComponent(const String& name)
        {
            static_assert(std::is_base_of<Component, ComponentType>::value, "Template argument must inherit from Component!");
            componentFunctionMap[name] = [](){ return MakeShared<ComponentType>(); };
        }

        // ��ϵ� ��� ������Ʈ�� ���丮 ���� ��ȯ
        const HashMap<String, ComponentCreateFunc>& GetFactoryMap() const
        {
            return componentFunctionMap;
        }

    private:
        HashMap<String, ComponentCreateFunc> componentFunctionMap;
    };
}