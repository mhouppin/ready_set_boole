#include "rpn_tree.hpp"

std::string negation_normal_form(const std::string &rpnStr)
{
    RPN_Tree tree;

    std::string rest = tree.rpn_parse(rpnStr);

    if (!tree.is_valid() || !rest.empty())
    {
        std::cout << "Invalid RPN string." << std::endl;
        return "";
    }

#ifdef DEBUG
    std::cout << "Initial RPN formula: " << tree << std::endl << std::endl;
#endif

    tree.to_negation_normal_form();
}
