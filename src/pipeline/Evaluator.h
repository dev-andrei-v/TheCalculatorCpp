#pragma once

#include "pipeline/Token.h"
#include <vector>
#include <functional>
#include <unordered_map>
#include <string>

namespace calc {

using VariableResolver = std::function<double(const std::string&)>;

class Evaluator {
public:
    Evaluator();
    double evaluate(const std::vector<Token>& postfix,
                    const VariableResolver& resolver = nullptr) const;

private:
    std::unordered_map<std::string, std::function<double(double, double)>> operations_;
};

} // namespace calc
