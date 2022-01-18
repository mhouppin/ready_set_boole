#include <algorithm>
#include <cstdint>
#include <iostream>
#include <utility>

constexpr double Div = 4294967296.0;
constexpr double Cldiv = Div - 1;

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

std::pair<uint16_t, uint16_t> reverse_map(double r)
{
    uint32_t d = std::min(r * Div, Cldiv);

    std::pair<uint16_t, uint16_t> point = {0, 0};

    for (uint16_t bit = 1; bit; bit <<= 1)
    {
        uint16_t xb = 1 & (d >> 1);
        uint16_t yb = 1 & (d ^ xb);

        if (bit != 1)
            rotate_quadrant(point.first, point.second, xb, yb);

        if (xb)
            point.first |= bit;
        else
            point.first &= ~bit;
        if (yb)
            point.second |= bit;
        else
            point.second &= ~bit;

        d >>= 2;
    }

    return point;
}

// Main takes one argument: the value of the point to reverse map.

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " linepos\n";
        return 1;
    }

    double r = std::atof(argv[1]);

    std::pair<uint16_t, uint16_t> point = reverse_map(r);

    std::cout << point.first << ' ' << point.second << '\n';

    return 0;
}
