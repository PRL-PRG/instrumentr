#include "Application.hpp"
#include "Package.hpp"
#include "Function.hpp"
#include "r_function.h"

using instrumentr::Function;
using instrumentr::FunctionSPtr;

SEXP r_function_create_function(SEXP r_name,
                                SEXP r_parameter_count,
                                SEXP r_definition) {
    std::string name = CHAR(asChar(r_name));
    int parameter_count = asInteger(r_parameter_count);
    FunctionSPtr function =
        std::make_shared<Function>(name, parameter_count, r_definition);
    return Function::to_sexp(function);
}

SEXP r_function_get_name(SEXP r_function) {
    FunctionSPtr function = Function::from_sexp(r_function);
    const std::string& name = function->get_name();
    return mkString(name.c_str());
}

SEXP r_function_get_parameter_count(SEXP r_function) {
    FunctionSPtr function = Function::from_sexp(r_function);
    int parameter_count = function->get_parameter_count();
    return ScalarInteger(parameter_count);
}

SEXP r_function_get_definition(SEXP r_function) {
    FunctionSPtr function = Function::from_sexp(r_function);
    SEXP r_definition = function->get_definition();
    return r_definition;
}
