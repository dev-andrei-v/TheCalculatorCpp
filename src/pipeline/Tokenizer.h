#pragma once

#include "pipeline/Token.h"
#include <vector>
#include <string>

namespace calc {

class Tokenizer {
public:
    std::vector<Token> tokenize(const std::string& input) const;

private:
    std::string readNumber(const std::string& input, size_t& pos) const;
    static bool isOperatorChar(char c);
};

} // namespace calc
