struct PSInput
{
    float4 position : SV_Position;
    float3 worldPosition : POSITION;
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
    float ambientPower;
    float shininess;
    float3 padding;
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
        
    float4 albedo = Texture.Sample(TextureSampler, input.uv);
    //if (albedo.a < 0.2f)
    //{
    //    clip(-1);
    //}
    
    [unroll]
    for (int i = 0; i < 1; i++)
    {
        float3 lightDir = -lights[i].direction;
        float diffuse = max(dot(normalize(lightDir), normalWorld), 0.0f);
        
        float3 viewDir = normalize(eyePosition - input.worldPosition);
        float3 reflectDir = reflect(-lightDir, normalWorld);
        float3 halfway = normalize(viewDir + lightDir);
        float specularPower = pow(max(dot(normalWorld, halfway), 0.0f), lights[i].shininess);
        float specular = specularPower * lights[i].intensity;
        
        color += albedo.rgb * lights[i].color * (diffuse + lights[i].ambientPower) + specular;
    }
    
    output.color = float4(color, 1.0f);
    return output;
}