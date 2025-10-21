#pragma once

#include <Base/Base.h>

#define REFLECT_START() \
    public: \
    Array<Field> GetFields() override { \
        Array<Field> fields;

#define ADD_PROPERTY(type, name) \
        fields.push_back({ #name, type, &name });

#define ADD_PTR_PROPERTY(type, name) \
        fields.push_back({ #name, type, name });

#define REFLECT_END() \
        return fields; \
    }

namespace Daydream
{
    enum class FieldType
    {
        Float,
        Vector3,
        Transform,
        String,
        Texture,
        Material,
        Light,
        ModelPtr,
        PBRValue,
        FieldTypeMax
    };

    struct Field
    {
        String name;    
        FieldType type;
        void* data; 
    };

    class IReflectable
    {
    public:
        virtual ~IReflectable() = default;
        virtual Array<Field> GetFields() = 0;
    };
}