#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <utility>

constexpr double Div = 4294967296.0;

void rotate_quadrant(uint16_t &x, uint16_t &y, uint16_t xb, uint16_t yb)
{
    if (yb == 0)
    {
        if (xb == 1)
        {
            x = UINT16_MAX - x;
            y = UINT16_MAX - y;
        }

        std::swap(x, y);
    }
}

double map(uint16_t x, uint16_t y)
{
    uint32_t d = 0;

    for (uint16_t bit = 32768; bit; bit >>= 1)
    {
        uint16_t xb = !!(x & bit);
        uint16_t yb = !!(y & bit);

        d += uint32_t(bit) * uint32_t(bit) * uint32_t(3 * xb ^ yb);

        rotate_quadrant(x, y, xb, yb);
    }

    return double(d) / Div;
}

// Main takes two arguments: the coordinates of the point to map.

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " x y\n";
        return 1;
    }

    uint16_t x = std::atoi(argv[1]);
    uint16_t y = std::atoi(argv[2]);

    std::cout << std::setprecision(10) << map(x, y) << '\n';

    return 0;
}
