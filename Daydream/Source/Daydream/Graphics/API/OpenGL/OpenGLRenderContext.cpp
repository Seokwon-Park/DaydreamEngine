#include "DaydreamPCH.h"
#include "OpenGLRenderContext.h"

#include "OpenGLGraphicsPipelineState.h"
#include "OpenGLTexture.h"
#include "OpenGLTextureView.h"
#include "OpenGLTextureCube.h"
#include "OpenGLBuffer.h"
#include "OpenGLFramebuffer.h"
#include "OpenGLUtility.h"

#include "glad/glad.h"

namespace Daydream
{
	OpenGLRenderContext::OpenGLRenderContext()
	{
		glCreateFramebuffers(1, &framebufferID);
	}
	void OpenGLRenderContext::SetViewport(UInt32 _x, UInt32 _y, UInt32 _width, UInt32 _height)
	{
		glViewport(_x, _y, _width, _height);
	}
	void OpenGLRenderContext::DrawIndexed(UInt32 _indexCount, UInt32 _startIndex, UInt32 _baseVertex)
	{
		glDrawElementsBaseVertex(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, (void*)(_startIndex * sizeof(uint32_t)), _baseVertex);
	}
	void OpenGLRenderContext::BeginRendering(const RenderingInfo& _renderingInfo)
	{
		std::vector<GLenum> drawBuffers;
		for (UInt64 i = 0; i < _renderingInfo.colorAttachments.size(); i++)
		{
			const AttachmentDesc& attachmentDesc = _renderingInfo.colorAttachments[i];
			Shared<OpenGLTextureView> openGLTextureView = SharedCast<OpenGLTextureView>(attachmentDesc.view);
			Shared<OpenGLGPUTexture> texture = SharedCast<OpenGLGPUTexture>(attachmentDesc.view->GetOriginTexture());

			const TextureDesc& textureDesc = texture->GetDesc();

			glNamedFramebufferTexture(
				framebufferID,
				(GLenum)(GL_COLOR_ATTACHMENT0 + i),
				openGLTextureView->GetTextureViewID(),
				0
			);

			drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + i);
		}

		// Depth attachment 연결
		if (_renderingInfo.depthAttachment.view != nullptr)
		{
			Shared<OpenGLTextureView> openGLTextureView = SharedCast<OpenGLTextureView>(_renderingInfo.depthAttachment.view);

			glNamedFramebufferTexture(framebufferID,
				GL_DEPTH_STENCIL_ATTACHMENT,
				openGLTextureView->GetTextureViewID(),
				0);
		}


		// Draw buffers 설정 (multiple render targets용)
		glNamedFramebufferDrawBuffers(framebufferID,
			(UInt32)drawBuffers.size(),
			drawBuffers.data());


		glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
		DAYDREAM_CORE_ASSERT(glCheckNamedFramebufferStatus(framebufferID, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		for (UInt64 i = 0; i < _renderingInfo.colorAttachments.size(); i++)
		{
			const AttachmentDesc& attachmentDesc = _renderingInfo.colorAttachments[i];
			ClearValue rtvClearValue = attachmentDesc.clearValue;

			if (attachmentDesc.loadOp == AttachmentLoadOp::Clear)
			{
				glClearNamedFramebufferfv(framebufferID, GL_COLOR, i, rtvClearValue.colorClearValue.color);
			}
		}

		if (_renderingInfo.depthAttachment.view != nullptr)
		{
			ClearValue dsvClearValue = _renderingInfo.depthAttachment.clearValue;
			Shared<OpenGLTextureView> openGLTextureView = SharedCast<OpenGLTextureView>(_renderingInfo.depthAttachment.view);

			float depthValue = _renderingInfo.depthAttachment.clearValue.depthClearValue;
			int stencilValue = _renderingInfo.depthAttachment.clearValue.stencilClearValue;
			if (_renderingInfo.depthAttachment.loadOp == AttachmentLoadOp::Clear)
			{
				glClearNamedFramebufferfi(framebufferID, GL_DEPTH_STENCIL, 0, depthValue, stencilValue);
			}
		}

		SetViewport(
			_renderingInfo.renderArea.x,
			_renderingInfo.renderArea.y,
			_renderingInfo.renderArea.width,
			_renderingInfo.renderArea.height
		);
		////glNamedFramebufferDrawBuffer(framebufferID, GL_COLOR_ATTACHMENT0);


	}
	void OpenGLRenderContext::EndRendering(const RenderingInfo& _renderingInfo)
	{
		for (UInt64 i = 0; i < _renderingInfo.colorAttachments.size(); i++)
		{
			glNamedFramebufferTexture(framebufferID,
				GL_COLOR_ATTACHMENT0 + i,
				0,
				0);
		}
		glNamedFramebufferTexture(framebufferID, GL_DEPTH_STENCIL_ATTACHMENT, 0, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	//void OpenGLRenderContext::BeginRenderPass(Shared<RenderPass> _renderPass, Shared<Framebuffer> _framebuffer)
	//{
	//	OpenGLFramebuffer* currentFramebuffer = Cast<OpenGLFramebuffer*>(_framebuffer.get());
	//	//if (currentFramebuffer->IsSwapchainBuffer() == true)
	//	//{
	//	//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//	//	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	//	//	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//	//}
	//	//else
	//	//{
	//	glBindFramebuffer(GL_FRAMEBUFFER, currentFramebuffer->GetFramebufferID());
	//	SetViewport(0, 0, currentFramebuffer->GetWidth(), currentFramebuffer->GetHeight());
	//	if (currentFramebuffer->HasDepthAttachment())
	//	{
	//		glEnable(GL_DEPTH_TEST);
	//		glDepthFunc(GL_LESS);
	//		glClear(GL_DEPTH_BUFFER_BIT);
	//	}
	//	else
	//	{
	//		glDisable(GL_DEPTH_TEST);
	//	}

	//	Color clearColor = _renderPass->GetClearColor();
	//	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	//	glClear(GL_COLOR_BUFFER_BIT);
	//	//}
	//}
	//void OpenGLRenderContext::EndRenderPass(Shared<RenderPass> _renderPass)
	//{
	//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//}
	void OpenGLRenderContext::BindPipelineState(Shared<GraphicsPipelineState> _pipelineState)
	{
		RenderContext::BindPipelineState(_pipelineState);
		OpenGLGraphicsPipelineState* pso = Cast<OpenGLGraphicsPipelineState*>(currentGraphicsPipelineState.get());
		pso->BindPipelineState();
	}
	void OpenGLRenderContext::BindVertexBuffer(Shared<VertexBuffer> _vertexBuffer)
	{
		OpenGLGPUBuffer* vertexBuffer = Cast<OpenGLGPUBuffer*>(_vertexBuffer->GetGPUBufferPtr());

		GLint currentVAO = 0;
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentVAO);
		glVertexArrayVertexBuffer(currentVAO, 0, vertexBuffer->GetBufferID(), 0, _vertexBuffer->GetStride());
	}
	void OpenGLRenderContext::BindIndexBuffer(Shared<IndexBuffer> _indexBuffer)
	{
		OpenGLGPUBuffer* indexBuffer = Cast<OpenGLGPUBuffer*>(_indexBuffer->GetGPUBufferPtr());

		GLint currentVAO = 0; // 결과를 저장할 변수
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentVAO);
		glVertexArrayElementBuffer(currentVAO, indexBuffer->GetBufferID());
	}
	void OpenGLRenderContext::BindShaderResourceView(const String& _name, Shared<TextureView> _textureView, Shared<Sampler> _sampler)
	{
		const ShaderReflectionData* bindingInfo = currentGraphicsPipelineState->GetBindingInfo(_name);
		if (bindingInfo == nullptr) return;

		OpenGLTextureView* glView = Cast<OpenGLTextureView*>(_textureView.get());
		OpenGLSampler* glSampler= Cast<OpenGLSampler*>(_sampler.get());
		glBindTextureUnit(bindingInfo->binding, glView->GetTextureViewID());
		glBindSampler(bindingInfo->binding, glSampler->GetSamplerID());

	}
	/*void OpenGLRenderContext::SetTexture2D(const String& _name, Shared<Texture2D> _texture)
	{
		RenderContext::SetTexture2D(_name, _texture);

		const ShaderReflectionData* bindingInfo = activePipelineState->GetBindingInfo(_name);
		if (bindingInfo == nullptr) return;

		OpenGLTexture2D* glTexture = Cast<OpenGLTexture2D*>(_texture.get());
		glBindTextureUnit(bindingInfo->binding, glTexture->GetTextureID());
		glBindSampler(bindingInfo->binding, glTexture->GetSamplerID());
	}
	void OpenGLRenderContext::SetTextureCube(const String& _name, Shared<TextureCube> _textureCube)
	{
		const ShaderReflectionData* bindingInfo = activePipelineState->GetBindingInfo(_name);
		if (bindingInfo == nullptr) return;

		OpenGLTextureCube* glTexture = Cast<OpenGLTextureCube*>(_textureCube.get());
		glBindTextureUnit(bindingInfo->binding, glTexture->GetTextureID());
		glBindSampler(bindingInfo->binding, glTexture->GetSamplerID());
	}*/
	void OpenGLRenderContext::SetConstantBuffer(const String& _name, Shared<ConstantBuffer> _buffer)
	{
		const ShaderReflectionData* bindingInfo = currentGraphicsPipelineState->GetBindingInfo(_name);
		if (bindingInfo == nullptr) return;

		OpenGLGPUBuffer* constantBuffer = Cast<OpenGLGPUBuffer*>(_buffer->GetGPUBufferPtr());
		glBindBufferBase(GL_UNIFORM_BUFFER, bindingInfo->binding, constantBuffer->GetBufferID());
	}

	void OpenGLRenderContext::CopyBuffer(Shared<GPUBuffer> _src, Shared<GPUBuffer> _dst, UInt32 _copySize)
	{
		OpenGLGPUBuffer* src = Cast<OpenGLGPUBuffer*>(_src.get());
		OpenGLGPUBuffer* dst = Cast<OpenGLGPUBuffer*>(_dst.get());

		// 소스 버퍼ID, 목적지 버퍼ID, 소스 오프셋, 목적지 오프셋, 복사할 크기
		glCopyNamedBufferSubData(src->GetBufferID(), dst->GetBufferID(), 0, 0, _copySize);
	}

	void Daydream::OpenGLRenderContext::CopyBufferToTexture(Shared<GPUBuffer> _src, Shared<GPUTexture> _dst)
	{
		Shared<OpenGLGPUBuffer> srcBuffer = SharedCast<OpenGLGPUBuffer>(_src);
		Shared<OpenGLGPUTexture> dstTexture = SharedCast<OpenGLGPUTexture>(_dst);

		GLuint bufferID = srcBuffer->GetBufferID();
		GLuint textureID = dstTexture->GetTextureID();

		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, bufferID);

		GLenum pixelFormat = GraphicsUtility::OpenGL::ConvertRenderFormatToGLDataFormat(dstTexture->GetFormat());
		GLenum pixelType = GraphicsUtility::OpenGL::ConvertRenderFormatToGLDataType(dstTexture->GetFormat());

		glTextureSubImage2D(
			textureID,
			0,           // 밉맵 레벨
			0,           // X 오프셋
			0,           // Y 오프셋
			_dst->GetWidth(),      // 복사할 너비
			_dst->GetHeight(),     // 복사할 높이
			pixelFormat, // 픽셀 포맷
			pixelType,   // 데이터 타입
			nullptr      // PBO 오프셋
		);

		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	}

	void OpenGLRenderContext::CopyTexture2D(Shared<Texture2D> _src, Shared<Texture2D> _dst)
	{
		Shared<OpenGLGPUTexture> src = SharedCast<OpenGLGPUTexture>(_src->GetGPUTexture());
		Shared<OpenGLGPUTexture> dst = SharedCast<OpenGLGPUTexture>(_dst->GetGPUTexture());
		glCopyImageSubData(
			src->GetTextureID(),          // 원본 텍스처 이름
			GL_TEXTURE_2D,       // 원본 텍스처 타입
			0,                   // 원본 밉맵 레벨
			0, 0, 0,             // 원본 오프셋 (x, y, z)
			dst->GetTextureID(),          // 대상 텍스처 이름
			GL_TEXTURE_2D,       // 대상 텍스처 타입
			0,                   // 대상 밉맵 레벨
			0, 0, 0,             // 대상 오프셋 (x, y, z)
			_src->GetWidth(),               // 복사할 너비
			_src->GetHeight(),              // 복사할 높이
			1                    // 복사할 깊이 (2D 텍스처는 1)
		);
	}
	void Daydream::OpenGLRenderContext::CopyTextureToCubemapFace(Shared<Texture2D> _srcTexture2D, Shared<TextureCube> _dstCubemap, UInt32 _faceIndex, UInt32 _mipLevel)
	{
		OpenGLGPUTexture* src = Cast<OpenGLGPUTexture*>(_srcTexture2D->GetGPUTexturePtr());
		OpenGLGPUTexture* dst = Cast<OpenGLGPUTexture*>(_dstCubemap->GetGPUTexturePtr());
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
		Shared<OpenGLTextureView> src = SharedCast<OpenGLTextureView>(_texture->GetDefaultSRV());
		glGenerateTextureMipmap(src->GetTextureViewID());
	}
}