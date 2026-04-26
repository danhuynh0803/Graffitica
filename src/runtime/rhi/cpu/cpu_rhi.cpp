#include "rhi/cpu/cpu_rhi.h"

namespace gr::renderer::rhi
{

// TODO separate to modules
//export module RenderModule;
class RasterizerState;
//template<typename T> class ImageView;
class Buffer;
class Framebuffer;

/*
    template<typename FORMAT>
    void CpuRHI::Clear(ImageView<FORMAT>& view, const vec4f& clearColor)
    {
        static_assert(std::is_base_of<ColorFormat, FORMAT>::value, "FORMAT must inherit from a COLOR FORMAT");
        auto size = view.width * view.height;
        //std::fill_n(view.data, size, FORMAT::to(clearColor));
        //std::fill(view.colorData.begin(), view.colorData.end(), clearColor);
        //std::memset(view.colorData, clearColor, size);
        for (int i = 0; i < size; ++i)
        {
            view.colorData[i] = clearColor;
        }
    }

    template<typename FORMAT>
    void CpuRHI::Clear(const ImageView<FORMAT>& view, float clearDepth)
    {
        static_assert(std::is_base_of<DepthFormat, FORMAT>::value, "FORMAT must inherit from a DEPTH FORMAT");
        auto size = view.width * view.height;
        std::fill_n(view.data, size, FORMAT::to(clearDepth));
    }

    // TODO extent for src and dst regions
    //void Blit(const ImageView& dst, const ImageView& src, int xOffset = 0, int yOffset = 0, int zOffset = 0);

    //void Draw(const ImageView& view, const Buffer& vb, U32 vertexCount, U32 firstVertex);

    void CpuRHI::DrawIndexedImmediate(const CommandBuffer& cmd, const Buffer& vb, U32 indexCount, U32 firstIndex, int vertexOffset);

    void CpuRHI::DrawIndexedTiled(const CommandBuffer& cmd,
        const Buffer& vb,
        U32 indexCount, U32 firstIndex, int vertexOffset);
*/

}

namespace gr::rhi::cpu
{

    BufferHandle CreateBuffer_CPU(const BufferDesc& desc)
    {
        std::cout << "CPU CreateBuffer called with size: " << desc.size << " and usageFlags: " << desc.usageFlags << std::endl;
        return BufferHandle();
    }

    void SetVertexBuffers_CPU(CommandList& cmdList, U32 numViews, BufferHandle views[])
    {
        std::cout << "CPU SetVertexBuffers called with numViews: " << numViews << std::endl;
    }

    RHIFunctionTable CPUTable = {
        CreateBuffer_CPU,
        SetVertexBuffers_CPU
    };

} // namespace gr::rhi::cpu