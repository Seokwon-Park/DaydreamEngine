struct VSInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
};

cbuffer World : register(b0)
{
    matrix world;
    matrix invTranspose;
};

cbuffer Camera : register(b1)
{
    matrix viewProjection;
};

struct VSOutput
{
    float4 position : SV_Position;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
};

VSOutput VSMain(VSInput input)
{
    VSOutput output = (VSOutput) 0;
    float4 position = float4(input.position, 1.0f);
    position = mul(position, world);
    output.position = mul(position, viewProjection);
    
    float4 normal = float4(input.normal, 1.0f);
    output.normal = mul(normal, invTranspose).xyz;
    output.normal = normalize(output.normal);
    output.uv = input.uv;
    return output;
}
