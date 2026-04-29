#include "DaydreamPCH.h"
#include "OpenGLTexture.h"

#include "glad/glad.h"
#include "OpenGLUtility.h"
#include "Daydream/Graphics/Utility/ImageLoader.h"

namespace Daydream
{
	OpenGLGPUTexture::OpenGLGPUTexture(const TextureDesc& _desc)
		:GPUTexture(_desc)
	{
		GLenum type;

		switch (_desc.type)
		{
		case TextureType::Unknown:
		case TextureType::Texture1D: type = GL_TEXTURE_1D; break;
		case TextureType::Texture1DArray: type = GL_TEXTURE_1D_ARRAY; break;
		case TextureType::Texture2D:type = GL_TEXTURE_2D; break;
		case TextureType::Texture2DArray:type = GL_TEXTURE_2D_ARRAY; break;
		case TextureType::Texture2DMultisample:type = GL_TEXTURE_2D_MULTISAMPLE; break;
		case TextureType::TextureCube:type = GL_TEXTURE_CUBE_MAP; break;
		case TextureType::TextureCubeArray:type = GL_TEXTURE_CUBE_MAP_ARRAY; break;
		case TextureType::Texture3D:type = GL_TEXTURE_3D; break;
		default:
			break;
		}
		glCreateTextures(type, 1, &textureID);

		GLenum internalFormat = 0, dataFormat = 0;
		internalFormat = GraphicsUtility::OpenGL::ConvertRenderFormatToGLFormat(desc.format);
		dataFormat = GraphicsUtility::OpenGL::ConvertRenderFormatToGLDataFormat(desc.format);
		
		DAYDREAM_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");
		glTextureStorage2D(textureID, desc.mipLevels, internalFormat, desc.width, desc.height);

	}

	OpenGLGPUTexture::~OpenGLGPUTexture()
	{
		glDeleteTextures(1, &textureID);
	}

	OpenGLTexture2D::OpenGLTexture2D(const TextureDesc& _desc, const void* _initialData)
		:Texture2D(_desc)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &textureID);

		GLenum internalFormat = 0, dataFormat = 0;
		internalFormat = GraphicsUtility::OpenGL::ConvertRenderFormatToGLFormat(desc.format);
		dataFormat = GraphicsUtility::OpenGL::ConvertRenderFormatToGLDataFormat(desc.format);

		DAYDREAM_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

		// 텍스처 저장공간 할당 (데이터는 nullptr)
		glTextureStorage2D(textureID, 1, internalFormat, desc.width, desc.height);

		//// 필터링 설정
		//glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//glTextureParameteri(textureID, GL_TEXTURE_WRAP_R, GL_REPEAT);
		//glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_REPEAT);


		//Array<UInt8> redData(width * height * 4, 0);
		//for (int i = 0; i < width*height; i++) {
		//	redData[i * 4] = 255;     // R
		//	redData[i * 4 + 1] = 0;     // G  
		//	redData[i * 4 + 2] = 0;     // B
		//	redData[i * 4 + 3] = 255;   // A
		//}
		if (_initialData)
		{
			glTextureSubImage2D(textureID, 0, 0, 0, desc.width, desc.height, dataFormat, GraphicsUtility::OpenGL::ConvertRenderFormatToGLDataType(desc.format), _initialData);
		}

		//glTextureSubImage2D(textureID, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, redData.data());

		//glTextureSubImage2D(textureID, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &textureID);
	}
	void OpenGLTexture2D::SetSampler(Shared<Sampler> _sampler)
	{
		textureSampler = static_cast<OpenGLSampler*>(_sampler.get());
	}

}
