#include "r_package_attach_callback.h"
#include "r_callback.h"
#include "PackageAttachCallback.hpp"

using instrumentr::PackageAttachCallback;

SEXP r_package_attach_callback_create_from_r_function(SEXP r_function) {
    return r_callback_create_from_r_function<PackageAttachCallback>(r_function);
}

SEXP r_package_attach_callback_create_from_c_function(SEXP r_c_function) {
    return r_callback_create_from_c_function<PackageAttachCallback>(r_c_function);
}
