struct VSInput
{
    float3 position : POSITION;
    float4 color : COLOR0;
};

cbuffer VS_Cbuffer : register(b0)
{
    matrix viewProjection;
}

struct VSOutput
{
    float4 position : SV_Position;
    float4 color : COLOR0;
};

VSOutput VSMain(VSInput input)
{
    VSOutput output = (VSOutput) 0;
    output.position = mul(float4(input.position, 1.0), viewProjection); // 월드 변환 생략
    output.color = input.color;
    return output;
}
