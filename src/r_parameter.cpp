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

SEXP r_parameter_get_argument_by_name(SEXP r_parameter, SEXP r_name) {
    ParameterSPtr parameter = Parameter::from_sexp(r_parameter);
    const std::string name(CHAR(asChar(r_name)));

    const std::vector<ArgumentSPtr>& arguments = parameter->get_arguments();
    int size = arguments.size();

    for (int i = 0; i < size; ++i) {
        ArgumentSPtr argument = arguments.at(i);
        if (argument->get_name() == name) {
            return Argument::to_sexp(argument);
        }
    }

    Rf_error("argument named '%s' does not exist", name.c_str());

    return R_NilValue;
}

SEXP r_parameter_get_argument_by_position(SEXP r_parameter, SEXP r_position) {
    ParameterSPtr parameter = Parameter::from_sexp(r_parameter);
    /* NOTE: 1 based indexing at R level and 0 based indexing at C++ level */
    int position = asInteger(r_position) - 1;

    const std::vector<ArgumentSPtr>& arguments = parameter->get_arguments();
    int size = arguments.size();

    if (position < 0 || position >= size) {
        Rf_error("accessing argument at position %d from a parameter with %d "
                 "argument(s)",
                 position + 1,
                 size);
        return R_NilValue;
    }

    ArgumentSPtr argument = arguments.at(position);
    return Argument::to_sexp(argument);
}
