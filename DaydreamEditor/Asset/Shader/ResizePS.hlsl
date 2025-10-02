struct PSInput
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD0;
};

struct PSOutput
{
    float4 color : SV_Target0;
};

[[vk::combinedImageSampler]][[vk::binding(0, 0)]]
Texture2D Texture : register(t0);
[[vk::combinedImageSampler]][[vk::binding(0, 0)]]
SamplerState TextureSampler : register(s0);

// ���� ���̴��κ��� ������ UV ��ǥ�� �޽��ϴ�.
PSOutput PSMain(PSInput input)
{
    PSOutput output = (PSOutput) 0;
    
    output.color = Texture.Sample(TextureSampler, input.uv);
    return output;
}
