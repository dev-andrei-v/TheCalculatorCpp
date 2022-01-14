#pragma once

#include <string>

namespace calc {

enum class TokenType {
    Number,
    Operator,
    LeftParen,
    RightParen,
    Variable
};

struct Token {
    TokenType type;
    std::string value;

    static Token number(const std::string& v) { return {TokenType::Number, v}; }
    static Token op(const std::string& v) { return {TokenType::Operator, v}; }
    static Token leftParen() { return {TokenType::LeftParen, "("}; }
    static Token rightParen() { return {TokenType::RightParen, ")"}; }
    static Token variable(const std::string& v) { return {TokenType::Variable, v}; }
};

} // namespace calc
