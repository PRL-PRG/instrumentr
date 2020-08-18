#include "r_function_attach_callback.h"
#include "r_callback.h"
#include "FunctionAttachCallback.hpp"

using instrumentr::FunctionAttachCallback;

SEXP r_function_attach_callback_create_from_r_function(SEXP r_function) {
    return r_callback_create_from_r_function<FunctionAttachCallback>(r_function);
}

SEXP r_function_attach_callback_create_from_c_function(SEXP r_c_function) {
    return r_callback_create_from_c_function<FunctionAttachCallback>(r_c_function);
}
