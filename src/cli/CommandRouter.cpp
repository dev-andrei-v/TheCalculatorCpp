#include "cli/CommandRouter.h"
#include "cli/FileIO.h"
#include <iostream>
#include <iomanip>

namespace calc {

std::string CommandRouter::trim(const std::string& s) {
    auto start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    auto end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

bool CommandRouter::startsWith(const std::string& s, const std::string& prefix) {
    return s.size() >= prefix.size() && s.compare(0, prefix.size(), prefix) == 0;
}

CommandRouter::CommandRouter(Calculator& calculator, int& precision)
    : calculator_(calculator), precision_(precision) {}

bool CommandRouter::dispatch(const std::string& input) {
    if (input == "help") { printHelp(); return true; }
    if (input == "vars") { printVariables(); return true; }

    if (input == "clear") {
        calculator_.variables().clear();
        std::cout << "All variables cleared." << std::endl;
        return true;
    }

    if (startsWith(input, "let "))       { handleLet(input.substr(4)); return true; }
    if (startsWith(input, "del "))       { handleDel(trim(input.substr(4))); return true; }
    if (startsWith(input, "load "))      { handleLoad(trim(input.substr(5))); return true; }
    if (startsWith(input, "save "))      { handleSave(trim(input.substr(5))); return true; }
    if (startsWith(input, "precision ")) { handlePrecision(trim(input.substr(10))); return true; }

    return false; // not a command — treat as expression
}

void CommandRouter::handleLet(const std::string& args) {
    auto eq = args.find('=');
    if (eq == std::string::npos) {
        throw std::runtime_error("Syntax: let <name> = <expression>");
    }

    std::string name = trim(args.substr(0, eq));
    std::string expr = trim(args.substr(eq + 1));

    if (name.empty() || expr.empty()) {
        throw std::runtime_error("Syntax: let <name> = <expression>");
    }

    double value = calculator_.evaluate(expr);
    calculator_.variables().set(name, value);
    std::cout << name << " = " << std::setprecision(precision_) << value << std::endl;
}

void CommandRouter::handleDel(const std::string& name) {
    if (calculator_.variables().remove(name)) {
        std::cout << "Deleted variable '" << name << "'." << std::endl;
    } else {
        std::cerr << "Variable '" << name << "' not found." << std::endl;
    }
}

void CommandRouter::handleLoad(const std::string& filename) {
    int prec = precision_;
    FileIO::loadExpressions(filename, calculator_,
        [prec](int, const std::string& expr, double result) {
            std::cout << std::setprecision(prec) << expr << " = " << result << std::endl;
        },
        [](int lineNum, const std::string& error) {
            std::cerr << "Line " << lineNum << ": " << error << std::endl;
        }
    );
}

void CommandRouter::handleSave(const std::string& args) {
    auto space = args.find(' ');
    std::string filename;
    std::string expr;

    if (space != std::string::npos) {
        filename = args.substr(0, space);
        expr = trim(args.substr(space + 1));
    } else {
        filename = args;
    }

    double value;
    if (expr.empty()) {
        if (!calculator_.variables().has("ans")) {
            throw std::runtime_error("No result to save. Evaluate an expression first.");
        }
        value = calculator_.variables().get("ans");
    } else {
        value = calculator_.evaluate(expr);
    }

    FileIO::saveValue(filename, value, precision_);
    std::cout << "Saved to " << filename << "." << std::endl;
}

void CommandRouter::handlePrecision(const std::string& value) {
    int p = std::stoi(value);
    if (p >= 0 && p <= 20) {
        precision_ = p;
        std::cout << "Precision set to " << p << "." << std::endl;
    } else {
        std::cerr << "Precision must be between 0 and 20." << std::endl;
    }
}

void CommandRouter::printHelp() {
    std::cout << "\nCommands:\n"
              << "  <expression>        Evaluate a math expression\n"
              << "  let <name> = <expr> Assign result to a variable\n"
              << "  vars                Show all variables\n"
              << "  del <name>          Delete a variable\n"
              << "  clear               Delete all variables\n"
              << "  load <file>         Evaluate expressions from a file\n"
              << "  save <file> [expr]  Save result to a file\n"
              << "  precision <n>       Set decimal precision (0-20)\n"
              << "  help                Show this help\n"
              << "  exit                Exit the calculator\n"
              << "\nOperators: + - * / ^ (power)\n"
              << "Parentheses: ( ) [ ]\n"
              << "Variables: 'ans' always holds the last result\n\n";
}

void CommandRouter::printVariables() const {
    const auto& vars = calculator_.variables().all();
    if (vars.empty()) {
        std::cout << "No variables defined." << std::endl;
        return;
    }

    for (const auto& [name, value] : vars) {
        std::cout << "  " << name << " = "
                  << std::setprecision(precision_) << value << std::endl;
    }
}

} // namespace calc
