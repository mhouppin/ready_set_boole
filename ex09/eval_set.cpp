#include <algorithm>
#include <sstream>
#include "rpn_tree.hpp"

std::vector<int> eval_set(const std::string& rpnStr, std::vector<std::vector<int> > sets)
{
    RPN_Variable vars[26];

    RPN_Tree tree;

    std::string rest = tree.rpn_parse(rpnStr);

    if (!tree.is_valid() || !rest.empty())
    {
        std::cout << "Invalid RPN string." << std::endl;
        return std::vector<int>();
    }

#ifdef DEBUG
    std::cout << "RPN formula: " << tree << std::endl << std::endl;
#endif

    // Get the list of variables used in the formula.
    size_t varCount = rpnvar_parse(rpnStr, vars);

    // Order variables so that we know how many sets we need
    std::sort(vars, vars + varCount, [](const RPN_Variable& a, const RPN_Variable& b) {
            return a.letter < b.letter;
        });

    if (size_t(vars[varCount - 1].letter - 'A') > sets.size())
    {
#ifdef DEBUG
        std::cerr << "Not enough sets specified for evaluation." << std::endl;
#endif
        return std::vector<int>();
    }

    tree.to_negation_normal_form();

    std::vector<int> omega;

    // K sets, N elements per set
    // PushAll + Sort + Uniq:
    // Compute O(KN * (1 + log(K) + log(N) + KN))
    // Sort + PushUniq:
    // Compute O(KN * (K + log(N)))

    for (std::vector<int> &set : sets)
        std::sort(set.begin(), set.end());

    std::vector<std::vector<int>::iterator> itList;
    std::vector<std::vector<int>*> setList;

    for (std::vector<int> &set : sets)
    {
        if (set.size())
        setList.push_back(&set);
        itList.push_back(set.begin());
    }

    while (setList.size())
    {
        std::vector<std::vector<int>::iterator>::iterator minValue = std::min_element(
            itList.begin(), itList.end(), [](const std::vector<int>::iterator &a, const std::vector<int>::iterator &b) {
                return *a < *b;
            });

        int val = **minValue;

        if (omega.empty() || val > omega.back())
            omega.push_back(val);

        ++*minValue;

        std::vector<std::vector<int>*>::iterator minSet = setList.begin() + (minValue - itList.begin());

        if ((*minSet)->end() == *minValue)
        {
            setList.erase(minSet);
            itList.erase(minValue);
        }
    }

    std::vector<int> finalSet = tree.eval_set(sets, omega);

    return finalSet;
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        std::cout << "usage: " << *argv << " rpn_formula <A_elements> [ <B_elements ... ]" << std::endl;
        return 1;
    }

    std::vector<std::vector<int> > sets;

    for (int i = 2; i < argc; ++i)
    {
        sets.push_back(std::vector<int>());

        std::istringstream is{std::string(argv[i])};
        std::string token;

        is >> std::skipws;

        while (is >> token)
            sets.back().push_back(std::stoi(token));
    }

    std::vector<int> finalSet = eval_set(argv[1], sets);

    std::cout << '[';

    for (std::vector<int>::iterator it = finalSet.begin(); it != finalSet.end(); ++it)
        std::cout << (it == finalSet.begin() ? "" : ", ") << (*it);

    std::cout << ']' << std::endl;

    return 0;
}
