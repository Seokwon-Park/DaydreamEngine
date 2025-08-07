struct VSInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
};

cbuffer Camera : register(b0)
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
    output.position = mul(float4(input.position, 1.0), viewProjection); // 월드 변환 생략
    output.normal = input.normal;
    output.uv = input.uv;
    return output;
}
