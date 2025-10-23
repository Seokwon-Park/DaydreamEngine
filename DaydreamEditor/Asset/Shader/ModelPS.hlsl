struct PSInput
{
    float4 position : SV_Position;
    float3 worldPosition : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 uv : TEXCOORD0;
};
    
struct DirectionalLight
{
    float3 direction;
    float intensity;
    float3 color;
    float padding1;
};

struct PointLight
{
    float3 position;
    float range;
    float3 color;
    float intensity;
};

struct SpotLight
{
    float3 position;
    float range;
    float3 direction;
    float intensity;
    float3 color;
    float innerConeCos;
    float3 padding1;
    float outerConeCos;
};

struct Light
{
    float3 color;
    uint type;
    float3 position;
    float intensity;
    float3 direction;
    float range; // point/spot에서만 사용
    float3 padding;
    float spotPower; // spot light cone
};

cbuffer Lights : register(b2)
{
    DirectionalLight dirLights[2];
    PointLight pointLights[4]; // 최대 32개 라이트
    SpotLight spotLights[4];
    float3 eyePosition;
    uint dirLightCount;
    float2 padding1;
    uint pointLightCount;
    uint spotLightCount;
};

float3 ComputeDirectinalLight(DirectionalLight light, float3 normal, float3 viewDir)
{
    float3 lightDir = -light.direction;
    float diffuse = saturate(dot(normalize(lightDir), normal));
    
    float3 reflectDir = reflect(-lightDir, normal);
    float3 halfway = normalize(viewDir + lightDir);
    float specularPower = pow(max(dot(normal, halfway), 0.0f), 32.0f);
    float specular = specularPower * light.intensity;
    
    return (diffuse + specular) * light.color;
}

float3 ComputePointLight(PointLight light, float3 normal, float3 worldPos)
{
    float3 lightDir = light.position - worldPos;
    float distance = length(lightDir);
    
    if (distance > light.range)
    {
        return float3(0.0f, 0.0f, 0.0f);
    }
    
    lightDir = normalize(lightDir);
    
    float attenuation = pow(saturate(1.0f - distance / light.range), 1.0f);
    float diffuse = saturate(dot(normal, lightDir));
    
    return diffuse * attenuation * light.color * light.intensity;
}

float3 ComputeSpotLight(SpotLight light, float3 normal, float3 worldPos)
{
    float3 lightDir = light.position - worldPos;
    float distance = length(lightDir);

    // 빛의 최대 범위를 벗어나면 계산 종료
    if (distance > light.range)
    {
        return float3(0.0f, 0.0f, 0.0f);
    }
    
    lightDir = normalize(lightDir);
    
    float attenuation = pow(saturate(1.0f - distance / light.range), 2.0f);
    float diffuseFactor = saturate(dot(normal, lightDir));
    
    float theta = dot(lightDir, -light.direction);
     // 부드러운 가장자리 영역의 크기 (코사인 공간)
    float spotAttenuation = smoothstep(light.outerConeCos, light.innerConeCos, theta);
    
    return light.color * light.intensity * diffuseFactor * attenuation * spotAttenuation;
}
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
Texture2D AlbedoTexture : register(t0);
[[vk::combinedImageSampler]][[vk::binding(0, 1)]]
SamplerState AlbedoTextureSampler : register(s0);

[[vk::combinedImageSampler]][[vk::binding(1, 1)]]
Texture2D NormalTexture : register(t1);
[[vk::combinedImageSampler]][[vk::binding(1, 1)]]
SamplerState NormalTextureSampler : register(s1);


PSOutput PSMain(PSInput input)
{
    PSOutput output = (PSOutput) 0;
    
    float4 albedo = AlbedoTexture.Sample(AlbedoTextureSampler, input.uv);
    if (albedo.a < 0.2f)
    {
        clip(-1);
    }
    
    float3 color = float3(0.0f, 0.0f, 0.0f);
    
    float3 normalWorld = input.normal;
     
    float3 normalTex = NormalTexture.Sample(NormalTextureSampler, input.uv).rgb;
    normalTex = 2.0 * normalTex - 1.0; // 범위 조절 [-1.0, 1.0]
    
    float3 N = normalWorld;
    float3 T = normalize(input.tangent - dot(input.tangent, N) * N);
    float3 B = cross(N, T);
      
    float3x3 TBN = float3x3(T, B, N);
    normalWorld = normalize(mul(normalTex, TBN));
        
    float3 viewDir = normalize(eyePosition - input.worldPosition);
    
    int i;
    for (i = 0; i < dirLightCount; i++)
    {
        float3 result = ComputeDirectinalLight(dirLights[i], normalWorld, viewDir);
        color += result * albedo.rgb;
    }
    
    for (i = 0; i < pointLightCount; i++)
    {
        float3 result = ComputePointLight(pointLights[i], normalWorld, input.worldPosition);
        color += result * albedo.rgb;
    }
    
    for (i = 0; i < spotLightCount; i++)
    {
        float3 result = ComputeSpotLight(spotLights[i], normalWorld, input.worldPosition);
        color += result * albedo.rgb;
    }
    
    output.color = float4(color, 1.0f);
    return output;
}