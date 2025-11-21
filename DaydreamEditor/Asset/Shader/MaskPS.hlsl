struct PSInput
{
    float4 position : SV_Position;
};

struct PSOutput
{
    uint color : SV_Target0;
};

PSOutput PSMain(PSInput input)
{
    PSOutput output = (PSOutput) 0;
    
    output.color = 1;
    
    return output;
}