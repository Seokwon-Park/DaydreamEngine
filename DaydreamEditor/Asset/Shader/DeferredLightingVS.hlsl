struct VSInput
{
    float3 position : POSITION;
    float2 uv : TEXCOORD0;
};

struct VSOutput
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD0;
};


VSOutput VSMain(VSInput input)
{
    VSOutput output = (VSOutput) 0;
    
    output.position = float4(input.position, 1.0f);
    output.uv = input.uv;
    
    return output;
}
