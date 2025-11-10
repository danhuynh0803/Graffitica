#include <SDL3/SDL.h>
#include <memory>
#include <random>

#include "renderer/resource.h"
#include "core/types.h"
#include "renderer/formats.h"
#include "renderer/mesh.h"
#include "util/timer.h"
#include "renderer/rasterizer_state.h"
#include "renderer/renderer.h"
#include "renderer/command.h"
#include "renderer/framebuffer.h"

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

    Buffer model = {
        .m_MeshData = std::make_shared<Mesh>("../assets/models/african_head.obj"),
    };

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    // gen random triangle colors to visualize
    for (int i = 0; i < 1000; ++i)
        model.m_VertexColors.emplace_back(dis(gen), dis(gen), dis(gen), 1.);

    //vbo.m_VertexCount = vbo.m_Positions.size();
    RasterizerState drawState{
        .fillMode = FILL_MODE::FILL_MODE_SOLID,
        .cullMode = CULL_MODE::CULL_MODE_BACK,
        .frontCounterClockwise = true,
    };

    // TODO refactor as template api is kinda annoying:
    // ideas: have image generate a view object?
    // can generate based on desired subresource range
    // 
    // pros for current system?
    // setting both formats allows possible resource alising
    // and conversion for formats in the backend?
    Image<FORMAT_D32_SFLOAT> depthImage(width, height);
    ImageView<FORMAT_D32_SFLOAT> depthView(depthImage);

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

        Image<FORMAT_R8G8B8A8_UNORM> colorImage(width, height);
        ImageView<FORMAT_R8G8B8A8_UNORM> colorView(colorImage);
        Framebuffer fb{
            .colorView = colorView,
            .depthView = depthView
        };

        //ImageView colorTarget(width, height, presentSurface->pixels);
        //const auto& color = std::get< ImageView<FORMAT_R8G8B8A8_UNORM> >(fb.colorAttachment);
        //const auto& color2 = cast(fb.colorAttachment);
        renderer::cmd::Clear(fb.colorView, {.4, .5, .7, 1.0});

        //ImageView depth(width, height, 

        // todo cmdbuffer interface?
        // bind cmds can simply assign pointers to various objects needed for rendering
        renderer::cmd::DrawIndexed(fb, drawState, model, model.m_MeshData->NumFaces(), 0, 0);
        std::memcpy(presentSurface->pixels, fb.colorView.data, width*height*sizeof(FORMAT_R8G8B8A8_UNORM));

        SDL_Rect rect{ .x = 0, .y = 0, .w = width, .h = height };
        SDL_BlitSurface(presentSurface, &rect, SDL_GetWindowSurface(window), &rect);

        SDL_UpdateWindowSurface(window);
    }
}
