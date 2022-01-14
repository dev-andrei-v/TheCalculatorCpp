#pragma once

#include "engine/Calculator.h"
#include "cli/CommandRouter.h"
#include <string>

namespace calc {

class CLI {
public:
    void run();
    static int evaluateAndPrint(const std::string& expression);

private:
    Calculator calculator_;
    int precision_ = 15;
    CommandRouter commands_{calculator_, precision_};

    void handleExpression(const std::string& expr);

    static std::string trim(const std::string& s);
};

} // namespace calc
