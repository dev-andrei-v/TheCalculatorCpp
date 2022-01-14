#pragma once

#include "pipeline/Tokenizer.h"
#include "pipeline/Parser.h"
#include "pipeline/Evaluator.h"
#include "engine/VariableStore.h"
#include <string>

namespace calc {

class Calculator {
public:
    double evaluate(const std::string& expression);

    VariableStore& variables();
    const VariableStore& variables() const;

private:
    Tokenizer tokenizer_;
    Parser parser_;
    Evaluator evaluator_;
    VariableStore variables_;
};

} // namespace calc
