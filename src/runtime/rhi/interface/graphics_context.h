#pragma once

#include <memory>
#include <SDL3/SDL.h>
#include "rhi/interface/rhi.h"
#include "rhi/interface/swapchain.h"

namespace gr::rhi
{

class IGraphicsContext
{
public:
    virtual ~IGraphicsContext() = default;
    virtual void SwapBackBuffers() = 0;
    virtual void UpdateBackBuffer(SDL_Surface* surfaceToUpdate) = 0;
    static IGraphicsContext* GetInstance() { return s_GraphicsContextInstance.get(); }

    template <typename TGraphicsContext>
    static IGraphicsContext* Create(SDL_Window* window)
    {
        // TODO create gr asserts to include location of error
        static_assert(std::is_base_of<IGraphicsContext, TGraphicsContext>::value, "Type needs to be of IGraphicsContext!");
        //TGraphicsContext::CreateInstance(window);
        s_GraphicsContextInstance = std::make_unique<TGraphicsContext>(window);
        return s_GraphicsContextInstance.get(); //TGraphicsContext::GetInstance();
    }

    virtual ISwapchain* GetSwapchain() = 0;
    virtual RHIContext* GetRHIContext() = 0;

protected:
    inline static std::unique_ptr<IGraphicsContext> s_GraphicsContextInstance;
};

}
