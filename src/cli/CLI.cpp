#include "cli/CLI.h"
#include <iostream>
#include <iomanip>

namespace calc {

std::string CLI::trim(const std::string& s) {
    auto start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    auto end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

int CLI::evaluateAndPrint(const std::string& expression) {
    try {
        Calculator calc;
        double result = calc.evaluate(expression);
        std::cout << result << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}

void CLI::run() {
    std::cout << "TheCalculator v2.0\n";
    std::cout << "Type 'help' for available commands, or enter an expression.\n\n";

    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;

        line = trim(line);
        if (line.empty()) continue;
        if (line == "exit" || line == "quit") break;

        try {
            if (!commands_.dispatch(line)) {
                handleExpression(line);
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}

void CLI::handleExpression(const std::string& expr) {
    double result = calculator_.evaluate(expr);
    calculator_.variables().set("ans", result);
    std::cout << std::setprecision(precision_) << result << std::endl;
}

} // namespace calc
