#include <algorithm>
#include <cstdlib>
#include <initializer_list>
#include <iostream>
#include <vector>

std::vector< std::vector<int> > powerset(std::vector<int> initialSet)
{
    size_t items = 1ul << initialSet.size();

    std::vector<std::vector<int> > ret(items);

    for (size_t i = 0; i < initialSet.size(); ++i)
        for (size_t b = 1ul << i; b < items; b += 2ul << i)
            for (size_t k = 0; k < 1ul << i; ++k)
                ret[b + k].push_back(initialSet[i]);

    // Sort the powerset by ascending size, and then by ascending number.
    std::sort(ret.begin(), ret.end(), [](const std::vector<int>& a, const std::vector<int>& b) {
        if (a.size() != b.size())
            return a.size() < b.size();
        else
            return a < b;
    });

    return ret;
}

int main(int argc, char **argv)
{
    std::vector<int> argList;

    for (int i = 1; i < argc; ++i)
        argList.push_back(atoi(argv[i]));

    std::vector<std::vector<int> > powers = powerset(argList);

    for (auto &v : powers)
    {
        std::cout << "{";
        for (auto &i : v)
            std::cout << i << (&i == &v.back() ? "" : ", ");
        std::cout << "}" << std::endl;
    }
}
