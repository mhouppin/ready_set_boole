#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <iostream>

uint32_t gray_code(uint32_t v)
{
    return v ^ (v >> 1);
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << " Usage: " << *argv << " lhs" << std::endl;
        return 1;
    }

    std::cout << gray_code(strtoul(argv[1], NULL, 10)) << std::endl;
    return 0;
}
