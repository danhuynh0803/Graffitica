#pragma once

#include <memory>
#include <SDL3/SDL.h>

namespace gr::rhi
{

class IGraphicsContext
{
public:
    //IGraphicsContext(void* window);
    virtual ~IGraphicsContext() = default;
    //virtual void PollEvents() = 0;
    virtual void UpdateBackBuffer(SDL_Surface* surfaceToUpdate) = 0;

    template <typename TGraphicsContext>
    static IGraphicsContext* Create(SDL_Window* window)
    {
        // TODO create gr asserts to include location of error
        static_assert(std::is_base_of<IGraphicsContext, TGraphicsContext>::value, "Type needs to be of IGraphicsContext!");
        //return std::make_unique<TGraphicsContext>(window);
        TGraphicsContext::CreateInstance(window);
        return TGraphicsContext::GetInstance();
    }

protected:

};

}
