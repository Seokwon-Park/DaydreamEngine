struct PSInput
{
    float4 position : SV_Position;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
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

//[[vk::binding(0, 1)]]
//Texture2D Texture : register(t0);
//[[vk::binding(1, 1)]]
//Texture2D NormalTexture : register(t1);
//[[vk::binding(0, 0)]]
//SamplerState TextureSampler : register(s0);


[[vk::combinedImageSampler]][[vk::binding(0, 1)]]
Texture2D Texture : register(t0);
[[vk::combinedImageSampler]][[vk::binding(0, 1)]]
SamplerState TextureSampler : register(s0);

[[vk::combinedImageSampler]][[vk::binding(1, 1)]]
Texture2D NormalTexture : register(t1);
[[vk::combinedImageSampler]][[vk::binding(1, 1)]]
SamplerState NormalTextureSampler : register(s1);


PSOutput PSMain(PSInput input)
{
    PSOutput output = (PSOutput) 0;
    
    float3 color = float3(0.0f, 0.0f, 0.0f);
    
    float3 normalWorld = input.normal;
    
    float3 normalTex = NormalTexture.Sample(NormalTextureSampler, input.uv).rgb;
    normalTex = 2.0 * normalTex - 1.0; // 범위 조절 [-1.0, 1.0]
    
    float3 N = normalWorld;
    float3 T = normalize(input.tangent - dot(input.tangent, N) * N);
    float3 B = cross(N, T);
        
        // matrix는 float4x4, 여기서는 벡터 변환용이라서 3x3 사용
    float3x3 TBN = float3x3(T, B, N);
    normalWorld = normalize(mul(normalTex, TBN));
    
    [unroll]
    for (int i = 0; i < 1; i++)
    {
    //output.color = Texture.Sample(TextureSampler, input.uv) + input.color;
        float3 lightDir = -lights[i].direction;
        float diffuse = max(dot(normalize(lightDir), normalWorld), 0.0f);
        
        float3 viewDirection = normalize(eyePosition - input.position.xyz);
        float3 reflectDir = reflect(-lightDir, normalWorld);
        float3 halfway = normalize(viewDirection + lightDir);
        float specularPower = pow(max(dot(normalWorld, halfway), 0.0f), 20.0f);
        
        color += lights[i].color * diffuse + float3(1.0f, 1.0f, 1.0f) * specularPower;
        //color += lights[i].color * ndotl + input.normal * specularPower;
    }
    
    output.color = Texture.Sample(TextureSampler, input.uv) + float4(color, 1.0f) * 0.3f;
    return output;
}