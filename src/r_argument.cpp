#include "../inst/include/Argument.hpp"
#include "r_argument.h"

using instrumentr::Argument;
using instrumentr::ArgumentSPtr;

SEXP r_argument_get_name(SEXP r_argument) {
    ArgumentSPtr argument = Argument::from_sexp(r_argument);
    return mkString(argument->get_name().c_str());
}

SEXP r_argument_is_evaluated(SEXP r_argument) {
    ArgumentSPtr argument = Argument::from_sexp(r_argument);
    return ScalarLogical(argument->is_evaluated());
}

SEXP r_argument_get_expression(SEXP r_argument) {
    ArgumentSPtr argument = Argument::from_sexp(r_argument);
    return argument->get_expression();
}

SEXP r_argument_get_result(SEXP r_argument) {
    ArgumentSPtr argument = Argument::from_sexp(r_argument);
    return argument->get_result();
}
