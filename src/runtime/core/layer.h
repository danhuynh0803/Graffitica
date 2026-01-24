#pragma once

#include <stdint.h>
#include <string>

#include "core/event.h"

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
    virtual void OnEvent(Event& event) {};

private:
    std::string m_Name;
    LayerFlags m_LayerFlags;
};

}
