#pragma once

#include "glad/glad.h"

#include "Daydream/Graphics/Resources/Sampler.h"

namespace Daydream::GraphicsUtility::OpenGL
{
	GLenum ConvertToShaderStageBit(ShaderType _type);
	GLenum ConvertToGLShaderType(ShaderType _type);

	UInt32 ConvertRenderFormatToGLFormat(RenderFormat _format);
	UInt32 ConvertRenderFormatToGLDataFormat(RenderFormat _format);

	GLint ConvertToGLFilter(FilterMode _filterMode);
	GLint ConvertToGLWrapMode(WrapMode _wrapMode);
}