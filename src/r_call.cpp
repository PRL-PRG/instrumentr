#include "../inst/include/Call.hpp"
#include "../inst/include/Argument.hpp"
#include "r_api.hpp"

SEXP r_call_get_id(SEXP r_call) {
    CallSPtr call = Call::from_sexp(r_call);
    return ScalarInteger(call->get_id());
}

SEXP r_call_get_package_name(SEXP r_call) {
    CallSPtr call = Call::from_sexp(r_call);
    return mkString(call->get_package_name().c_str());
}

SEXP r_call_get_function_name(SEXP r_call) {
    CallSPtr call = Call::from_sexp(r_call);
    return mkString(call->get_function_name().c_str());
}

SEXP r_call_get_parameter_count(SEXP r_call) {
    CallSPtr call = Call::from_sexp(r_call);
    return ScalarInteger(call->get_parameter_count());
}

SEXP r_call_get_state(SEXP r_call) {
    CallSPtr call = Call::from_sexp(r_call);
    return mkString(call_state_to_string(call->get_state()).c_str());
}

SEXP r_call_get_argument(SEXP r_call, SEXP r_position) {
    CallSPtr call = Call::from_sexp(r_call);
    int position = asInteger(r_position);
    return Argument::to_sexp(call->get_argument(position));
}
