#include <catch2/catch_template_test_macros.hpp>
#include "runtime/scene/ecs/ecs.h"

using namespace gr;

#define TEST_TYPES TransformComponent //RenderableComponent

vec3f A(0, 1, 2);
vec3f B(1, 2, 3);
vec3f C(5, 6, 7);
vec3f D(-10, -10, -10);

TEST_CASE("add component", "[ecs-add]")
{
    EntityRegistry reg;
    auto e1 = reg.CreateEntity();
    auto e2 = reg.CreateEntity();

    TransformComponent T1{ A, B, C };
    TransformComponent T2{ A, B, C };
    reg.AddComponent<TransformComponent>(e1, T1);
    reg.AddComponent<TransformComponent>(e2, T2);

    SECTION("adding position component and retrieving data")
    {
        const auto& r1 = reg.GetComponent<TransformComponent>(e1);
        const auto& r2 = reg.GetComponent<TransformComponent>(e2);

        REQUIRE((r1.position + r2.position) == (A + A));
        REQUIRE((r1.rotation + r2.rotation) == (B + B));
        REQUIRE((r1.scale + r2.scale) == (C + C));
        // TODO vec3f missing * operator where LHS is int/float?
        //REQUIRE((r1.scale + r2.scale) == 2*C);
    }
}

TEST_CASE("remove component", "[ecs-remove]")
{
    EntityRegistry reg;
    auto e1 = reg.CreateEntity();
    TransformComponent T1{ A, B, C };
    reg.AddComponent<TransformComponent>(e1, T1);

    reg.RemoveComponent<TransformComponent>(e1);
    reg.GetComponent<TransformComponent>(e1);
}
