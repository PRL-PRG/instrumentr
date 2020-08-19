#include "r_gc_allocation_callback.h"
#include "r_callback.h"
#include "GcAllocationCallback.hpp"

using instrumentr::GcAllocationCallback;

SEXP r_gc_allocation_callback_create_from_r_function(SEXP r_function) {
    return r_callback_create_from_r_function<GcAllocationCallback>(r_function);
}

SEXP r_gc_allocation_callback_create_from_c_function(SEXP r_c_function) {
    return r_callback_create_from_c_function<GcAllocationCallback>(r_c_function);
}
