#include "Parameter.hpp"
#include "Call.hpp"
#include "Argument.hpp"
#include "r_parameter.h"

using instrumentr::Argument;
using instrumentr::ArgumentSPtr;
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
    const std::vector<ArgumentSPtr>& arguments = parameter->get_arguments();
    int size = arguments.size();
    SEXP r_arguments = PROTECT(allocVector(VECSXP, size));

    for (int i = 0; i < size; ++i) {
        SET_VECTOR_ELT(r_arguments, i, Argument::to_sexp(arguments.at(i)));
    }

    if (parameter->is_vararg()) {
        SEXP r_names = PROTECT(allocVector(STRSXP, size));
        for (int i = 0; i < size; ++i) {
            SET_STRING_ELT(
                r_names, i, mkChar(arguments.at(i)->get_name().c_str()));
        }
        Rf_setAttrib(r_arguments, R_NamesSymbol, r_names);
        UNPROTECT(1);
    }

    UNPROTECT(1);
    return r_arguments;
}
