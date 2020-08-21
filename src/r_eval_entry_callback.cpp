#include "r_eval_entry_callback.h"
#include "r_callback.h"
#include "EvalEntryCallback.hpp"

using instrumentr::EvalEntryCallback;

SEXP r_eval_entry_callback_create_from_r_function(SEXP r_function) {
    return r_callback_create_from_r_function<EvalEntryCallback>(r_function);
}

SEXP r_eval_entry_callback_create_from_c_function(SEXP r_c_function) {
    return r_callback_create_from_c_function<EvalEntryCallback>(r_c_function);
}
