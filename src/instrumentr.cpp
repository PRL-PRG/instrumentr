#include "instrumentr.hpp"
#include "Context.hpp"
#include "Application.hpp"
#include "Package.hpp"
#include "Function.hpp"
#include "Call.hpp"
#include "Parameter.hpp"
#include "Argument.hpp"
#include "utilities.h"
#include <vector>

namespace instrumentr {

SEXP UndefinedObject = NULL;
std::vector<ContextSPtr> contexts;

const char* get_commit_hash() {
    return GIT_COMMIT_HASH;
}

SEXP get_undefined_object() {
    return UndefinedObject;
}

bool is_undefined_object(SEXP object) {
    return object == UndefinedObject;
}

bool is_defined_object(SEXP object) {
    return !(is_undefined_object(object));
}

void add_context(ContextSPtr context) {
    contexts.push_back(context);
}

void remove_context(ContextSPtr context) {
    for (int index = contexts.size() - 1; index >= 0; --index) {
        if (contexts[index] == context) {
            contexts.erase(contexts.begin() + index);
            return;
        }
    }
}

bool is_tracing_enabled() {
    return !contexts.empty() && contexts.back()->is_tracing_enabled();
}

void set_tracing_status(bool tracing_status) {
    for (int index = 0; index < contexts.size(); ++index) {
        contexts[index]->set_tracing_status(tracing_status);
    }
}

void enable_tracing() {
    set_tracing_status(true);
}

void disable_tracing() {
    set_tracing_status(false);
}

void reinstate_tracing() {
    for (int index = 0; index < contexts.size(); ++index) {
        contexts[index]->reinstate_tracing();
    }
}

void initialize_instrumentr(SEXP r_package_environment,
                            SEXP r_state_environment,
                            SEXP r_undefined_object) {
    if (UndefinedObject == NULL) {
        initialize_utilities(r_package_environment, r_state_environment);
        UndefinedObject = r_undefined_object;
        Context::initialize();
        Application::initialize();
        CallStack::initialize();
        Package::initialize();
        Function::initialize();
        Call::initialize();
        Parameter::initialize();
        Argument::initialize();
    }
}

void finalize_instrumentr() {
    if (UndefinedObject != NULL) {
        finalize_utilities();
        UndefinedObject = NULL;
        Context::finalize();
        Application::finalize();
        CallStack::finalize();
        Package::finalize();
        Function::finalize();
        Call::initialize();
        Parameter::initialize();
        Argument::initialize();
    }
}

} // namespace instrumentr
