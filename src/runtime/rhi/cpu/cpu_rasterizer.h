#include "cpu_command_list.h"
#include "cpu_rhi.h"

namespace gr::rhi
{

//void DrawIndexedCPUImpl(const CPUCommandList& cmd, const Buffer& vb, U32 indexCount, U32 firstIndex, int vertexOffset);
//void DrawIndexedTiledCPUImpl(const CPUCommandList& cmd, const Buffer& vb, U32 indexCount, U32 firstIndex, int vertexOffset);
// TODO move this to commandlist
void DrawIndexedCPUImpl(const CPUCommandList& cmd, U32 indexCount, U32 instanceCount, U32 startIndexLocation, int baseVertexLocation, U32 startInstanceLocation);
void DrawIndexedTiledCPUImpl(const CPUCommandList& cmd, U32 indexCount, U32 instanceCount, U32 startIndexLocation, int baseVertexLocation, U32 startInstanceLocation);

} // namespace gr::rhi