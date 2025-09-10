#include "DaydreamPCH.h"
#include "OpenGLTextureCube.h"

#include "glad/glad.h"
#include "OpenGLUtility.h"

namespace Daydream
{
	OpenGLTextureCube::OpenGLTextureCube(const TextureDesc& _desc, const Array<Array<UInt8>>& _initialData)
	{
        glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &textureID);

        width = _desc.width;
        height = _desc.height;

        GLenum internalFormat = 0, dataFormat = 0;
        internalFormat = GraphicsUtility::OpenGL::ConvertRenderFormatToGLFormat(_desc.format);
        dataFormat = GraphicsUtility::OpenGL::ConvertRenderFormatToGLDataFormat(_desc.format);

        glTextureStorage2D(textureID, 1, internalFormat, _desc.width, _desc.height);
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
                _initialData[i].data() // ���� �ȼ� ������
            );
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
    void OpenGLTextureCube::SetSampler(Shared<Sampler> _sampler)
    {
        textureSampler = static_cast<OpenGLSampler*>(_sampler.get());
    }
}

