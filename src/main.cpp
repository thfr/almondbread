#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#include <cassert>
#include <complex>
#include <cstdint>
#include <iostream>
#include <vector>

using namespace std;

using mandelbrot_iter_t = uint8_t;

mandelbrot_iter_t mandelbrot(complex<double> c)
{
    mandelbrot_iter_t iterations = 0;
    complex<double> c_iter       = c;
    while ((abs(c_iter) <= 2.0) && iterations < 255) {
        ++iterations;
        c_iter = (c_iter * c_iter) + c;
    }
    return iterations;
}

void print_screen(vector<mandelbrot_iter_t>& content, size_t cols, size_t rows)
{
    for (size_t row = 0; row < rows; ++row) {
        size_t index_base = row * cols;
        for (size_t col = 0; col < cols; ++col) {
            cout << static_cast<int>(content[index_base + col]) % 10;
        }
        cout << "\n";
    }
}


vector<mandelbrot_iter_t> renderMandelbrot(size_t x_points, size_t y_points, complex<double> center,
                                           double x_delta_per_pixel, double y_delta_per_pixel)
{
    complex<double> start{center.real() - x_points / 2 * x_delta_per_pixel,
                          center.imag() + y_points / 2 * y_delta_per_pixel};
    complex<double> stop{center.real() + x_points / 2 * x_delta_per_pixel,
                         center.imag() - y_points / 2 * y_delta_per_pixel};
    double r_step = (stop.real() - start.real()) / x_points;
    double i_step = (stop.imag() - start.imag()) / y_points;

    vector<mandelbrot_iter_t> iters(x_points * y_points, 0);

    double i_value = start.imag();
    for (size_t i_steps = 0; i_steps < y_points; ++i_steps) {
        size_t index_base = i_steps * x_points;
        double r_value    = start.real();
        for (size_t r_steps = 0; r_steps < x_points; ++r_steps) {
            iters[index_base + r_steps] = mandelbrot({r_value, i_value});
            r_value += r_step;
        }
        i_value += i_step;
    }

    return iters;
}

void iterationsToARGB8888(const vector<mandelbrot_iter_t>& iters, vector<uint8_t>& pixels)
{
    assert(iters.size() != 0);
    assert(4 * iters.size() == pixels.size());

    for (size_t i = 0; i < iters.size(); ++i) {
        auto i_pixel        = i * 4;
        pixels[i_pixel + 0] = (iters[i] >> 0) & 0xFF;
        pixels[i_pixel + 1] = (iters[i] >> 2) & 0xFF;
        pixels[i_pixel + 2] = (iters[i] >> 4) & 0xFF;
        pixels[i_pixel + 3] = (iters[i] >> 6) & 0xFF;
    }
}

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
    const unsigned int texHeight = 1024;

    auto window = SDL_CreateWindow("Almondbread", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                   texWidth, texHeight, SDL_WINDOW_SHOWN);
    // auto screenSurface = SDL_GetWindowSurface(window);

    auto renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    auto texture  = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                     SDL_TEXTUREACCESS_STREAMING, texWidth, texHeight);

    vector<uint8_t> pixels(texWidth * texHeight * 4, 0);

    SDL_Event event;
    bool running = true;
    while (running) {
        const Uint64 start = SDL_GetPerformanceCounter();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        while (SDL_PollEvent(&event)) {
            if ((SDL_QUIT == event.type) ||
                (SDL_KEYDOWN == event.type && SDL_SCANCODE_ESCAPE == event.key.keysym.scancode)) {
                running = false;
                break;
            }
        }

        // calc mandelbrot
        // TODO: calc directly to to pixels
        auto iters =
            renderMandelbrot(texWidth, texHeight, {-0.5, 0}, 2.0 / static_cast<double>(texWidth),
                             2.0 / static_cast<double>(texHeight));
        iterationsToARGB8888(iters, pixels);

        SDL_UpdateTexture(texture, NULL, &pixels[0], texWidth * 4);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

        const Uint64 end         = SDL_GetPerformanceCounter();
        const static Uint64 freq = SDL_GetPerformanceFrequency();
        const double seconds     = (end - start) / static_cast<double>(freq);
        cout << "Frame time: " << seconds * 1000.0 << "ms" << endl;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
