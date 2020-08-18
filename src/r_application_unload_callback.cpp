#include "r_application_unload_callback.h"
#include "r_callback.h"
#include "ApplicationUnloadCallback.hpp"

using instrumentr::ApplicationUnloadCallback;

SEXP r_application_unload_callback_create_from_r_function(SEXP r_function) {
    return r_callback_create_from_r_function<ApplicationUnloadCallback>(r_function);
}

SEXP r_application_unload_callback_create_from_c_function(SEXP r_c_function) {
    return r_callback_create_from_c_function<ApplicationUnloadCallback>(r_c_function);
}
