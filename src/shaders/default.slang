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

//======================================================================================
// CPU compute part

struct VSInput
{
    float3 position;
    float4 color;
    float3 normal;
    float2 uv;
}

// Note: Slang does not support HLSL semantics for CPU compilation
struct VSOutput
{
    float4 position;
    float4 color;
    float3 normal;
    float2 uv;
};

VSOutput VertexImpl(VSInput input)
{
    VSOutput o;
    o.position = float4(input.position, 1.0);
    o.color = input.color;
    // TODO
    //o.normal = input.normal;
    //o.uv = input.uv;

    return o;
}

float4 PixelImpl(VSOutput input)
{
    return input.color;
}

uniform StructuredBuffer<VSInput> RVertexInput;
uniform RWStructuredBuffer<VSOutput> RWVertexOutput;

[shader("compute")]
[numthreads(1,1,1)]
void cpu_VSMain(uint3 tid : SV_DispatchThreadID)
{
    RWVertexOutput[tid.x] = VertexImpl(RVertexInput[tid.x]);
}

uniform StructuredBuffer<VSOutput> RPixelInput;
uniform RWStructuredBuffer<float4> RWPixelOutput;

[shader("compute")]
[numthreads(1,1,1)]
void cpu_PSMain(uint3 tid : SV_DispatchThreadID)
{
    RWPixelOutput[tid.x] = PixelImpl(RPixelInput[tid.x]);
}
