#pragma once
#include "core/types.h"
#include "util/math/vector.h"
#include "rhi/resource.h"
#include "rhi/command_buffer.h"
#include "developer/profiler/profiler.h"

namespace gr::renderer
{

class Renderer
{
public:
    void Init();
    void Draw(const rhi::CommandBuffer& cmd, const Buffer& vb, U32 vertexCount, U32 firstVertex);

private:

};

} // namespace gr::renderer
