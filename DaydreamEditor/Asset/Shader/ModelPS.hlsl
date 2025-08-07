

struct PSInput
{
    float4 position : SV_Position;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
};

cbuffer Light : register(b0)
{
    float3 eyeWorld;
    float3 lightPos;
    float3 lightDir;
};

struct PSOutput
{
    float4 color : SV_Target0;
};

//[[vk::combinedImageSampler]][[vk::binding(1)]]
//Texture2D Texture : register(t0);
//[[vk::combinedImageSampler]][[vk::binding(1)]]
//SamplerState TextureSampler : register(s0);

PSOutput PSMain(PSInput input)
{
    PSOutput output = (PSOutput) 0;
    //output.color = Texture.Sample(TextureSampler, input.uv) + input.color;
    float3 lightVec = float3(1.0f, 1.0f, -1.0f);
 
    float lightSt = dot(normalize(lightVec), input.normal);
    output.color = float4(1.0f, 1.0f, 1.0f, 1.0f) * lightSt;
    return output;
}