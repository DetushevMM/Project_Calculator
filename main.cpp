#include "calculator.hpp"

#include <iostream>
#include <string>

int main() {
    std::cout << "=============================\n";
    std::cout << "      C++ Calculator\n";
    std::cout << "=============================\n";
    std::cout << "Enter an arithmetic expression.\n";
    std::cout << "Type 'exit' to quit.\n\n";

    std::string input;

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);

        if (input == "exit") {
            break;
        }

        try {
            int result = CalculateExpression(input);
            std::cout << "= " << result << '\n';
        }
        catch (const UnknownSymbolError& e) {
            std::cerr << "Unknown symbol: " << e.what() << '\n';
        }
        catch (const WrongExpressionError& e) {
            std::cerr << "Wrong expression: " << e.what() << '\n';
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << '\n';
        }
    }

    return 0;
}