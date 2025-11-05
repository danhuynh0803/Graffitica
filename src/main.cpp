#include <SDL3/SDL.h>
#include <memory>
#include "renderer/resource.h"
#include "core/types.h"
#include "renderer/formats.h"
#include "renderer/renderer.h"
#include "renderer/mesh.h"

// TODO refactor window and inputs out of main
int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    int width = 800;
    int height = 600;

    SDL_Window* window = SDL_CreateWindow("Graffitica",
        width, height,
        SDL_WINDOW_RESIZABLE);

    int mouse_x = 0;
    int mouse_y = 0;

    SDL_Surface* presentSurface = nullptr;

    // TODO convert
    Buffer vbo {
        .m_Positions = {
            {50, 50, 0.},
            {100, 50, 0.},
            {75, 100, 0.},

            {250, 500, 0.},
            {750, 500, 0.},
            {500, 0, 0.},

            // Test skewed triangle for flat top/bottom rasterization
            {150, 250, 0.},
            {200, 250, 0.},
            {350, 500, 0.},
        },
        .m_VertexColors = {
            { 1, 0, 0, 1 },
            { 0, 1, 0, 1 },
            { 0, 0, 1, 1 },
        },
        .m_VertexCount = 9
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
        renderer::cmd::Draw(colorTarget, vbo, vbo.m_VertexCount, 0);

        SDL_Rect rect{ .x = 0, .y = 0, .w = width, .h = height };
        SDL_BlitSurface(presentSurface, &rect, SDL_GetWindowSurface(window), &rect);

        SDL_UpdateWindowSurface(window);
    }
}
