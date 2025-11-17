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

    template <typename TGraphicsContext>
    static std::unique_ptr<TGraphicsContext> Create(SDL_Window* window)
    {
        // TODO create gr asserts to include location of error
        static_assert(std::is_base_of<IGraphicsContext, TGraphicsContext>::value, "Type needs to be of IGraphicsContext!");
        return std::make_unique<TGraphicsContext>(window);
    }

protected:

};

}
