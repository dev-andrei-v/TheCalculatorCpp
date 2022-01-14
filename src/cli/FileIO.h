#pragma once

#include "engine/Calculator.h"
#include <string>
#include <functional>

namespace calc {

using LineResultCallback = std::function<void(int lineNum, const std::string& expr, double result)>;
using LineErrorCallback = std::function<void(int lineNum, const std::string& error)>;

class FileIO {
public:
    static void loadExpressions(const std::string& filename,
                                Calculator& calculator,
                                const LineResultCallback& onResult,
                                const LineErrorCallback& onError);

    static void saveValue(const std::string& filename, double value, int precision);
};

} // namespace calc
