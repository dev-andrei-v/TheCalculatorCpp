#include "pipeline/Parser.h"
#include <stack>
#include <stdexcept>

namespace calc {

Parser::Parser() {
    operators_["+"] = {1, false};
    operators_["-"] = {1, false};
    operators_["*"] = {2, false};
    operators_["/"] = {2, false};
    operators_["^"] = {3, true};
}

std::vector<Token> Parser::toPostfix(const std::vector<Token>& tokens) const {
    std::vector<Token> output;
    std::stack<Token> opStack;

    for (const auto& token : tokens) {
        switch (token.type) {
        case TokenType::Number:
        case TokenType::Variable:
            output.push_back(token);
            break;

        case TokenType::Operator: {
            auto it = operators_.find(token.value);
            if (it == operators_.end()) {
                throw std::runtime_error("Unknown operator: " + token.value);
            }
            const auto& info = it->second;

            while (!opStack.empty() && opStack.top().type == TokenType::Operator) {
                auto topIt = operators_.find(opStack.top().value);
                if (topIt == operators_.end()) break;

                const auto& topInfo = topIt->second;
                if ((!info.rightAssociative && info.precedence <= topInfo.precedence) ||
                    (info.rightAssociative && info.precedence < topInfo.precedence)) {
                    output.push_back(opStack.top());
                    opStack.pop();
                } else {
                    break;
                }
            }
            opStack.push(token);
            break;
        }

        case TokenType::LeftParen:
            opStack.push(token);
            break;

        case TokenType::RightParen:
            while (!opStack.empty() && opStack.top().type != TokenType::LeftParen) {
                output.push_back(opStack.top());
                opStack.pop();
            }
            if (opStack.empty()) {
                throw std::runtime_error("Mismatched parentheses");
            }
            opStack.pop();
            break;
        }
    }

    while (!opStack.empty()) {
        if (opStack.top().type == TokenType::LeftParen) {
            throw std::runtime_error("Mismatched parentheses");
        }
        output.push_back(opStack.top());
        opStack.pop();
    }

    return output;
}

} // namespace calc
