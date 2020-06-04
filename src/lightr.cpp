#include "../inst/include/lightr.hpp"
#include "../inst/include/Parameter.hpp"
#include "utilities.h"
#include <vector>

namespace lightr {

std::vector<bool> tracing_status;

bool is_tracing_enabled() {
    return !tracing_status.empty() && tracing_status.back();
}

void enable_tracing() {
    tracing_status.push_back(true);
}

void disable_tracing() {
    tracing_status.push_back(false);
}

void reinstate_tracing() {
    if (!tracing_status.empty()) {
        tracing_status.pop_back();
    }
}

void initialize_lightr(SEXP r_package_environment, SEXP r_state_environment) {
    initialize_utilities(r_package_environment, r_state_environment);
    Context::initialize();
    Application::initialize();
    CallStack::initialize();
    Package::initialize();
    Function::initialize();
    Call::initialize();
    Parameter::initialize();
    Argument::initialize();
}

} // namespace lightr
