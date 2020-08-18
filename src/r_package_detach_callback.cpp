#include "r_package_detach_callback.h"
#include "r_callback.h"
#include "PackageDetachCallback.hpp"

using instrumentr::PackageDetachCallback;

SEXP r_package_detach_callback_create_from_r_function(SEXP r_function) {
    return r_callback_create_from_r_function<PackageDetachCallback>(r_function);
}

SEXP r_package_detach_callback_create_from_c_function(SEXP r_c_function) {
    return r_callback_create_from_c_function<PackageDetachCallback>(r_c_function);
}
