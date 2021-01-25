#include "r_object_coerce_callback.h"
#include "r_callback.h"
#include "ObjectCoerceCallback.hpp"

using instrumentr::ObjectCoerceCallback;

SEXP r_object_coerce_callback_create_from_r_function(SEXP r_function) {
    return r_callback_create_from_r_function<ObjectCoerceCallback>(r_function);
}

SEXP r_object_coerce_callback_create_from_c_function(SEXP r_c_function) {
    return r_callback_create_from_c_function<ObjectCoerceCallback>(
        r_c_function);
}
