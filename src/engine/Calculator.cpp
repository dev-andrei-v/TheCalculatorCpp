#include "engine/Calculator.h"

namespace calc {

double Calculator::evaluate(const std::string& expression) {
    auto tokens = tokenizer_.tokenize(expression);
    auto postfix = parser_.toPostfix(tokens);

    auto resolver = [this](const std::string& name) -> double {
        return variables_.get(name);
    };

    return evaluator_.evaluate(postfix, resolver);
}

VariableStore& Calculator::variables() {
    return variables_;
}

const VariableStore& Calculator::variables() const {
    return variables_;
}

} // namespace calc
