#include "core/system_time.h"

namespace gr
{

SystemTime::SystemTime()
    : timeScale(1.0f),
      deltaTime(0.0),
      start(std::chrono::steady_clock::now()),
      last(std::chrono::steady_clock::now())
{}

void SystemTime::Update()
{
    start = std::chrono::steady_clock::now();

    // update dt
    deltaTime =
        std::chrono::duration_cast<std::chrono::duration<double>>(start - last).count() * timeScale;

    // update last timestamp
    last = start;
}

}
