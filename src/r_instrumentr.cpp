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
