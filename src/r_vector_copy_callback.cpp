#include "r_vector_copy_callback.h"
#include "r_callback.h"
#include "VectorCopyCallback.hpp"

using instrumentr::VectorCopyCallback;

SEXP r_vector_copy_callback_create_from_r_function(SEXP r_function) {
    return r_callback_create_from_r_function<VectorCopyCallback>(r_function);
}

SEXP r_vector_copy_callback_create_from_c_function(SEXP r_c_function) {
    return r_callback_create_from_c_function<VectorCopyCallback>(r_c_function);
}
