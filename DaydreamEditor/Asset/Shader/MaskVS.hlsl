struct VSInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 uv : TEXCOORD0;
};

cbuffer World : register(b0)
{
    matrix world;
    matrix invTranspose;
};

cbuffer Camera : register(b1)
{
    matrix view;
    matrix projection;
    matrix viewProjection;
};

struct VSOutput
{
    float4 position : SV_Position;
};


VSOutput VSMain(VSInput input)
{
    VSOutput output = (VSOutput) 0;
    
    float4 position = float4(input.position, 1.0f);
    position = mul(position, world);
    float3 dummy = input.normal - input.normal + input.tangent - input.tangent + float3(input.uv - input.uv, 0.0f);
    float4 dummyy = float4(dummy, 0.0f);
    output.position = mul(position, viewProjection) + dummyy - dummyy;
    
    return output;
}
