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
        RPN_Tree *tmp;

        switch (left->op)
        {
            case RPN_NOT:
                // Simpliy double "not"s.
                tmp = left;
                *this = *(left->left);
                tmp->left->left = nullptr;
                tmp->left->right = nullptr;
                delete tmp;

                // Call ourselves again since we modified ourselves.
                to_negation_normal_form();
                return ;

            case RPN_AND:
                // "not (a & b)" is "(not a) | (not b)".
                op = RPN_OR;
                tmp = left;

                // Move child left operand to our left.
                left = new RPN_Tree;
                left->op = RPN_NOT;
                left->left = tmp->left;
                left->right = nullptr;

                // Move child right operand to our right.
                right = new RPN_Tree;
                right->op = RPN_NOT;
                right->left = tmp->right;
                right->right = nullptr;

                // Delete the old tree.
                tmp->left = nullptr;
                tmp->right = nullptr;
                delete tmp;
                break ;

            case RPN_OR:
                // "not (a | b)" is "(not a) & (not b)".
                op = RPN_AND;
                tmp = left;

                // Move child left operand to our left.
                left = new RPN_Tree;
                left->op = RPN_NOT;
                left->left = tmp->left;
                left->right = nullptr;

                // Move child right operand to our right.
                right = new RPN_Tree;
                right->op = RPN_NOT;
                right->left = tmp->right;
                right->right = nullptr;

                // Delete the old tree.
                tmp->left = nullptr;
                tmp->right = nullptr;
                delete tmp;
                break ;

            case RPN_IMPLY:
                // "not (a => b)" is "(not a) | b".
                op = RPN_OR;
                tmp = left;

                // Move child left operand to our left.
                left = new RPN_Tree;
                left->op = RPN_NOT;
                left->left = tmp->left;
                left->right = nullptr;

                // Move child right operand to our right.
                right = tmp->right;

                // Delete the old tree.
                tmp->left = nullptr;
                tmp->right = nullptr;
                delete tmp;
                break ;

            case RPN_EQUIV:
                // "not (a <=> b)" is "(not a) & b | a & (not b)".
                op = RPN_OR;
                tmp = left;

                // We keep the previous subtree for the left child.
                left = new RPN_Tree;
                left->op = RPN_AND;
                left->left = new RPN_Tree;
                left->left->op = RPN_NOT;
                left->left->left = tmp->left;
                left->right = tmp->right;

                // We need to duplicte the subtree for the right child.
                right = new RPN_Tree;
                right->op = RPN_AND;
                right->left = tmp->left->deepcopy();
                right->right = new RPN_Tree;
                right->right->op = RPN_NOT;
                right->right->left = tmp->right->deepcopy();

                // Delete the old tree.
                tmp->left = nullptr;
                tmp->right = nullptr;
                delete tmp;
                break ;

            default:
                // Nothing to do.
                break ;
        }
    }
    else if (op == RPN_IMPLY)
    {
        // "a => b" is "!a | b".
        op = RPN_OR;
        RPN_Tree *tmp = left;

        left = new RPN_Tree;
        left->op = RPN_NOT;
        left->left = tmp;
    }
    else if (op == RPN_EQUIV)
    {
        // "a <=> b" is "(a => b) & (b => a)".
        op = RPN_AND;
        RPN_Tree *tmpLeft = left;
        RPN_Tree *tmpRight = right;

        // We keep the previous subtree for the left child.
        left = new RPN_Tree;
        left->op = RPN_IMPLY;
        left->left = tmpLeft;
        left->right = tmpRight;

        // We need to duplicte the subtree for the right child.
        right = new RPN_Tree;
        right->op = RPN_IMPLY;
        right->left = tmpRight->deepcopy();
        right->right = tmpLeft->deepcopy();
    }

    if (left)
        left->to_negation_normal_form();
    if (right)
        right->to_negation_normal_form();
}

std::vector<int> RPN_Tree::eval_set(const std::vector<std::vector<int> >& set_list, const std::vector<int>& omega) const
{
    std::vector<int> ret;

    switch (op)
    {
        case RPN_SELF:
            ret = set_list[self - 'A'];
            break ;

        case RPN_NOT:
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
            break ;

        case RPN_AND:
            {
                std::vector<int> left_set = left->eval_set(set_list, omega);
                std::vector<int> right_set = right->eval_set(set_list, omega);
                std::vector<int>::const_iterator left_it = left_set.begin();
                std::vector<int>::const_iterator right_it = right_set.begin();

                while (left_it != left_set.end() && right_it != right_set.end())
                {
                    if (*left_it == *right_it)
                    {
                        ret.push_back(*left_it);
                        ++left_it;
                        ++right_it;
                    }
                    else if (*left_it > *right_it)
                        ++right_it;
                    else
                        ++left_it;
                }
            }
            break ;

        case RPN_OR:
            {
                std::vector<int> left_set = left->eval_set(set_list, omega);
                std::vector<int> right_set = right->eval_set(set_list, omega);
                std::vector<int>::const_iterator left_it = left_set.begin();
                std::vector<int>::const_iterator right_it = right_set.begin();

                while (left_it != left_set.end() || right_it != right_set.end())
                {
                    if (left_it == left_set.end())
                    {
                        ret.push_back(*right_it);
                        ++right_it;
                    }
                    else if (right_it == right_set.end())
                    {
                        ret.push_back(*left_it);
                        ++left_it;
                    }
                    else if (*left_it == *right_it)
                    {
                        ret.push_back(*left_it);
                        ++left_it;
                        ++right_it;
                    }
                    else if (*left_it > *right_it)
                    {
                        ret.push_back(*right_it);
                        ++right_it;
                    }
                    else
                    {
                        ret.push_back(*left_it);
                        ++left_it;
                    }
                }
            }
            break ;

        default:
            // We shouldn't arrive here with a RPN formula in NNF.
            break ;
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
