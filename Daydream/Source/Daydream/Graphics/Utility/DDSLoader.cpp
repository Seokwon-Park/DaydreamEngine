#include "DaydreamPCH.h"
#include "DDSLoader.h"

namespace Daydream::DDSLoader
{
    DDSImage LoadDDS(const FilePath& _path)
    {
        std::ifstream f(_path.ToString(), std::ios::binary);
        if (!f) throw std::runtime_error("Failed to open DDS");

        DDSHeader hdr{};
        f.read((char*)&hdr, sizeof(hdr));
        if (hdr.magic != DDS_MAGIC) throw std::runtime_error("Not a DDS");

        DDSImage img{};
        img.width = hdr.width;
        img.height = hdr.height;
        img.mipMapCount = hdr.mipMapCount ? hdr.mipMapCount : 1;

        bool isCube = (hdr.caps2 & DDSCAPS2_CUBEMAP) != 0;
        img.arraySize = isCube ? 6 : 1;

        // 포맷 판별
        if (hdr.pixelFormat.flags & 0x4) { // DDPF_FOURCC
            switch (hdr.pixelFormat.fourCC) {
            case '1TXD': img.format = DDSFormat::BC1; break; // DXT1
            case '3TXD': img.format = DDSFormat::BC2; break; // DXT3
            case '5TXD': img.format = DDSFormat::BC3; break; // DXT5
            default: throw std::runtime_error("Unsupported FOURCC");
            }
        }
        else {
            img.format = DDSFormat::RGBA8;
        }

        // face/mip 데이터 읽기
        img.faces.resize(img.arraySize);
        UInt32 width = img.width, height = img.height;
        for (uint32_t face = 0; face < img.arraySize; ++face) {
            width = img.width; height = img.height;
            img.faces[face].resize(img.mipMapCount);
            for (uint32_t mip = 0; mip < img.mipMapCount; ++mip) {
                uint32_t size;
                if (img.format == DDSFormat::RGBA8) {
                    size = width * height * 4;
                }
                else {
                    uint32_t blockSize = (img.format == DDSFormat::BC1) ? 8 : 16;
                    size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
                }
                img.faces[face][mip].resize(size);
                f.read((char*)img.faces[face][mip].data(), size);
                width = std::max(1u, width / 2);
                height = std::max(1u, height / 2);
            }
        }
        return img;
    }
}