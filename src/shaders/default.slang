
cbuffer CameraData : register(b0)
{
    float4x4 view;
    float4x4 proj;
    float4x4 viewProj;
}

struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

Texture2D gTexture : register(t0);
SamplerState gSamplerLinear : register(s0);

[shader("vertex")]
PSInput VSMain(float4 position : POSITION, float4 color : COLOR, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
    PSInput o;

    o.position = mul(viewProj, float4(position.xyz, 1.0));
    o.color = color; //float4(1.0, 0.0, 1.0, 1.0);
    o.normal = normal;
    o.uv = uv;

    return o;
}

[shader("fragment")]
float4 PSMain(PSInput input) : SV_TARGET
{
    return gTexture.Sample(gSamplerLinear, input.uv);
}


//======================================================================================
//======================================================================================
//======================================================================================
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
    //o.color = input.color;
    // TODO
    //o.normal = input.normal;
    //o.uv = input.uv;

    return o;
}

float4 PixelImpl(VSOutput input)
{
    return float4(1,0,1,1); //input.color;
}

uniform StructuredBuffer<VSInput> RVertexInput;
uniform RWStructuredBuffer<VSOutput> RWVertexOutput;

[shader("compute")]
[numthreads(1,1,1)]
void cpu_VSMain(uint3 tid : SV_DispatchThreadID)
{
    //RWVertexOutput[tid.x] = VertexImpl(RVertexInput[tid.x]);
}

uniform StructuredBuffer<VSOutput> RPixelInput;
uniform RWStructuredBuffer<float4> RWPixelOutput;

[shader("compute")]
[numthreads(1,1,1)]
void cpu_PSMain(uint3 tid : SV_DispatchThreadID)
{
    //RWPixelOutput[tid.x] = PixelImpl(RPixelInput[tid.x]);
}
