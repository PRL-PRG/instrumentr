#include "../inst/include/Application.hpp"
#include "../inst/include/Package.hpp"
#include "../inst/include/Function.hpp"
#include "r_api.h"

using lightr::Function;
using lightr::FunctionSPtr;

SEXP r_function_create(SEXP r_function_name,
                       SEXP r_parameter_count,
                       SEXP r_function_object) {
    std::string function_name = CHAR(asChar(r_function_name));
    int parameter_count = asInteger(r_parameter_count);
    FunctionSPtr function = std::make_shared<Function>(
        function_name, parameter_count, r_function_object);
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

SEXP r_function_get_object(SEXP r_function) {
    FunctionSPtr function = Function::from_sexp(r_function);
    SEXP r_function_object = function->get_object();
    return r_function_object;
}
