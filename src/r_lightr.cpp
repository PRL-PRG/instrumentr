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
