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

PSOutput PSMain(PSInput input)
{
    PSOutput output = (PSOutput) 0;
    
    float3 color = TextureCubemap.SampleLevel(TextureCubemapSampler, input.worldPosition, 0.0).rgb;
    float gamma = 2.2;
    color = color / (color + float3(1.0, 1.0, 1.0)); // Reinhard Tonemapping
    color = pow(color, float3(1.0 / gamma, 1.0 / gamma, 1.0 / gamma)); // Gamma Correction
  
    output.color = float4(color, 1.0f);
    
    return output;
}