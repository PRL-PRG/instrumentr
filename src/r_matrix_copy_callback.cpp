#include "r_matrix_copy_callback.h"
#include "r_callback.h"
#include "MatrixCopyCallback.hpp"

using instrumentr::MatrixCopyCallback;

SEXP r_matrix_copy_callback_create_from_r_function(SEXP r_function) {
    return r_callback_create_from_r_function<MatrixCopyCallback>(r_function);
}

SEXP r_matrix_copy_callback_create_from_c_function(SEXP r_c_function) {
    return r_callback_create_from_c_function<MatrixCopyCallback>(r_c_function);
}
