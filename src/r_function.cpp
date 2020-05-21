#include "../inst/include/Application.hpp"
#include "../inst/include/Package.hpp"
#include "../inst/include/Function.hpp"
#include "r_api.hpp"

using lightr::Function;
using lightr::FunctionSPtr;
using lightr::Package;
using lightr::PackageSPtr;

SEXP r_function_get_package(SEXP r_function) {
    FunctionSPtr function = Function::from_sexp(r_function);
    PackageSPtr package = function->get_package();
    return Package::to_sexp(package);
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
