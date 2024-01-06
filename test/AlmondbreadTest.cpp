#include <gtest/gtest.h>

#include <Mandelbrot.hpp>
#include <complex>

namespace almondbread {

using namespace std;

size_t mandelbrotIterations(double real, double imag, size_t maxIter)
{
    constexpr double zMax = 2.0L;
    complex<double> c{real, imag};
    complex<double> cIter = c;
    size_t iterations     = 0;

    while ((abs(cIter) <= zMax) && iterations < maxIter) {
        ++iterations;
        cIter = (cIter * cIter) + c;
    }
    return iterations;
}

void calcIterationsForComplexView(vector<size_t>& iterations, const ComplexView<double>& view)
{
    iterations.resize(view.xPixel * view.yPixel);
    double realStart = view.center.real() - view.realRange / 2.0L;
    double imagStart = view.center.imag() - view.imagRange / 2.0L;
    double realStep  = view.realRange / view.xPixel;
    double imagStep  = view.imagRange / view.yPixel;

    double imag = imagStart;
    for (size_t row = 0; row < view.yPixel; ++row) {
        double real = realStart;
        for (size_t col = 0; col < view.xPixel; ++col) {
            iterations[row * view.xPixel + col] =
                mandelbrotIterations(real, imag, view.maxIterations);
            real += realStep;
        }
        imag += imagStep;
    }
}

TEST(AlmondbreadTests, ComplexRangeTest)
{
    ComplexView<double> a{{-0.5, 0.0}, 1.0, 1.4321, 1024, 1024, 1024};
    ComplexView<double> b{{-0.5, 0.0}, 1.0, 1.4321, 1024, 1024, 1024};
    ComplexView<double> c = a;

    ASSERT_EQ(a, b);

    c.center = a.center + complex<double>{0.1, 0};
    ASSERT_NE(a, c);

    c = a;
    c.xPixel += 1;
    ASSERT_NE(a, c);
    c.xPixel -= 1;
    ASSERT_EQ(a, c);
}

TEST(AlmondbreadTests, MandelbrotCorrectness)
{

    ComplexView<double> view{{-0.5, 0.0}, 1.0, 3.0, 640, 480, 255};
    Mandelbrot<double> mb;
    auto iterations = mb.getIterations(view);
    vector<size_t> iterTest{};
    calcIterationsForComplexView(iterTest, view);

    ASSERT_EQ(iterations.size(), iterTest.size());

    for (size_t i = 0; i < iterations.size(); ++i) {
        EXPECT_EQ(iterations[i], iterTest[i]) << " differs at index " << i;
    }
}

}  // namespace almondbread
