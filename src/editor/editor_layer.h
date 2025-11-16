#pragma once

#include "core/layer.h"

namespace gr
{

class EditorLayer : public Layer
{
public:
    EditorLayer();
    virtual ~EditorLayer() = default;

    //virtual void OnAttach() override;
    //virtual void OnDetach() override;
    virtual void OnUpdate(double dt) override;
    //virtual void OnOverlayUpdate() {};
    //virtual void OnEvent() override;

private:
    std::string m_Name;
};


}
