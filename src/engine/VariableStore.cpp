#include "engine/VariableStore.h"

namespace calc {

void VariableStore::set(const std::string& name, double value) {
    variables_[name] = value;
}

double VariableStore::get(const std::string& name) const {
    auto it = variables_.find(name);
    if (it == variables_.end()) {
        throw std::runtime_error("Undefined variable: " + name);
    }
    return it->second;
}

bool VariableStore::has(const std::string& name) const {
    return variables_.count(name) > 0;
}

bool VariableStore::remove(const std::string& name) {
    return variables_.erase(name) > 0;
}

void VariableStore::clear() {
    variables_.clear();
}

bool VariableStore::empty() const {
    return variables_.empty();
}

const std::map<std::string, double>& VariableStore::all() const {
    return variables_;
}

} // namespace calc
