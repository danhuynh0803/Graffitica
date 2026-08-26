#pragma once

#include <SDL3/SDL.h>
#include "rhi/interface/graphics_context.h"
#include "cpu_swapchain.h"
#include "core/types.h"

namespace gr::rhi
{

class CPUGraphicsContext final : public IGraphicsContext
{
public:
    static CPUGraphicsContext* CreateInstance(SDL_Window * window);
    static CPUGraphicsContext* GetInstance() { return s_CPUGraphicsContextInstance.get(); }

    CPUGraphicsContext(CPUGraphicsContext&&) = delete;
    CPUGraphicsContext(const CPUGraphicsContext&) = delete;
    ~CPUGraphicsContext() = default;

    virtual void SwapBackBuffers() override {};
    virtual void UpdateBackBuffer(SDL_Surface* surfaceToUpdate) override;
    virtual IRHIRuntime* GetRHIRuntime() override { return m_RHIRuntime.get(); }

    CPUSwapchain* GetSwapchain() const { return m_Swapchain.get(); }
    //CPU_RHI* GetRHIInstance() { return m_CPURHIInstance.get(); }

public:
    CPUGraphicsContext(SDL_Window* window);

private:
    inline static std::unique_ptr<CPUGraphicsContext> s_CPUGraphicsContextInstance;
    std::unique_ptr<CPU_RHI> m_CPURHIInstance;
    std::unique_ptr<IRHIRuntime> m_RHIRuntime;
    std::unique_ptr<CPUSwapchain> m_Swapchain;
};

}
