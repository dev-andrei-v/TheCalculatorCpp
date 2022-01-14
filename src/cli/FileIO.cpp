#include "cli/FileIO.h"
#include <fstream>
#include <iomanip>
#include <stdexcept>

namespace calc {

static std::string trim(const std::string& s) {
    auto start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    auto end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

void FileIO::loadExpressions(const std::string& filename,
                              Calculator& calculator,
                              const LineResultCallback& onResult,
                              const LineErrorCallback& onError) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    std::string line;
    int lineNum = 0;
    while (std::getline(file, line)) {
        ++lineNum;
        line = trim(line);
        if (line.empty()) continue;

        try {
            double result = calculator.evaluate(line);
            calculator.variables().set("ans", result);
            onResult(lineNum, line, result);
        } catch (const std::exception& e) {
            onError(lineNum, e.what());
        }
    }
}

void FileIO::saveValue(const std::string& filename, double value, int precision) {
    std::ofstream file(filename, std::ios::app);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    file << std::setprecision(precision) << value << std::endl;
}

} // namespace calc
