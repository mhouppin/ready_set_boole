#include <iostream>
#include <string>
#include <cstring>

bool eval_formula(const std::string& formula)
{
    if (formula.empty())
    {
        std::cout << "Formula cannot be empty" << std::endl;
        return false;
    }

    std::string solver(formula);

    while (solver.size() > 1)
    {
        size_t operatorIndex = solver.find_first_of("!&|^>=");

        if (operatorIndex == std::string::npos)
        {
            std::cout << "Missing operator for the remaining booleans" << std::endl;
            return false;
        }

        char op = solver[operatorIndex];

        if (operatorIndex < 2 - (op == '!'))
        {
            std::cout << "Not enough parameters for the operator" << std::endl;
            return false;
        }

        size_t p = operatorIndex - 2 + (op == '!');

        char first = solver[p];

        if (first != '0' && first != '1')
        {
            std::cout << "First parameter is not a boolean" << std::endl;
            return false;
        }

        char second = solver[p + 1];

        if (!strchr("01!", second))
        {
            std::cout << "Second parameter is not a boolean" << std::endl;
            return false;
        }

        if (second == '!')
        {
            solver.erase(p + 1, 1);
            solver[p] = (first == '1') ? '0' : '1';
            continue ;
        }

        bool left = (first == '1');
        bool right = (second == '1');
        bool result;

        switch (op)
        {
            case '&': result =  left & right; break;
            case '|': result =  left | right; break;
            case '^': result =  left ^ right; break;
            case '>': result = !left | right; break;
            case '=': result = !left ^ right; break;

            default: // Unreachable
                result = false ;
                break ;
        }

        solver.erase(p + 1, 2);
        solver[p] = result ? '1' : '0';
    }

    if (solver[0] != '0' && solver[0] != '1')
    {
        std::cout << "Last formula character isn't a boolean" << std::endl;
        return false;
    }

    return solver[0] == '1';
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        std::cout << "Usage: " << *argv << " RPN_operation [RPN_operation_2 ...]" << std::endl;
        std::cout << std::endl;
        std::cout << "Note: enclose the operation into simple quotes so that";
        std::cout << " the shell doesn't interpret it" << std::endl;
        return 1;
    }

    for (int i = 1; i < argc; ++i)
        std::cout << (eval_formula(argv[i]) ? "True" : "False") << std::endl;

    return 0;
}
