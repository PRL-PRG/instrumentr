#include "../inst/include/Call.hpp"
#include "../inst/include/Function.hpp"
#include "../inst/include/Parameter.hpp"
#include "r_api.h"

using lightr::Call;
using lightr::CallSPtr;
using lightr::Function;
using lightr::Parameter;

SEXP r_call_get_function(SEXP r_call) {
    CallSPtr call = Call::from_sexp(r_call);
    return Function::to_sexp(call->get_function());
}

SEXP r_call_get_state(SEXP r_call) {
    CallSPtr call = Call::from_sexp(r_call);
    return mkString(call_state_to_string(call->get_state()).c_str());
}

SEXP r_call_get_parameters(SEXP r_call) {
    CallSPtr call = Call::from_sexp(r_call);
    const std::vector<std::shared_ptr<Parameter>>& parameters =
        call->get_parameters();
    SEXP r_parameters = PROTECT(allocVector(VECSXP, parameters.size()));
    for (int i = 0; i < parameters.size(); ++i) {
        SET_VECTOR_ELT(r_parameters, i, Parameter::to_sexp(parameters.at(i)));
    }
    UNPROTECT(1);
    return r_parameters;
}
