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
};

[[vk::combinedImageSampler]][[vk::binding(0, 1)]]
Texture2D AlbedoTexture : register(t0);
[[vk::combinedImageSampler]][[vk::binding(0, 1)]]
SamplerState AlbedoTextureSampler : register(s0);

[[vk::combinedImageSampler]][[vk::binding(1, 1)]]
Texture2D NormalTexture : register(t1);
[[vk::combinedImageSampler]][[vk::binding(1, 1)]]
SamplerState NormalTextureSampler : register(s1);

[[vk::combinedImageSampler]][[vk::binding(2, 1)]]
Texture2D RoughnessTexture : register(t2);
[[vk::combinedImageSampler]][[vk::binding(2, 1)]]
SamplerState RoughnessTextureSampler : register(s2);

[[vk::combinedImageSampler]][[vk::binding(3, 1)]]
Texture2D MetallicTexture : register(t3);
[[vk::combinedImageSampler]][[vk::binding(3, 1)]]
SamplerState MetallicTextureSampler : register(s3);

[[vk::combinedImageSampler]][[vk::binding(4, 1)]]
Texture2D AOTexture : register(t4);
[[vk::combinedImageSampler]][[vk::binding(4, 1)]]
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
    // [0, 1] ������ [-1, 1] ������ ��ȯ
    //tangentNormal = (tangentNormal * 2.0f) - 1.0f;

    // MRAO �� (Metallic / Roughness / AO)
    float ao = AOTexture.Sample(AOTextureSampler, input.uv).r; // AO�� �ؽ�ó �� �״�� ���
    float roughness = RoughnessTexture.Sample(RoughnessTextureSampler, input.uv).g;
    float metallic = MetallicTexture.Sample(MetallicTextureSampler, input.uv).b;
    float3 MRAO = float3(ao, roughness, metallic);

    //--- 2. View Space ��� ��� (��� ����) ---
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
    output.material = float4(MRAO, 1.0f);
    
    return output;
}