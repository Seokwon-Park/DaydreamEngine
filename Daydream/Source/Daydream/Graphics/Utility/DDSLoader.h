#pragma once

namespace Daydream::DDSLoader
{
    enum class DDSFormat { BC1, BC2, BC3, RGBA8 };

    struct DDSImage {
        UInt32 width;
        UInt32 height;
        UInt32 mipMapCount;
        UInt32 arraySize; // 큐브맵이면 6
        DDSFormat format;
        // faces[face][mip]
        Array<Array<Array<UInt8>>> faces;
    };

    // DDS 헤더 구조체 (간략화)
#pragma pack(push,1)
    struct DDSHeader {
        UInt32 magic;
        UInt32 size;
        UInt32 flags;
        UInt32 height;
        UInt32 width;
        UInt32 pitchOrLinearSize;
        UInt32 depth;
        UInt32 mipMapCount;
        UInt32 reserved1[11];
        struct {
            UInt32 size;
            UInt32 flags;
            UInt32 fourCC;
            UInt32 rgbBitCount;
            UInt32 rMask, gMask, bMask, aMask;
        } pixelFormat;
        UInt32 caps;
        UInt32 caps2;
        UInt32 caps3;
        UInt32 caps4;
        UInt32 reserved2;
    };
#pragma pack(pop)

    constexpr UInt32 DDS_MAGIC = 0x20534444; // "DDS "
    constexpr UInt32 DDSCAPS2_CUBEMAP = 0x00000200;
    constexpr UInt32 DDSCAPS2_CUBEMAP_ALLFACES = 0x0000FC00;

    DDSImage LoadDDS(const FilePath& _path);
}