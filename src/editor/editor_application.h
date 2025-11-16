#pragma once

#include "core/application.h"

namespace gr
{

class EditorApplication : public Application
{
public:
    EditorApplication(const ApplicationProperties& props)
        : Application(props)
    {
        // layer system
    }
};

}
