#pragma once

#include "rhi/interface/graphics_context.h"
#include "cpu_swapchain.h"
#include "core/types.h"

namespace gr::rhi
{

class CPUGraphicsContext final : public IGraphicsContext
{
public:
    CPUGraphicsContext(void* window);

    //CPUSwapchain* GetSwapchain() const { return m_Swapchain.get(); }
    std::weak_ptr<CPUSwapchain> GetSwapchain() const { return m_Swapchain; }

private:
    //std::unique_ptr<CPUSwapchain> m_Swapchain;
    std::shared_ptr<CPUSwapchain> m_Swapchain;
};

}
