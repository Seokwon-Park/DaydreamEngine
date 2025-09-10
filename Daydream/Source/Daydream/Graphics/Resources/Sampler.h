#pragma once

namespace Daydream
{
    enum class FilterMode
    {
        Nearest,        // 최근접 필터링
        Linear,         // 선형 필터링
        NearestMipmapNearest, // 밉맵 사용, 최근접 필터링
        LinearMipmapNearest,  // 밉맵 사용, 선형 필터링
        NearestMipmapLinear,  // 밉맵 사용, 선형 필터링
        LinearMipmapLinear    // 밉맵 사용, 선형 필터링
    };

    enum class WrapMode
    {
        Repeat,         // 반복
        ClampToEdge,    // 경계에 고정
        ClampToBorder,  // 경계색으로 고정
        MirrorRepeat    // 거울처럼 반복
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