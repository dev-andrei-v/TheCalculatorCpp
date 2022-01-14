#include "pipeline/Evaluator.h"
#include <stack>
#include <cmath>
#include <stdexcept>

namespace calc {

Evaluator::Evaluator() {
    operations_["+"] = [](double a, double b) { return a + b; };
    operations_["-"] = [](double a, double b) { return a - b; };
    operations_["*"] = [](double a, double b) { return a * b; };
    operations_["/"] = [](double a, double b) {
        if (b == 0.0) throw std::runtime_error("Division by zero");
        return a / b;
    };
    operations_["^"] = [](double a, double b) { return std::pow(a, b); };
}

double Evaluator::evaluate(const std::vector<Token>& postfix,
                           const VariableResolver& resolver) const {
    std::stack<double> stack;

    for (const auto& token : postfix) {
        switch (token.type) {
        case TokenType::Number:
            stack.push(std::stod(token.value));
            break;

        case TokenType::Variable: {
            if (!resolver) {
                throw std::runtime_error("Undefined variable: " + token.value);
            }
            stack.push(resolver(token.value));
            break;
        }

        case TokenType::Operator: {
            if (stack.size() < 2) {
                throw std::runtime_error("Invalid expression");
            }
            double right = stack.top(); stack.pop();
            double left = stack.top(); stack.pop();

            auto it = operations_.find(token.value);
            if (it == operations_.end()) {
                throw std::runtime_error("Unknown operator: " + token.value);
            }
            stack.push(it->second(left, right));
            break;
        }

        default:
            throw std::runtime_error("Unexpected token in postfix expression");
        }
    }

    if (stack.size() != 1) {
        throw std::runtime_error("Invalid expression");
    }

    return stack.top();
}

} // namespace calc
