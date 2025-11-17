#pragma once

#include <memory>

namespace gr::rhi
{

class IGraphicsContext
{
public:
    //IGraphicsContext(void* window);
    virtual ~IGraphicsContext() = default;

    template <typename TGraphicsContext>
    static std::unique_ptr<TGraphicsContext> Create(void* window);

protected:

};

}
