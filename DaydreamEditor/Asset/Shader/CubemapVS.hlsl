struct VSInput
{
    float3 position : POSITION;
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
    float3 worldPosition : POSITION;
};

VSOutput VSMain(VSInput input)
{
    VSOutput output = (VSOutput) 0;
    output.position = float4(input.position, 0.0f);
    output.worldPosition = input.position;

    return output;
}
