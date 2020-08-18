#include "r_application_detach_callback.h"
#include "r_callback.h"
#include "ApplicationDetachCallback.hpp"

using instrumentr::ApplicationDetachCallback;

SEXP r_application_detach_callback_create_from_r_function(SEXP r_function) {
    return r_callback_create_from_r_function<ApplicationDetachCallback>(r_function);
}

SEXP r_application_detach_callback_create_from_c_function(SEXP r_c_function) {
    return r_callback_create_from_c_function<ApplicationDetachCallback>(r_c_function);
}
