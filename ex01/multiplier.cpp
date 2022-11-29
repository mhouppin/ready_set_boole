#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <iostream>

uint32_t multiplier(uint32_t a, uint32_t b)
{
    uint32_t ret = 0;

    while (b && a)
    {
        if (b & 1)
        {
            uint32_t c = a;
            while (c)
            {
                uint32_t carry = ret & c;
                ret ^= c;
                c = carry << 1;
            }
        }

        b >>= 1;
        a <<= 1;
    }

    return ret;
}

int main(void)
{
    srand(time(NULL));

    for (size_t i = 0; i < 100; ++i)
    {
        uint32_t a = rand() % 1000;
        uint32_t b = rand() % 1000;

        std::cout << a << " * " << b << " = " << multiplier(a, b) << " (real " << (a * b) << ")" << std::endl;
    }
}
