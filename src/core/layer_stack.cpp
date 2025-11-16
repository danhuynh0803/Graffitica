#include "core/layer_stack.h"
#include "core/layer.h"

namespace gr
{

void LayerStack::PushLayer(Layer* layer)
{
    m_Layers.emplace(m_Layers.begin() + m_StackIndex, layer);
    m_StackIndex++;
}

void LayerStack::PopLayer(Layer* layer)
{
    auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_StackIndex, layer);
    if (it != m_Layers.begin() + m_StackIndex)
    {
        m_Layers.erase(it);
        delete *it;
        m_StackIndex--;
    }
}

}
