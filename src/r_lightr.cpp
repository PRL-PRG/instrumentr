#include "../inst/include/lightr.hpp"
#include "r_api.h"

using lightr::Application;
using lightr::Context;
using lightr::ContextSPtr;

SEXP r_lightr_get_application() {
    return Application::to_sexp(lightr::get_application());
}

SEXP r_lightr_get_context() {
    return Context::to_sexp(lightr::get_context());
}

SEXP r_lightr_set_context(SEXP r_context) {
    ContextSPtr context = Context::from_sexp(r_context);
    lightr::set_context(context);
    return R_NilValue;
}

SEXP r_lightr_is_tracing_enabled() {
    return ScalarLogical(lightr::is_tracing_enabled());
}

SEXP r_lightr_disable_tracing() {
    lightr::disable_tracing();
    return R_NilValue;
}

SEXP r_lightr_enable_tracing() {
    lightr::enable_tracing();
    return R_NilValue;
}

SEXP r_lightr_reinstate_tracing() {
    lightr::reinstate_tracing();
    return R_NilValue;
}
