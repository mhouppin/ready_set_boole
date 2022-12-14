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

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout << " Usage: " << *argv << " lhs rhs" << std::endl;
        return 1;
    }

    std::cout << multiplier(strtoul(argv[1], NULL, 10), strtoul(argv[2], NULL, 10)) << std::endl;
    return 0;
}
