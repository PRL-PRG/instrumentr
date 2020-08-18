#include "r_application_load_callback.h"
#include "r_callback.h"
#include "ApplicationLoadCallback.hpp"

using instrumentr::ApplicationLoadCallback;

SEXP r_application_load_callback_create_from_r_function(SEXP r_function) {
    return r_callback_create_from_r_function<ApplicationLoadCallback>(r_function);
}

SEXP r_application_load_callback_create_from_c_function(SEXP r_c_function) {
    return r_callback_create_from_c_function<ApplicationLoadCallback>(r_c_function);
}
