
#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <complex>
#include <cstdint>
#include <iostream>
#include <vector>

using namespace std;

uint8_t mandelbrot(complex<double> c)
{
    uint8_t iterations     = 0;
    complex<double> c_iter = c;
    while ((abs(c_iter) <= 2.0) && iterations < 255) {
        ++iterations;
        c_iter = (c_iter * c_iter) + c;
    }
    return iterations;
}

void print_screen(vector<uint8_t>& content, size_t cols, size_t rows)
{
    for (size_t row = 0; row < rows; ++row) {
        size_t index_base = row * cols;
        for (size_t col = 0; col < cols; ++col) {
            cout << static_cast<int>(content[index_base + col]) % 10;
        }
        cout << "\n";
    }
}


vector<uint8_t> renderMandelbrot(size_t x_points, size_t y_points, complex<double> center,
                                 double x_delta_per_pixel, double y_delta_per_pixel)
{
    complex<double> start{center.real() - x_points / 2 * x_delta_per_pixel,
                          center.imag() + y_points / 2 * y_delta_per_pixel};
    complex<double> stop{center.real() + x_points / 2 * x_delta_per_pixel,
                         center.imag() - y_points / 2 * y_delta_per_pixel};
    double r_step = (stop.real() - start.real()) / x_points;
    double i_step = (stop.imag() - start.imag()) / y_points;

    vector<uint8_t> iters(x_points * y_points, 0);

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

int main()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
        return -1;
    }

    auto window = SDL_CreateWindow("Almondbread", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                   640, 480, SDL_WINDOW_SHOWN);
    auto screenSurface = SDL_GetWindowSurface(window);
    SDL_LockSurface(screenSurface);
    /* do stuff */
    SDL_UnlockSurface(screenSurface);

    auto r_points = 150;
    auto i_points = 50;
    auto iters    = renderMandelbrot(r_points, i_points, {-0.5, 0}, 0.025, 0.05);

    print_screen(iters, r_points, i_points);

    return 0;
}
