#pragma once


namespace Steins
{
	enum class ShaderDataType
	{
		None = 0, 
		Float,
		Float2,
		Float3, 
		Float4, 
		Int, 
		Int2,
		Int3,
		Int4,
		Mat3x3,
		Mat4x4,
		Bool
	};

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

		STEINS_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	struct BufferElement
	{
		String name = "";
		ShaderDataType type = ShaderDataType::None;
		UInt32 size = 0;
		UInt32 offset = 0;
		Bool normalized = false;

		BufferElement() {}

		BufferElement(ShaderDataType _type, const String& _name, Bool _normalized = false)
			: name(_name), type(_type), size(ShaderDataTypeSize(_type)), offset(0), normalized(_normalized)
		{}

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

			STEINS_CORE_ASSERT(false, "Unknown ShaderDataType!");
			return 0;
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout() {}

		BufferLayout(const std::initializer_list<BufferElement>& _elements)
			: elements(_elements)
		{
			CalculateOffsetsAndStride();
		}

		inline UInt32 GetStride() const { return stride; }
		inline const Array<BufferElement>& GetElements() const { return elements; }

		Array<BufferElement>::Iterator begin() { return elements.begin(); }
		Array<BufferElement>::Iterator end() { return elements.end(); }
		Array<BufferElement>::ConstIterator begin() const { return elements.begin(); }
		Array<BufferElement>::ConstIterator end() const { return elements.end(); }
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

		static VertexBuffer* Create(Float32* _vertices, UInt32 _size);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static IndexBuffer* Create(UInt32* _vertices, UInt32 _size);
	};
}

