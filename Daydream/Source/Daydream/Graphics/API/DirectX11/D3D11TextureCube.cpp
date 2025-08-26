#include "DaydreamPCH.h"
#include "D3D11TextureCube.h"

#include "Daydream/Graphics/API/DirectX/D3DUtility.h"
#include "Daydream/Graphics/Utility/DDSLoader.h"

namespace Daydream
{

    D3D11TextureCube::D3D11TextureCube(D3D11RenderDevice* _device, const TextureDesc& _desc, Array<const void*> _initialData)
    {
        device = _device;
        desc = _desc;

        D3D11_TEXTURE2D_DESC textureDesc = {};
        textureDesc.Width = desc.width;
        textureDesc.Height = desc.height;
        textureDesc.MipLevels = 1;
        textureDesc.ArraySize = 6; // ť����� 6���� �ؽ�ó �迭�Դϴ�.
        textureDesc.Format = GraphicsUtility::DirectX::ConvertRenderFormatToDXGIFormat(_desc.format); // DXGI_FORMAT ������ ��
        textureDesc.SampleDesc.Count = 1;
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.Usage = D3D11_USAGE_DEFAULT; // �Ϲ����� �ؽ�ó ����
        textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        textureDesc.CPUAccessFlags = 0;
        textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE; 

        D3D11_SUBRESOURCE_DATA subresourceData[6];
        bool hasInitialData = (_initialData.size() == 6);

        if (hasInitialData)
        {
            for (int i = 0; i < 6; ++i)
            {
                subresourceData[i].pSysMem = _initialData[i];
                subresourceData[i].SysMemPitch = textureDesc.Width * sizeof(UInt8) * 4;
                subresourceData[i].SysMemSlicePitch = 0; // 2D �ؽ�ó������ ������ ����
            }
        }

        HRESULT hr = device->GetDevice()->CreateTexture2D(
            &textureDesc,
            hasInitialData ? subresourceData : nullptr,
            texture.GetAddressOf() // ������ ���ҽ��� ����� ��� ����
        );
    }

}