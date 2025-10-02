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
Texture2D Texture : register(t0);
[[vk::combinedImageSampler]][[vk::binding(0, 1)]]
SamplerState TextureSampler : register(s0);

[[vk::combinedImageSampler]][[vk::binding(1, 1)]]
Texture2D NormalTexture : register(t1);
[[vk::combinedImageSampler]][[vk::binding(1, 1)]]
SamplerState NormalTextureSampler : register(s1);

[[vk::combinedImageSampler]][[vk::binding(2, 1)]]
TextureCube irradianceTexture : register(t2);
[[vk::combinedImageSampler]][[vk::binding(2, 1)]]
SamplerState irradianceTextureSampler : register(s2);

[[vk::combinedImageSampler]][[vk::binding(3, 1)]]
TextureCube prefilter : register(t3);
[[vk::combinedImageSampler]][[vk::binding(3, 1)]]
SamplerState prefilterSampler : register(s3);

[[vk::combinedImageSampler]][[vk::binding(4, 1)]]
Texture2D brdfLUT : register(t4);
[[vk::combinedImageSampler]][[vk::binding(4, 1)]]
SamplerState brdfLUTSampler : register(s4);

static const float PI = 3.14159265359;

float DistributionGGX(float3 N, float3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(float3 N, float3 V, float3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
float3 fresnelSchlick(float cosTheta, float3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
// ----------------------------------------------------------------------------
float3 fresnelSchlickRoughness(float cosTheta, float3 F0, float roughness)
{
    return F0 + (max(float3(1.0 - roughness, 1.0 - roughness, 1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

PSOutput PSMain(PSInput input)
{
    PSOutput output = (PSOutput) 0;
    
    float3 albedo = 0.2;
    float metallic = 0.1;
    float roughness = 0.1;
    float ao = 0.0f;
    
    float3 normalWorld = input.normal;
    
    float3 normalTex = NormalTexture.Sample(NormalTextureSampler, input.uv).rgb;
    normalTex = 2.0 * normalTex - 1.0; // 범위 조절 [-1.0, 1.0]
    
    float3 N = normalWorld;
    float3 T = normalize(input.tangent - dot(input.tangent, N) * N);
    float3 B = cross(N, T);
      
    float3x3 TBN = float3x3(T, B, N);
    N = normalize(mul(normalTex, TBN));
    
    float3 V = normalize(eyePosition - input.worldPosition);
    float3 R = reflect(-V, N);

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    float3 F0 = float3(0.04, 0.04, 0.04);
    F0 = lerp(F0, albedo, metallic);

    // reflectance equation
    float3 Lo = float3(0.0, 0.0, 0.0);
    for (int i = 0; i < pointLightCount; ++i)
    {
        // calculate per-light radiance
        float3 L = normalize(pointLights[i].position - input.worldPosition);
        float3 H = normalize(V + L);
        float distance = length(pointLights[i].position - input.worldPosition);
        float attenuation = 1.0 / (distance * distance);
        float3 radiance = pointLights[i].color * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);
        float G = GeometrySmith(N, V, L, roughness);
        float3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);
        
        float3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
        float3 specular = numerator / denominator;
        
         // kS is equal to Fresnel
        float3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        float3 kD = float3(1.0, 1.0, 1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - metallic;
            
        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);

        // add to outgoing radiance Lo
        Lo += (kD * albedo / PI + specular) * radiance * NdotL; // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    }
    
    // ambient lighting (we now use IBL as the ambient term)
    float3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
    
    float3 kS = F;
    float3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;
    
    float3 irradiance = irradianceTexture.Sample(irradianceTextureSampler, N).rgb;
    float3 diffuse = irradiance * albedo;
    
    // sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
    const float MAX_REFLECTION_LOD = 4.0;
    float3 prefilteredColor = prefilter.Sample(prefilterSampler, R).rgb;
    float2 brdf = brdfLUT.Sample(brdfLUTSampler, float2(max(dot(N, V), 0.0), roughness)).rg;
    float3 specular = prefilteredColor * (F * brdf.x + brdf.y);

    float3 ambient = (kD * diffuse + specular) * ao;
    
    float3 color = ambient + Lo;

    // HDR tonemapping
    color = color / (color + float3(1.0, 1.0, 1.0));
    // gamma correct
    color = pow(color, float3(1.0 / 2.2, 1.0 / 2.2, 1.0 / 2.2));
    output.color = float4(color, 1.0f);
    return output;
}