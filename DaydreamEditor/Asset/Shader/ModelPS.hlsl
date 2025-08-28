struct PSInput
{
    float4 position : SV_Position;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
};
    
struct Light
{
    float3 color;
    int type;
    float3 position;
    float intensity;
    float3 direction;
    float padding;
};

cbuffer Lights : register(b2)
{
    Light lights[32]; // 최대 32개 라이트
    float3 eyePosition;
    int lightCount;
};

//cbuffer MaterialConstants : register(b3)
//{
//    int useAlbedoTexture;
//    int useNormalMap;
//    int useAOMap;
//    int padding;
//}

struct PSOutput
{
    float4 color : SV_Target0;
};

[[vk::combinedImageSampler]][[vk::binding(4)]]
Texture2D Texture : register(t0);
[[vk::combinedImageSampler]][[vk::binding(4)]]
SamplerState TextureSampler : register(s0);

PSOutput PSMain(PSInput input)
{
    PSOutput output = (PSOutput) 0;
    
    float3 color = float3(0.0f, 0.0f, 0.0f);
    [unroll]
    for (int i = 0; i < 1; i++)
    {
    //output.color = Texture.Sample(TextureSampler, input.uv) + input.color;
        float3 lightDir = -lights[i].direction;
        float ndotl = max(dot(normalize(lightDir), input.normal), 0.0f);
        
        float3 viewDirection = normalize(eyePosition - input.position.xyz);
        float3 reflectDir = reflect(-lightDir, input.normal);
        float3 halfway = normalize(viewDirection + lightDir);
        float specularPower = pow(max(dot(input.normal, halfway), 0.0f), 20.0f);
        
        //color += lights[i].color * ndotl + float3(1.0f, 1.0f, 1.0f) * specularPower;
        color += lights[i].color * ndotl + input.normal * specularPower;
    }
    float2 uv = input.uv - input.uv;
    
    
    output.color = float4(color, 1.0f) + float4(uv, 0.0f, 0.0f);
    return output;
}