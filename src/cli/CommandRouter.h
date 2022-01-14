#pragma once

#include "engine/Calculator.h"
#include <string>

namespace calc {

class CommandRouter {
public:
    explicit CommandRouter(Calculator& calculator, int& precision);

    // Returns false if the input is an expression (not a command).
    bool dispatch(const std::string& input);

private:
    Calculator& calculator_;
    int& precision_;

    void handleLet(const std::string& args);
    void handleDel(const std::string& name);
    void handleLoad(const std::string& filename);
    void handleSave(const std::string& args);
    void handlePrecision(const std::string& value);
    void printVariables() const;

    static void printHelp();
    static std::string trim(const std::string& s);
    static bool startsWith(const std::string& s, const std::string& prefix);
};

} // namespace calc
