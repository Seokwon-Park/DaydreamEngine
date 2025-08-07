struct PSInput
{
    float4 position : SV_Position;
    float4 color : COLOR0;
    float2 uv: TEXCOORD0;
};

struct PSOutput
{
    float4 color : SV_Target0;
};

[[vk::combinedImageSampler]][[vk::binding(1)]]
Texture2D Texture : register(t0);
[[vk::combinedImageSampler]][[vk::binding(1)]]
SamplerState TextureSampler : register(s0);

PSOutput PSMain(PSInput input)
{
    PSOutput output = (PSOutput) 0;
    output.color = Texture.Sample(TextureSampler, input.uv) + input.color;
    return output;
}