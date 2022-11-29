#include <cstdint>
#include <cstdlib>
#include <ctime>
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

int main(void)
{
    srand(time(NULL));

    for (size_t i = 0; i < 100; ++i)
    {
        uint32_t a = rand() % 1000;
        uint32_t b = rand() % 1000;

        std::cout << a << " + " << b << " = " << adder(a, b) << " (real " << (a + b) << ")" << std::endl;
    }
}
