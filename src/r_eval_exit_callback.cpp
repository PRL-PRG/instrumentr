#include "r_eval_exit_callback.h"
#include "r_callback.h"
#include "EvalExitCallback.hpp"

using instrumentr::EvalExitCallback;

SEXP r_eval_exit_callback_create_from_r_function(SEXP r_function) {
    return r_callback_create_from_r_function<EvalExitCallback>(r_function);
}

SEXP r_eval_exit_callback_create_from_c_function(SEXP r_c_function) {
    return r_callback_create_from_c_function<EvalExitCallback>(r_c_function);
}
