#include <algorithm>
#include "rpn_tree.hpp"

void print_truth_table(const std::string& rpnStr)
{
    RPN_Variable vars[26];

    RPN_Tree tree;

    std::string rest = tree.rpn_parse(rpnStr);

    if (!tree.is_valid() || !rest.empty())
    {
        std::cout << "Invalid RPN string." << std::endl;
        return ;
    }

#ifdef DEBUG
    std::cout << "RPN formula: " << tree << std::endl << std::endl;
#endif

    // Get the list of variables used in the formula.
    size_t varCount = rpnvar_parse(rpnStr, vars);

    // Order variables for a nicer output
    std::sort(vars, vars + varCount, [](const RPN_Variable& a, const RPN_Variable& b) {
            return a.letter < b.letter;
        });

    // Print the header
    for (size_t i = 0; i < varCount; ++i)
        std::cout << "| " << vars[i].letter << " ";

    std::cout << "| = |" << std::endl;
    for (size_t i = 0; i < varCount; ++i)
        std::cout << "|---";
    
    std::cout << "|---|" << std::endl;

    // Iterate over all possible combinations of variable values.
    for (uint32_t bitstate = 0; bitstate < (1 << varCount); ++bitstate)
    {
        for (size_t i = 0; i < varCount; ++i)
        {
            vars[i].value = !!(bitstate & (1 << (varCount - i - 1)));
            std::cout << "| " << vars[i].value << " ";
        }

        tree.propagate(vars, varCount);
        tree.evaluate();

        std::cout << "| " << tree.selfValue << " |" << std::endl;
    }
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <rpn_string>" << std::endl;
        return 1;
    }

    print_truth_table(argv[1]);
}
