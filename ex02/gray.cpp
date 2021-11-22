#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <iostream>

uint32_t gray_code(uint32_t v)
{
    return v ^ (v >> 1);
}

int main(void)
{
    srand(time(NULL));

    for (size_t i = 0; i < 100; ++i)
    {
        uint32_t a = rand() % 1000;

        std::cout << "gray_code(" << a << ") = " << gray_code(a) << std::endl;
    }
}
