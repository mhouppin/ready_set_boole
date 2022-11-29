#pragma once
#include <iostream>
#include <string>
#include <vector>

struct RPN_Variable
{
    char letter;
    bool value;
};

size_t rpnvar_parse(const std::string& rpnStr, RPN_Variable vars[26]);

enum RPN_Operator
{
    RPN_SELF,
    RPN_NOT,
    RPN_AND,
    RPN_OR,
    RPN_XOR,
    RPN_IMPLY,
    RPN_EQUIV,
    RPN_OPCOUNT,
    RPN_NONE = RPN_OPCOUNT
};

std::string rpn_op_str(RPN_Operator op);

struct RPN_Tree
{
    char self;
    bool selfValue;
    RPN_Tree *left;
    RPN_Tree *right;
    RPN_Operator op;

    RPN_Tree() : self('\0'), selfValue(false), left(nullptr), right(nullptr), op(RPN_NONE) { }

    inline ~RPN_Tree()
    {
        if (left)
            delete left;

        if (right)
            delete right;
    }

    inline RPN_Tree *deepcopy() const
    {
        RPN_Tree *newTree = new RPN_Tree;
        newTree->self = self;
        newTree->selfValue = selfValue;
        newTree->op = op;

        if (left)
            newTree->left = left->deepcopy();
        else
            newTree->left = nullptr;

        if (right)
            newTree->right = right->deepcopy();
        else
            newTree->right = nullptr;

        return newTree;
    }

    // Parses a string written in Reverse Polish Notation.
    std::string rpn_parse(const std::string& rpnStr);
    std::string rpn_out() const;

    std::vector<int> eval_set(const std::vector<std::vector<int> >& set_list, const std::vector<int>& omega) const;

    // Recursively checks if none of the siblings have their op set to RPN_NONE.
    inline bool is_valid(void) const
    {
        if (op == RPN_NONE)
            return false;

        if (left)
            return left->is_valid() && (!right || right->is_valid());

        return true;
    }

    // Recursively sets the variable values in the tree.
    inline void propagate(RPN_Variable *rpnVar, size_t varCount)
    {
        if (op != RPN_SELF)
        {
            left->propagate(rpnVar, varCount);
            if (op != RPN_NOT)
                right->propagate(rpnVar, varCount);
        }
        else
        {
            for (size_t i = 0; i < varCount; ++i)
                if (rpnVar[i].letter == self)
                {
                    selfValue = rpnVar[i].value;
                    break ;
                }
        }
    }

    // Recursively evaluates the tree.
    inline void evaluate(void)
    {
        if (op == RPN_SELF)
            return ;

        left->evaluate();

        if (op == RPN_NOT)
        {
            selfValue = !left->selfValue;
            return ;
        }

        right->evaluate();

        switch (op)
        {
            case RPN_AND:   selfValue =  left->selfValue & right->selfValue; break ;
            case RPN_OR:    selfValue =  left->selfValue | right->selfValue; break ;
            case RPN_XOR:   selfValue =  left->selfValue ^ right->selfValue; break ;
            case RPN_IMPLY: selfValue = !left->selfValue | right->selfValue; break ;
            case RPN_EQUIV: selfValue = !left->selfValue ^ right->selfValue; break ;

            default: // Normally unreachable
                selfValue = false;
                break ;
        }
    }
};

std::ostream& operator<<(std::ostream& os, const RPN_Operator& op);
std::ostream& operator<<(std::ostream& os, const RPN_Tree& tree);
