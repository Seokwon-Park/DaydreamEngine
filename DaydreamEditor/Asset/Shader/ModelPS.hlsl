struct PSInput
{
    float4 position : SV_Position;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
};
    
struct Light
{
    int type;
    float3 color;
    float3 position;
    float intensity;
    float3 direction;
    float padding;
};

cbuffer Lights : register(b0)
{
    Light lights[32]; // 최대 32개 라이트
    int lightCount;
    float3 padding;
};

struct PSOutput
{
    float4 color : SV_Target0;
};

//[[vk::combinedImageSampler]][[vk::binding(1)]]
//Texture2D Texture : register(t0);
//[[vk::combinedImageSampler]][[vk::binding(1)]]
//SamplerState TextureSampler : register(s0);

PSOutput PSMain(PSInput input)
{
    PSOutput output = (PSOutput) 0;
    
    float4 color = float4(0.0f, 0.0f, 0.0f, 1.0f);
    [unroll]
    for (int i = 0; i < 1; i++)
    {
    //output.color = Texture.Sample(TextureSampler, input.uv) + input.color;
        float3 lightVec = -lights[i].direction;
        float ndotl = max(dot(normalize(lightVec), input.normal), 0.0f);
        
        color += float4(lights[i].color, 1.0f) * ndotl;
    }
    output.color = color;
    return output;
}