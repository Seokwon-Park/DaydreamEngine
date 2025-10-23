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
                    textureID,          // �ؽ�ó ��ü ID
                    0,                  // �Ӹ� ����
                    0, 0, i,            // xoffset, yoffset, zoffset (i�� ���� �ε���)
                    _desc.width,        // �ʺ�
                    _desc.height,       // ����
                    1,                  // ���� (�� ���̹Ƿ� 1)
                    dataFormat,         // �ȼ� ������ ����
                    GraphicsUtility::OpenGL::ConvertRenderFormatToGLDataType(_desc.format),   // �ȼ� ������ Ÿ��
                    _initialData[i] // ���� �ȼ� ������
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

