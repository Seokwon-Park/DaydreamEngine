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
Texture2D AlbedoTexture : register(t0);
[[vk::combinedImageSampler]][[vk::binding(3, 0)]]
SamplerState AlbedoTextureSampler : register(s0);

[[vk::combinedImageSampler]][[vk::binding(4, 0)]]
Texture2D NormalTexture : register(t1);
[[vk::combinedImageSampler]][[vk::binding(4, 0)]]
SamplerState NormalTextureSampler : register(s1);

[[vk::combinedImageSampler]][[vk::binding(5, 0)]]
Texture2D RoughnessTexture : register(t2);
[[vk::combinedImageSampler]][[vk::binding(5, 0)]]
SamplerState RoughnessTextureSampler : register(s2);

[[vk::combinedImageSampler]][[vk::binding(6, 0)]]
Texture2D MetallicTexture : register(t3);
[[vk::combinedImageSampler]][[vk::binding(6, 0)]]
SamplerState MetallicTextureSampler : register(s3);

[[vk::combinedImageSampler]][[vk::binding(7, 0)]]
Texture2D AOTexture : register(t4);
[[vk::combinedImageSampler]][[vk::binding(7, 0)]]
SamplerState AOTextureSampler : register(s4);

PSOutput PSMain(PSInput input)
{
    PSOutput output = (PSOutput) 0;
    
    float4 albedo = AlbedoTexture.Sample(AlbedoTextureSampler, input.uv);
    //float4 albedoColor = albedoMap * g_BaseColorFactor;

    if (albedo.a < 0.5f)
    {
        discard;
    }

    //float3 tangentNormal = input.tangent;
    // [0, 1] 범위를 [-1, 1] 범위로 변환
    //tangentNormal = (tangentNormal * 2.0f) - 1.0f;

    // MRAO 맵 (Metallic / Roughness / AO)
    float ao = AOTexture.Sample(AOTextureSampler, input.uv).r; // AO는 텍스처 값 그대로 사용
    float roughness = RoughnessTexture.Sample(RoughnessTextureSampler, input.uv).g;
    float metallic = MetallicTexture.Sample(MetallicTextureSampler, input.uv).b;
    float3 MRAO = float3(ao, roughness, metallic);

    //--- 2. View Space 노멀 계산 (노멀 매핑) ---
    float3 N = normalize(input.normal);
    float3 T = normalize(input.tangent - dot(input.tangent, N) * N);
    float3 B = cross(N, T);
    float3x3 TBN = float3x3(T, B, N);

    float3 normal = NormalTexture.Sample(NormalTextureSampler, input.uv).rgb * 2.0 - 1.0;
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