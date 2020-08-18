#include "Argument.hpp"
#include "r_argument.h"

using instrumentr::Argument;
using instrumentr::ArgumentSPtr;
using instrumentr::from_sexp;
using instrumentr::to_sexp;

SEXP r_argument_get_name(SEXP r_argument) {
    ArgumentSPtr argument = from_sexp<Argument>(r_argument);
    return mkString(argument->get_name().c_str());
}

SEXP r_argument_is_evaluated(SEXP r_argument) {
    ArgumentSPtr argument = from_sexp<Argument>(r_argument);
    return ScalarLogical(argument->is_evaluated());
}

SEXP r_argument_get_expression(SEXP r_argument) {
    ArgumentSPtr argument = from_sexp<Argument>(r_argument);
    return argument->get_expression();
}

SEXP r_argument_get_result(SEXP r_argument) {
    ArgumentSPtr argument = from_sexp<Argument>(r_argument);
    return argument->get_result();
}
