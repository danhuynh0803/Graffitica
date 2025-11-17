#include "rhi/interface/graphics_context.h"
#include "rhi/cpu/cpu_graphics_context.h"

namespace gr::rhi
{

template <typename TGraphicsContext>
std::unique_ptr<TGraphicsContext> IGraphicsContext::Create(void* window)
{
    // TODO create gr asserts to include location of error
    //static_assert(std::is_base_of<IGraphicsContext, TGraphicsContext>::value, "Type needs to be of IGraphicsContext!");
    //return std::make_unique<TGraphicsContext>(window);
    return nullptr;
}

}
