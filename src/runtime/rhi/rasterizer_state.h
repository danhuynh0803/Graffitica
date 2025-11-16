#pragma once

enum class CULL_MODE
{
    CULL_MODE_NONE = 0,
    CULL_MODE_FRONT,
    CULL_MODE_BACK,
};

enum class FILL_MODE
{
    FILL_MODE_WIREFRAME = 0,
    FILL_MODE_SOLID,
};

struct RasterizerState
{
    FILL_MODE   fillMode = FILL_MODE::FILL_MODE_SOLID;
    CULL_MODE   cullMode = CULL_MODE::CULL_MODE_BACK;
    bool        frontCounterClockwise = true;
    int         depthBias = 0;
    float       depthBiasClamp = 0.f;
    float       slopeScaledDepthBias = 0.f;
    bool        depthClipEnable = true;
};
