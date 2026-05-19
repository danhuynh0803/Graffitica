#pragma once

#include "rhi/rasterizer_state.h"

namespace gr::rhi
{

struct RHIShader
{
    // On CPU RHI, this will be fn pointer to shader functions
    void* pShaderFn;

    void* pShaderByteCode;
    U32 byteCodeLength;
};

/*
RHI Pipeline Handles
*/
struct GraphicsPipelineDesc
{
    RasterizerState rasterizerState;
    DepthStencilState depthStencilState;
    //InputLayoutState inputLayoutState;
    RHIShader vertexShader;
    RHIShader pixelShader;
};

struct ComputePipelineDesc
{
    RHIShader computeShader;
};

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