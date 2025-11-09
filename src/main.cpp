#include <SDL3/SDL.h>
#include <memory>
#include "renderer/resource.h"
#include "core/types.h"
#include "renderer/formats.h"
#include "renderer/mesh.h"
#include "util/timer.h"
#include "renderer/rasterizer_state.h"
#include "renderer/renderer.h"
#include "renderer/command.h"
#include <random>

// TODO refactor window and inputs out of main
int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    int width = 1600;
    int height = 800;

    SDL_Window* window = SDL_CreateWindow("Graffitica",
        width, height,
        SDL_WINDOW_RESIZABLE);

    int mouse_x = 0;
    int mouse_y = 0;

    SDL_Surface* presentSurface = nullptr;

    // TODO move to unit testing framework
    Buffer vbo {
        .m_Positions = {
            // edge testing
            { 100, 100, 0},
            { 700, 100, 0},
            { 100, 500, 0},

            { 100, 500, 0},
            { 700, 500, 0},
            { 700, 100, 0},
        },
        // TODO allow hex conversion for colors?
        .m_VertexColors = {
            { 1, 0, 0, 1 },
            { 0, 1, 0, 1 },
            { 0, 0, 1, 1 },

            { 1, (105. / 255), (180. / 255), 1},
            { 0, 1, 1, 1 },
            { 0, 0, 1, 1 },

            { 1, 0, 1, 1 },
            { 0, 1, 1, 1 },
            { 1, 1, 1, 1 },
        },
    };

    Buffer model = {
        .m_MeshData = std::make_shared<Mesh>("../assets/models/african_head.obj"),
    };

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    // gen random triangle colors to visualize
    for (int i = 0; i < 100; ++i)
        model.m_VertexColors.emplace_back(dis(gen), dis(gen), dis(gen), 1.);

    //vbo.m_VertexCount = vbo.m_Positions.size();
    RasterizerState drawState{
        .fillMode = FILL_MODE::FILL_MODE_SOLID,
        .cullMode = CULL_MODE::CULL_MODE_BACK,
        .frontCounterClockwise = true,
    };

    bool running = true;
    while (running)
    {
        for (SDL_Event event; SDL_PollEvent(&event);) switch (event.type)
        {
        case SDL_EVENT_QUIT:
            running = false;
            break;
        case SDL_EVENT_MOUSE_MOTION:
            mouse_x = event.motion.x;
            mouse_y = event.motion.y;
            break;
        case SDL_EVENT_WINDOW_RESIZED:
            if (presentSurface)
                SDL_DestroySurface(presentSurface);
            presentSurface = nullptr;
            width = event.window.data1;
            height = event.window.data2;
        }

        if (!running)
            break;

        if (!presentSurface)
        {
            presentSurface = SDL_CreateSurface(width, height, SDL_PIXELFORMAT_RGBA32);
            SDL_SetSurfaceBlendMode(presentSurface, SDL_BLENDMODE_NONE);
        }

        ImageView colorTarget(width, height, presentSurface->pixels);
        renderer::cmd::Clear(colorTarget, {.4, .5, .7, 1.0});

        // todo cmdbuffer interface?
        // bind cmds can simply assign pointers to various objects needed for rendering
        renderer::cmd::DrawIndexed(drawState, colorTarget, model, model.m_MeshData->NumFaces(), 0, 0);

        SDL_Rect rect{ .x = 0, .y = 0, .w = width, .h = height };
        SDL_BlitSurface(presentSurface, &rect, SDL_GetWindowSurface(window), &rect);

        SDL_UpdateWindowSurface(window);
    }
}
