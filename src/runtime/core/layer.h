#pragma once

#include <stdint.h>
#include <string>

namespace gr
{

// For unique behavior for specific layers
enum class LayerFlags : uint8_t
{
    DEFAULT = 0,
    RENDER  = 1 << 0,
    OVERLAY = 1 << 1,
    GAME    = 1 << 2,
};

class Layer
{
public:
    Layer(const std::string& name, LayerFlags flags);
    virtual ~Layer() = default;

    virtual void OnUpdate(double dt) {};
    virtual void OnEvent() {};

private:
    std::string m_Name;
    LayerFlags m_LayerFlags;
};

}
