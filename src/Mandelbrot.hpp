#ifndef ALMOND_BREAD_MANDELBROT
#define ALMOND_BREAD_MANDELBROT

#include <array>
#include <cassert>
#include <complex>
#include <memory>
#include <thread>
#include <vector>

namespace almondbread {

constexpr double MANDELBROT_ABSOLUTE_LIMIT = 2.0;


/// View of a Complex plain focused around a center point with real and imaginary range
template <typename Ttype>
struct ComplexView
{
    bool operator==(const ComplexView<Ttype>& cr) const
    {
        return (cr.center == center) && (cr.realRange == realRange) &&
               (imagRange == cr.imagRange) && (xPixel == cr.xPixel) && (yPixel == cr.yPixel);
    };

    bool operator!=(const ComplexView<Ttype>& cr) const { return !(cr == *this); };

    std::complex<Ttype> center;
    Ttype realRange;
    Ttype imagRange;
    size_t xPixel;
    size_t yPixel;
    size_t maxIterations;
};


template <typename TFloatType>
class Mandelbrot
{
public:
    void renderToARGB8888(std::vector<uint8_t>& pixels, ComplexView<TFloatType>& view)
    {
        assert(pixels.size() == (4 * view.xPixel * view.yPixel));

        // calculate iterations
        if (view != currentView || !lastViewCalculated) {
            currentView = view;
            calcIterations();
            lastViewCalculated = true;
        }

        // iterations to pixels
        // TODO: use some kind of histogram
        for (size_t y = 0; y < currentView.yPixel; ++y) {
            for (size_t x = 0; x < currentView.xPixel; ++x) {
                size_t iterIndex       = y * currentView.xPixel + x;
                size_t pixelIndex      = iterIndex * 4;
                auto value             = iterations[iterIndex];
                pixels[pixelIndex + 0] = ((value >> 0) & 0xFF);
                pixels[pixelIndex + 1] = ((value >> 1) & 0xFF);
                pixels[pixelIndex + 2] = ((value >> 2) & 0xFF);
            }
        }
    };

    std::vector<size_t>& getIterations(ComplexView<TFloatType>& view)
    {

        if (view != currentView || !lastViewCalculated) {
            currentView = view;
            calcIterations();
            lastViewCalculated = true;
        }

        return iterations;
    }

private:
    /// Calculate the current view for each pixel
    ///
    /// Calculation is done via the iteration formula
    ///   z[n+1] = z[n]^2 + c;
    ///   z[0] = 0
    ///   -> z[1] = c
    ///
    /// Simplify the calculation by extracting the real and the imaginary components
    /// z[n+1] = ar + i * ai
    /// z[n]   = br + i * bi
    /// c      = cr + i * ci
    ///
    /// ar + i * ai = (br + i * bi) * (br + i * bi) + cr + i * ci
    /// ar + i * ai = br^2 + i * 2 * br * bi - bi^2 + cr + i * ci
    /// ar = br^2 - bi^2 + cr
    /// ai = 2 * br * bi + ci
    void calcIterations(size_t cpus = 4)
    {
        iterations.resize(currentView.xPixel * currentView.yPixel);

        TFloatType centerReal = currentView.center.real();
        TFloatType centerImag = currentView.center.imag();
        TFloatType rangeReal  = currentView.realRange;
        TFloatType rangeImag  = currentView.imagRange;

        // start upper left corner
        TFloatType realStart = centerReal - rangeReal / 2.0;
        TFloatType realStep  = rangeReal / currentView.xPixel;
        TFloatType imagStart = centerImag + rangeImag / 2.0;
        TFloatType imagStep  = -rangeImag / currentView.yPixel;

        auto calcRows = [this, imagStep, imagStart, realStep, realStart](size_t startRow,
                                                                         size_t lastRow) {
            for (size_t row = startRow; row < lastRow; ++row) {
                // descending direction on the imaginary axis
                TFloatType imag = imagStart + row * imagStep;
                TFloatType real = realStart;
                for (size_t col = 0; col < currentView.xPixel; ++col) {
                    // ascending direction on the real axis

                    TFloatType ar = real;  // z[1]
                    TFloatType ai = imag;  // z[1]
                    TFloatType cr = real;  // z[0]
                    TFloatType ci = imag;  // z[0]


                    size_t numIterations = 0;
                    while ((numIterations < currentView.maxIterations) &&
                           ((ai * ai + ar * ar) <= 4.0)) {
                        TFloatType br = ar;
                        TFloatType bi = ai;
                        ar            = br * br - bi * bi + cr;
                        ai            = 2 * br * bi + ci;
                        ++numIterations;
                    }
                    iterations[row * currentView.xPixel + col] = numIterations;
                    real += realStep;
                }
            }
        };

        // threading
        std::vector<std::thread> threads;
        size_t rowsPerThread = currentView.yPixel / cpus;
        for (size_t cpuNumber = 0; cpuNumber < cpus; ++cpuNumber) {
            size_t startRow = cpuNumber * rowsPerThread;
            if ((cpuNumber + 1) == cpus) {
                threads.emplace_back(calcRows, startRow, currentView.yPixel);
            }
            else {
                threads.emplace_back(calcRows, startRow, startRow + rowsPerThread);
            }
        }

        // wait for all threads to finish
        for (auto& t : threads) {
            t.join();
        }
    };


    ComplexView<TFloatType> currentView{{-0.5, 0.0}, 3.0, 2.0, 800, 600, 255};
    bool lastViewCalculated = false;
    std::vector<size_t> iterations{};
};

}  // namespace almondbread


#endif  // ALMOND_BREAD_MANDELBROT
