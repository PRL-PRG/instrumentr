#include <R.h>
#include <R_ext/Error.h>
#include <Rdefines.h>
#include <memory>
#include "instrumentr.hpp"

using instrumentr::from_sexp;
using instrumentr::to_sexp;

template <typename T>
SEXP r_callback_create_from_r_function(SEXP r_function) {
    std::shared_ptr<T> callback = std::make_shared<T>(r_function, true);
    return to_sexp<T>(callback);
}

template <typename T>
SEXP r_callback_create_from_c_function(SEXP r_c_function) {
    void* c_function = R_ExternalPtrAddr(r_c_function);
    std::shared_ptr<T> callback = std::make_shared<T>(c_function, false);
    return to_sexp<T>(callback);
}

extern "C" {
SEXP r_callback_is_r_callback(SEXP r_callback);
SEXP r_callback_is_c_callback(SEXP r_callback);
SEXP r_callback_get_function(SEXP r_callback);
}
