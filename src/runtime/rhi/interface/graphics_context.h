#pragma once

#include <memory>
#include <SDL3/SDL.h>
#include "rhi/interface/rhi.h"

namespace gr::rhi
{

class IGraphicsContext
{
public:
    virtual ~IGraphicsContext() = default;
    virtual void SwapBackBuffers() = 0;
    virtual void UpdateBackBuffer(SDL_Surface* surfaceToUpdate) = 0;

    template <typename TGraphicsContext>
    static IGraphicsContext* Create(SDL_Window* window)
    {
        // TODO create gr asserts to include location of error
        static_assert(std::is_base_of<IGraphicsContext, TGraphicsContext>::value, "Type needs to be of IGraphicsContext!");
        TGraphicsContext::CreateInstance(window);
        return TGraphicsContext::GetInstance();
    }

    //static ISwapchain* GetSwapchain() { return GetInstance()->GetSwapchain(); }

protected:

};

}
