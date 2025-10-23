


// --- ���� ����/IBL �Է� (Forward ���̴��� ����) ---

// (cbuffer Lights : register(b2) ... )
// (cbuffer Material : register(b3) ... ) - exposure, gamma ������ �ʿ�
// (TextureCube IrradianceTexture : register(t5) ... )
// (TextureCube Prefilter : register(t6) ... )
// (Texture2D BRDFLUT : register(t7) ... )
// (SamplerState ...Sampler : register(s5-s7) ... )

// (������ ������ ����ü �� �����)
struct PSInput
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD0;
};

struct PSOutput
{
    float4 color : SV_Target0;
};

//================================================================================
// ���� ����ü
//================================================================================
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

cbuffer Lights : register(b2)
{
    DirectionalLight dirLights[2];
    PointLight pointLights[4];
    SpotLight spotLights[4];
    float3 eyePosition;
    uint dirLightCount;
    float2 padding1;
    uint pointLightCount;
    uint spotLightCount;
};
// (cbuffer Material : register(b3) ... )


//Texture2D t_NormalBuffer : register(t9);
//Texture2D t_AlbedoBuffer : register(t10);
//Texture2D t_MaterialBuffer : register(t11);

//// G-Buffer ���ø��� ���� ���÷� (Point Sampler ����)
//SamplerState g_PointSampler : register(s8);

[[vk::combinedImageSampler]][[vk::binding(0, 1)]]
Texture2D PositionTexture : register(t0);
[[vk::combinedImageSampler]][[vk::binding(0, 1)]]
SamplerState PositionTextureSampler : register(s0);

[[vk::combinedImageSampler]][[vk::binding(1, 1)]]
Texture2D AlbedoTexture : register(t1);
[[vk::combinedImageSampler]][[vk::binding(1, 1)]]
SamplerState AlbedoTextureSampler : register(s1);

[[vk::combinedImageSampler]][[vk::binding(2, 1)]]
Texture2D NormalTexture : register(t2);
[[vk::combinedImageSampler]][[vk::binding(2, 1)]]
SamplerState NormalTextureSampler : register(s2);

[[vk::combinedImageSampler]][[vk::binding(3, 1)]]
Texture2D RMAOTexture : register(t3);
[[vk::combinedImageSampler]][[vk::binding(3, 1)]]
SamplerState RMAOTextureSampler : register(s3);

[[vk::combinedImageSampler]][[vk::binding(4, 1)]]
TextureCube IrradianceTexture : register(t4);
[[vk::combinedImageSampler]][[vk::binding(4, 1)]]
SamplerState IrradianceTextureSampler : register(s4);

[[vk::combinedImageSampler]][[vk::binding(5, 1)]]
TextureCube Prefilter : register(t5);
[[vk::combinedImageSampler]][[vk::binding(5, 1)]]
SamplerState PrefilterSampler : register(s5);

[[vk::combinedImageSampler]][[vk::binding(6, 1)]]
Texture2D BRDFLUT : register(t6);
[[vk::combinedImageSampler]][[vk::binding(6, 1)]]
SamplerState BRDFLUTSampler : register(s6);

//================================================================================
// PBR �Լ��� (PBR ���̴����� ������)
//================================================================================
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
    return nom / max(denom, 0.0001); // �и� 0 ����
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;
    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;
    return nom / max(denom, 0.0001); // �и� 0 ����
}

float GeometrySmith(float3 N, float3 V, float3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
    return ggx1 * ggx2;
}

float3 FresnelSchlick(float cosTheta, float3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float3 FresnelSchlickRoughness(float cosTheta, float3 F0, float roughness)
{
    return F0 + (max(float3(1.0 - roughness, 1.0 - roughness, 1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

//================================================================================
// PBR ��� ���� ��� �Լ�
//================================================================================
float3 CalculatePBR(float3 L, float3 V, float3 N, float3 F0, float3 albedo, float metallic, float roughness, float3 radiance)
{
    float3 H = normalize(V + L);
    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    float3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);
    float3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
    float3 specular = numerator / max(denominator, 0.00001);
    float3 kS = F;
    float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
    kD *= 1.0f - metallic;
    float NdotL = max(dot(N, L), 0.0);
    return (kD * albedo / PI + specular) * radiance * NdotL;
}

float3 ComputePBRDirectionalLight(DirectionalLight light, float3 V, float3 N, float3 F0, float3 albedo, float metallic, float roughness)
{
    float3 L = normalize(-light.direction);
    float3 radiance = light.color * light.intensity;
    return CalculatePBR(L, V, N, F0, albedo, metallic, roughness, radiance);
}

float3 ComputePBRPointLight(PointLight light, float3 worldPos, float3 V, float3 N, float3 F0, float3 albedo, float metallic, float roughness)
{
    float3 L = light.position - worldPos;
    float distance = length(L);
    if (distance > light.range)
    {
        return float3(0.0, 0.0, 0.0);
    }
    L = normalize(L);
    float attenuation = 1.0 / (distance * distance);
    float3 radiance = light.color * light.intensity * attenuation;
    return CalculatePBR(L, V, N, F0, albedo, metallic, roughness, radiance);
}

float3 ComputePBRSpotLight(SpotLight light, float3 worldPos, float3 V, float3 N, float3 F0, float3 albedo, float metallic, float roughness)
{
    float3 L = light.position - worldPos;
    float distance = length(L);
    if (distance > light.range)
    {
        return float3(0.0, 0.0, 0.0);
    }
    L = normalize(L);
    float theta = dot(L, normalize(-light.direction));
    float spotEffect = smoothstep(light.outerConeCos, light.innerConeCos, theta);
    float attenuation = 1.0 / (distance * distance + 0.001);
    float3 radiance = light.color * light.intensity * attenuation * spotEffect;
    return CalculatePBR(L, V, N, F0, albedo, metallic, roughness, radiance);
}


// --- ������ �н� �ȼ� ���̴� ---
PSOutput PSMain(PSInput input)
{
    PSOutput output = (PSOutput) 0;
    
    // --- 1. G-Buffer���� ������ ���ø� ---
    float2 uv = input.uv;
    float4 posData = PositionTexture.Sample(PositionTextureSampler, uv);
    float3 worldPosition = posData.xyz;
    
    // G-Buffer�� ����ִ� �ȼ�(���)�� ��ŵ
    // (G-Buffer ���̴����� position.w = 1.0���� �����Ƿ�, 0.0�̸� ����ִ� ��)
    if (posData.w == 0.0)
    {
        discard; // �Ǵ� ����(��ī�̹ڽ�) ���
    }

    float3 N = NormalTexture.Sample(NormalTextureSampler, uv).xyz;
    float3 albedo = AlbedoTexture.Sample(AlbedoTextureSampler, uv).rgb;
    float4 matData = RMAOTexture.Sample(RMAOTextureSampler, uv);
    float metallic = matData.b;
    float roughness = matData.g;
    float ao = matData.r;

    // --- (G-Buffer ���̴��� 1, 2�� �ܰ谡 �� �ڵ�� ��ü��) ---
    
    // --- 3. �⺻ ���� �� F0 ��� (Forward ���̴��� ����) ---
    // (eyePosition�� cbuffer Lights(b2)���� ������)
    float3 V = normalize(eyePosition - worldPosition);
    float3 R = reflect(-V, N);

    float3 F0 = lerp(float3(0.04, 0.04, 0.04), albedo, metallic);

    // --- 4. ���� ����(Direct Lighting) ��� (Forward ���̴��� *������ ����*) ---
    float3 Lo = float3(0.0, 0.0, 0.0);
    uint i;
    for (i = 0; i < dirLightCount; ++i)
    {
        Lo += ComputePBRDirectionalLight(dirLights[i], V, N, F0, albedo, metallic, roughness);
    }
    for (i = 0; i < pointLightCount; ++i)
    {
        Lo += ComputePBRPointLight(pointLights[i], worldPosition, V, N, F0, albedo, metallic, roughness);
    }
    for (i = 0; i < spotLightCount; ++i)
    {
        Lo += ComputePBRSpotLight(spotLights[i], worldPosition, V, N, F0, albedo, metallic, roughness);
    }

    // --- 5. ���� ����(IBL) ��� (Forward ���̴��� *������ ����*) ---
    float3 F_ibl = FresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
    float3 kS_ibl = F_ibl;
    float3 kD_ibl = 1.0 - kS_ibl;
    kD_ibl *= 1.0 - metallic;
    
    float3 irradiance = IrradianceTexture.Sample(IrradianceTextureSampler, N).rgb;
    float3 diffuseIBL = (irradiance * albedo) / PI;
    
    uint width, height, maxMipLevel;
    Prefilter.GetDimensions(0, width, height, maxMipLevel);
    
    const float MAX_REFLECTION_LOD = maxMipLevel;
    float3 prefilteredColor = Prefilter.SampleLevel(PrefilterSampler, R, roughness * MAX_REFLECTION_LOD).rgb;
    float2 brdf = BRDFLUT.Sample(BRDFLUTSampler, float2(max(dot(N, V), 0.0), roughness)).rg;
    float3 specularIBL = prefilteredColor * (F_ibl * brdf.x + brdf.y);
    
    float3 ambient = (kD_ibl * diffuseIBL + specularIBL) * ao;
    
    // --- 6. ���� ���� ���� (Forward ���̴��� ����) ---
    float3 color = Lo + ambient;

    // --- 7. ��ó��: �� ���� �� ���� ���� (Forward ���̴��� ����) ---
    // (exposure, gamma�� cbuffer Material(b3)���� ������)
    float gamma = 2.2f;
    color = color / (color + float3(1.0, 1.0, 1.0)); // Reinhard Tonemapping
    color = color * 1.0f;
    color = pow(color, float3(1.0 / gamma, 1.0 / gamma, 1.0 / gamma)); // Gamma Correction

    output.color = float4(color, 1.0f);
    return output;
}