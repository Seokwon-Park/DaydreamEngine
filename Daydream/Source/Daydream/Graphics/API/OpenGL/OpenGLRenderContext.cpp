#include "DaydreamPCH.h"
#include "OpenGLRenderContext.h"

#include "OpenGLPipelineState.h"
#include "OpenGLTexture.h"
#include "OpenGLTextureCube.h"
#include "OpenGLBuffer.h"
#include "OpenGLFramebuffer.h"

#include "glad/glad.h"

namespace Daydream
{
	void OpenGLRenderContext::SetViewport(UInt32 _x, UInt32 _y, UInt32 _width, UInt32 _height)
	{
		glViewport(_x, _y, _width, _height);
	}
	void OpenGLRenderContext::SetClearColor(const Color& _color)
	{
		//glClearColor(_color.r, _color.g, _color.b, _color.a);
	}
	void OpenGLRenderContext::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void OpenGLRenderContext::DrawIndexed(UInt32 _indexCount, UInt32 _startIndex, UInt32 _baseVertex)
	{
		glDrawElementsBaseVertex(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, (void*)(_startIndex * sizeof(uint32_t)), _baseVertex);
	}
	void OpenGLRenderContext::BeginRenderPass(Shared<RenderPass> _renderPass, Shared<Framebuffer> _framebuffer)
	{
		OpenGLFramebuffer* currentFramebuffer = Cast<OpenGLFramebuffer*>(_framebuffer.get());
		if (currentFramebuffer->IsBackbuffer() == true)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
			//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		else
		{
			glBindFramebuffer(GL_FRAMEBUFFER, currentFramebuffer->GetFramebufferID());
			SetViewport(0, 0, currentFramebuffer->GetWidth(), currentFramebuffer->GetHeight());
			if (currentFramebuffer->HasDepthAttachment())
			{
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_LESS);
				glClear(GL_DEPTH_BUFFER_BIT);
			}
			else
			{
				glDisable(GL_DEPTH_TEST);
			}

			Color clearColor = _renderPass->GetClearColor();
			glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
			glClear(GL_COLOR_BUFFER_BIT);
		}
	}
	void OpenGLRenderContext::EndRenderPass(Shared<RenderPass> _renderPass)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void OpenGLRenderContext::BindPipelineState(Shared<PipelineState> _pipelineState)
	{
		RenderContext::BindPipelineState(_pipelineState);
		OpenGLPipelineState* pso = Cast<OpenGLPipelineState*>(currentPipelineState.get());
		pso->BindPipelineState();
	}
	void OpenGLRenderContext::BindVertexBuffer(Shared<VertexBuffer> _vertexBuffer)
	{
		OpenGLVertexBuffer* vertexBuffer = Cast<OpenGLVertexBuffer*>(_vertexBuffer.get());

		GLint currentVAO = 0;
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentVAO);
		glVertexArrayVertexBuffer(currentVAO, 0, vertexBuffer->GetBufferID(), 0, vertexBuffer->GetStride());
	}
	void OpenGLRenderContext::BindIndexBuffer(Shared<IndexBuffer> _indexBuffer)
	{
		OpenGLIndexBuffer* indexBuffer = Cast<OpenGLIndexBuffer*>(_indexBuffer.get());

		GLint currentVAO = 0; // 결과를 저장할 변수
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentVAO);
		glVertexArrayElementBuffer(currentVAO, indexBuffer->GetBufferID());
	}
	void OpenGLRenderContext::SetTexture2D(const String& _name, Shared<Texture2D> _texture)
	{
		RenderContext::SetTexture2D(_name, _texture);

		const ShaderReflectionData* bindingInfo = currentPipelineState->GetBindingInfo(_name);
		if (bindingInfo == nullptr) return;

		OpenGLTexture2D* glTexture = Cast<OpenGLTexture2D*>(_texture.get());
		glBindTextureUnit(bindingInfo->binding, glTexture->GetTextureID());
		glBindSampler(bindingInfo->binding, glTexture->GetSamplerID());
	}
	void OpenGLRenderContext::SetTextureCube(const String& _name, Shared<TextureCube> _textureCube)
	{
		const ShaderReflectionData* bindingInfo = currentPipelineState->GetBindingInfo(_name);
		if (bindingInfo == nullptr) return;

		OpenGLTextureCube* glTexture = Cast<OpenGLTextureCube*>(_textureCube.get());
		glBindTextureUnit(bindingInfo->binding, glTexture->GetTextureID());
		glBindSampler(bindingInfo->binding, glTexture->GetSamplerID());
	}
	void OpenGLRenderContext::SetConstantBuffer(const String& _name, Shared<ConstantBuffer> _buffer)
	{
		const ShaderReflectionData* bindingInfo = currentPipelineState->GetBindingInfo(_name);
		if (bindingInfo == nullptr) return;

		OpenGLConstantBuffer* glBuffer = Cast<OpenGLConstantBuffer*>(_buffer.get());
		glBindBufferBase(GL_UNIFORM_BUFFER, bindingInfo->binding, glBuffer->GetBufferID());
	}

	void OpenGLRenderContext::CopyTexture2D(Shared<Texture2D> _src, Shared<Texture2D> _dst)
	{
		OpenGLTexture2D* src = static_cast<OpenGLTexture2D*>(_src.get());
		OpenGLTexture2D* dst = static_cast<OpenGLTexture2D*>(_dst.get());
		glCopyImageSubData(
			static_cast<UInt32>(reinterpret_cast<UInt64>(_src->GetNativeHandle())),          // 원본 텍스처 이름
			GL_TEXTURE_2D,       // 원본 텍스처 타입
			0,                   // 원본 밉맵 레벨
			0, 0, 0,             // 원본 오프셋 (x, y, z)
			static_cast<UInt32>(reinterpret_cast<UInt64>(_dst->GetNativeHandle())),          // 대상 텍스처 이름
			GL_TEXTURE_2D,       // 대상 텍스처 타입
			0,                   // 대상 밉맵 레벨
			0, 0, 0,             // 대상 오프셋 (x, y, z)
			_src->GetWidth(),               // 복사할 너비
			_src->GetHeight(),              // 복사할 높이
			1                    // 복사할 깊이 (2D 텍스처는 1)
		);
	}
	void OpenGLRenderContext::CopyTextureToCubemapFace(Shared<TextureCube> _dstCubemap, UInt32 _faceIndex, Shared<Texture2D> _srcTexture2D, UInt32 _mipLevel)
	{
		OpenGLTexture2D* src = Cast<OpenGLTexture2D*>(_srcTexture2D.get());
		OpenGLTextureCube* dst = Cast<OpenGLTextureCube*>(_dstCubemap.get());
		glCopyImageSubData(
			src->GetTextureID(),      // 원본 텍스처 핸들
			GL_TEXTURE_2D,        // 원본 타겟 타입
			0,                    // 원본 밉 레벨
			0, 0, 0,              // 원본 좌표 (x, y, z)
			dst->GetTextureID(),         // 대상 텍스처 핸들
			GL_TEXTURE_CUBE_MAP,  // 대상 타겟 타입
			_mipLevel,             // 대상 밉 레벨
			0, 0, _faceIndex,      // 대상 좌표 (x, y, layer) - faceIndex가 레이어를 지정!
			src->GetWidth(), src->GetHeight(), 1      // 복사할 크기
		);
	}
	void OpenGLRenderContext::GenerateMips(Shared<Texture> _texture)
	{
		UInt32 textureID = 0;
		switch (_texture->GetType())
		{
		case TextureType::Texture2D:
			textureID = Cast<OpenGLTexture2D*>(_texture.get())->GetTextureID();
			break;
		case TextureType::TextureCube:
			textureID = Cast<OpenGLTextureCube*>(_texture.get())->GetTextureID();
			break;
		}
		glGenerateTextureMipmap(textureID);
	}
}