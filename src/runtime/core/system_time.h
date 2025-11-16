#pragma once

#include <chrono>

namespace gr
{

class SystemTime
{
public:
    SystemTime();
    ~SystemTime() = default;
    void    Update();
    double  deltaTime;
    float   timeScale = 1.0f;

private:
    std::chrono::steady_clock::time_point start;
    std::chrono::steady_clock::time_point last;
};

}
