#include <cstdint>
#include <cstdlib>
#include <iostream>

uint32_t adder(uint32_t a, uint32_t b)
{
    while (b)
    {
        uint32_t carry = a & b;
        a ^= b;
        b = carry << 1;
    }

    return a;
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout << " Usage: " << *argv << " lhs rhs" << std::endl;
        return 1;
    }

    std::cout << adder(strtoul(argv[1], NULL, 10), strtoul(argv[2], NULL, 10)) << std::endl;
    return 0;
}
