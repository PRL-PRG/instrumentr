#include "r_object_duplicate_callback.h"
#include "r_callback.h"
#include "ObjectDuplicateCallback.hpp"

using instrumentr::ObjectDuplicateCallback;

SEXP r_object_duplicate_callback_create_from_r_function(SEXP r_function) {
    return r_callback_create_from_r_function<ObjectDuplicateCallback>(
        r_function);
}

SEXP r_object_duplicate_callback_create_from_c_function(SEXP r_c_function) {
    return r_callback_create_from_c_function<ObjectDuplicateCallback>(
        r_c_function);
}
