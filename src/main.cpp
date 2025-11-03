#include <SDL3/SDL.h>
#include <memory>

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

        // clear color (0xAABBGGRR)
        std::fill_n((uint32_t*)presentSurface->pixels, width * height, 0xFF888888);

        SDL_Rect rect{ .x = 0, .y = 0, .w = width, .h = height };
        SDL_BlitSurface(presentSurface, &rect, SDL_GetWindowSurface(window), &rect);

        SDL_UpdateWindowSurface(window);
    }
}
