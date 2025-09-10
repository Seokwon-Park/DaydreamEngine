struct PSInput
{
    float4 position : SV_Position;
    float3 worldPosition : POSITION;
};

struct PSOutput
{
    float4 color : SV_Target0;
};

[[vk::combinedImageSampler]][[vk::binding(1)]]
TextureCube TextureCubemap : register(t0);
[[vk::combinedImageSampler]][[vk::binding(1)]]
SamplerState TextureCubemapSampler : register(s0);

float4 PSMain(PSInput input) : SV_TARGET
{
    PSOutput output = (PSOutput) 0;
    
    output.color = TextureCubemap.Sample(TextureCubemapSampler, input.worldPosition);
    return output.color;
}