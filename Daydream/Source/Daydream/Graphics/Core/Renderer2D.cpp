#include "DaydreamPCH.h"
//#include "Renderer2D.h"
//
//#include "RenderCommand.h"
//
//#include "Daydream/Graphics/Resources/Buffer.h"
//#include "Daydream/Graphics/Resources/Texture.h"
//
//
//namespace Daydream
//{
//	struct QuadVertex
//	{
//		glm::vec3 position;
//		glm::vec4 color;
//		glm::vec2 texCoord;
//	};
//
//	struct Renderer2DData
//	{
//		const UInt32 maxQuads = 10000;
//		const UInt32 maxVertices = maxQuads * 4;
//		const UInt32 maxIndices = maxQuads * 6;
//		const UInt32 maxTextureSlots= 32;
//
//		Shared<VertexBuffer> quadVertexBuffer;
//		Shared<IndexBuffer> quadIndexBuffer;
//		Shared<Texture2D> whiteTexture;
//
//		UInt32 quadIndexCount = 0;
//		QuadVertex* quadVertexBufferBase = nullptr;
//		QuadVertex* quadVertexBufferPtr = nullptr;
//	};
//
//	static Renderer2DData data;
//
//	void Renderer2D::Init()
//	{
//		data.quadVertexBuffer = VertexBuffer::CreateDynamic(data.maxVertices * sizeof(QuadVertex), sizeof(QuadVertex), 0, nullptr);
//
//		data.quadVertexBufferBase = new QuadVertex[data.maxVertices];
//		data.quadVertexBufferPtr = data.quadVertexBufferBase;
//		UInt32* quadIndices = new UInt32[data.maxIndices];
//		UInt32 offset = 0;
//		for (UInt32 i = 0; i < data.maxIndices; i += 6)
//		{
//			quadIndices[i + 0] = offset + 0;
//			quadIndices[i + 1] = offset + 1;
//			quadIndices[i + 2] = offset + 2;
//
//			quadIndices[i + 3] = offset + 2;
//			quadIndices[i + 4] = offset + 3;
//			quadIndices[i + 5] = offset + 0;
//
//			offset += 4;
//		}
//
//		data.quadIndexBuffer = IndexBuffer::Create(quadIndices, data.maxIndices);
//		delete[] quadIndices;
//	}
//	void Renderer2D::Shutdown()
//	{
//		delete[] data.quadVertexBufferBase;
//		data.quadVertexBuffer.reset();
//		data.quadIndexBuffer.reset();
//	}
//	void Renderer2D::BeginScene(const Camera& _camera)
//	{
//	}
//	void Renderer2D::EndScene()
//	{
//		data.quadVertexBuffer->SetData(data.quadVertexBufferBase, (data.quadVertexBufferPtr - data.quadVertexBufferBase) * (UInt32)sizeof(QuadVertex));
//		data.quadVertexBuffer->Bind();
//		data.quadIndexBuffer->Bind();
//		RenderCommand::DrawIndexed(data.quadIndexCount);
//	}
//	void Renderer2D::DrawQuad(const Vector3& _position, const Vector2& _size, const Vector4& _color)
//	{
//		Matrix4x4 trans = Matrix4x4::Translate(Matrix4x4(), _position);
//		DrawQuad(trans, _color);
//
//	}
//	void Renderer2D::DrawQuad(const Matrix4x4& _transform, const Vector4& _color)
//	{
//		constexpr UInt64 quadVertexCount = 4;
//		constexpr Vector4 Color = { 1.0f,0.0f, 0.0f, 1.0f };
//		constexpr Vector2 textureCoords[] = { { 0.0f, 1.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f } };
//		Vector4 quadVertexPositions[4];
//
//		quadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
//		quadVertexPositions[1] = { -0.5f, 0.5f, 0.0f, 1.0f };
//		quadVertexPositions[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
//		quadVertexPositions[3] = { 0.5f, -0.5f, 0.0f, 1.0f };
//
//		for (size_t i = 0; i < quadVertexCount; i++)
//		{
//			data.quadVertexBufferPtr->position = _transform * quadVertexPositions[i];
//			data.quadVertexBufferPtr->color = _color;
//			data.quadVertexBufferPtr->texCoord = textureCoords[i];
//			data.quadVertexBufferPtr++;
//		}
//		data.quadIndexCount += 6;
//	}
//}