#include "cpu_command_list.h"
#include "cpu_rhi.h"

namespace gr::rhi
{

void DrawIndexedCPUImpl(const CPUCommandList& cmd, const Buffer& vb, U32 indexCount, U32 firstIndex, int vertexOffset);


void DrawIndexedTiledCPUImpl(const CPUCommandList& cmd, const Buffer& vb, U32 indexCount, U32 firstIndex, int vertexOffset);

} // namespace gr::rhi