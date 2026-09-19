#include "cpu_pipeline.h"

namespace gr::rhi
{

using pfnVS = Varyings(*)(VertexAttributes);
using pfnPS = vec4f(*)(Varyings);

// Note: void* stub just to keep symmetric with gfx apis needing the device handle to create resources.
// CPU will not make use of it 
CPUGraphicsPipeline::CPUGraphicsPipeline(void*, const GraphicsPipelineDesc& desc)
  : m_RasterizerState(desc.rasterizerState),
    m_DepthStencilState(desc.depthStencilState),
    m_InputLayoutStates(desc.inputLayoutStates)
{
    fnVert = reinterpret_cast<pfnVS>(desc.VS.pShaderFn);
    fnPixel = reinterpret_cast<pfnPS>(desc.PS.pShaderFn);

    // TODO works with just separated vertex streams and no interleaved data atm
    // Add that support later
    for (const auto& input : m_InputLayoutStates)
    {
        m_InputTypeToBufferIdx[input.eInputType] = input.inputSlot;
    }

}

} // namespace gr::rhi