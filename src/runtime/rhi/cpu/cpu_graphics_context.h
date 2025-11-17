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
    static std::shared_ptr<CPUGraphicsContext> CreateInstance(SDL_Window* window);
    static CPUGraphicsContext* GetInstance() { return s_CPUGraphicsContextInstance.get(); }

    CPUGraphicsContext(CPUGraphicsContext&&) = delete;
    CPUGraphicsContext(const CPUGraphicsContext&) = delete;
    ~CPUGraphicsContext() = default;

    virtual void UpdateBackBuffer(SDL_Surface* surfaceToUpdate) override;

    //CPUSwapchain* GetSwapchain() const { return m_Swapchain.get(); }
    CPUSwapchain* GetSwapchain() const { return m_Swapchain.get(); }

public:
    CPUGraphicsContext(SDL_Window* window);

private:
    inline static std::shared_ptr<CPUGraphicsContext> s_CPUGraphicsContextInstance;
    //std::unique_ptr<CPUSwapchain> m_Swapchain;
    std::shared_ptr<CPUSwapchain> m_Swapchain;
};

}
