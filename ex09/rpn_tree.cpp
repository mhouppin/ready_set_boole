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

std::vector<int> RPN_Tree::eval_set(const std::vector<std::vector<int> >& set_list, const std::vector<int>& omega) const
{
    std::vector<int> ret;

    if (op == RPN_SELF)
        ret = set_list[self - 'A'];

    else if (op == RPN_NOT)
    {
        std::vector<int> child_set = left->eval_set(set_list, omega);
        std::vector<int>::const_iterator child_it = child_set.begin();

        for (const int& elem : omega)
        {
            if (child_it == child_set.end() || *child_it != elem)
                ret.push_back(elem);
            else
                ++child_it;
        }
    }

    else
    {
        std::vector<int> left_set = left->eval_set(set_list, omega);
        std::vector<int> right_set = right->eval_set(set_list, omega);
        std::vector<int>::const_iterator left_it = left_set.begin();
        std::vector<int>::const_iterator right_it = right_set.begin();

        for (int value : omega)
        {
            bool lvalue = (left_it != left_set.end() && value == *left_it);
            bool rvalue = (right_it != right_set.end() && value == *right_it);

            if (lvalue) ++left_it;
            if (rvalue) ++right_it;

            bool result;

            switch (op)
            {
                case RPN_AND:   result =  lvalue & rvalue; break;
                case RPN_OR:    result =  lvalue | rvalue; break;
                case RPN_XOR:   result =  lvalue ^ rvalue; break;
                case RPN_IMPLY: result = !lvalue | rvalue; break;
                case RPN_EQUIV: result = !lvalue ^ rvalue; break;

                default: // Unreachable
                    result = 0;
                    break ;
            }

            if (result)
                ret.push_back(value);
        }
    }

    return ret;
}

std::string rpn_op_str(RPN_Operator op)
{
    switch (op)
    {
        case RPN_NOT:   return "!";
        case RPN_AND:   return "&";
        case RPN_OR:    return "|";
        case RPN_XOR:   return "^";
        case RPN_IMPLY: return ">";
        case RPN_EQUIV: return "=";
        default:        return "?";
    }
}

std::string rpn_op_pretty_str(RPN_Operator op)
{
    switch (op)
    {
        case RPN_NOT:   return "!";
        case RPN_AND:   return "&";
        case RPN_OR:    return "|";
        case RPN_XOR:   return "^";
        case RPN_IMPLY: return "=>";
        case RPN_EQUIV: return "<=>";
        default:        return "?";
    }
}

// std::ostream overload for debugging purposes on RPN_Operator.
std::ostream& operator<<(std::ostream& os, const RPN_Operator& op)
{
    os << rpn_op_pretty_str(op);
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
