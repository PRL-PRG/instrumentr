#include "../inst/include/Parameter.hpp"
#include "../inst/include/Call.hpp"
#include "../inst/include/Argument.hpp"
#include "r_parameter.h"

using instrumentr::Argument;
using instrumentr::Parameter;
using instrumentr::ParameterSPtr;

SEXP r_parameter_get_name(SEXP r_parameter) {
    ParameterSPtr parameter = Parameter::from_sexp(r_parameter);
    return mkString(parameter->get_name().c_str());
}

SEXP r_parameter_get_position(SEXP r_parameter) {
    ParameterSPtr parameter = Parameter::from_sexp(r_parameter);
    return ScalarInteger(parameter->get_position());
}

SEXP r_parameter_is_missing(SEXP r_parameter) {
    ParameterSPtr parameter = Parameter::from_sexp(r_parameter);
    return ScalarLogical(parameter->is_missing());
}

SEXP r_parameter_is_vararg(SEXP r_parameter) {
    ParameterSPtr parameter = Parameter::from_sexp(r_parameter);
    return ScalarLogical(parameter->is_vararg());
}

SEXP r_parameter_get_arguments(SEXP r_parameter) {
    ParameterSPtr parameter = Parameter::from_sexp(r_parameter);
    const std::vector<std::shared_ptr<Argument>>& arguments =
        parameter->get_arguments();
    SEXP r_arguments = PROTECT(allocVector(VECSXP, arguments.size()));
    for (int i = 0; i < arguments.size(); ++i) {
        SET_VECTOR_ELT(r_arguments, i, Argument::to_sexp(arguments.at(i)));
    }
    UNPROTECT(1);
    return r_arguments;
}
