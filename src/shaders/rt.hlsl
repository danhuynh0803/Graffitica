//#include "shared/rt_payload.h"

struct Payload
{
    float3 color;
    bool isSpecular;
    bool isMissed;
};

// Store AS as an SRV
RaytracingAccelerationStructure inSceneAS : register(t0);
RWTexture2D<float4> outUAV : register(u0);

// TODO pass CBV for camera and environment info
// hardcode to get working
static const float3 cameraOrigin = float3(0,0,1);
static const float3 background = float3(0.24, 0.44, 0.72);

[shader("raygeneration")]
void RayGeneration()
{
    uint2 idx = DispatchRaysIndex().xy;
    // width, height, depth from DISPATCH_RAYS_DESC
    float2 sizeXY = DispatchRaysDimensions().xy;

    float2 uv = idx / sizeXY;
    const float fov = 1.8f;
    float3 target = float3(
        (uv.x*2 - 1) * fov * (sizeXY.x / sizeXY.y),
        (1 - uv.y) * 4 - 2 + cameraOrigin.y,
        0
    );

    RayDesc ray;
    ray.Origin = cameraOrigin;
    ray.Direction = target - cameraOrigin;
    ray.TMin = 0.001;
    ray.TMax = 1000;

    Payload payload;
    payload.isSpecular = true;
    payload.isMissed = false;

    TraceRay(
        inSceneAS,
        RAY_FLAG_NONE,
        0xFF,   // InstanceOcclusionMask
        0,      // RayContributionToHitGroupIndex
        0,      // MultiplierForGeometryContributionToHitGroupIndex
        0,      // MissShaderIndex
        ray,
        payload
    );

    outUAV[idx] = float4(payload.color, 1.0f);
}

[shader("miss")]
void Miss(inout Payload payload)
{
    payload.color = background;
    payload.isMissed = true;
}

[shader("closesthit")]
void ClosestHit(inout Payload payload, BuiltInTriangleIntersectionAttributes attribs)
{
    float2 uv = attribs.barycentrics;
    payload.color = float3(uv, 1.0);
}
