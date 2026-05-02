struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
};

PSInput VSMain(float4 position : POSITION, float4 color : COLOR, float3 normal : NORMAL, float2 uv : TEXCOORD0)
{
    PSInput result;

    result.position = float4(position.xyz, 1.0);
    result.color = color; //float4(1.0, 0.0, 1.0, 1.0);
    result.normal = normal;
    result.uv = uv;

    return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    //return input.color; //* float4(1.0, 0.0, 1.0f, 1.0f);
    return float4(input.uv, 1.0, 1.0);
}
