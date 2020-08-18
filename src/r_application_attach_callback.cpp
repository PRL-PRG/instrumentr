#include "r_application_attach_callback.h"
#include "r_callback.h"
#include "ApplicationAttachCallback.hpp"

using instrumentr::ApplicationAttachCallback;

SEXP r_application_attach_callback_create_from_r_function(SEXP r_function) {
    return r_callback_create_from_r_function<ApplicationAttachCallback>(r_function);
}

SEXP r_application_attach_callback_create_from_c_function(SEXP r_c_function) {
    return r_callback_create_from_c_function<ApplicationAttachCallback>(r_c_function);
}
