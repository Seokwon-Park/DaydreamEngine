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
		std::string name = "";
		ShaderDataType type = ShaderDataType::None;
		UInt32 size = 0;
		UInt32 offset = 0;
		Bool normalized = false;

		BufferElement() {}

		BufferElement(ShaderDataType _type, const std::string& _name, Bool _normalized = false)
			: name(_name), type(_type), size(ShaderDataTypeSize(_type)), offset(0), normalized(_normalized)
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

			STEINS_CORE_ASSERT(false, "Unknown ShaderDataType!");
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
		inline const std::vector<BufferElement>& GetElements() const { return elements; }

		std::vector<BufferElement>::iterator begin() { return elements.begin(); }
		std::vector<BufferElement>::iterator end() { return elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return elements.end(); }
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
		std::vector<BufferElement> elements;
		UInt32 stride = 0;
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		const BufferLayout& GetLayout() const { return layout; }
		void SetLayout(const BufferLayout& _layout) { layout = _layout; }

		static Shared<VertexBuffer> Create(Float32* _vertices, UInt32 _size);
	protected:
		BufferLayout layout;
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual UInt32 GetCount() const = 0;

		static Shared<IndexBuffer> Create(UInt32* _vertices, UInt32 _size);
	};
}

