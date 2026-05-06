#pragma once

#include "glad/glad.h"

#include "Daydream/Enum/RendererEnums.h"
#include "Daydream/Graphics/Resources/Buffer.h"
#include "Daydream/Graphics/Resources/Sampler.h"


namespace Daydream::GraphicsUtility::OpenGL
{
	GLenum ConvertToOpenGLMemoryUsage(MemoryUsage _usage);

	GLenum ConvertToOpenGLTextureTarget(TextureType _type);

	GLenum ConvertToShaderStageBit(ShaderType _type);
	GLenum ConvertToGLShaderType(ShaderType _type);

	UInt32 ConvertRenderFormatToGLFormat(RenderFormat _format);
	UInt32 ConvertRenderFormatToGLDataFormat(RenderFormat _format);
	UInt32 ConvertRenderFormatToGLDataType(RenderFormat _format);

	GLint ConvertToGLMinFilter(FilterMode _filterMode);
	GLint ConvertToGLMagFilter(FilterMode _filterMode);
	GLint ConvertToGLWrapMode(WrapMode _wrapMode);
}