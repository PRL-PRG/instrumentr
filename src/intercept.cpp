#include "r_api.h"

bool interception_enabled = true;

SEXP r_lightr_intercept_call_entry(SEXP package_name,
                                   SEXP function_name,
                                   SEXP function,
                                   SEXP environment) {
    //std::cerr << "Entered " << CHAR(asChar(package_name))
    //          << "::" << CHAR(asChar(function_name)) << std::endl;
    return R_NilValue;
}

SEXP r_lightr_intercept_call_exit(SEXP package_name,
                                  SEXP function_name,
                                  SEXP result,
                                  SEXP failed) {
    //std::cerr << "Exited " << CHAR(asChar(package_name))
    //          << "::" << CHAR(asChar(function_name)) << std::endl;
    return R_NilValue;
}

SEXP r_lightr_interception_is_enabled() {
    return ScalarLogical(interception_enabled);
}

SEXP r_lightr_disable_interception() {
    interception_enabled = false;
    return R_NilValue;
}

SEXP r_lightr_enable_interception() {
    interception_enabled = true;
    return R_NilValue;
}
