#include <algorithm>
#include "rpn_tree.hpp"

size_t rpnvar_parse(const std::string& rpnStr, RPN_Variable vars[26])
{
    size_t varCount = 0;

    for (char c : rpnStr)
    {
        if (c >= 'A' && c <= 'Z')
        {
            size_t i;

            for (i = 0; i < varCount; ++i)
                if (vars[i].letter == c)
                    break ;

            if (i == varCount)
                vars[varCount++].letter = c;
        }
    }

    return varCount;
}

std::string RPN_Tree::rpn_parse(const std::string& rpnStr)
{
    // Empty strings are invalid RPN strings.
    if (rpnStr.empty())
    {
        op = RPN_NONE;
        return "";
    }

    // If the last character is a letter, it's a variable.
    // Store it in self and return the preceding string.
    if (rpnStr.back() >= 'A' && rpnStr.back() <= 'Z')
    {
        op = RPN_SELF;
        self = rpnStr.back();
        return rpnStr.substr(0, rpnStr.size() - 1);
    }

    // If the last character is one of '!' '&', '|', '^', '>', '=',
    // it's an operator. Everything else is invalid.
    // Store the corresponding type in op.
    switch (rpnStr.back())
    {
        case '!': op = RPN_NOT;   break ;
        case '&': op = RPN_AND;   break ;
        case '|': op = RPN_OR;    break ;
        case '^': op = RPN_XOR;   break ;
        case '>': op = RPN_IMPLY; break ;
        case '=': op = RPN_EQUIV; break ;

        default:
            op = RPN_NONE;
            return "";
    }

    if (op != RPN_NOT)
    {
        right = new RPN_Tree;
        std::string leftStr = right->rpn_parse(rpnStr.substr(0, rpnStr.size() - 1));
        left = new RPN_Tree;
        return left->rpn_parse(leftStr);
    }
    else
    {
        left = new RPN_Tree;
        return left->rpn_parse(rpnStr.substr(0, rpnStr.size() - 1));
    }
}

std::string RPN_Tree::rpn_out() const
{
    if (op == RPN_SELF)
        return std::string(1, self);
    else if (op == RPN_NOT)
        return left->rpn_out() + "!";
    else
        return left->rpn_out() + right->rpn_out() + rpn_op_str(op);
}

void RPN_Tree::to_negation_normal_form()
{
    if (op == RPN_NOT)
    {
        // Simpliy double "not"s.
        if (left->op == RPN_NOT)
        {
            RPN_Tree *tmp = left;
            *this = *(left->left);
            delete tmp->left;
            delete tmp;

            // Call ourselves again since we modified ourselves.
            to_negation_normal_form();
            return ;
        }
    }
}

// std::ostream overload for debugging purposes on RPN_Operator.
std::ostream& operator<<(std::ostream& os, const RPN_Operator& op)
{
    os << rpn_op_str(op);
    return os;
}

// std::ostream overload for debugging purposes on RPN_Tree.
std::ostream& operator<<(std::ostream& os, const RPN_Tree& tree)
{
    if (tree.op == RPN_NONE)
        return os << "";

    if (tree.op == RPN_SELF)
        return os << tree.self;

    if (tree.op == RPN_NOT)
        return os << "!" << *tree.left;

    os << "(";
    os << *tree.left;
    os << " " << tree.op << " ";
    os << *tree.right;
    os << ")";

    return os;
}
