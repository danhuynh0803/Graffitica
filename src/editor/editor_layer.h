#pragma once

#include "core/layer.h"
#include "core/event.h"

namespace gr
{

class EditorLayer : public Layer
{
public:
    EditorLayer(const std::string& name = "EditorLayer");
    virtual ~EditorLayer() = default;

    //virtual void OnAttach() override;
    //virtual void OnDetach() override;
    virtual void OnUpdate(double dt) override;
    //virtual void OnOverlayUpdate() {};
    virtual void OnEvent(Event& event) override;

private:
    std::string m_Name;
};


}
