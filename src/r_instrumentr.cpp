#include "r_instrumentr.h"
#include "r_call.h"
#include "utilities.h"
#include "instrumentr.hpp"
#include "Context.hpp"
#include "Application.hpp"
#include "Package.hpp"
#include "Function.hpp"
#include "Call.hpp"

using instrumentr::Application;
using instrumentr::ApplicationSPtr;
using instrumentr::Call;
using instrumentr::CallSPtr;
using instrumentr::CallStack;
using instrumentr::CallStackSPtr;
using instrumentr::Context;
using instrumentr::ContextSPtr;
using instrumentr::Function;
using instrumentr::FunctionSPtr;
using instrumentr::Package;
using instrumentr::PackageSPtr;

SEXP r_instrumentr_get_commit_hash() {
    return mkString(instrumentr::get_commit_hash());
}

SEXP r_instrumentr_is_undefined_object(SEXP object) {
    return ScalarLogical(instrumentr::is_undefined_object(object));
}

SEXP r_instrumentr_is_defined_object(SEXP object) {
    return ScalarLogical(instrumentr::is_defined_object(object));
}

SEXP r_instrumentr_is_tracing_enabled() {
    return ScalarLogical(instrumentr::is_tracing_enabled());
}

SEXP r_instrumentr_disable_tracing() {
    instrumentr::disable_tracing();
    return R_NilValue;
}

SEXP r_instrumentr_enable_tracing() {
    instrumentr::enable_tracing();
    return R_NilValue;
}

SEXP r_instrumentr_reinstate_tracing() {
    instrumentr::reinstate_tracing();
    return R_NilValue;
}

SEXP r_instrumentr_initialize_instrumentr(SEXP r_package_environment,
                                          SEXP r_state_environment,
                                          SEXP r_undefined_object) {
    instrumentr::initialize_instrumentr(
        r_package_environment, r_state_environment, r_undefined_object);
    return R_NilValue;
}

SEXP r_instrumentr_finalize_instrumentr() {
    instrumentr::finalize_instrumentr();
    return R_NilValue;
}

SEXP r_instrumentr_get_object_details(SEXP r_value,
                                      SEXP r_variable,
                                      SEXP r_environment,
                                      SEXP r_peek) {
    SEXP r_actual_value = r_value;

    if (r_variable != R_NilValue) {
        const char* variable = CHAR(STRING_ELT(r_variable, 0));

        r_actual_value = Rf_findVarInFrame(r_environment, Rf_install(variable));

        bool peek = LOGICAL(r_peek)[0];

        if (peek && TYPEOF(r_actual_value) == PROMSXP) {
            r_actual_value = PREXPR(r_actual_value);
        }
    }

    char* address;

    if (asprintf(&address, "%p", (void*) r_actual_value) == -1) {
        Rf_error("Getting address of SEXP failed");
    }

    SEXP r_result = PROTECT(Rf_allocVector(VECSXP, 2));
    SET_VECTOR_ELT(r_result, 0, Rf_mkString(address));
    SET_VECTOR_ELT(
        r_result, 1, Rf_mkString(Rf_type2char(TYPEOF(r_actual_value))));

    SEXP r_names = PROTECT(Rf_allocVector(STRSXP, 2));
    SET_STRING_ELT(r_names, 0, mkChar("address"));
    SET_STRING_ELT(r_names, 1, mkChar("type"));

    Rf_setAttrib(r_result, R_NamesSymbol, r_names);

    UNPROTECT(2);

    return r_result;
}
