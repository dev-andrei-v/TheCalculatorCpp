#include "pipeline/Tokenizer.h"
#include <cctype>
#include <stdexcept>

namespace calc {

std::vector<Token> Tokenizer::tokenize(const std::string& input) const {
    std::vector<Token> tokens;
    size_t i = 0;

    while (i < input.size()) {
        char c = input[i];

        if (std::isspace(static_cast<unsigned char>(c))) {
            ++i;
            continue;
        }

        if (std::isdigit(static_cast<unsigned char>(c)) || c == '.') {
            tokens.push_back(Token::number(readNumber(input, i)));
            continue;
        }

        // Unary minus: at start, after an operator, or after a left paren
        if (c == '-' && (tokens.empty() ||
                         tokens.back().type == TokenType::Operator ||
                         tokens.back().type == TokenType::LeftParen)) {
            ++i;
            if (i < input.size() && (std::isdigit(static_cast<unsigned char>(input[i])) || input[i] == '.')) {
                tokens.push_back(Token::number("-" + readNumber(input, i)));
            } else {
                tokens.push_back(Token::number("0"));
                tokens.push_back(Token::op("-"));
            }
            continue;
        }

        if (c == '(' || c == '[') {
            tokens.push_back(Token::leftParen());
            ++i;
            continue;
        }

        if (c == ')' || c == ']') {
            tokens.push_back(Token::rightParen());
            ++i;
            continue;
        }

        if (isOperatorChar(c)) {
            tokens.push_back(Token::op(std::string(1, c)));
            ++i;
            continue;
        }

        if (std::isalpha(static_cast<unsigned char>(c))) {
            std::string name;
            while (i < input.size() && std::isalpha(static_cast<unsigned char>(input[i]))) {
                name += input[i];
                ++i;
            }
            tokens.push_back(Token::variable(name));
            continue;
        }

        throw std::runtime_error("Unexpected character: '" + std::string(1, c) + "'");
    }

    if (tokens.empty()) {
        throw std::runtime_error("Empty expression");
    }

    return tokens;
}

std::string Tokenizer::readNumber(const std::string& input, size_t& pos) const {
    std::string num;
    bool hasDot = false;

    while (pos < input.size()) {
        char c = input[pos];
        if (std::isdigit(static_cast<unsigned char>(c))) {
            num += c;
            ++pos;
        } else if (c == '.' && !hasDot) {
            hasDot = true;
            num += c;
            ++pos;
        } else {
            break;
        }
    }

    if (num.empty() || num == ".") {
        throw std::runtime_error("Invalid number");
    }

    return num;
}

bool Tokenizer::isOperatorChar(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

} // namespace calc
