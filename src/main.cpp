
#include <complex>
#include <cstdint>
#include <iostream>
#include <vector>

using namespace std;

uint8_t mandelbrot(complex<double> c)
{
    uint8_t iterations     = 0;
    complex<double> c_iter = c;
    while ((abs(c_iter) <= 2.0) && iterations < 9) {
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
            cout << static_cast<int>(content[index_base + col]);
        }
        cout << "\n";
    }
}

int main()
{
    const size_t r_points = 150;
    const size_t i_points = 50;
    const complex<double> start(-2.0, 1.0);
    const complex<double> stop(1.0, -1.0);
    const double r_step = (stop.real() - start.real()) / r_points;
    const double i_step = (stop.imag() - start.imag()) / i_points;

    vector<uint8_t> iters(r_points * i_points, 0);

    double i_value = start.imag();
    for (size_t i_steps = 0; i_steps < i_points; ++i_steps) {
        size_t index_base = i_steps * r_points;
        double r_value    = start.real();
        for (size_t r_steps = 0; r_steps < r_points; ++r_steps) {
            iters[index_base + r_steps] = mandelbrot({r_value, i_value});
            r_value += r_step;
        }
        i_value += i_step;
    }

    print_screen(iters, r_points, i_points);

    return 0;
}
