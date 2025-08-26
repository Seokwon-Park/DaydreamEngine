struct PSInput
{
    float4 position : SV_Position;
    float3 worldPosition : POSITION;
};

[[vk::combinedImageSampler]][[vk::binding(1)]]
TextureCube TextureCubemap : register(t0);
[[vk::combinedImageSampler]][[vk::binding(1)]]
SamplerState TextureCubemapSampler : register(s0);

float4 PSMain(PSInput input) : SV_TARGET
{
    return TextureCubemap.Sample(TextureCubemapSampler, input.worldPosition);
}