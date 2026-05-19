#pragma once

#include "rhi/interface/pipeline.h"

namespace gr::rhi
{

class CPUGraphicsPipeline
{
public:
    CPUGraphicsPipeline(const GraphicsPipelineDesc& desc);

private:
    
};

class CPUComputePipeline
{
public:
    CPUComputePipeline(const ComputePipelineDesc& desc);

private:
};

} // namespace gr::rhi
