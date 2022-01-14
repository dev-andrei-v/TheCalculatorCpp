#pragma once

#include <string>
#include <map>
#include <stdexcept>

namespace calc {

class VariableStore {
public:
    void set(const std::string& name, double value);
    double get(const std::string& name) const;
    bool has(const std::string& name) const;
    bool remove(const std::string& name);
    void clear();
    bool empty() const;
    const std::map<std::string, double>& all() const;

private:
    std::map<std::string, double> variables_;
};

} // namespace calc
