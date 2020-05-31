#include "../inst/include/Argument.hpp"
#include "r_api.h"

using lightr::Argument;
using lightr::ArgumentSPtr;

SEXP r_argument_get_name(SEXP r_argument) {
    ArgumentSPtr argument = Argument::from_sexp(r_argument);
    return mkString(argument->get_name().c_str());
}

SEXP r_argument_is_evaluated(SEXP r_argument) {
    ArgumentSPtr argument = Argument::from_sexp(r_argument);
    return ScalarLogical(argument->is_evaluated());
}
