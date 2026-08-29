#pragma once

#include <vector>
#include <SDL3/SDL.h>
#include "rhi/interface/swapchain.h"
#include "rhi/resource.h"
#include "rhi/formats.h"
#include "cpu_rhi.h"

namespace gr::rhi
{

class CPUSwapchain final : public ISwapchain
{
public:
    CPUSwapchain(const SwapchainProperties& props, CPU_RHI* rhiInstance);
    ~CPUSwapchain() = default;
    void UpdateBackBuffer(SDL_Surface* surfaceToUpdate);

public:
    virtual void Present() override { /* TODO no-op on cpu because CPUGfxContext is handling it */ }

private:
    CPU_RHI* m_RHIInstance;
};

}
