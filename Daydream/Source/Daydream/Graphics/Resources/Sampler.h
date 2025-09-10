#pragma once

namespace Daydream
{
    enum class FilterMode
    {
        Nearest,        // �ֱ��� ���͸�
        Linear,         // ���� ���͸�
        NearestMipmapNearest, // �Ӹ� ���, �ֱ��� ���͸�
        LinearMipmapNearest,  // �Ӹ� ���, ���� ���͸�
        NearestMipmapLinear,  // �Ӹ� ���, ���� ���͸�
        LinearMipmapLinear    // �Ӹ� ���, ���� ���͸�
    };

    enum class WrapMode
    {
        Repeat,         // �ݺ�
        ClampToEdge,    // ��迡 ����
        ClampToBorder,  // �������� ����
        MirrorRepeat    // �ſ�ó�� �ݺ�
    };

    enum class ComparisonFunc
    {
        Never,
        Less,
        Equal,
        LessEqual,
        Greater,
        NotEqual,
        GreaterEqual,
        Always
    };

    struct SamplerDesc
    {
        FilterMode minFilter = FilterMode::Linear;
        FilterMode magFilter = FilterMode::Linear;
        FilterMode mipFilter = FilterMode::Linear;

        WrapMode wrapU = WrapMode::Repeat;
        WrapMode wrapV = WrapMode::Repeat;
        WrapMode wrapW = WrapMode::Repeat;

        float minLod = 0.0f;
        float maxLod = 1000.0f;
        float lodBias = 0.0f;

        float maxAnisotropy = 1.0f;
        bool comparisonEnable = false;
        ComparisonFunc comparisonFunc = ComparisonFunc::Never;
        float borderColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    };

	class Sampler
	{
	public:
		virtual ~Sampler() = default;

        static Shared<Sampler> Create(const SamplerDesc& _desc);
	private:

	};
}