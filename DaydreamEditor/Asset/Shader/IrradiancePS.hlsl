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

float radicalInverse_VdC(uint bits)
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

float2 hammersley(uint i, uint N)
{
    return float2(float(i) / float(N), radicalInverse_VdC(i));
}

float3 importanceSampleGGX(float2 Xi, float3 N)
{
    float a = 1.0; // Roughness, Ȯ�걤������ 1.0 (���� ��ģ ����)
    float phi = 2.0 * PI * Xi.x;
    float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a * a - 1.0) * Xi.y));
    float sinTheta = sqrt(1.0 - cosTheta * cosTheta);

    // ���� ��ǥ�迡�� ��ī��Ʈ ��ǥ��� ��ȯ
    float3 H;
    H.x = cos(phi) * sinTheta;
    H.y = sin(phi) * sinTheta;
    H.z = cosTheta;

    // ź��Ʈ ���� ���͸� ���� �������� ��ȯ
    float3 up = abs(N.z) < 0.999 ? float3(0.0, 0.0, 1.0) : float3(1.0, 0.0, 0.0);
    float3 tangent = normalize(cross(up, N));
    float3 bitangent = cross(N, tangent);

    float3 samplefloat = tangent * H.x + bitangent * H.y + N * H.z;
    return normalize(samplefloat);
}


PSOutput PSMain(PSInput input)
{
    PSOutput output = (PSOutput) 0;

    float3 N = normalize(input.worldPosition);
    float3 irradiance = float3(0.0f, 0.0f, 0.0f);
    
    const uint numSamples = 4096;
    
    uint width;
    uint height;
    uint maxMipLevel;
    TextureCubemap.GetDimensions(0, width, height, maxMipLevel);
       
    for (uint i = 0u; i < numSamples; ++i)
    {
        float2 xi = hammersley(i, numSamples);
        float3 h = importanceSampleGGX(xi, N);
        
        // ���� ���ø�. L = 2 * dot(N, h) * h - N; ������,
        // Ȯ�� �ݻ��� ��� ���� ���� ��ü�� ���� ���� L�� ��
        float3 L = normalize(h);

        float ndotl = max(dot(N, L), 0.0);
        if (ndotl > 0.0)
        {
            // 1. PDF (Ȯ�� �е� �Լ�) ���
            // importanceSampleGGX(a=1.0)�� �ڻ��� ���� ���ø��� �ٻ�ġ�̹Ƿ�
            // �ڻ��� ���� ���ø��� PDF�� (NdotL / PI)�� ����մϴ�.
            float pdf = max(ndotl, 0.0001) / PI; // 0���� ������ �� ����

            // 2. Mip 0 �ؼ��� �ָ��� �ޱ� ���
            float solidAngleTexel = 4.0 * PI / (6.0 * float(width * width));

            // 3. ���� ������ �ָ��� �ޱ� ���
            float solidAngleSample = 1.0 / (float(numSamples) * pdf);

            // 4. ���� LOD (Mip Level) ��� (GPU Gems 3)
            // ���� ����(solidAngleSample)�� �ؼ� ����(solidAngleTexel)����
            // �ξ� ũ�� (== ���̾��ö��� �ĺ�) ���� Mip ������ ���
            float lod = 0.5 * log2(solidAngleSample / solidAngleTexel);
            lod = max(0.0, lod); // ���� ����

            // 5. ���� lod�� ����� ���ø�
            irradiance += TextureCubemap.SampleLevel(TextureCubemapSampler, L, lod).rgb;
        }
    }
    irradiance = irradiance / float(numSamples) * PI;
    
    
    //float3 N = normalize(input.worldPosition);

    //float3 irradiance = 0.0f;
    
    //uint width;
    //uint height;
    //uint maxMipLevel;
    //TextureCubemap.GetDimensions(0, width, height, maxMipLevel);
    //// tangent space calculation from origin point
    //float3 up = float3(0.0, 1.0, 0.0);
    //float3 right = normalize(cross(up, N));
    //up = normalize(cross(N, right));
       
    //float sampleDelta = 0.025;
    //float nrSamples = 0.0f;
    //for (float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
    //{
    //    for (float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
    //    {
    //        // spherical to cartesian (in tangent space)
    //        float3 tangentSample = float3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
    //        // tangent space to world
    //        float3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N;

    //        irradiance += TextureCubemap.SampleLevel(TextureCubemapSampler, sampleVec, maxMipLevel/2.0f).rgb * cos(theta) * sin(theta);
    //        nrSamples++;
    //    }
    //}
    //irradiance = PI * irradiance * (1.0 / float(nrSamples));
    
    output.color = float4(irradiance, 1.0);
    return output;
}