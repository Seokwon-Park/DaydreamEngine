#pragma once

#include "glad/glad.h"

namespace Daydream::GraphicsUtility::OpenGL
{
	GLenum ConvertToShaderStageBit(ShaderType _type);
	GLenum ConvertToGLShaderType(ShaderType _type);

	UInt32 ConvertRenderFormatToGLFormat(RenderFormat _format);
	UInt32 ConvertRenderFormatToGLDataFormat(RenderFormat _format);
}