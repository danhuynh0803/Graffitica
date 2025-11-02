#include <SDL3/SDL.h>

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
        }

        if (!running)
            break;
    }
}
