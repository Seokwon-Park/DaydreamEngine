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
Texture2D u_Texture : register(t0);
[[vk::combinedImageSampler]][[vk::binding(1)]]
SamplerState u_TextureSampler : register(s0);

PSOutput PSMain(PSInput input)
{
    PSOutput output = (PSOutput) 0;
    output.color = u_Texture.Sample(u_TextureSampler, input.uv) + input.color;
    return output;
}