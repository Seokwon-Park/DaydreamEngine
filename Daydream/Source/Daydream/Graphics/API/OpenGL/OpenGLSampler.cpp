#include "DaydreamPCH.h"
#include "OpenGLSampler.h"

#include "glad/glad.h"
#include "OpenGLUtility.h"

namespace Daydream
{
	OpenGLSampler::OpenGLSampler(const SamplerDesc& _desc)
	{
		glCreateSamplers(1, &samplerID);

		glSamplerParameteri(samplerID, GL_TEXTURE_MIN_FILTER, GraphicsUtility::OpenGL::ConvertToGLFilter(_desc.minFilter));
		glSamplerParameteri(samplerID, GL_TEXTURE_MAG_FILTER, GraphicsUtility::OpenGL::ConvertToGLFilter(_desc.magFilter));

		glSamplerParameteri(samplerID, GL_TEXTURE_WRAP_S, GraphicsUtility::OpenGL::ConvertToGLWrapMode(_desc.wrapU));
		glSamplerParameteri(samplerID, GL_TEXTURE_WRAP_T, GraphicsUtility::OpenGL::ConvertToGLWrapMode(_desc.wrapV));
		glSamplerParameteri(samplerID, GL_TEXTURE_WRAP_R, GraphicsUtility::OpenGL::ConvertToGLWrapMode(_desc.wrapW));
	}
	OpenGLSampler::~OpenGLSampler()
	{
		glDeleteSamplers(1, &samplerID);
	}
}
