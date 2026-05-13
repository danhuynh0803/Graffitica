#pragma once

#include "util/math/vector.h"

namespace gr::rhi::cpu
{


// Base payload struct for testing
struct Payload
{
    vec3f color;
};

template <typename TPayload>
class IRayIntersection
{
public:
    virtual void Main(TPayload& payload) = 0;
};

template <typename TPayload>
class TriangleIntersection final : public IRayIntersection
{
public:
    virtual void Main(TPayload& payload) override {

    }
};

template <typename TPayload>
class SphereIntersection final : public IRayIntersection
{
public:
    virtual void Main(TPayload& payload) override {

    }
};


}
