struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

[shader("vertex")]
PSInput VSMain(float4 position : POSITION, float4 color : COLOR) //, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
    PSInput result;

    result.position = float4(position.xyz, 1.0);
    result.color = color; //float4(1.0, 0.0, 1.0, 1.0);
    return result;
}

[shader("fragment")]
float4 PSMain(PSInput input) : SV_TARGET
{
    return input.color; //* float4(1.0, 0.0, 1.0f, 1.0f);
}
