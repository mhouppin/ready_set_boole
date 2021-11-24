#include <algorithm>
#include "rpn_tree.hpp"

bool sat(const std::string& rpnStr)
{
    RPN_Variable vars[26];

    RPN_Tree tree;

    std::string rest = tree.rpn_parse(rpnStr);

    if (!tree.is_valid() || !rest.empty())
    {
        std::cout << "Invalid RPN string." << std::endl;
        return false;
    }

#ifdef DEBUG
    std::cout << "RPN formula: " << tree << std::endl << std::endl;
#endif

    // Get the list of variables used in the formula.
    size_t varCount = rpnvar_parse(rpnStr, vars);

    // Iterate over all possible combinations of variable values.
    for (uint32_t bitstate = 0; bitstate < (1 << varCount); ++bitstate)
    {
        for (size_t i = 0; i < varCount; ++i)
            vars[i].value = !!(bitstate & (1 << (varCount - i - 1)));

        tree.propagate(vars, varCount);
        tree.evaluate();

        // If the top tree node is true, then the formula is satisfiable.
        if (tree.selfValue)
        {
#ifdef DEBUG
            // Log all variable values for which the formula is true.
            std::cout << "Satisfiable for: ";
            for (size_t i = 0; i < varCount; ++i)
                std::cout << (i != 0 ? ", " : "") << vars[i].letter << (vars[i].value ? " true" : " false");
            std::cout << std::endl;
#endif
            return true;
        }
    }

    return false;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <rpn_string>" << std::endl;
        return 1;
    }

    std::cout << (sat(argv[1]) ? "Satisfiable" : "Unsatisfiable") << std::endl;
}
