#include "../inst/include/lightr.hpp"
#include "../inst/include/Context.hpp"
#include "../inst/include/Application.hpp"
#include "../inst/include/Package.hpp"
#include "../inst/include/Function.hpp"
#include "../inst/include/Call.hpp"
#include "../inst/include/Parameter.hpp"
#include "../inst/include/Argument.hpp"
#include "utilities.h"
#include <vector>

namespace lightr {

std::vector<bool> tracing_status;

SEXP InvalidValue = NULL;

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

SEXP get_invalid_value() {
    return InvalidValue;
}

bool is_invalid_value(SEXP value) {
    return value == InvalidValue;
}

bool is_valid_value(SEXP value) {
    return !(is_invalid_value(value));
}

void initialize_lightr(SEXP r_package_environment,
                       SEXP r_state_environment,
                       SEXP r_invalid_value) {
    initialize_utilities(r_package_environment, r_state_environment);
    InvalidValue = r_invalid_value;
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
