#pragma once

#include <vector>
#include "rhi/rasterizer_state.h"
#include "rhi/resource.h"

namespace gr::rhi
{

struct RHIShader
{
    // On CPU RHI, this will be fn pointer to shader functions
    const void* pShaderFn;

    const void* pShaderByteCode;
    U32 byteCodeLength;
};

/*
RHI Pipeline Handles
*/
// Note: Reminder to not change to being enum class
// so that it implicitly converts to U32 without casting
enum ShaderStageFlagBits : U32
{
    VERTEX_BIT = BIT(0),
    HULL_BIT = BIT(1),
    DOMAIN_BIT = BIT(2),
    GEOMETRY_BIT = BIT(3),
    PIXEL_BIT = BIT(4),
    COMPUTE_BIT = BIT(5),
    ALL_GRAPHICS = 0x0000001F,
    ALL = 0x7FFFFFFF,
    //RAYGEN_BIT = 0x00000100,
    //ANY_HIT_BIT = 0x00000200,
    //CLOSEST_HIT_BIT = 0x00000400,
    //MISS_BIT = 0x00000800,
    //INTERSECTION_BIT = 0x00001000,
    //CALLABLE_BIT = 0x00002000,
    AMPLIFICATION_BIT = BIT(6), //0x00000040,
    MESH_BIT = BIT(7), //0x00000080,
};

struct DescriptorSetBinding
{
    U32 binding;
    DescriptorResourceType descriptorType;
    U32 descriptorCount;
    ShaderStageFlagBits stageFlags;
};

struct PipelineLayout
{
    std::vector<DescriptorSetBinding> descriptorSetBindings;
};

enum class PipelineBindPoint
{
    Graphics = 0,
    Compute,
};

struct GraphicsPipelineDesc
{
    RasterizerState rasterizerState;
    DepthStencilState depthStencilState;
    PipelineLayout pipelineLayout;
    std::vector<InputLayoutState> inputLayoutStates;
    RHIShader VS;
    RHIShader PS;
};

struct ComputePipelineDesc
{
    
    RHIShader CS;
};

typedef U64 GraphicsPipelineHandle;
typedef U64 ComputePipelineHandle;

struct RHIGraphicsPipeline
{
    //RHIGraphicsPipeline(void* pNativePipeline) : pNativePipeline(pNativePipeline) {}
//private:
    void* pNativePipeline;
    //friend RHIGraphicsPipelineHandle CreateGraphicsPipeline_CPU(const GraphicsPipelineDesc& desc);
};


struct RHIComputePipeline
{
    void* pNativePipeline;
};

using RHIGraphicsPipelineHandle = RHIGraphicsPipeline*;
using RHIComputePipelineHandle  = RHIComputePipeline*;

} // namespace gr::rhi
