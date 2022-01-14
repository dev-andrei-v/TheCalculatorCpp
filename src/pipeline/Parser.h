#pragma once

#include "pipeline/Token.h"
#include <vector>
#include <unordered_map>
#include <string>

namespace calc {

struct OperatorInfo {
    int precedence;
    bool rightAssociative;
};

class Parser {
public:
    Parser();
    std::vector<Token> toPostfix(const std::vector<Token>& tokens) const;

private:
    std::unordered_map<std::string, OperatorInfo> operators_;
};

} // namespace calc
