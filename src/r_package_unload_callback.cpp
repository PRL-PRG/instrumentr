#include "r_package_unload_callback.h"
#include "r_callback.h"
#include "PackageUnloadCallback.hpp"

using instrumentr::PackageUnloadCallback;

SEXP r_package_unload_callback_create_from_r_function(SEXP r_function) {
    return r_callback_create_from_r_function<PackageUnloadCallback>(r_function);
}

SEXP r_package_unload_callback_create_from_c_function(SEXP r_c_function) {
    return r_callback_create_from_c_function<PackageUnloadCallback>(r_c_function);
}
