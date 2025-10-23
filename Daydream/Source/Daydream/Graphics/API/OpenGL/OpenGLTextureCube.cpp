#include "DaydreamPCH.h"
#include "OpenGLTextureCube.h"

#include "glad/glad.h"
#include "OpenGLUtility.h"

namespace Daydream
{
	OpenGLTextureCube::OpenGLTextureCube(const TextureDesc& _desc, const Array<const void*>& _initialData)
	{
        width = _desc.width;
        height = _desc.height;

        textures.resize(6);

        glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &textureID);

        GLenum internalFormat = 0, dataFormat = 0;
        internalFormat = GraphicsUtility::OpenGL::ConvertRenderFormatToGLFormat(_desc.format);
        dataFormat = GraphicsUtility::OpenGL::ConvertRenderFormatToGLDataFormat(_desc.format);

        glTextureStorage2D(textureID, _desc.mipLevels, internalFormat, _desc.width, _desc.height);

        if (_initialData.size() == 6)
        {
            for (unsigned int i = 0; i < _initialData.size(); i++)
            {
                glTextureSubImage3D(
                    textureID,          // 텍스처 객체 ID
                    0,                  // 밉맵 레벨
                    0, 0, i,            // xoffset, yoffset, zoffset (i가 면의 인덱스)
                    _desc.width,        // 너비
                    _desc.height,       // 높이
                    1,                  // 깊이 (한 면이므로 1)
                    dataFormat,         // 픽셀 데이터 포맷
                    GraphicsUtility::OpenGL::ConvertRenderFormatToGLDataType(_desc.format),   // 픽셀 데이터 타입
                    _initialData[i] // 실제 픽셀 데이터
                );
            }
        }
	}
    OpenGLTextureCube::~OpenGLTextureCube()
    {
        glDeleteTextures(1, &textureID);
    }

    void OpenGLTextureCube::SetSampler(Shared<Sampler> _sampler)
    {
        textureSampler = static_cast<OpenGLSampler*>(_sampler.get());
    }

    void OpenGLTextureCube::GenerateMips()
    {
        glGenerateTextureMipmap(textureID);
    }
}

