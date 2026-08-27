#pragma once

#include <string>
#include "core/types.h"
#include "formats.h"

namespace gr::rhi
{

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

enum class DEPTH_COMPARISON_FUNC
{
    COMPARISON_NONE = 0,
    COMPARISON_FUNC_NEVER = 1,
    COMPARISON_FUNC_LESS = 2,
    COMPARISON_FUNC_EQUAL = 3,
    COMPARISON_FUNC_LESS_EQUAL = 4,
    COMPARISON_FUNC_GREATER = 5,
    COMPARISON_FUNC_NOT_EQUAL = 6,
    COMPARISON_FUNC_GREATER_EQUAL = 7,
    COMPARISON_FUNC_ALWAYS = 8
};

enum class DEPTH_WRITE_MASK
{
    DEPTH_WRITE_MASK_ZERO = 0,
    DEPTH_WRITE_MASK_ALL = 1
};

// TODO remove/rename to DepthStencilState
struct DepthStencilDesc
{
    bool                    depthEnable;
    DEPTH_WRITE_MASK        depthWriteMask;
    DEPTH_COMPARISON_FUNC   depthFunc;
    bool                    stencilEnable;
    U8                      stencilReadMask;
    U8                      stencilWriteMask;

    // TODO StencilOp Desc
    // most likely never gonna be used by CPU case,
    // but reminder that those fields are missing
};

struct DepthStencilState
{
    bool                    depthEnable;
    DEPTH_WRITE_MASK        depthWriteMask;
    DEPTH_COMPARISON_FUNC   depthFunc;
    bool                    stencilEnable;
    U8                      stencilReadMask;
    U8                      stencilWriteMask;
};

enum class InputClass
{
    PER_VERTEX   = 0,
    PER_INSTANCE = 1,
};

struct InputLayoutState
{
    std::string semanticName;
    U32         semanticIndex;
    GrFormat    format;
    U32         inputSlot;
    U32         alignedByteOffset;
    InputClass  inputSlotClass;
    U32         instanceDataStepRate;
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

} // namespace gr::rhi