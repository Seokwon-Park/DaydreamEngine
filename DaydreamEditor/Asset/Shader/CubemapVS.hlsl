struct VSInput
{
    float3 position : POSITION;
};

//cbuffer World : register(b0)
//{
//    matrix world;
//    matrix invTranspose;
//};

cbuffer Camera : register(b0)
{
    matrix viewProjection;
};

struct VSOutput
{
    float4 position : SV_Position;
    float3 worldPosition : POSITION;
};

VSOutput VSMain(VSInput input)
{
    VSOutput output = (VSOutput) 0;
    output.position = mul(float4(input.position, 1.0f),viewProjection);
    output.worldPosition = input.position;

    return output;
}
