#include "core/layer.h"

namespace gr
{

Layer::Layer(const std::string& name, LayerFlags flags)
    : m_Name(name), m_LayerFlags(flags)
{}

}
