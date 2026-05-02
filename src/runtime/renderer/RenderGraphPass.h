#pragma once

#include "rhi/rasterizer_state.h"

namespace gr
{

struct PassDesc
{
    // TODO
};

class RenderGraphPass
{
public:
    //void SetShaderState(ShaderHandle handle);
    void SetDepthStencilState(const DepthStencilDesc& desc);
    void SetRasterizerState(const RasterizerState& desc);

private:

};

} // namespace gr
