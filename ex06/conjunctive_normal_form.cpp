#include "rpn_tree.hpp"

std::string conjunctive_normal_form(const std::string &rpnStr)
{
    RPN_Tree tree;

    std::string rest = tree.rpn_parse(rpnStr);

    if (!tree.is_valid() || !rest.empty())
    {
        std::cout << "Invalid RPN string." << std::endl;
        return "";
    }

#ifdef DEBUG
    std::cout << "Initial RPN formula: " << tree << std::endl;
#endif

    tree.to_conjunctive_normal_form();

#ifdef DEBUG
    std::cout << "Conjunctive normal form: " << tree << std::endl;
#endif

    return tree.rpn_out();
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <RPN formula>" << std::endl;
        return 1;
    }

    std::string rpnStr = argv[1];
    std::string cnf = conjunctive_normal_form(rpnStr);

    std::cout << cnf << std::endl;

    return 0;
}
