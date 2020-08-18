#include "r_package_load_callback.h"
#include "r_callback.h"
#include "PackageLoadCallback.hpp"

using instrumentr::PackageLoadCallback;

SEXP r_package_load_callback_create_from_r_function(SEXP r_function) {
    return r_callback_create_from_r_function<PackageLoadCallback>(r_function);
}

SEXP r_package_load_callback_create_from_c_function(SEXP r_c_function) {
    return r_callback_create_from_c_function<PackageLoadCallback>(r_c_function);
}
