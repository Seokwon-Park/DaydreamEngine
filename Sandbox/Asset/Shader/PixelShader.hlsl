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

Texture2D g_texture : register(t0);

SamplerState g_sampler : register(s0);

PSOutput PSMain(PSInput input)
{
    PSOutput output = (PSOutput) 0;
    output.color = input.color;
    output.color = g_texture.Sample(g_sampler, input.uv);
    return output;
}