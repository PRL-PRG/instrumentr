#include "r_gc_unmark_callback.h"
#include "r_callback.h"
#include "GcUnmarkCallback.hpp"

using instrumentr::GcUnmarkCallback;

SEXP r_gc_unmark_callback_create_from_r_function(SEXP r_function) {
    return r_callback_create_from_r_function<GcUnmarkCallback>(r_function);
}

SEXP r_gc_unmark_callback_create_from_c_function(SEXP r_c_function) {
    return r_callback_create_from_c_function<GcUnmarkCallback>(r_c_function);
}
