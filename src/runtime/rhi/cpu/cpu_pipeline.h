#pragma once

#include <functional>
#include "rhi/interface/pipeline.h"

// TODO move this to cpu-rhi
#include "rhi/shader.h"

namespace gr::rhi
{

//class CPUPipeline
//{
//public:
//    CPUPipeline(const GraphicsPipelineDesc& desc);
//    CPUPipeline(const ComputePipelineDesc& desc);
//private:
//};

class CPUGraphicsPipeline
{
public:
    CPUGraphicsPipeline(void*, const GraphicsPipelineDesc& desc);

    friend class CPU_RHI;
    friend class CPUCommandList;

private:
    // Graphics State
    RasterizerState m_RasterizerState;
    DepthStencilState m_DepthStencilState;
    std::vector<InputLayoutState> m_InputLayoutStates;
    // Shader functions
    // TODO replace with slang
    std::function<Varyings(VertexAttributes)> fnVert;
    std::function<vec4f(Varyings)> fnPixel;
};

class CPUComputePipeline
{
public:
    CPUComputePipeline(const ComputePipelineDesc& desc);

private:
    ComputePipelineDesc m_Desc;
};

} // namespace gr::rhi
