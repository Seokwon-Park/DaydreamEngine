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
Texture2D Texture: register(t0);
[[vk::combinedImageSampler]][[vk::binding(1)]]
SamplerState TextureSampler : register(s0);


float2 SampleSphericalMap(float3 v)
{
    const float2 invAtan = float2(0.1591f, 0.3183f);
    
    float2 uv = float2(atan2(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    
    uv.y = 1.0f - uv.y;
    
    return uv;
}


PSOutput PSMain(PSInput input) 
{
    PSOutput output = (PSOutput) 0;
    
    float2 uv = SampleSphericalMap(normalize(input.worldPosition));
    float3 color = Texture.Sample(TextureSampler, uv).rgb;
    
    output.color = float4(color, 1.0f);
    
    return output;
}

