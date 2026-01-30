#pragma once
#include "Daydream/Enum/RendererEnums.h"
#include "Daydream/Graphics/Resources/Framebuffer.h"


namespace Daydream
{
	class RenderDevice;
	class VertexBuffer;
	class IndexBuffer;
	class Mesh;
	class Material;
	class Texture2D;
	class TextureCube;
	class ConstantBuffer;
	class PipelineState;

	class RenderContext
	{
	public:
		virtual ~RenderContext() = default;

		virtual void BeginCommandList() = 0;
		virtual void EndCommandList() {};
		virtual void SetViewport(UInt32 _x, UInt32 _y, UInt32 _width, UInt32 _height) = 0;
		virtual void SetClearColor(const Color& _color) = 0;
		virtual void Clear() = 0;
		virtual void DrawIndexed(UInt32 _indexCount, UInt32 _startIndex = 0, UInt32 _baseVertex = 0) = 0;

		virtual void BeginRenderPass(Shared<RenderPass> _renderPass, Shared<Framebuffer> _framebuffer) {};
		virtual void EndRenderPass(Shared<RenderPass> _renderPass) {};

		virtual void BindPipelineState(Shared<PipelineState> _pipelineState);

		virtual void BindVertexBuffer(Shared<VertexBuffer> _vertexBuffer) {};
		virtual void BindIndexBuffer(Shared<IndexBuffer> _indexBuffer) {};
		virtual void BindMesh(Shared<Mesh> _mesh) {};

		virtual void SetTexture2D(const String& _name, Shared<Texture2D> _texture) ;
		virtual void SetTextureCube(const String& _name, Shared<TextureCube> _textureCube) {};
		virtual void SetConstantBuffer(const String& _name, Shared<ConstantBuffer> _buffer) {};
		virtual void SetMaterial(Shared<Material> _material) {};

		virtual void Submit() {};

		virtual void CopyTexture2D(Shared<Texture2D> _src, Shared<Texture2D> _dst) {};
		virtual void CopyTextureToCubemapFace(Shared<TextureCube> _dstCubemap, UInt32 _faceIndex, Shared<Texture2D> _srcTexture2D, UInt32 _mipLevel = 0) {};

		virtual void GenerateMips(Shared<Texture> _texture) {}

	protected:
		Shared<PipelineState> currentPipelineState;
	private:
	};
}