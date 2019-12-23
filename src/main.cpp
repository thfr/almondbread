#include "Mandelbrot.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_video.h>

#include <cassert>
#include <complex>
#include <cstdint>
#include <iostream>
#include <vector>

using namespace std;
using namespace almondbread;

int main()
{
    // NOTE: this is a adapted example from https://stackoverflow.com/a/33312056

    // Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
        return -1;
    }
    atexit(SDL_Quit);

    const unsigned int texWidth  = 1024;
    const unsigned int texHeight = 768;
    const size_t maxIterations   = 1024;
    const double realRange       = 3.0;
    const double imagRange       = (texWidth > texHeight) ? realRange * texHeight / texWidth
                                                    : realRange * texHeight / texWidth;

    Mandelbrot<double> mb;
    ComplexView<double> mbViewDefault{{-0.5, 0.0}, realRange, imagRange,
                                      texWidth,    texHeight, maxIterations};
    ComplexView<double> mbView = mbViewDefault;

    auto window = SDL_CreateWindow("Almondbread", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                   texWidth, texHeight, SDL_WINDOW_SHOWN);
    // auto screenSurface = SDL_GetWindowSurface(window);

    auto renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    auto texture  = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                     SDL_TEXTUREACCESS_STREAMING, texWidth, texHeight);

    vector<uint8_t> pixels(texWidth * texHeight * 4, SDL_ALPHA_OPAQUE);

    SDL_Event event;
    bool running = true;
    while (running) {
        const Uint64 start = SDL_GetPerformanceCounter();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        while (SDL_PollEvent(&event) == 1) {
            if ((SDL_QUIT == event.type) ||
                (SDL_KEYDOWN == event.type && SDL_SCANCODE_ESCAPE == event.key.keysym.scancode)) {
                running = false;
                break;
            }
            if (!(SDL_KEYDOWN == event.type)) {
                continue;
            }
            switch (event.key.keysym.sym) {
            // ZOOM out:
            case SDLK_MINUS:
            case SDLK_KP_MINUS:
                mbView.imagRange = 1.0 / 0.75 * mbView.imagRange;
                mbView.realRange = 1.0 / 0.75 * mbView.realRange;
                break;
            // ZOOM in:
            case SDLK_PLUS:
            case SDLK_KP_PLUS:
                mbView.imagRange = 0.75 * mbView.imagRange;
                mbView.realRange = 0.75 * mbView.realRange;
                break;
            // move up:
            case SDLK_UP:
                mbView.center += complex<double>{0, 0.25 * mbView.imagRange};
                break;
            // move down:
            case SDLK_DOWN:
                mbView.center += complex<double>{0, -0.25 * mbView.imagRange};
                break;
            // move left:
            case SDLK_LEFT:
                mbView.center += complex<double>{-0.25 * mbView.realRange, 0};
                break;
            // move right:
            case SDLK_RIGHT:
                mbView.center += complex<double>{0.25 * mbView.realRange, 0};
                break;
            case SDLK_0:
                mbView.center    = mbViewDefault.center;
                mbView.realRange = mbViewDefault.realRange;
                mbView.imagRange = mbViewDefault.imagRange;
                break;
            // quit:
            case SDLK_q:
                running = false;
                break;
            default:
                break;
            }
        }

        // calc mandelbrot
        mb.renderToARGB8888(pixels, mbView);

        SDL_UpdateTexture(texture, NULL, &pixels[0], texWidth * 4);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

        const Uint64 end         = SDL_GetPerformanceCounter();
        const static Uint64 freq = SDL_GetPerformanceFrequency();
        const double seconds     = (end - start) / static_cast<double>(freq);
        if (seconds > 0.1) {
            cout << "Frame time: " << seconds * 1000.0 << "ms" << endl;
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
