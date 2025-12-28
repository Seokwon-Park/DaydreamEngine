struct VSInput
{
    float3 position : POSITION;
};

struct VSOutput
{
    float4 position : SV_Position;
};

cbuffer World : register(b0)
{
    matrix world;
    matrix invTranspose;
};

cbuffer LightSpace : register(b1)
{
    matrix lightViewProj;
};

VSOutput VSMain(VSInput input)
{
    VSOutput output;

    float4 worldPos = mul(float4(input.position, 1.0f), world);
    output.position = mul(worldPos, lightViewProj);

    return output;
}
