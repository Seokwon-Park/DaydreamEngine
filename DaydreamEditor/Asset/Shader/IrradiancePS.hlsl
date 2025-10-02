struct PSInput
{
    float4 position : SV_Position;
    float3 worldPosition : POSITION;
};

struct PSOutput
{
    float4 color : SV_Target0;
};

[[vk::combinedImageSampler]][[vk::binding(1)]]
TextureCube TextureCubemap : register(t0);
[[vk::combinedImageSampler]][[vk::binding(1)]]
SamplerState TextureCubemapSampler : register(s0);

static const float PI = 3.14159265359;

float2 Hammersley(uint i, uint N)
{
    uint bits = i;
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    float radicalInverse = float(bits) * 2.3283064365386963e-10; // 1/2^32
    return float2(float(i) / float(N), radicalInverse);
}

float3 ImportanceSampleHemisphere(float2 Xi)
{
    float phi = Xi.y * 2.0 * PI;
    float cosTheta = sqrt(1.0 - Xi.x);
    float sinTheta = sqrt(1.0 - cosTheta * cosTheta);
    
    float3 h;
    h.x = cos(phi) * sinTheta;
    h.y = sin(phi) * sinTheta;
    h.z = cosTheta;
    return h;
}


PSOutput PSMain(PSInput input)
{
    PSOutput output = (PSOutput) 0;
    
    //float3 irradiance = float3(0.0f, 0.0f, 0.0f);
  
    //float3 up = float3(0.0f, 1.0f, 0.0f);
    //float3 right = normalize(cross(up, normal));
    //up = normalize(cross(normal, right));
  
    //float sampleDelta = 0.025;
    //float nrSamples = 0.0;
    //for (float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
    //{
    //    for (float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
    //    {
    //        // spherical to cartesian (in tangent space)
    //        float3 tangentSample = float3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
    //        // tangent space to world
    //        float3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal;

    //        irradiance += TextureCubemap.Sample(TextureCubemapSampler, sampleVec).rgb * cos(theta) * sin(theta);
    //        nrSamples++;
    //    }
    //}
    //irradiance = PI * irradiance * (1.0 / float(nrSamples));
    
    float3 N = normalize(input.worldPosition);

    // Tangent Space를 만드는 안정적인 방법
    float3 right;
    if (abs(N.y) > 0.999f)
    {
        right = float3(1.0f, 0.0f, 0.0f);
    }
    else
    {
        right = normalize(cross(float3(0.0f, 1.0f, 0.0f), N));
    }
    float3 up = cross(N, right);

    float3 irradiance = float3(0.0f, 0.0f, 0.0f);
    
    const uint SAMPLE_COUNT = 512u; // 수백 개면 충분!
    for (uint i = 0u; i < SAMPLE_COUNT; ++i)
    {
        float2 Xi = Hammersley(i, SAMPLE_COUNT);
        float3 tangentSample = ImportanceSampleHemisphere(Xi);
        
        // Tangent Space에서 World Space로 변환
        float3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N;

        // HLSL에서는 .Sample(샘플러, 좌표) 형식으로 텍스처를 읽어옵니다.
        irradiance += TextureCubemap.Sample(TextureCubemapSampler, sampleVec).rgb;
    }
    
    // 최종 결과 계산
    irradiance = PI * irradiance * (1.0f / float(SAMPLE_COUNT));
    
    output.color = float4(irradiance, 1.0f);
    return output;
}