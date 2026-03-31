struct PSInput
{
    float4 position : SV_Position;
    float3 worldPosition : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 uv : TEXCOORD0;
};

struct PSOutput
{
    float4 position : SV_Target0;
    float4 normal : SV_Target1;
    float4 albedo : SV_Target2;
    float4 material : SV_Target3;
    uint entityHandle : SV_Target4;
};

cbuffer Entity : register(b2)
{
    uint entityHandle;
};

[[vk::combinedImageSampler]][[vk::binding(3, 0)]]
Texture2D mat_AlbedoMap : register(t0);
[[vk::combinedImageSampler]][[vk::binding(3, 0)]]
SamplerState mat_AlbedoTextureSampler : register(s0);

[[vk::combinedImageSampler]][[vk::binding(4, 0)]]
Texture2D mat_NormalMap : register(t1);
[[vk::combinedImageSampler]][[vk::binding(4, 0)]]
SamplerState mat_NormalTextureSampler : register(s1);

[[vk::combinedImageSampler]][[vk::binding(5, 0)]]
Texture2D mat_RoughnessMap : register(t2);
[[vk::combinedImageSampler]][[vk::binding(5, 0)]]
SamplerState mat_RoughnessTextureSampler : register(s2);

[[vk::combinedImageSampler]][[vk::binding(6, 0)]]
Texture2D mat_MetallicMap : register(t3);
[[vk::combinedImageSampler]][[vk::binding(6, 0)]]
SamplerState mat_MetallicTextureSampler : register(s3);

[[vk::combinedImageSampler]][[vk::binding(7, 0)]]
Texture2D mat_AOMap : register(t4);
[[vk::combinedImageSampler]][[vk::binding(7, 0)]]
SamplerState mat_AOTextureSampler : register(s4);

PSOutput PSMain(PSInput input)
{
    PSOutput output = (PSOutput) 0;
    
    float4 albedo = mat_AlbedoMap.Sample(mat_AlbedoTextureSampler, input.uv);
    //float4 albedoColor = albedoMap * g_BaseColorFactor;

    if (albedo.a < 0.5f)
    {
        discard;
    }

    //float3 tangentNormal = input.tangent;
    // [0, 1] 범위를 [-1, 1] 범위로 변환
    //tangentNormal = (tangentNormal * 2.0f) - 1.0f;

    // MRAO 맵 (Metallic / Roughness / AO)
    float ao = mat_AOMap.Sample(mat_AOTextureSampler, input.uv).r; // AO는 텍스처 값 그대로 사용
    float roughness = mat_RoughnessMap.Sample(mat_RoughnessTextureSampler, input.uv).g;
    float metallic = mat_MetallicMap.Sample(mat_MetallicTextureSampler, input.uv).b;
    float3 MRAO = float3(ao, roughness, metallic);

    //--- 2. View Space 노멀 계산 (노멀 매핑) ---
    float3 N = normalize(input.normal);
    float3 T = normalize(input.tangent - dot(input.tangent, N) * N);
    float3 B = cross(N, T);
    float3x3 TBN = float3x3(T, B, N);

    float3 normal = mat_NormalMap.Sample(mat_NormalTextureSampler, input.uv).rgb * 2.0 - 1.0;
    normal = normalize(mul(normal, TBN));

    //output.position = float4(input.worldPosition, 1.0f);
    output.position = float4(input.worldPosition, 1.0f);
    output.normal = float4(normal, 1.0f);
    output.albedo = albedo;
    //output.albedo = float4(1.0f, 1.0f, 0.0f, 1.0f);
    output.material = float4(MRAO, 1.0f);
    output.entityHandle = entityHandle;
    
    return output;
}