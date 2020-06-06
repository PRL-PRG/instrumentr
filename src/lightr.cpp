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

std::vector<bool> tracing_status_stack;
std::vector<ExecutionContext> execution_context_stack;

SEXP InvalidValue = NULL;

bool is_tracing_enabled() {
    return !tracing_status_stack.empty() && tracing_status_stack.back();
}

void set_tracing_status(bool tracing_status) {
    tracing_status_stack.push_back(tracing_status);
}

void enable_tracing() {
    tracing_status_stack.push_back(true);
}

void disable_tracing() {
    tracing_status_stack.push_back(false);
}

void reinstate_tracing() {
    if (!tracing_status_stack.empty()) {
        tracing_status_stack.pop_back();
    }
}

void clear_tracing() {
    tracing_status_stack.clear();
}

ExecutionContext peek_execution_context() {
    return execution_context_stack.back();
}

void push_execution_context(ExecutionContext execution_context) {
    execution_context_stack.push_back(execution_context);
}

ExecutionContext pop_execution_context() {
    ExecutionContext execution_context = execution_context_stack.back();
    execution_context_stack.pop_back();
    return execution_context;
}

void clear_execution_context() {
    execution_context_stack.clear();
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
