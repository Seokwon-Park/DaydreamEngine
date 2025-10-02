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

        glTextureStorage2D(textureID, 1, internalFormat, _desc.width, _desc.height);

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
                    GL_UNSIGNED_BYTE,   // 픽셀 데이터 타입
                    _initialData[i] // 실제 픽셀 데이터
                );
            }
        }
        //glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        //glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //glTextureParameteri(textureID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        //glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        //glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
    OpenGLTextureCube::~OpenGLTextureCube()
    {
        glDeleteTextures(1, &textureID);
    }

    void OpenGLTextureCube::Update(UInt32 _faceIndex, Shared<Texture2D> _texture)
    {
        textures[_faceIndex] = _texture;

        glCopyImageSubData(
            static_cast<UInt32>(reinterpret_cast<UInt64>(_texture->GetNativeHandle())),      // 원본 텍스처 핸들
            GL_TEXTURE_2D,        // 원본 타겟 타입
            0,                    // 원본 밉 레벨
            0, 0, 0,              // 원본 좌표 (x, y, z)
            textureID,         // 대상 텍스처 핸들
            GL_TEXTURE_CUBE_MAP,  // 대상 타겟 타입
            0,                    // 대상 밉 레벨
            0, 0, _faceIndex,      // 대상 좌표 (x, y, layer) - faceIndex가 레이어를 지정!
            width, height, 1      // 복사할 크기
        );
    }

    void OpenGLTextureCube::SetSampler(Shared<Sampler> _sampler)
    {
        textureSampler = static_cast<OpenGLSampler*>(_sampler.get());
    }
}

