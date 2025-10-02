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
                    textureID,          // �ؽ�ó ��ü ID
                    0,                  // �Ӹ� ����
                    0, 0, i,            // xoffset, yoffset, zoffset (i�� ���� �ε���)
                    _desc.width,        // �ʺ�
                    _desc.height,       // ����
                    1,                  // ���� (�� ���̹Ƿ� 1)
                    dataFormat,         // �ȼ� ������ ����
                    GL_UNSIGNED_BYTE,   // �ȼ� ������ Ÿ��
                    _initialData[i] // ���� �ȼ� ������
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
            static_cast<UInt32>(reinterpret_cast<UInt64>(_texture->GetNativeHandle())),      // ���� �ؽ�ó �ڵ�
            GL_TEXTURE_2D,        // ���� Ÿ�� Ÿ��
            0,                    // ���� �� ����
            0, 0, 0,              // ���� ��ǥ (x, y, z)
            textureID,         // ��� �ؽ�ó �ڵ�
            GL_TEXTURE_CUBE_MAP,  // ��� Ÿ�� Ÿ��
            0,                    // ��� �� ����
            0, 0, _faceIndex,      // ��� ��ǥ (x, y, layer) - faceIndex�� ���̾ ����!
            width, height, 1      // ������ ũ��
        );
    }

    void OpenGLTextureCube::SetSampler(Shared<Sampler> _sampler)
    {
        textureSampler = static_cast<OpenGLSampler*>(_sampler.get());
    }
}

