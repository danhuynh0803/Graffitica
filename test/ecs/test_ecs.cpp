#include <catch2/catch_template_test_macros.hpp>
#include "runtime/scene/ecs/ecs.h"

// Probably worth finding a better way of handling va args? But this suffices for unit tests purposes
#define TEST_TYPES gr::TransformComponent //RenderableComponent

vec3f A(0, 1, 2);
vec3f B(1, 2, 3);
vec3f C(5, 6, 7);
vec3f D(-10, -10, -10);

TEST_CASE("add component", "[ecs-add]")
{
    gr::TransformComponent{ A, B, C };
    SECTION("adding position component and retrieving data")
    {
        //REQUIRE((A + A) == B);
    }
}

TEST_CASE("remove component", "[ecs-remove]")
{

}
