#pragma once

#include "Daydream/Enum/RendererEnums.h"

namespace Daydream
{
	static UInt32 ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:    return 4;
		case ShaderDataType::Float2:   return 4 * 2;
		case ShaderDataType::Float3:   return 4 * 3;
		case ShaderDataType::Float4:   return 4 * 4;
		case ShaderDataType::Int:      return 4;
		case ShaderDataType::Int2:     return 4 * 2;
		case ShaderDataType::Int3:     return 4 * 3;
		case ShaderDataType::Int4:     return 4 * 4;
		case ShaderDataType::Mat3x3:   return 4 * 3 * 3;
		case ShaderDataType::Mat4x4:   return 4 * 4 * 4;
		case ShaderDataType::Bool:     return 1;
		}

		DAYDREAM_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	struct BufferElement
	{
		std::string name = "";
		std::string semantic = "";
		ShaderDataType type = ShaderDataType::None;
		UInt32 size = 0;
		UInt32 offset = 0;
		Bool normalized = false;

		BufferElement() {}

		BufferElement(ShaderDataType _type, const std::string& _name, std::string _semantic, Bool _normalized = false)
			: name(_name), semantic(_semantic), type(_type), size(ShaderDataTypeSize(_type)), offset(0), normalized(_normalized)
		{
		}

		UInt32 GetComponentCount() const
		{
			switch (type)
			{
			case ShaderDataType::Float:   return 1;
			case ShaderDataType::Float2:  return 2;
			case ShaderDataType::Float3:  return 3;
			case ShaderDataType::Float4:  return 4;
			case ShaderDataType::Int:     return 1;
			case ShaderDataType::Int2:    return 2;
			case ShaderDataType::Int3:    return 3;
			case ShaderDataType::Int4:    return 4;
			case ShaderDataType::Mat3x3:    return 3 * 3;
			case ShaderDataType::Mat4x4:    return 4 * 4;
			case ShaderDataType::Bool:    return 1;
			}

			DAYDREAM_CORE_ASSERT(false, "Unknown ShaderDataType!");
			return 0;
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout() = default;
		BufferLayout(const BufferLayout&) = default;
		BufferLayout& operator=(const BufferLayout&) = default;

		BufferLayout(const std::initializer_list<BufferElement>& _elements)
			: elements(_elements)
		{
			CalculateOffsetsAndStride();
		}

		inline UInt32 GetStride() const { return stride; }
		inline const Array<BufferElement>& GetElements() const { return elements; }

		Array<BufferElement>::iterator begin() { return elements.begin(); }
		Array<BufferElement>::iterator end() { return elements.end(); }
		Array<BufferElement>::const_iterator begin() const { return elements.begin(); }
		Array<BufferElement>::const_iterator end() const { return elements.end(); }
	private:
		void CalculateOffsetsAndStride()
		{
			UInt32 offset = 0;
			stride = 0;
			for (auto& element : elements)
			{
				element.offset = offset;
				offset += element.size;
				stride += element.size;
			}
		}
	private:
		Array<BufferElement> elements;
		UInt32 stride = 0;
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetData(const void* _data, UInt32 _dataSize) = 0;

		static Shared<VertexBuffer> CreateDynamic(UInt32 _size, UInt32 _stride, UInt32 _initialDataSize = 0, const void* _initialData = nullptr);
		static Shared<VertexBuffer> CreateStatic(UInt32 _size, UInt32 _stride, const void* _initialData);
	protected:
		BufferUsage usage;
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		UInt32 GetCount() const { return indexCount; }

		static Shared<IndexBuffer> Create(UInt32* _indices, UInt32 _count);
	protected:
		UInt32 indexCount;

	};

	class ConstantBuffer
	{
	public:
		virtual ~ConstantBuffer() {}

		//virtual void Bind(UInt32 _slot) const = 0;

		virtual void Update(const void* _data, UInt32 _size) = 0;

		virtual void* GetNativeHandle() = 0;
		UInt32 GetSize() { return size; }

		static Shared<ConstantBuffer> Create(UInt32 _size);
	protected:
		void* data;
		UInt32 size;

	};

}

